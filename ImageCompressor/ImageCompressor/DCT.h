/*
	输入：Mat

	1、Mat分成8x8块，不足补齐
	2、对每一块，根据dct矩阵得到8x8个系数，量化
	3、将所有块拼起来

	输出：DCT/iDCT系数Mat
*/
#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "fstream"
#include "regex"
#include "string.h"
#include "time.h"
#include "Windows.h"
#include "math.h"
#include "stdio.h"

using namespace cv;
using namespace std;

class DCT {
public:

	DCT()
	{
		DCTMat = Mat::zeros(8, 8, CV_64FC1);
		iDCTMat = Mat::zeros(8, 8, CV_64FC1);
		mask = Mat::zeros(8, 8, CV_64FC1);

		SetDCTMat();
		SetiDCTMat();
		SetMask();
	}

	void SetDCTMat() // 生成dct矩阵
	{
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) 
			{
				double alpha = (i == 0) ? sqrt(1.0f / 8) : sqrt(2.0f / 8);
				DCTMat.at<double>(i, j) = alpha * cos((j + 0.5f) * CV_PI * i / 8);
			}

	}

	void SetiDCTMat() // 逆dct矩阵
	{
		for (int j = 0; j < 8; ++j)
			for (int i = 0; i < 8; ++i)
				iDCTMat.at<double>(i, j) = DCTMat.at<double>(j, i);
	}

	void SetMask() // 用于量化
	{
		mask = (Mat_<double>(8, 8)
		 << 1, 1, 1, 1, 1, 0, 0, 0,
			1, 1, 1, 1, 1, 0, 0, 0,
			1, 1, 1, 1, 0, 0, 0, 0,
			1, 1, 1, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0);
	}

	Mat DCT8x8(Mat image); // DCT变换

	Mat iDCT8x8(Mat image); // 逆变换

private:
	Mat DCTMat;
	Mat iDCTMat;
	Mat mask; // 8x8 量化表（实际上直接把系数去掉了）
};