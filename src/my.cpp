#include "my.h"

//二值化
void mythreshold(Mat &img, uchar T)
{
	int n1 = img.rows;
	int nc = img.cols * img.channels();
	if (img.isContinuous())//判断图像是否连续
	{
		nc = nc * n1;
		n1 = 1;
	}
	for (int i = 0; i < n1; i++)
	{
		uchar *p = img.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			if (p[j] > T)
				p[j] = 0;
			else p[j] = 255;
		}
	}
}
