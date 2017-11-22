//#pragma once 确保头文件只被编译一次的一种方法
#ifndef _LOCATION_H_
#define _LOCATION_H_

#include "my.h"

class Location
{
private:
	Mat img;		//类内处理图像
	Mat img_show;	//显示结果
	Mat dstimg;		//输出图像

	Point point1;		//二维码定位点1
	Point point2;		//二维码定位点2
	Point point3;		//二维码定位点3
	Point point4;		//二维码定位点4(求得的点)

	Point center;	//二维码旋转中心
	float angle;	//二维码旋转角度

	int judge_point(Point point1, Point point2, Point point);
	void point_locat();		//定位二维码四个点
	void rotate_info();		//得到旋转信息
	void retate();			//进行旋转
public:
	Location(Mat &imgg);//构造函数
	~Location();		//析构函数

	void located();		//执行
	void get_dstimg(Mat &img);	//得到结果
};

#endif
