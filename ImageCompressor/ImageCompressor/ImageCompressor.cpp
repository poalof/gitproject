#pragma warning(disable:4996)
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "fstream"
#include "string.h"
#include "Windows.h"
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/utils/logger.hpp>

#include "HuffmanCode.h"
#include "DCT.h"
#include "Order.h"


using namespace cv;
using namespace std;

/*
	<文件格式>
	通道数 | rows | cols | 通道1大小 | 通道1数据 | 通道2大小 | ...
*/

Mat Subsample(Mat img, int factor);

// 压缩
void Compress(string srcPath, string dstPath)
{
	cout << "Compressing..." << endl;

	Mat src = imread(srcPath, IMREAD_UNCHANGED);
	if (!src.data)  //判断是否有数据
	{
		cout << "Can not open file!" << endl;
		system("pause");
		return;
	}

	// 获得图像的通道数、大小
	vector<char> head;
	int channel = src.channels();
	int row = src.rows;
	int col = src.cols;

	char *p = reinterpret_cast<char*>(&channel);
	for (int i = 0; i < 4; i++)
		head.push_back(*(p + i));
	p = reinterpret_cast<char*>(&row);
	for (int i = 0; i < 4; i++)
		head.push_back(*(p + i));
	p = reinterpret_cast<char*>(&col);
	for (int i = 0; i < 4; i++)
		head.push_back(*(p + i));

	// 彩色图像分离RGB通道->YCrCb，作为压缩的单元
	vector<Mat> channels;
	if (channel == 3)
	{
		Mat ycrcbImage;
		cvtColor(src, ycrcbImage, COLOR_BGR2YCrCb);

		// 分离通道
		
		split(ycrcbImage, channels);
		channels[1] = Subsample(channels[1], 2); // Cr Cb下采样，4:2:0
		channels[2] = Subsample(channels[2], 2);
	}
	else
		channels.push_back(src);

	//cout << channel;

	// 输出的字节流
	vector<char> result(head.begin(), head.end());

	for (int i = 0; i < channel; i++)
	{
		int curSize; // 当前通道的大小
		DCT quantizer;
		HuffmanCode encoder;

		// DCT
		Mat dctImg = quantizer.DCT8x8(channels[i]); // dct + quantization

		// order + RLE
		// 没有把DC和AC分开，如果分开的话DC和AC编码表不一样，但是我又不用JPEG的做，感觉。。。意义不大
		vector<int> orderData;
		for (int y = 0; y < dctImg.rows; y += 8)
		{
			for (int x = 0; x < dctImg.cols; x += 8)
			{
				Mat block = dctImg(Rect(x, y, 8, 8));
				vector<int> tmp = Order::ZigZag(block);
				orderData.insert(orderData.end(), tmp.begin(), tmp.end());
			}
		}
		//cout << "origin size: " << orderData.size() << endl;
		orderData = Order::RLE_Encode(orderData);
		//cout << "ordered size: " << orderData.size() << endl;

		// Huffman Encoding
		vector<char> bitSeq = encoder.Encode(orderData); // 编码得到的输出值
		
		curSize = bitSeq.size();
		p = reinterpret_cast<char*>(&curSize);
		for (int k = 0; k < 4; k++)
			result.push_back(*(p + k)); // 1. size
		result.insert(result.end(), bitSeq.begin(), bitSeq.end()); // 2. data
	}

	// 保存文件
	ofstream outfile(dstPath, ios::binary | ios::out);
	outfile.write(result.data(), result.size());
	outfile.close();

	cout << "\nCompressed image saved! Total size: " << result.size() << " Bytes." << endl;
}

