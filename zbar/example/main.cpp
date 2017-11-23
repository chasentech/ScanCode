#include <zbar.h>
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
//using namespace zbar;
using namespace cv;
int main()
{
	Mat img = imread("code.jpg");
	if (img.empty())
	{
		cout << "img load failed" << endl;
		return 0;
	}
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	//图片转换为zbar中Image
	int width = img_gray.cols;
	int height = img_gray.rows;
	uchar *raw = (uchar *)img_gray.data;
	zbar::Image imageZbar(width, height, "Y800", raw, width * height);

	//进行扫描
	zbar::ImageScanner scanner;
	scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
	scanner.scan(imageZbar);
	zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();

	//输出结果
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
		cout << "ScanCode is failed" << endl;
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
		cout << "条码：" << endl << symbol->get_data() << endl << endl;
	}
	imageZbar.set_data(NULL, 0);	//释放
	imshow("img", img)
		;
	waitKey(0);
	return 0;
}


//int main(int argc, char*argv[])
//{
//	zbar::ImageScanner scanner;
//	scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
//	Mat image = imread("code2.jpg");
//	if (!image.data)
//	{
//		cout << "请确认图片" << endl;
//		system("pause");
//		return 0;
//	}
//	Mat imageGray;
//	cvtColor(image, imageGray, CV_RGB2GRAY);
//	int width = imageGray.cols;
//	int height = imageGray.rows;
//	uchar *raw = (uchar *)imageGray.data;
//	zbar::Image imageZbar(width, height, "Y800", raw, width * height);
//	scanner.scan(imageZbar); //扫描条码    
//	zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
//
//
//	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
//	{
//		cout << "查询条码失败，请检查图片！" << endl;
//	}
//	for (; symbol != imageZbar.symbol_end(); ++symbol)
//	{
//		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
//		cout << "条码：" << endl << symbol->get_data() << endl << endl;
//	}
//	imshow("Source Image", image);
//	waitKey();
//	imageZbar.set_data(NULL, 0);
//	return 0;
//}
