#include "my.h"
#include "location.h"
#include "recognize.h"

int main()
{
	Mat img = imread("二维码正.jpg");
	Mat img_show = img.clone();
	cvtColor(img, img, COLOR_BGR2GRAY);		//灰度化
	imshow("test1", img);

	mythreshold(img, 100);					//二值化
	imshow("二值化", img);

	Mat dstimg;
	rotate(img, dstimg);
	imshow("dstimg", img);




//#ifdef DEBUG
//	cout << "轮廓总数为 " << contours.size() << endl;
//#endif
	////简单进行图像旋转
	//Point center = Point(img.cols / 2, img.rows / 2);
	//double scale = 1.0;
	//Mat rotMat(2, 3, CV_32FC1);
	//rotMat = getRotationMatrix2D(center, angle, scale);
	//warpAffine(img_show, img_show, rotMat, img_show.size());

	//imshow("img_show", img_show);
	waitKey(0);
	return 0;
}