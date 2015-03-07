// ParallelSURF.cpp : Defines the entry point for the console application.
//

#pragma warning (disable : 4996)

#ifdef _WIN32
	#include "stdafx.h"
#endif

#include "surflib.h"
#include <ctime>
#include <iostream>

#ifdef linux
	#include <getopt.h>
#endif

#define MYFLOAT_ERROR 2

bool cmpImages(IplImage *img1, IplImage *img2){
	// set up variables for data access
	int height1 = img1->height;
	int width1 = img1->width;
	int step1 = img1->widthStep / sizeof(float);
	float *data1 = (float *)img1->imageData;

	int height2 = img2->height;
	int width2 = img2->width;
	int step2 = img2->widthStep / sizeof(float);
	float *data2 = (float *)img2->imageData;

	if (height1 != height2 || width1 != width2)
		return 0;
	for (int i = 0; i < height1 * width1; i++) {
		if (fabs(data1[i] - data2[i]) > MYFLOAT_ERROR) {
			printf("%d, %f, % f, %f\n", i, data1[i], data2[i], fabs(data1[i] - data2[i]));
		}
		//	return FALSE;
	}

	return 1;
}

int integralImage(char path[], int gpu){
	IplImage *img = cvLoadImage(path);

	clock_t start = clock();
	IplImage *serialIM = Integral(img, 0);
	clock_t end = clock();
	std::cout << "Serial Integral image took: " << float(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;

	start = clock();
	IplImage *parallelIM = Integral(img, 1);
	end = clock();
	std::cout << "Parallel Integral image took: " << float(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;

	if (cmpImages(serialIM, parallelIM)) {
		printf("OK");
	}
	else {
		printf("NOT OK");
	}

	getchar();
	return 0;
}

int fastHessian(char path[], int gpu) {
	return 0;
}

int surf(char path[], int gpu) {
	// Declare Ipoints and other stuff
	IpVec ipts;
	IplImage *img = cvLoadImage(path);

	// Detect and describe interest points in the image
	clock_t start = clock();
	surfDetDes(img, ipts, false, 5, 4, 2, 0.0004f, gpu);
	clock_t end = clock();

	std::cout << "OpenSURF found: " << ipts.size() << " interest points" << std::endl;
	std::cout << "OpenSURF took: " << float(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;

	// Draw the detected points
	drawIpoints(img, ipts);

	// Display the result
	showImage(img);
	return 0;
}
int main(int argc, char** argv)
{
	/*extern char *optarg;
	extern int optind, opterr, optopt;
	int option, option_index;

	static struct option long_options[] =
	{
		{ "help", no_argument, 0, 'h' },
		{ "send-inventory", required_argument, 0, 'i' },
		{ "send-net-usage", required_argument, 0, 'N' },
		{ "update", required_argument, 0, 'u' },
		{ "url", required_argument, 0, 'l' },
		{ "phost", required_argument, 0, 'H' },
		{ "pport", required_argument, 0, 'p' },
		{ "puid", required_argument, 0, 'U' },
		{ "ppasswd", required_argument, 0, 'w' },
		{ "nic", required_argument, 0, 'n' },
		{ 0, 0, 0, 0 }
	};

	while ((option = getopt_long(argc, argv, "hNi:u:l:H:p:U:w:n:",
		long_options, &option_index)) != -1)
	{	}*/

	if (argc < 4) {
		printf("Missing arguments\n");
		return -1;
	}

	switch (atoi(argv[1])){
		case 1: return integralImage(argv[2], atoi(argv[3]));
		case 2: return fastHessian(argv[2], atoi(argv[3]));
		case 3: return surf(argv[2], atoi(argv[3]));
		default: return printf("Invalid argument");
	}

	return 0;
}

