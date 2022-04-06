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

void Laplacian(Mat src, Mat dst , int mask[3][3])
{
	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			//saturate_cast��ƪ��@�ΧY�O:��B�⧹��A���G���t�A�h�ର0�A���G�W�X255�A�h��255
			dst.at<uchar>(i, j) = saturate_cast<uchar>(mask[1][1] * src.at<uchar>(i, j) +
				(mask[0][0] * src.at<uchar>(i - 1, j - 1) + mask[1][0] * src.at<uchar>(i, j - 1) + mask[2][0] * src.at<uchar>(i + 1, j - 1) +
					mask[0][1] * src.at<uchar>(i - 1, j) + mask[2][1] * src.at<uchar>(i + 1, j) +
					mask[0][2] * src.at<uchar>(i - 1, j + 1) + mask[1][2] * src.at<uchar>(i, j + 1) + mask[2][2] * src.at<uchar>(i + 1, j + 1)));
			//�N��X�Ӫ��Դ��Դ��P�쥻�Ȭۥ[�s�^�h
			dst.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) +  dst.at<uchar>(i, j));
		}
	}
}

//  Laplacian mask �F�� �A�t�ȶV�j�Y���{��
int mask1[3][3] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 }; //�|�{��A�ҽk�{��
int mask2[3][3] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 }; //�|�{��A�Y���{��A�A�XPeppers
int mask3[3][3] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 }; //�K�{��A������Ӽҽk
int mask4[3][3] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };//�K�{��A�Y���{��A�A�XJetplane && Lake

int main() {
/*************************	Jetplane.bmp	***************************/
	Mat src1 = imread("Jetplane.bmp", -1);
	Mat des1 = imread("Jetplane.bmp", -1);
	// ���Դ��Դ��U��
	Laplacian(src1, des1, mask4);
	// ��ܵ��G
	showImage(src1, Draw(src1, 1), "Jetplane_Original");		// ��ܭ��
	showImage(des1, Draw(des1, 1), "Jetplane_Laplacian");// ��ܩԴ��Դ����G

/*************************	Lake.bmp	***************************/
	Mat src2 = imread("Lake.bmp", -1);
	Mat des2 = imread("Lake.bmp", -1);
	// ���Դ��Դ��U��
	Laplacian(src2, des2, mask4);
	// ��ܵ��G
	showImage(src2, Draw(src2, 5), "Lake_Original");		// ��ܭ��
	showImage(des2, Draw(des2, 5), "Lake_Laplacian");// ��ܩԴ��Դ����G

/*************************	Peppers.bmp	***************************/
	Mat src3 = imread("Peppers.bmp", -1);
	Mat des3 = imread("Peppers.bmp", -1);
	// ���Դ��Դ��U��
	Laplacian(src3, des3, mask2);
	// ��ܵ��G
	showImage(src3, Draw(src3, 15), "Peppers_Original");		// ��ܭ��
	showImage(des3, Draw(des3, 15), "Peppers_Laplacian");// ��ܩԴ��Դ����G

	waitKey(0);
	return -1;
}

