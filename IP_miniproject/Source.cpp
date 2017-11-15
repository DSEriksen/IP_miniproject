#include <iostream>
#include <opencv2\opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

# define PI 3.14159265358979323846

int interpolate(Mat _mat, double _x, double _y) {
	int Vx = (int)round(_x);
	int Vy = (int)round(_y);

	if (Vx >= 0 && Vx < _mat.cols && Vy >= 0 && Vy < _mat.rows) {
		return (int)_mat.at<uchar>(Vx, Vy);
	}
	else {
		return -1;
	}
}

Vec3d multiplyVecMat(Vec3d _V, Mat _mat) {

	Vec3d vec = _V;

	for (int i = 0; i < _V.rows; i++) {
		//cout << "current vec-iteration = [" << i << "]" << endl;

		double sum = 0;
		for (int j = 0; j < _mat.cols; j++) {
			//cout << "current mat-iteration = [" << j << "]" << endl;
			//cout << "multiplying [" << _mat.at<double>(i, j) << "] with [" << vec[j] << "]" << endl;

			sum += _mat.at<double>(i, j) * vec[j];
		}
		//cout << "vec iteration [" << i << "] sum is currently = [" << sum << "]" << endl << endl;
		vec[i] = sum;
	}
	return vec;
}

Mat transform(double _dx, double _dy) {
	Mat transMat = Mat(3, 3, CV_64FC1);

	return transMat;
}

Mat createNewRotMat(Mat _mat, double _rx, double _ry, double alpha, double beta, double theta) {
	Mat newRotMat = Mat::zeros(_mat.rows, _mat.cols, _mat.type());
	_mat.copyTo(newRotMat);
	
	

	return newRotMat;
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

	/*
	Mat rotMat = Mat(2, 3, CV_64FC1);
	rotMat.at<double>(0,0) = alpha;
	rotMat.at<double>(0,1) = beta;
	rotMat.at<double>(0,2) = ((1 - alpha)*cenX) - (beta*cenY);
	rotMat.at<double>(1,0) = (beta*(-1));
	rotMat.at<double>(1,1) = alpha;
	rotMat.at<double>(1,2) = (beta*cenX) + ((1 - alpha)*cenY);
	*/

	Mat rotMat = Mat(3, 3, CV_64FC1);
	//row1
	rotMat.at<double>(0, 0) = cos(angle);
	rotMat.at<double>(0, 1) = sin(angle);
	rotMat.at<double>(0, 2) = 0;
	//row2
	rotMat.at<double>(1, 0) = sin(angle)*(-1);
	rotMat.at<double>(1, 1) = cos(angle);
	rotMat.at<double>(1, 2) = 0;
	//row3
	rotMat.at<double>(2, 0) = 0;
	rotMat.at<double>(2, 1) = 0;
	rotMat.at<double>(2, 2) = 1;


	return rotMat;
}

Mat rotate(Mat _mat, Mat _rotMat) {

	namedWindow("dst");

	Mat dst = Mat::zeros(Size(_mat.rows, _mat.cols), _mat.type());
	_mat.copyTo(dst);
	//dst.convertTo(dst, CV_64FC1);

	imshow("dst", dst);

	Mat rotMat = Mat::zeros(Size(_rotMat.rows, _rotMat.cols), _rotMat.type());
	_rotMat.copyTo(rotMat);


	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++){

			//1. create unit vector
				//create vector (1 dim array) which consists of the rotation matrix(so far the 2x2) multiplied into x,y and 1

			Vec3d V = Vec3d(x, y, 1);
			Vec3d Vm = multiplyVecMat(V, rotMat);

			//2. create pixel value
				//create a pixel int, which is defined by the interpolation of the dst position in matrix, and the values of the vector above

			int pixel = interpolate(dst, Vm[0], Vm[1]);
			//cout << pixel << endl << endl;

			//3. set the current position in the destination array to the current pixel value.
				//set the pixelvalue in the output array (dst)
			//cout << dst;

			if (pixel > -1) {
				dst.at<uchar>(x, y) = pixel;	
			}
			else {
				dst.at<uchar>(x, y) = 0;
			}
			
			
		}
		
	}
	//cout << dst;


	//namedWindow("dst2");
	//imshow("dst2", dst);
	return dst;
}



int main(int, char** argv) {

	Mat image = imread("dog.jpg");
	Mat newImage = readImage(image);

	//cheat
	Mat rotationMatrix = getRotationMatrix2D(Point(250, 250), 45, 1);

	//custom
	Mat rotMat = createRotationMatrix(1, newImage.rows/2,newImage.cols/2, 1);

	cout << "Rotation Matrix = " << rotMat << endl;


/*	Print multiply test
	Vec3d V = Vec3d(4, 4, 1);
	cout << "Vector org = " << V << endl;
	Vec3d Vm = multiplyVecMat(V, rotMat);
	cout << "Vector mult = " << Vm << endl;
*/

	namedWindow("dog", CV_WINDOW_AUTOSIZE);
	imshow("dog", newImage);

	//cheat
	//warpAffine(newImage, newImage, rotationMatrix, newImage.size());

	//custom
	Mat rotatedImage = rotate(newImage, rotMat);


	//rotated display
	namedWindow("dog rotate", CV_WINDOW_AUTOSIZE);
	imshow("dog rotate", rotatedImage);
	

	waitKey(0);
	return 0;
}