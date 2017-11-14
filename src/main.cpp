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
	Mat temp = img.clone();
	//location(img, temp);

	float angle = 0;
	//hierarchy[i][0]：第i条轮廓下一条轮廓
	//hierarchy[i][1]：第i条轮廓上一条轮廓
	//hierarchy[i][2]：第i条轮第一条子轮廓
	//hierarchy[i][3]：第i条轮第一条父轮廓
	vector<vector <Point> > contours;
	vector<Vec4i> hierarchy;				//保存轮廓信息
	findContours(temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Rect r0 = boundingRect(Mat(contours[i]));			//最小包围矩形
		RotatedRect r1 = minAreaRect(Mat(contours[i]));	//最小旋转矩形

		if (r0.area() > 100 && hierarchy[i][2] > 0 && hierarchy[i][3] > 0)  //轮廓面积大于100 && 存在子轮廓 && 存在父轮廓
		{
			int up_number = hierarchy[i][3];		//父轮廓序号
			Rect r2 = boundingRect(Mat(contours[hierarchy[i][3]]));		//父轮廓
			float w = r2.width;
			float h = r2.height;
			float rate = (float)(w / h);
			if (rate < 1.2 && rate > 0.8)		//长宽比
			{
				//rectangle(img_show, r2, Scalar(0, 0, 255), 2);
				drawContours(img_show, contours, up_number, Scalar(0, 0, 255), 2);	//第三个参数为轮廓的序号
				angle = r1.angle;
			}

		}
	}
#ifdef DEBUG
	cout << "轮廓总数为 " << contours.size() << endl;
#endif
	//简单进行图像旋转
	Point center = Point(img.cols / 2, img.rows / 2);
	double scale = 1.0;
	Mat rotMat(2, 3, CV_32FC1);
	rotMat = getRotationMatrix2D(center, angle, scale);
	warpAffine(img_show, img_show, rotMat, img_show.size());

	imshow("img_show", img_show);
	waitKey(0);
	return 0;
}