#include"helper.h"
using namespace std;

void Add_Complex(Complex * src1, Complex *src2, Complex *dst) {
	dst->imagin = src1->imagin + src2->imagin;
	dst->real = src1->real + src2->real;
}
void Sub_Complex(Complex * src1, Complex *src2, Complex *dst) {
	dst->imagin = src1->imagin - src2->imagin;
	dst->real = src1->real - src2->real;
}
void Multy_Complex(Complex * src1, Complex *src2, Complex *dst) {
	double r1 = 0.0, r2 = 0.0;
	double i1 = 0.0, i2 = 0.0;
	r1 = src1->real;
	r2 = src2->real;
	i1 = src1->imagin;
	i2 = src2->imagin;
	dst->imagin = r1*i2 + r2*i1;
	dst->real = r1*r2 - i1*i2;
}


void getWN(double n, double size_n, Complex * dst) {
	double x = 2.0*M_PI*n / size_n;
	dst->imagin = -sin(x);
	dst->real = cos(x);
}

void re_getWN(double n, double size_n, Complex * dst) {
	double x = -2.0*M_PI*n / size_n;
	dst->imagin = -sin(x);
	dst->real = cos(x);
}

int FFT_remap(Complex * src, int size_n) {

	if (size_n == 1)
		return 0;
	Complex * temp = (Complex *)malloc(sizeof(Complex)*size_n);
	for (int i = 0; i<size_n; i++)
		if (i % 2 == 0)
			temp[i / 2] = src[i];
		else
			temp[(size_n + i) / 2] = src[i];
	for (int i = 0; i<size_n; i++)
		src[i] = temp[i];
	free(temp);
	FFT_remap(src, size_n / 2);
	FFT_remap(src + size_n / 2, size_n / 2);
	return 1;
}

void FFT(Complex * src, Complex * dst, int size_n) {

	FFT_remap(src, size_n);
	// for(int i=0;i<size_n;i++)
	//    printf("%lf\n",src[i]);
	clock_t start, end;
	start = clock();
	int k = size_n;
	int z = 0;
	while (k /= 2) {
		z++;
	}
	k = z;
	if (size_n != (1 << k))
		exit(0);
	Complex * src_com = (Complex*)malloc(sizeof(Complex)*size_n);
	if (src_com == NULL)
		exit(0);
	for (int i = 0; i<size_n; i++) {
		src_com[i].real = src[i].real;
		src_com[i].imagin = src[i].imagin;
	}
	for (int i = 0; i<k; i++) {
		z = 0;
		for (int j = 0; j<size_n; j++) {
			if ((j / (1 << i)) % 2 == 1) {
				Complex wn;
				getWN(z, size_n, &wn);
				Multy_Complex(&src_com[j], &wn, &src_com[j]);
				z += 1 << (k - i - 1);
				Complex temp;
				int neighbour = j - (1 << (i));
				temp.real = src_com[neighbour].real;
				temp.imagin = src_com[neighbour].imagin;
				Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
				Sub_Complex(&temp, &src_com[j], &src_com[j]);
			}
			else
				z = 0;
		}

	}

	for (int i = 0; i<size_n; i++) {
		dst[i].imagin = src_com[i].imagin;
		dst[i].real = src_com[i].real;
	}
	end = clock();
	printf("FFT use time :%lfs for Datasize of:%d\n", (double)(end - start) / CLOCKS_PER_SEC, size_n);
}

