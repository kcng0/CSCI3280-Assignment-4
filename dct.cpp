#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <time.h>
using namespace std;
#define PI 3.14159265358979
int QuantizationMatrix[8][8] = {
	{3, 5, 7, 9, 11, 13, 15, 17}, 
	{5, 7, 9, 11, 13, 15, 17, 19},
	{7, 9, 11, 13, 15, 17, 19, 21},
	{9, 11,	13,	15,	17,	19,	21,	23},
	{11, 13, 15, 17, 19, 21, 23, 25},
	{13, 15, 17, 19, 21, 23, 25, 27},
	{15, 17, 19, 21, 23, 25, 27, 29},
	{17, 19, 21, 23, 25, 27, 29, 31}
	};


int main(int argc, char** argv)
{
	clock_t tStart = clock();
	if (argc != 3)
	{
		cout << "Arguments prompt: dct.exe <img_path> <apply_idct>" << endl;
		return 0;
	}
	string imgPath = argv[1];
	bool need_idct = stoi(argv[2]);

	//! read input image
	
	Bitmap s_img(imgPath.c_str());
	int rows = s_img.getHeight(), cols = s_img.getWidth();
	cout << "Apply DCT on image ("<<rows<<", "<<cols<< ")." << endl;
	


	//! 2D DCT for every 8x8 block (assume that the input image resolution is fixed to 256)	
	// The quantized coefficients should be stored into 'coeffArray'
	double coeffArray[256][256]={0};
	double dct_r[8][8]={0};
	int blockRow = rows / 8, blockCol = cols / 8;


	for (int i = 0; i < blockRow; i++)
	{
		for (int j = 0; j < blockCol; j++)
		{
			int xpos = j*8, ypos = i*8;

		//! apply DCT on block_ij (basic requirement)
		//TODO
		// Apply 1D DCT to a row vi

		for (int v = 0; v < 8 ; v++) {
			for (int u = 0; u < 8; u++) {
			double temp = 0;
			for (int x = 0; x < 8; x++) {
				unsigned char p;
				s_img.getPixel(x + xpos , v + ypos, p);
				//! preprocess by shifting pixel values by 128
				//TODO
				temp +=cos((2*x + 1) * u * PI / 16) * ((double) p - 128);
			}
		double cu;
		if (u == 0) cu = 1/sqrt(2); else cu = 1;
		dct_r[v][u] = (1/2.0) * cu * temp ;
			}
		}

		// Apply 1D DCT to column ui
		for (int v = 0; v < 8 ; v++) {
			for (int u = 0; u < 8; u++) {
			double temp = 0;
			for (int y = 0; y < 8; y++) {
				temp +=cos( (2*y + 1) * v * PI / 16) * dct_r[y][u];
			}
		double cv;
		if (v == 0) cv = 1/sqrt(2); else cv = 1;
		coeffArray[v + ypos][u + xpos] = (1/2.0) * cv  * temp;
			}
		}
			//! quantize the frequency coefficient of this block (basic requirement)
			//TODO
			for (int y = 0; y < 8; y++) 
				for (int x = 0; x < 8; x++) 
					coeffArray[y + ypos][x + xpos] = round (coeffArray[y + ypos][x + xpos] / QuantizationMatrix[y][x]);
				
		}
	}
	
	//! output the computed coefficient array
	FILE *fp = fopen("coeffs.txt", "w");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			fprintf(fp, "%3.3lf ", coeffArray[c][r]);
		}
		fprintf(fp, "\n");
	}
	cout << "Quantized coefficients saved!" << endl;
	double qt = (double)(clock() - tStart) / CLOCKS_PER_SEC;
	printf("Quantization time used : %.2fs\n", qt);
	if (need_idct)
	{
		Bitmap reconstructedImg(cols, rows);
		//! apply IDCT on the quantized coefficients (enhancement part)
		for (int i = 0; i < blockRow; i++)
		{
			for (int j = 0; j < blockCol; j++)
			{
				int xpos = j*8, ypos = i*8;
				//! apply de-quantization on block_ij 
				//TODO
				for (int y = 0; y < 8; y++)
					for (int x = 0; x < 8; x++)
						{
							coeffArray[y + ypos][x + xpos] *= QuantizationMatrix[y][x];
						}

				//! apply IDCT on this block
				//TODO

				// Apply 1D IDCT to a row yi
				for (int y = 0; y < 8; y++) {
					for (int x = 0; x < 8; x++) {
						double temp = 0;
						for (int u = 0; u < 8; u++) {
							double cu;
							if (u == 0) cu = 1/sqrt(2); else cu = 1;
							temp += cu * cos( (2* x + 1) * u * PI / 16) * coeffArray[y + ypos][u + xpos];
						}
						dct_r[y][x] = temp / 2;
					}
				}

				// Apply 1D IDCT to a column xi
				for (int y = 0; y < 8; y++) {
					for (int x = 0; x < 8; x++)
						{
							double temp = 0;
							for (int v = 0; v < 8; v++) {
								double cv;
								if (v == 0) cv = 1 / sqrt(2); else cv = 1;
								temp += cv * cos((2*y + 1 ) * v * PI / 16) * dct_r[v][x];
							}
							coeffArray[y + ypos][x + xpos] = temp / 2;
						}
				}
				//! shiftting back the pixel value range to 0~255
				//TODO
				
				for (int y = 0; y < 8; y++) {
					for (int x = 0; x < 8; x++) {
						int temp = round (coeffArray[y + ypos][x + xpos] + 128);
						unsigned char pixel = temp;
						reconstructedImg.setPixel(x + xpos, y + ypos, pixel);
					}
				}
			}
		}
		string savePath = "reconstructedImg.bmp";
		reconstructedImg.save(savePath.c_str());
		cout << "reconstructed image saved!" << endl;
		double rt = (double)(clock() - tStart) / CLOCKS_PER_SEC - qt;
		printf("Reconstuction time used: %.2fs", rt);
	}

	return 0;
}