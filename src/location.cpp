#include "my.h"
#include "location.h"

//构造函数
Location::Location(Mat & imgg)
{
	img = imgg;
#ifdef DEBUG
	img_show = img.clone();
#endif
}
//析构函数
Location::~Location()
{

}

//判断point3在point1,point2所在事项的上下方
int judge_point(Point point1, Point point2, Point point)
{
	int k = (point2.y - point1.y) / (point1.x - point2.x);	//y方向与笛卡尔坐标系相反
	int value = k * (point.x - point1.x) + point1.y - point.y;
	if (value > 0)
		return 1;
	else return -1;
}
//找四个点最小包围矩形
Rect Location::judge_area(Point point1, Point point2, Point point3, Point point4)
{
	int xx[4] = { point1.x, point2.x, point3.x, point4.x };
	int yy[4] = { point1.y, point2.y, point3.y, point4.y };
	int x_area[2] = { point1.x, point1.x };
	int y_area[2] = { point2.y, point2.y };
	for (int i = 0; i < 4; i++)
	{
		if (xx[i] < x_area[0]) x_area[0] = xx[i];
		if (xx[i] > x_area[1]) x_area[1] = xx[i];

		if (yy[i] < y_area[0]) y_area[0] = yy[i];
		if (yy[i] > y_area[1]) y_area[1] = yy[i];
	}
	x_area[0] -= 40; if (x_area[0] < 0) x_area[0] = 0;
	x_area[1] += 40; if (x_area[1] > img.cols) x_area[1] = img.cols;
	y_area[0] -= 40; if (y_area[0] < 0) y_area[0] = 0;
	y_area[1] += 40; if (y_area[1] > img.rows) y_area[1] = img.rows;
	return Rect(x_area[0], y_area[0], x_area[1] - x_area[0], y_area[1] - y_area[0]);
}
//获取三个点的坐标
void Location::point_locat()
{
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);		//灰度化
	mythreshold(img_gray, 100);					//二值化
	imshow("二值化", img_gray);

	vector<Rect> rect;
	Mat temp = img_gray.clone();
	vector< vector <Point> > contours;
	vector<Vec4i> hierarchy;				//保存轮廓信息
											//hierarchy[i][0]：第i条轮廓下一条轮廓
											//hierarchy[i][1]：第i条轮廓上一条轮廓
											//hierarchy[i][2]：第i条轮第一条子轮廓
											//hierarchy[i][3]：第i条轮第一条父轮廓
	findContours(temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Rect r0 = boundingRect(Mat(contours[i]));			//最小包围矩形
		RotatedRect r1 = minAreaRect(Mat(contours[i]));		//最小旋转矩形
		if (r0.area() < 100)		//轮廓面积大于100
			continue;

		if (hierarchy[i][2] > 0 && hierarchy[i][3] > 0)  //存在子轮廓 && 存在父轮廓
		{
			int up_number = hierarchy[i][3];		//父轮廓序号
			Rect r2 = boundingRect(Mat(contours[hierarchy[i][3]]));		//父轮廓
			float w = r2.width;
			float h = r2.height;
			float rate = (float)(w / h);
			float rate_area = (float)r2.area() / (float)r0.area();		//父轮廓与子轮廓之比
			if (rate < 1.2 && rate > 0.8 && rate_area < 2)		//长宽比
			{
				rect.push_back(r2);
#ifdef DEBUG
				rectangle(img_show, r2, Scalar(200), 1);
#endif
			}
		}
	}
	cout << "rect.size=" << rect.size() << endl;
	if (rect.size() == 3)
	{
		point1 = Point(rect[0].x + (rect[0].width >> 1), rect[0].y + (rect[0].height >> 1));
		point2 = Point(rect[1].x + (rect[1].width >> 1), rect[1].y + (rect[1].height >> 1));
		point3 = Point(rect[2].x + (rect[2].width >> 1), rect[2].y + (rect[2].height >> 1));
#ifdef DEBUG
		//cout << "point1=" << point1 << " point2" << point2 << " point3" << point3 << endl;
		circle(img_show, point1, 4, Scalar(0, 255, 0), 2);
		circle(img_show, point2, 8, Scalar(0, 255, 0), 2);
		circle(img_show, point3, 12, Scalar(0, 255, 0), 2);
#endif
	}
}
//获取旋转信息
void Location::rotate_info()
{
	if (point1.x != 0 && point1.y != 0)
	{
		//找出三个点所构成的三角形的斜边
		int long1 = abs(point2.x - point3.x) + abs(point2.y - point3.y);
		int long2 = abs(point1.x - point3.x) + abs(point1.y - point3.y);
		int long3 = abs(point1.x - point2.x) + abs(point1.y - point2.y);

		int mark = 0;
		if (long1 > long2)
		{
			if (long2 > long3) mark = 1;	//long1最长，point1为直角点
			else if (long1 > long3) mark = 1;
			else mark = 3;
		}
		else {
			if (long2 < long3) mark = 3;
			else mark = 2;
		}

		switch (mark)
		{
			//point1为直角点
			case 1:point4.x = point2.x + point3.x - point1.x; point4.y = point2.y + point3.y - point1.y;
				center.x = (point2.x + point3.x) >> 1;
				center.y = (point2.y + point3.y) >> 1;

				angle = atan2(center.y - point1.y, point1.x - center.x);//long1最长，point1为直角点
				if (judge_point(point2, point3, point1) < 0)//在直线下方则加180度
				{
					angle += 3.1415926;
				}
#ifdef DEBUG
				cout << "mark=" << mark << endl;
				//cout << "point4=" << point4 << endl;
				//cout << "center=" << center << endl;
				//cout << "angle=" << angle * 180.0 / 3.1415 << endl;
				circle(img_show, point4, 16, Scalar(0, 255, 0), 2);
				circle(img_show, center, 8, Scalar(0, 255, 0), 2);
				line(img_show, center, point1, Scalar(255, 0, 0), 2);
#endif
			break;
			//point2为直角点
			case 2:point4.x = point1.x + point3.x - point2.x; point4.y = point1.y + point3.y - point2.y;
				center.x = (point1.x + point3.x) >> 1;
				center.y = (point1.y + point3.y) >> 1;

				angle = atan2(center.y - point2.y, point2.x - center.x);
				if (judge_point(point1, point3, point2) < 0)
				{
					angle += 3.1415926;
				}
#ifdef DEBUG
				cout << "mark=" << mark << endl;
				//cout << "point4=" << point4 << endl;
				//cout << "center=" << center << endl;
				//cout << "angle=" << angle * 180.0 / 3.1415 << endl;
				circle(img_show, point4, 16, Scalar(0, 255, 0), 2);
				circle(img_show, center, 8, Scalar(0, 255, 0), 2);
				line(img_show, center, point2, Scalar(255, 0, 0), 2);
#endif
			break;
			//point3为直角点
			case 3:point4.x = point1.x + point2.x - point3.x; point4.y = point1.y + point2.y - point3.y;
				center.x = (point1.x + point2.x) >> 1;
				center.y = (point1.y + point2.y) >> 1;

				angle = atan2(center.y - point3.y, point3.x - center.x);
				if (judge_point(point1, point2, point3) < 0)
				{
					angle += 3.1415926;
				}
#ifdef DEBUG
				cout << "mark=" << mark << endl;
				//cout << "point4=" << point4 << endl;
				//cout << "center=" << center << endl;
				//cout << "angle=" << angle * 180.0 / 3.1415 << endl;
				circle(img_show, point4, 16, Scalar(0, 255, 0), 2);
				circle(img_show, center, 8, Scalar(0, 255, 0), 2);
				line(img_show, center, point3, Scalar(255, 0, 0), 2);
#endif
			break;
		}
		Rect rect_show = judge_area(point1, point2, point3, point4);
#ifdef DEBUG
		rectangle(img_show, rect_show, Scalar(0, 0, 255), 2);
#endif
		dstimg = img(rect_show);
		cvtColor(dstimg, dstimg, COLOR_BGR2GRAY);
	}
}
//简单进行旋转
void Location::retate()
{
	if (center.x != 0 && center.y != 0 && angle != 0)
	{
		double scale = 1.0;
		angle *= 57.297;// == angle = angle * 180.0 / 3.1415;
		angle = 135 - angle;

		Point center = Point(dstimg.cols / 2, dstimg.rows / 2);
		Mat rotMat(2, 3, CV_32FC1);
		rotMat = getRotationMatrix2D(center, angle, scale);

		//重新获取图像大小
		Rect bbox = cv::RotatedRect(center, dstimg.size(), angle).boundingRect();
		double *p = rotMat.ptr<double>(0) + 2;
		*p += (bbox.width >> 1) - center.x;
		p = rotMat.ptr<double>(1) + 2;
		*p += (bbox.height >> 1) - center.y;
		//进行旋转
		warpAffine(dstimg, dstimg, rotMat, bbox.size());
	}
}
//执行
void Location::located()
{
	point_locat();
	rotate_info();
	retate();
#ifdef DEBUG
	imshow("location_show", img_show);
#endif
}
//获取输出图像
void Location::get_dstimg(Mat & img)
{
	img = dstimg;
}