void reFFT(Complex * src, Complex * dst, int size_n) {

	FFT_remap(src, size_n);
	// for(int i=0;i<size_n;i++)
	//    printf("%lf\n",src[i]);
	clock_t start, end;
	start = clock();
	int k = size_n;
	int z = 0;
	while (k /= 2) {
		z++;
	}
	k = z;
	if (size_n != (1 << k))
		exit(0);
	Complex * src_com = (Complex*)malloc(sizeof(Complex)*size_n);
	if (src_com == NULL)
		exit(0);
	for (int i = 0; i<size_n; i++) {
		src_com[i].real = src[i].real;
		src_com[i].imagin = src[i].imagin;
	}
	for (int i = 0; i<k; i++) {
		z = 0;
		for (int j = 0; j<size_n; j++) {
			if ((j / (1 << i)) % 2 == 1) {
				Complex wn;
				re_getWN(z, size_n, &wn);
				Multy_Complex(&src_com[j], &wn, &src_com[j]);
				z += 1 << (k - i - 1);
				Complex temp;
				int neighbour = j - (1 << (i));
				temp.real = src_com[neighbour].real;
				temp.imagin = src_com[neighbour].imagin;
				Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
				Sub_Complex(&temp, &src_com[j], &src_com[j]);
			}
			else
				z = 0;
		}

	}

	for (int i = 0; i<size_n; i++) {
		dst[i].imagin = src_com[i].imagin/size_n;
		dst[i].real = src_com[i].real/size_n;
	}
	end = clock();
	printf("FFT use time :%lfs for Datasize of:%d\n", (double)(end - start) / CLOCKS_PER_SEC, size_n);
}

void change_image(uchar *data, Complex *dst[Len],int length,int width,int step ){

  int l_chg = (length - Len) / 2;
  int w_chg = (width - Wid) / 2;
  if (l_chg<0 || w_chg<0) {
	 std::cout << "error size!" << endl;
	  while (true);
  }
  for (int i = 0; i< Len; i++)
  {
	  for (int j = 0; j< Wid; j++)
	  {
		  (*(dst[i]+j)).real = data[(i+l_chg)*step + j+w_chg];
		  (*(dst[i]+j)).imagin = 0;
	  }
  }
}

void fft_2d(Complex *src[Len], Complex *dst[Len]){
	Complex row[Wid];
	Complex col[Len];
	Complex d_row[Wid];
	Complex d_col[Len];
	Complex* table[Len];
	for (int i = 0; i < Len; i++) {
		table[i] = (Complex*)malloc(sizeof(Complex)*Wid);
	}
	for(int i=0;i<Len;i++){
		for(int j=0;j<Wid;j++){
			row[j]=*(src[i]+j);
		}
		FFT(row, d_row, Wid);
		for (int j = 0; j<Wid; j++) {
			*(table[i]+j) = d_row[j];
		}
	}

	for (int i = 0; i<Wid; i++) {
		for (int j = 0; j<Len; j++) {
			col[j] = *(table[j] + i);
		}
		FFT(col, d_col, Len);
		for (int j = 0; j<Len; j++) {
			*(table[j] + i) =col[j];
		}
	}
	for (int i = 0; i < Len; i++) {
		for (int j = 0; j < Wid; j++) {
			*(dst[i] + j) = *(table[i] + j);
		}
	}
}

void refft_2d(Complex *src[Len], Complex *dst[Len]) {
	Complex row[Wid];
	Complex col[Len];
	Complex d_row[Wid];
	Complex d_col[Len];
	Complex* table[Len];
	for (int i = 0; i < Len; i++) {
		table[i] = (Complex*)malloc(sizeof(Complex)*Wid);
	}
	for (int i = 0; i<Wid; i++) {
		for (int j = 0; j<Len; j++) {
			col[j] = *(src[j] + i);
		}
		reFFT(col, d_col, Len);
		for (int j = 0; j<Len; j++) {
			*(table[j] + i) = col[j];
		}
	}

	for (int i = 0; i<Len; i++) {
		for (int j = 0; j<Wid; j++) {
			row[j] = *(table[i] + j);
		}
		reFFT(row, d_row, Wid);
		for (int j = 0; j<Wid; j++) {
			*(table[i] + j) = d_row[j];
		}
	}

	for (int i = 0; i < Len; i++) {
		for (int j = 0; j < Wid; j++) {
			*(dst[i]+j) = *(table[i] + j);
		}
	}
}

