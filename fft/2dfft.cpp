#include"helper.h"
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>

using namespace std;

void main() {
	//cout << "OK1" << endl;
	//cout << sizeof(Complex)*Wid/8 << "KB" << endl;
	//Complex* a2d[Len];
	//for (int j = 0; j < Len; j++) {
	//	a2d[j] = (Complex*)malloc(sizeof(Complex)*Wid);
	//}
	////cout << "OK" << endl;
	//for (int i = 0; i < Len; i++) {
	//	for (int j = 0; j < Wid; j++) {
	//		a2d[i][j].real = i*j;
	//		a2d[i][j].imagin = 0;
	//	}
	//}
	//show_img_re(a2d, Len, Wid);



	//Complex test[16];
	//for (int i = 0; i < 16; i++) {
	//	test[i].real = i;
	//	test[i].imagin = 0;
	//}
	//Complex tmp[16];
	//Complex res[16];
	//show_re(test, 16);
	//FFT(test, tmp , 16);
	//show_re(tmp, 16);
	//reFFT(tmp, res, 16);
	//show_re(res,16);
	cout << "输入待处理的图像" << endl;
	string path;
	cin >> path;
	int i = 0, j = 0, temp = 0;
	IplImage * img = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_GRAYSCALE);//ͼƬ·��

	int height = img->height;
	int width = img->width;
	int step = img->widthStep;
	uchar *data = (uchar*)img->imageData;
	float size = height*width;
	/*cout << "输入任意键查看原图像" << endl;
	char ch;
	cin >> ch;

	cvNamedWindow("demo", CV_WINDOW_AUTOSIZE);
	cvShowImage("demo", img);
	cvWaitKey(0);*/

	Complex *dst_org[Len];
	//dst = (Complex**)malloc(sizeof(Complex*)*Len);
	for (int i = 0; i<Len; i++)
		dst_org[i] = (Complex*)malloc(sizeof(Complex)*Wid);

	change_image(data, dst_org, height, width, step);

	Complex *dst_frq[Len];
	//dst = (Complex**)malloc(sizeof(Complex*)*Len);
	for (int i = 0; i<Len; i++)
		dst_frq[i] = (Complex*)malloc(sizeof(Complex)*Wid);
	fft_2d(dst_org, dst_frq);
	/*show_img_re(dst_frq, Len, Wid);*/
	LB(0.1, 0, dst_frq);
	Complex *dst_back[Len];
	for (int i = 0; i<Len; i++)
		dst_back[i] = (Complex*)malloc(sizeof(Complex)*Wid);
	refft_2d(dst_frq, dst_back);
	show_img_re(dst_back, Len, Wid);
	
	//for (int i = 0; i < 5;i++) {
	//	for (int j = 0; j < 5; j++) {
	//		cout << dst[i][j].real<<" ";
	//	}
	//	cout << "\n";
	//}
	//getchar();

	//FFT(input, dst, SIZE);
	/*for (int i = 0; i < 10; i++) {
		cout << dst[i].imagin << endl;
		cout << dst[i].real << endl;
	}*/
	getchar();
}
