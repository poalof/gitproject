#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <vector>

using namespace std;
using namespace cv;

static const uint8_t zigzagTable[64] =
{
	0,  1,  5,  6,  14, 15, 27, 28,
    2,  4,  7,  13, 16, 26, 29, 42,
	3,  8,  12, 17, 25, 30, 41, 43,
	9,  11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63
};

class Order
{
public:
	static vector<int> ZigZag(Mat dct); // 一个块

	static Mat iZigZag(vector<int> ); // 一个块

	static vector<int> RLE_Decode(const vector<int>& encoded_data);

	static vector<int> RLE_Encode(const vector<int>& data);
	
};