void show_re(Complex **src,uchar* data,int step) {
	;
	
}

void test_2d(){
	/*int **a;
	a = (int**)malloc(sizeof(int*)*5);
	for (int i = 0; i<5; i++)
		a[i] = (int*)malloc(sizeof(int)*4);
		*/
	int a[4][5];
	int k=0;
	for(int i=0;i<5;i++){
		for(int j=0;j<4;j++){
			k++;
			a[i][j]=k;
		}
	}
	int *l2 = (int*)(a+0);
	int *w2 = (int*)(a[0]+0);
	for(int i=0;i<4;i++){
		cout<<"l2: "<<l2[i]<<endl;
	}
	for(int i=0;i<5;i++){
		cout<<"w2: "<<w2[i]<<endl;
	}
}

void show_re(Complex*a, int size) {
	for (int i = 0; i < size; i++) {
		cout << a[i].real << " ";
	}
	cout << "\n***\n" << endl;
}

void LB(double percent, int type,Complex* data[Len]) {
	//type==0 低通(滤去中间) 1高通
	Complex zer;
	zer.imagin = 0;
	zer.real = 0;
	int l = percent*Len/2;
	int w = percent*Wid/2;
	for (int i = 0; i < Len; i++) {
		for (int j = 0; j < Wid; j++) {
			if (i<l || i>Len - l || j<w || j>Wid - w) {
				if (type == 1) {
					*(data[i]+j) = zer;
				}
			}
			else {
				if (type == 0) {
					*(data[i] + j) = zer;
				}
			}
		}
	}
}

int linear_255(double max, double min, int num) {
	double k;
	k = 255 / (max - min);
	int ret;
	ret = k*(num - min + 1);
	return ret;
}

void show_img_re(Complex* a[Len], int l,int w) {
	int max = -1000;
	int min = 1000;
	uchar img[Len][Wid] = {0};
	if (l != Len || w != Wid) {
		cout << "warning 输出大小不匹配。。。 可能丢失数据" << endl;
		while (true);
	}
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < w; j++) {
				if ((*(a[i]+j)).real > max) {
				max = (*(a[i] + j)).real;
			}
			if ((*(a[i] + j)).real < min) {
				min = (*(a[i] + j)).real;
			}
		}
	}
	cout << "max: " << max << endl;
	cout << "min: " << min << endl;
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < w; j++) {
			uchar tmp = linear_255(max, min, (int)((*(a[i] + j)).real));
			img[i][j] = tmp;
		}
	}


	char* imgdata;
	imgdata = (char*)malloc(sizeof(char)*Len*Wid);
	for (int i = 0; i < Len; i++) {
		for (int j = 0; j < Wid; j++) {
			imgdata[i*Wid + j] = img[i][j];
		}
	}

	CvSize cz = (Len, Wid);
	IplImage *imge = cvCreateImage(cz,8,1);
	imge->imageData = imgdata;
	cvNamedWindow("my image",1);
	imge->height = Len;
	imge->width = Wid;
	cvShowImage("my image", imge);
	cvWaitKey(0);
	getchar();
	cvDestroyWindow("my image");
	cvReleaseImage(&imge);
}

void even(char* src[Len], char* dst[Len]) {
	uchar temp;
	int step = 8;
	int size = Wid*Len;
	unsigned int hist[256] = { 0 };
	for (int i = 0; i<Len; i++)
	{
		for (int j = 0; j<Wid; j++)
		{
			temp = *(src[i]+j);
			hist[temp]++;
		}
	}
	float histPDF[256] = { 0 };
	for (int i = 0; i<255; i++)
	{
		histPDF[i] = (float)hist[i] / size;
	}

	float histCDF[256] = { 0 };
	for (int i = 0; i<256; i++)
	{
		if (0 == i) histCDF[i] = histPDF[i];
		else histCDF[i] = histCDF[i - 1] + histPDF[i];
	}

	int histEQU[256] = { 0 };
	for (int i = 0; i<256; i++)
	{
		histEQU[i] = (int)(255.0 * histCDF[i] + 0.5);
	}
	for (int i = 0; i<Len; i++)
	{
		for (int j = 0; j<Wid; j++)
		{
			temp = *(src[i] + j);
			*(src[i] + j) = histEQU[temp];
		}
	}
}

