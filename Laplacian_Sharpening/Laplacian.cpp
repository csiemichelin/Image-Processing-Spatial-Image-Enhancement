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

void Laplacian(Mat src, Mat dst , int mask[3][3])
{
	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			//saturate_cast函數的作用即是:當運算完後，結果為負，則轉為0，結果超出255，則為255
			dst.at<uchar>(i, j) = saturate_cast<uchar>(mask[1][1] * src.at<uchar>(i, j) +
				(mask[0][0] * src.at<uchar>(i - 1, j - 1) + mask[1][0] * src.at<uchar>(i, j - 1) + mask[2][0] * src.at<uchar>(i + 1, j - 1) +
					mask[0][1] * src.at<uchar>(i - 1, j) + mask[2][1] * src.at<uchar>(i + 1, j) +
					mask[0][2] * src.at<uchar>(i - 1, j + 1) + mask[1][2] * src.at<uchar>(i, j + 1) + mask[2][2] * src.at<uchar>(i + 1, j + 1)));
			//將算出來的拉普拉斯與原本值相加存回去
			dst.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) +  dst.at<uchar>(i, j));
		}
	}
}

//  Laplacian mask 鄰域 ，差值越大凸顯臨邊
int mask1[3][3] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 }; //四臨域，模糊臨邊
int mask2[3][3] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 }; //四臨域，凸顯臨邊，適合Peppers
int mask3[3][3] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 }; //八臨域，全部整個模糊
int mask4[3][3] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };//八臨域，凸顯臨邊，適合Jetplane && Lake

int main() {
/*************************	Jetplane.bmp	***************************/
	Mat src1 = imread("Jetplane.bmp", -1);
	Mat des1 = imread("Jetplane.bmp", -1);
	// 做拉普拉斯銳化
	Laplacian(src1, des1, mask4);
	// 顯示結果
	showImage(src1, Draw(src1, 1), "Jetplane_Original");		// 顯示原圖
	showImage(des1, Draw(des1, 1), "Jetplane_Laplacian");// 顯示拉普拉斯結果

/*************************	Lake.bmp	***************************/
	Mat src2 = imread("Lake.bmp", -1);
	Mat des2 = imread("Lake.bmp", -1);
	// 做拉普拉斯銳化
	Laplacian(src2, des2, mask4);
	// 顯示結果
	showImage(src2, Draw(src2, 5), "Lake_Original");		// 顯示原圖
	showImage(des2, Draw(des2, 5), "Lake_Laplacian");// 顯示拉普拉斯結果

/*************************	Peppers.bmp	***************************/
	Mat src3 = imread("Peppers.bmp", -1);
	Mat des3 = imread("Peppers.bmp", -1);
	// 做拉普拉斯銳化
	Laplacian(src3, des3, mask2);
	// 顯示結果
	showImage(src3, Draw(src3, 15), "Peppers_Original");		// 顯示原圖
	showImage(des3, Draw(des3, 15), "Peppers_Laplacian");// 顯示拉普拉斯結果

	waitKey(0);
	return -1;
}

