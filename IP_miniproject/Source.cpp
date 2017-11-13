#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;


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

	/*Mat imgSmall = _image;
	resize(imgSmall, imgSmall, Size(5, 5));
	namedWindow("dog small");
	imshow("dog small", imgSmall);
	cout << "Matrix = " << imgSmall << endl;*/
}

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

int main(int, char** argv) {

	Mat image = imread("dog.jpg");
	Mat newImage = readImage(image);
	Mat rotationMatrix = getRotationMatrix2D(Point(250, 250), 45, 0.5);

	namedWindow("dog", CV_WINDOW_AUTOSIZE);
	imshow("dog", newImage);

	warpAffine(newImage, newImage, rotationMatrix, newImage.size());
	namedWindow("dog rotate", CV_WINDOW_AUTOSIZE);
	imshow("dog rotate", newImage);


	waitKey(0);
	return 0;
}