void get_re(Complex* a[Len],double* b[Len], int l, int w) {
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < w; j++) {
			*(b[i] + j) = (*(a[i] + j)).real;
		}		
	}
}

void get_im(Complex* a[Len], double* b[Len], int l, int w) {
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < w; j++) {
			*(b[i] + j) = (*(a[i] + j)).imagin;
		}
	}
}

void map2gray(double* a[Len], int* b[Len],int l,int w) {
	int max = -10000, min = +10000;
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < w; j++) {
			if (*(a[i] + j) > max) {
				max = *(a[i] + j);
				min = *(a[i] + j);
			}
		}
	}

}

//void even_img_re(Complex* a[Len], Complex* b[Len],int l, int w) {
//	int max = -1000;
//	int min = 1000;
//	uchar* img[Len] = { 0 };
//	uchar* dst[Len] = { 0 };
//	if (l != Len || w != Wid) {
//		cout << "warning 输出大小不匹配。。。 可能丢失数据" << endl;
//		while (true);
//	}
//	for (int i = 0; i < l; i++) {
//		for (int j = 0; j < w; j++) {
//			if ((*(a[i] + j)).real > max) {
//				max = (*(a[i] + j)).real;
//			}
//			if ((*(a[i] + j)).real < min) {
//				min = (*(a[i] + j)).real;
//			}
//		}
//	}
//	cout << "max: " << max << endl;
//	cout << "min: " << min << endl;
//	for (int i = 0; i < l; i++) {
//		for (int j = 0; j < w; j++) {
//			uchar tmp = linear_255(max, min, (int)((*(a[i] + j)).real));
//			img[i][j] = tmp;
//		}
//	}
//	even
//	uchar temp;
//	int step = 8;
//	int size = Wid*Len;
//	unsigned int hist[256] = { 0 };
//	for (int i = 0; i<Len; i++)
//	{
//		for (int j = 0; j<Wid; j++)
//		{
//			temp = img[i][j];
//			hist[temp]++;
//		}
//	}
//	float histPDF[256] = { 0 };
//	for (int i = 0; i<255; i++)
//	{
//		histPDF[i] = (float)hist[i] / size;
//	}
//
//	float histCDF[256] = { 0 };
//	for (int i = 0; i<256; i++)
//	{
//		if (0 == i) histCDF[i] = histPDF[i];
//		else histCDF[i] = histCDF[i - 1] + histPDF[i];
//	}
//
//	int histEQU[256] = { 0 };
//	for (int i = 0; i<256; i++)
//	{
//		histEQU[i] = (int)(255.0 * histCDF[i] + 0.5);
//	}
//	for (int i = 0; i<Len; i++)
//	{
//		for (int j = 0; j<Wid; j++)
//		{
//			temp = img[i][j];
//			even_img[i][j] = histEQU[temp];
//		}
//	}
//
//	
//
//	char* imgdata;
//	imgdata = (char*)malloc(sizeof(char)*Len*Wid);
//	for (int i = 0; i < Len; i++) {
//		for (int j = 0; j < Wid; j++) {
//			imgdata[i*Wid + j] = even_img[i][j];
//		}
//	}
//
//	CvSize cz = (Len, Wid);
//	IplImage *imge = cvCreateImage(cz, 8, 1);
//	imge->imageData = imgdata;
//	cvNamedWindow("my image", 1);
//	imge->height = Len;
//	imge->width = Wid;
//	cvShowImage("my image", imge);
//	cvWaitKey(0);
//	getchar();
//	cvDestroyWindow("my image");
//	cvReleaseImage(&imge);
//}