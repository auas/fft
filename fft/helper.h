#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define Len 1024
#define Wid 512
#define VALUE_MAX 1000
struct Complex_ {
	double real;
	double imagin;
};
typedef struct Complex_ Complex;

void Add_Complex(Complex * src1, Complex *src2, Complex *dst);
void Sub_Complex(Complex * src1, Complex *src2, Complex *dst);
void Multy_Complex(Complex * src1, Complex *src2, Complex *dst);


void getWN(double n, double size_n, Complex * dst);
void re_getWN(double n, double size_n, Complex * dst);
//
int FFT_remap(Complex * src, int size_n);
//fft
void FFT(Complex * src, Complex * dst, int size_n);

void reFFT(Complex * src, Complex * dst, int size_n);

void change_image(uchar *data, Complex *dst[Len], int length, int width, int step);

void fft_2d(Complex *src[Len], Complex *dst[Len]);

void refft_2d(Complex *src[Len], Complex *dst[Len]);

void test_2d();

void show_re(Complex*a, int size);

void LB(double percent, int type, Complex** data);// ÂË²¨

int linear_255(double max, double min, int num);

void show_img_re(Complex* a[Len], int l, int w);

void even(char* a[Len], char* dst[Len]);

//void even_img_re(Complex* a[Len], Complex* b[Len],int l, int w);