#include "my.h"

bool location(Mat &srcimg, vector<Rect> &rect)
{
	Mat temp = srcimg.clone();
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
				rect.push_back(r2);
			}
		}
	}
	if (rect.size() == 3)
		return true;
	else return false;
}
void rotate(Mat &srcimg, Mat &dstimg)
{
	//dstimg = srcimg;
	vector<Rect> rect;
	bool flag = location(srcimg, rect);
	if (flag == true)
	{
		Point point1 = Point(rect[0].x + (rect[0].width >> 1), rect[0].y + (rect[0].height >> 1));
		Point point2 = Point(rect[1].x + (rect[1].width >> 1), rect[1].y + (rect[1].height >> 1));
		Point point3 = Point(rect[2].x + (rect[2].width >> 1), rect[2].y + (rect[2].height >> 1));

		int long1 = abs(point2.x - point3.x) + abs(point2.y - point3.y);
		int long2 = abs(point1.x - point3.x) + abs(point1.y - point3.y);
		int long3 = abs(point1.x - point2.x) + abs(point1.y - point2.y);

		int mark = 0;
		if (long1 > long2)
		{
			if (long2 > long3) mark = 1;
			else if (long1 > long3) mark = 1;
			else mark = 3;
		}
		else {
			if (long2 < long3) mark = 3;
			else mark = 2;
		}

		cout << "mark=" << mark << endl;
		float rate = 0.0;
		switch (mark)
		{
		case 1:rate = (float)(point2.y - point1.y) / (float)(point2.x - point1.x); break;
		case 2:rate = (float)(point3.y - point2.y) / (float)(point3.x - point2.x); break;
		case 3:rate = (float)(point1.y - point3.y) / (float)(point1.x - point3.x); break;
		}
		cout << "rate=" << rate << endl;


	}
}