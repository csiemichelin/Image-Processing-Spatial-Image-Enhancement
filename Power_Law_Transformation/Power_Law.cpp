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

void Power_Law(Mat src, Mat dst, double G)
{
	//讀取Mat
	uchar* readImage = src.data;
	uchar* writeImage = dst.data;
	double c; //常數 將轉換後影像調整回0~255
	c = 255 / (pow(255, G));
	for (int i = 0; i < src.total(); i++)
	{
		int r = readImage[i];
		int s = c * pow(r, G);
		writeImage[i] = s;
	}
}

int main() {
	double G; //Gamma值
/*************************	Jetplane.bmp	***************************/
	G = 1.6; // 輸入Gamma值
	Mat src1 = imread("Jetplane.bmp", -1);
	Mat des1 = imread("Jetplane.bmp", -1);
	// 做power_law&&直方圖
	Power_Law(src1, des1, G);
	// 顯示結果
	showImage(src1, Draw(src1, 1), "Jetplane_Original");		// 顯示原圖
	showImage(des1, Draw(des1, 1), "Jetplane_Power_Law  G=1.6");// 顯示Power_Law結果

/*************************	Lake.bmp	***************************/
	G = 0.45; // 輸入Gamma值
	Mat src2 = imread("Lake.bmp", -1);
	Mat des2 = imread("Lake.bmp", -1);
	// 做power_law&&直方圖
	Power_Law(src2, des2, G);
	// 顯示結果
	showImage(src2, Draw(src2, 5), "Lake_Original");		// 顯示原圖
	showImage(des2, Draw(des2, 5), "Lake_Power_Law  G=0.45");// 顯示Power_Law結果

/*************************	Peppers.bmp	***************************/
	G = 4.1; // 輸入Gamma值
	Mat src3 = imread("Peppers.bmp", -1);
	Mat des3 = imread("Peppers.bmp", -1);
	// 做power_law&&直方圖
	Power_Law(src3, des3, G);
	// 顯示結果
	showImage(src3, Draw(src3, 15), "Peppers_Original");		// 顯示原圖
	showImage(des3, Draw(des3, 15), "Peppers_Power_Law  G=4.1");// 顯示Power_Law結果

	waitKey(0);
	return -1;
}