// 解压
Mat Decompress(string path)
{
	cout << "Loading..." << endl;

	// 打开并解码文件
	ifstream infile(path, ios::binary | ios::in);
	if (!infile.is_open())
	{
		cout << "Invalid path!" << endl;
		system("pause");
		return Mat_<double>();
	}

	infile.seekg(0, ios::end);
	int size = infile.tellg();
	infile.seekg(0, ios::beg);
	vector<char> read_data(size);
	infile.read(read_data.data(), read_data.size());
	infile.close();

	// 读头 channel | row | col | 1 | 2 | ...
	char pdata[4]{read_data[0], read_data[1], read_data[2], read_data[3]};
	int *p = reinterpret_cast<int*>(pdata);
	int channel = *p; // 1.
	for (int i = 0; i < 4; i++)
		pdata[i] = read_data[4+i];
	int row = *p; // 2.
	for (int i = 0; i < 4; i++)
		pdata[i] = read_data[8 + i];
	int col = *p; // 3.
	
	// 原图的大小，填充
	int pRow = row % 8 == 0 ? row : row + 8 - row % 8;
	int pCol = col % 8 == 0 ? col : col + 8 - col % 8;

	vector<Mat> channels; // 合成

	int fp = 12; // 读取字节流
	// 解析通道
	for (int i = 0; i < channel; i++)
	{
		for (int j = 0; j < 4; j++)
			pdata[j] = read_data[fp++];
		int cSize = *p; // 通道字节数

		HuffmanCode decoder;
		DCT quantizer;
		// Huffman解码
		vector<char> curData(read_data.begin()+fp, read_data.begin() + fp+cSize);
		vector<int> decodeData = decoder.Decode(curData);

		// RLE解码 + izigzag
		vector<int> reorderData = Order::RLE_Decode(decodeData);
		//cout << "reorder size: " << reorderData.size() << endl;

		if (i != 0)
		{
			pRow = (row / 2) % 8 == 0 ? (row / 2) : (row / 2) + 8 - (row / 2) % 8;
			pCol = (col / 2) % 8 == 0 ? (col / 2) : (col / 2) + 8 - (col / 2) % 8;
		}
		Mat	reMat = Mat::zeros(pRow, pCol, CV_32SC1);

		int cnt = 0;
		for (int y = 0; y < pRow; y += 8)
		{
			for (int x = 0; x < pCol; x += 8)
			{
				vector<int> tmpV(reorderData.begin() + cnt * 64, reorderData.end());
				Mat tmp = Order::iZigZag(tmpV);
				tmp.copyTo(reMat(Rect(x, y, 8, 8)));
				cnt++;
			}
		}

		// idct
		Mat idct = quantizer.iDCT8x8(reMat);
		channels.push_back(idct);

		fp += cSize;
	}

	// 组合成 RGB 图像 
	Mat grayRGBImage;
	if (channel == 3)
	{
		channels[0] = channels[0](Range(0, row), Range(0, col));
		channels[1] = channels[1](Range(0,row/2), Range(0,col/2));
		channels[2] = channels[2](Range(0, row / 2), Range(0, col / 2));
		// 对Cr通道进行双线性插值
		resize(channels[1], channels[1], Size(col, row), INTER_LINEAR);
		// 对Cb通道进行双线性插值
		resize(channels[2], channels[2], Size(col, row), INTER_LINEAR);
		
		merge(channels, grayRGBImage);
		cvtColor(grayRGBImage, grayRGBImage, COLOR_YCrCb2BGR);
	}
	else
		grayRGBImage = channels[0](Range(0,row), Range(0,col));

	cout << "Image loaded!" << endl;
	return grayRGBImage;
}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	while (1)
	{
		cout << "Choose your operation: " << endl;
		int choice = 0;
		cout << "1 --- Open an image and compress" << endl;
		cout << "2 --- Load a compressed file and show image" << endl;
		cout << "0 --- Quit" << endl;
		cin >> choice;
		cin.get();

		if (choice == 0) break;
		
		if (choice == 1) // 读取图片并压缩
		{
			// ../pictures/small.bmp
			cout << "\nInput source image path>";
			string srcPath, dstPath;
			getline(cin, srcPath);
			
			cout << "\nInput save path>";
			getline(cin, dstPath);

			Compress(srcPath, dstPath);
		}

		else if (choice == 2) // 读取压缩文件
		{
			// 解压图片，从path读入
			string path;
			cout << "\nInput file path>";
			getline(cin, path);
			Mat dst = Decompress(path);
			if (dst.type() != 0)
				continue;
			namedWindow("Image", WINDOW_NORMAL);
			cout << "\nPress <ESC> to exit\n";
			imshow("Image", dst);
			while (1)
				if (waitKey() == 27) break;
			destroyAllWindows();
			//waitKey(0);
		}

		else
		{
			cout << "Invalid input!" << endl;
		}
	}

	//system("pause");
	return 0;
}


Mat Subsample(Mat img, int factor)
{
	int output_width = img.cols / factor;
	int output_height = img.rows / factor;
	Mat output(output_height, output_width, img.type());

	for (int i = 0; i < output_height; ++i) 
	{
		for (int j = 0; j < output_width; ++j) 
		{
			output.at<uchar>(i, j) = img.at<uchar>(i * factor, j * factor);
		}
	}

	return output;
}