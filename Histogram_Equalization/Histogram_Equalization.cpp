//公式:https://codeinfo.space/imageprocessing/histogram-equalization/
#define _CRT_SECURE_NO_WARNINGS //必須在include前
#include <cstdio>                   
#include <math.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// 畫直方圖Histogram
Mat Draw(Mat& src, int num) {
	Mat dstHist;
	int dims = 1;
	float hranges[] = { 0, 256 }; //範圍 0-255
	const float* ranges = { hranges };   //需要為const類型
	int size = 256;
	int channels = 0;

	// 計算圖像的直方圖
	calcHist(&src, 1, &channels, Mat(), dstHist, dims, &size, &ranges); //cv2.calcHist(影像, 通道, 遮罩, 區間數量, 數值範圍)
	Mat dstImage(size, size, CV_8U, Scalar(255)); // 0為黑色 255白色,scalar()背景亮度

	//找最大值和最小值
	double maxValue = 0;
	double minValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0); //在cv中用的是cvGetMinMaxHistValue

	//繪製直方圖
	//saturate_cast函數的作用即是:當運算完後，結果為負，則轉為0，結果超出255，則為255
	int htp = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++) {
		float binValue = dstHist.at<float>(i); //注意hist中是float類型
		//拉伸到0-max
		int realValue = static_cast<int>(binValue * htp * num / maxValue);
		line(dstImage, Point(i, size - 1), Point(i, 255 - realValue), Scalar(0));
	}
	return dstImage;
}


// 將圖像跟直方圖放在同一視窗
void showImage(Mat src1, Mat src2, String s)
{
	// 建構圖像的大小
	int width = src1.cols + src2.cols;
	int height = max(src1.rows, src2.rows);
	Mat MergeImage = Mat::zeros(Size(width, height), src1.type());
	// 建立圖像ROI:是從圖像中選擇的一個圖像區域，這個區域是你的圖像分析所關注的重點。
	Rect r1(0, 0, src1.cols, src1.rows);
	Rect r2(0, 0, src2.cols, src2.rows);
	r2.x = src1.cols;
	// copy內容
	src1.copyTo(MergeImage(r1));
	src2.copyTo(MergeImage(r2));
	imshow(s, MergeImage);
	//waitKey(0);
}

void  Histogram_Equalization(Mat src, Mat dst)
{
	int n = 65536; //n 為整張圖的pixel數256*256
	int maxG = 255; //gray_level的最大值
	int nj[256]; // 對應到gray level為rk的pixel個數
	double Pr[256]; // 出現機率
	double Sk[256]; // 均衡化值(新的像素值)
	int int_Sk[256];

	// 利用mset將nj矩陣初始為0
	memset(nj, 0, 256 * sizeof(nj[0]));

	//計算gray level為rk的pixel個數
	uchar* readImage = src.data;
	for (int i = 0; i < src.total(); i++)
	{
		int rk = readImage[i];//num 為gray level
		nj[rk]++; //用nk存gray level為rk的pixel個數
	}

	// 出現機率(Pr) = 出現次數(nj) / 像素總數 (n)
	for (int i = 0; i < 256; i++)
	{
		Pr[i] = double(nj[i]) / double(n);
	}
	// 要將色彩空間範圍擴展為[0, 255] 共 256 個像素級度，就必須將原累計機率乘以255，得到均衡化值
	Sk[0] = Pr[0] * maxG ;
	//做sumation
	for (int i = 1; i < 256; i++)
	{
		Sk[i] = Pr[i] * double(maxG) + Sk[i - 1]; 
	}
	
	// 利用mset將result矩陣初始為0
	memset(int_Sk, 0, 256 * sizeof(int_Sk[0]));

	//double轉int
	for (int i = 0; i < 256; i++)
	{
		int_Sk[i] = Sk[i]; //double 轉 int 直接捨去浮點數的部分
	}
	// 將均衡化值寫回新的像素中
	uchar* writeImage = dst.data;
	for (int i = 0; i < dst.total(); i++)
	{
		int num = writeImage[i]; ///num 為 writeImage[i]的gray level
		writeImage[i] = int_Sk[num]; //writeImage[i]為該gray level的均衡化值
	}
}

int main() {
/*************************	Jetplane.bmp	***************************/
	Mat src1 = imread("Jetplane.bmp", -1);
	Mat des1 = imread("Jetplane.bmp", -1);
	// 做Histogram_Equalization&&直方圖
	Histogram_Equalization(src1, des1);
	// 顯示結果
	showImage(src1, Draw(src1, 1), "Jetplane_Original");		// 顯示原圖
	showImage(des1, Draw(des1, 1), "Jetplane_Histogram_Equalization");// 顯示Histogram_Equalization結果

/*************************	Lake.bmp	***************************/
	Mat src2 = imread("Lake.bmp", -1);
	Mat des2 = imread("Lake.bmp", -1);
	// 做Histogram_Equalization&&直方圖
	Histogram_Equalization(src2, des2);
	// 顯示結果
	showImage(src2, Draw(src2, 5), "Lake_Original");		// 顯示原圖
	showImage(des2, Draw(des2, 5), "Lake_Histogram_Equalization");// 顯示Histogram_Equalization結果

/*************************	Peppers.bmp	***************************/
	Mat src3 = imread("Peppers.bmp", -1);
	Mat des3 = imread("Peppers.bmp", -1);
	// 做Histogram_Equalization&&直方圖
	Histogram_Equalization(src3, des3);
	// 顯示結果
	showImage(src3, Draw(src3, 15), "Peppers_Original");		// 顯示原圖
	showImage(des3, Draw(des3, 15), "Peppers_Histogram_Equalization");// 顯示Histogram_Equalization結果

	waitKey(0);
	return -1;
}
