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
		cout << "Arguments prompt: dct.exe <txt_path> <output_name>" << endl;
		return 0;
	}
	string txtPath = argv[1];
	string imgPath = argv[2];
	double coeffArray[256][256] = {0};
	double dct_r[8][8] = {0};
	int rows = 256;
	int cols = 256;
	int blockRow = rows / 8;
	int blockCol = cols / 8;
	FILE * fp = fopen(txtPath.c_str(), "r");
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			fscanf(fp, "%lf", &coeffArray[c][r]);
		}
		fgetc(fp);
	}
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
		reconstructedImg.save(imgPath.c_str());
		cout << "reconstructed image saved!" << endl;
		double rt = (double)(clock() - tStart) / CLOCKS_PER_SEC;
		printf("Reconstuction time used: %.2fs", rt);


	return 0;
}