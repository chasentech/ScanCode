#include "my.h"
#include <zbar.h>

void recognized(Mat &img)
{
	//图片转换为zbar中Image
	int width = img.cols;
	int height = img.rows;
	uchar *raw = (uchar *)img.data;
	zbar::Image imageZbar(width, height, "Y800", raw, width * height);

	//进行扫描
	zbar::ImageScanner scanner;
	scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
	scanner.scan(imageZbar);
	zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();

	//输出结果
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
		cout << "    ScanCode is failed" << endl;
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "    类型：" << symbol->get_type_name() << "条码：" << symbol->get_data() << endl;
	}
	imageZbar.set_data(NULL, 0);	//释放
	//imshow("img", img);
}