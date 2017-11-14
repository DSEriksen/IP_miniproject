#include <iostream>
#include <opencv2\opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

# define PI 3.14159265358979323846

bool detectKey(int ms) {
	int key = waitKey(ms);
	if (key == 32) {
		return true;
	}
	else if (key == 8) {
		return true;
	}
	return false;  
}

void basicDisPrint(Mat _image) {

	cvtColor(_image, _image, COLOR_BGR2GRAY);
	namedWindow("dog");

	imshow("dog", _image);
	cout << "rows: " << _image.rows << endl;
	cout << "cols: " << _image.cols << endl;
	cout << "pixels: " << _image.rows * _image.cols << endl;
	cout << "adkaspdk: " << _image.at<Point>(Point(5, 3));

	//opencv scaling
	/*Mat imgSmall = _image;
	resize(imgSmall, imgSmall, Size(5, 5));
	namedWindow("dog small");
	imshow("dog small", imgSmall);
	cout << "Matrix = " << imgSmall << endl;*/
}


//reading an image (thats already been read through imread) and copying each pixel to a new mat
Mat readImage(Mat _image) {

	cvtColor(_image, _image, COLOR_BGR2GRAY);
	Mat image_forl = Mat::zeros(Size(_image.rows, _image.cols), _image.type());

	for (int y = 0; y < _image.rows; y++) {
		for (int x = 0; x < _image.cols; x++) {

				int temp = _image.at<uchar>(x, y);
				image_forl.at<uchar>(x, y) = _image.at<uchar>(x, y);
		}
	}
	return image_forl;
}

//rotation matrix creation with angle decimals
Mat createRotationMatrix(double theta, double x, double y, double _scale) {
	double angle = theta * PI / 180;
	int cenX = x;
	int cenY = y;
	double scale = _scale;
	double alpha = scale*cos(angle);
	double beta = scale*sin(angle);

	
	Mat rotMat = Mat(2, 3, CV_64FC1);
	rotMat.at<double>(0,0) = alpha;
	rotMat.at<double>(0,1) = beta;
	rotMat.at<double>(0,2) = ((1 - alpha)*cenX) - (beta*cenY);
	rotMat.at<double>(1,0) = (beta*(-1));
	rotMat.at<double>(1,1) = alpha;
	rotMat.at<double>(1,2) = (beta*cenX) + ((1 - alpha)*cenY);
	
/*
	Mat rotMat = Mat(2, 2, CV_64FC1);
	rotMat.at<double>(0, 0) = cos(angle);
	rotMat.at<double>(0, 1) = sin(angle) *(-1);
	rotMat.at<double>(1, 0) = sin(angle);
	rotMat.at<double>(1, 1) = cos(angle);
*/
	return rotMat;
}

Mat rotate(Mat _mat, Mat _rotMat) {

	Mat dst = Mat::zeros(Size(_mat.rows, _mat.cols), _mat.type());
	_mat.copyTo(dst);
	dst.convertTo(dst, CV_64FC1);

	Mat rotMat = Mat::zeros(Size(_rotMat.rows, _rotMat.cols), _rotMat.type());
	_rotMat.copyTo(rotMat);

	double p1;
	double p2;
	double Rx = (rotMat.at<double>(0, 0)) + (rotMat.at<double>(0, 1)) + (rotMat.at<double>(0, 2));
	double Ry = (rotMat.at<double>(1, 0)) - (rotMat.at<double>(1, 1)) - (rotMat.at<double>(1, 2));
	cout << "rotate() rotMat = " << endl << rotMat;
	cout << endl << "Rx = " << Rx;
	cout << endl << "Ry = " << Ry;

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < dst.cols; x++){

			p1 = dst.at<double>(x, y);
			p2 = dst.at<double>((int)Rx, (int)Ry);
		}
	}




	//find point 1
	//find point 2
	//tempsave point 1
	//set point 1 to point 2 
	//set point 2 to tempsave 



	
/*
	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++) {
			p1 = dst.at<uchar>(x, y);
			p2 = dst.at<uchar>(
								(_rotMat.at<double>(0,0)*x) + (_rotMat.at<double>(0,1)*y) - //x * cos(a) + sin(a)


			//find point2
			temp = dst.at<uchar>(

				)

		}
	}
*/


	return dst;
}

int main(int, char** argv) {

	Mat image = imread("dog.jpg");
	Mat newImage = readImage(image);

	//cheat
	//Mat rotationMatrix = getRotationMatrix2D(Point(250, 250), 45, 0.7);

	//custom
	Mat rotMat = createRotationMatrix(20, newImage.rows/4,newImage.cols/2, 1);

	namedWindow("dog", CV_WINDOW_AUTOSIZE);
	imshow("dog", newImage);

	//cheat
	//warpAffine(newImage, newImage, rotMat, newImage.size());

	//custom
	Mat rotatedMat = rotate(newImage, rotMat);


	//rotated display
	namedWindow("dog rotate", CV_WINDOW_AUTOSIZE);
	imshow("dog rotate", newImage);
	

	waitKey(0);
	return 0;
}