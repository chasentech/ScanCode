#include "my.h"
#include "location.h"
#include "recognize.h"

int main()
{
	Mat img = imread("二维码正.jpg");
	resize(img, img, Size(648, 419));//缩小图像，方便处理

	//简单进行图像旋转，进行调试
	Point center = Point(img.cols / 2, img.rows / 2);
	double scale = 1.0;
	Mat rotMat(2, 3, CV_32FC1);
	rotMat = getRotationMatrix2D(center, 180, scale);
	warpAffine(img, img, rotMat, img.size());

	Mat img_show = img.clone();

	//开始计时
	double time_;
	time_ = static_cast<double>(getTickCount());

	cvtColor(img, img, COLOR_BGR2GRAY);		//灰度化
	imshow("test1", img);

	mythreshold(img, 100);					//二值化
	imshow("二值化", img);

	Location location(img);//类实例化
	location.located();//调用函数进行处理

	//输出时间
	time_ = (static_cast<double>(getTickCount()) - time_) / getTickFrequency();
	double FPS = 1 / time_;
	cout << "time = " << time_ * 1000 << "ms" << endl;


	imshow("img_show", img_show);
	waitKey(0);
	return 0;
}