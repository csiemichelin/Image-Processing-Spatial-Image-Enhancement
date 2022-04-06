#define _CRT_SECURE_NO_WARNINGS //�����binclude�e
#include <cstdio>                   
#include <math.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// �e�����Histogram
Mat Draw(Mat& src, int num) {
	Mat dstHist;
	int dims = 1;
	float hranges[] = { 0, 256 }; //�d�� 0-255
	const float* ranges = { hranges };   //�ݭn��const����
	int size = 256;
	int channels = 0;

	// �p��Ϲ��������
	calcHist(&src, 1, &channels, Mat(), dstHist, dims, &size, &ranges); //cv2.calcHist(�v��, �q�D, �B�n, �϶��ƶq, �ƭȽd��)
	Mat dstImage(size, size, CV_8U, Scalar(255)); // 0���¦� 255�զ�,scalar()�I���G��

	//��̤j�ȩM�̤p��
	double maxValue = 0;
	double minValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0); //�bcv���Ϊ��OcvGetMinMaxHistValue

	//ø�s�����
	//saturate_cast��ƪ��@�ΧY�O:��B�⧹��A���G���t�A�h�ର0�A���G�W�X255�A�h��255
	int htp = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++) {
		float binValue = dstHist.at<float>(i); //�`�Nhist���Ofloat����
		//�Ԧ���0-max
		int realValue = static_cast<int>(binValue * htp * num / maxValue);
		line(dstImage, Point(i, size - 1), Point(i, 255 - realValue), Scalar(0));
	}
	return dstImage;
}

// �N�Ϲ��򪽤�ϩ�b�P�@����
void showImage(Mat src1, Mat src2, String s)
{
	// �غc�Ϲ����j�p
	int width = src1.cols + src2.cols;
	int height = max(src1.rows, src2.rows);
	Mat MergeImage = Mat::zeros(Size(width, height), src1.type());
	// �إ߹Ϲ�ROI:�O�q�Ϲ�����ܪ��@�ӹϹ��ϰ�A�o�Ӱϰ�O�A���Ϲ����R�����`�����I�C
	Rect r1(0, 0, src1.cols, src1.rows);
	Rect r2(0, 0, src2.cols, src2.rows);
	r2.x = src1.cols;
	// copy���e
	src1.copyTo(MergeImage(r1));
	src2.copyTo(MergeImage(r2));
	imshow(s, MergeImage);
	//waitKey(0);
}

void Power_Law(Mat src, Mat dst, double G)
{
	//Ū��Mat
	uchar* readImage = src.data;
	uchar* writeImage = dst.data;
	double c; //�`�� �N�ഫ��v���վ�^0~255
	c = 255 / (pow(255, G));
	for (int i = 0; i < src.total(); i++)
	{
		int r = readImage[i];
		int s = c * pow(r, G);
		writeImage[i] = s;
	}
}

int main() {
	double G; //Gamma��
/*************************	Jetplane.bmp	***************************/
	G = 1.6; // ��JGamma��
	Mat src1 = imread("Jetplane.bmp", -1);
	Mat des1 = imread("Jetplane.bmp", -1);
	// ��power_law&&�����
	Power_Law(src1, des1, G);
	// ��ܵ��G
	showImage(src1, Draw(src1, 1), "Jetplane_Original");		// ��ܭ��
	showImage(des1, Draw(des1, 1), "Jetplane_Power_Law  G=1.6");// ���Power_Law���G

/*************************	Lake.bmp	***************************/
	G = 0.45; // ��JGamma��
	Mat src2 = imread("Lake.bmp", -1);
	Mat des2 = imread("Lake.bmp", -1);
	// ��power_law&&�����
	Power_Law(src2, des2, G);
	// ��ܵ��G
	showImage(src2, Draw(src2, 5), "Lake_Original");		// ��ܭ��
	showImage(des2, Draw(des2, 5), "Lake_Power_Law  G=0.45");// ���Power_Law���G

/*************************	Peppers.bmp	***************************/
	G = 4.1; // ��JGamma��
	Mat src3 = imread("Peppers.bmp", -1);
	Mat des3 = imread("Peppers.bmp", -1);
	// ��power_law&&�����
	Power_Law(src3, des3, G);
	// ��ܵ��G
	showImage(src3, Draw(src3, 15), "Peppers_Original");		// ��ܭ��
	showImage(des3, Draw(des3, 15), "Peppers_Power_Law  G=4.1");// ���Power_Law���G

	waitKey(0);
	return -1;
}

