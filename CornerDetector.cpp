/*
 * Author: WANG, Haoan <Haoan.Wang@de.bertrandt.com>, Team B231, 
 * Bertrandt Ingenieurbuero GmbH, Ingolstadt Deutschland (Germany)
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

Mat src, src_gray;

// parameters for harris corner
int thresh = 200;
int max_thresh = 255;
char* source_window = "Source image";
char* corners_window = "Corners detected";

// parameters for shi-tomasi
int maxCorners = 23;
int maxTrackbar = 100;
RNG rng (12345);

void goodFeaturesToTrack (int, void*);
void HarrisCorner (int, void*);

// HarrisCorner Function
void HarrisCorner (int, void*)
{
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros (src.size (), CV_32FC1);
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
	cornerHarris (src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
	normalize (dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat ());
	convertScaleAbs (dst_norm, dst_norm_scaled);
	std::ofstream writer ("Harris_Corners_Result.txt");
	for (int j = 0; j < dst_norm.rows; j++)
	{
		bool exist = false;
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float> (j, i) > thresh)
			{
				exist = true;
				circle (dst_norm_scaled, Point (i, j), 5, Scalar (0), 2, 8, 0);
				writer << "[" << i << "," << j << "]" << "  ";
			}
		}
		if (exist)
		{
			writer << "\n";
		}
	}
	writer.close ();
	namedWindow (corners_window, WINDOW_AUTOSIZE);
	imshow (corners_window, dst_norm_scaled);
}

// Shi-Tomasi Corner Function
void goodFeaturesToTrack (int, void*)
{
	if (maxCorners < 1)
	{
		maxCorners = 1;
	}
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	Mat copy;
	copy = src.clone ();
	std::ofstream writer ("Shi-Tomasi_Corners_Result.txt");
	goodFeaturesToTrack (src_gray, corners, maxCorners, qualityLevel, minDistance, Mat (), blockSize, useHarrisDetector, k);
	cout << "** Number of corners detected: " << corners.size () << endl;
	int r = 4;
	for (size_t i = 0; i < corners.size (); i++)
	{
		circle (copy, corners[i], r, Scalar (rng.uniform (0, 255), rng.uniform (0, 255), rng.uniform (0, 255)), -1, 8, 0);
		writer << corners[i] << "\n";
	}
	namedWindow (source_window, WINDOW_AUTOSIZE);
	imshow (source_window, copy);
}

// Main Function
int main (int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./CornerDetector <Corner Type [1](harris) [2](shi-tomasi)> <image file directory and name>\n";
		exit (0);
	}
	std::string arg = argv[1];
	if (arg == "1")
	{
		src = imread (argv[2], 1);
		cvtColor (src, src_gray, COLOR_BGR2GRAY);
		namedWindow (source_window, WINDOW_AUTOSIZE);
		createTrackbar ("Threshold: ", source_window, &thresh, max_thresh, HarrisCorner);
		imshow (source_window, src);
		HarrisCorner (0, 0);
		waitKey (0);
		return (0);
	}
	else if (arg == "2")
	{
		src = imread (argv[2], 1);
		cvtColor (src, src_gray, COLOR_BGR2GRAY);
		namedWindow (source_window, WINDOW_AUTOSIZE);
		createTrackbar ("Max  corners:", source_window, &maxCorners, maxTrackbar, goodFeaturesToTrack);
		imshow (source_window, src);
		goodFeaturesToTrack (0, 0);
		waitKey (0);
		return (0);
	}
	else
	{
		std::cout << "Wrong Argument!" << std::endl;
		return (-1);
	}
}
