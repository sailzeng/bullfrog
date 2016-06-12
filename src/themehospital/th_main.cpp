#include "th_predefine.h"

int main(int argc, char *argv[])
{
	int ret = 0;
	QCoreApplication a(argc, argv);

	BOOL bret = ::SetConsoleOutputCP(CP_UTF8);
	if (bret == FALSE)
	{
	    return -1;
	}

	HANDLE handle_out = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle_out == INVALID_HANDLE_VALUE)
	{
	    return -1;
	}
	//设置屏幕缓冲区和输出屏幕大小
	COORD coord = { 161, 481 };
	bret = ::SetConsoleScreenBufferSize(handle_out, coord);
	if (bret == FALSE)
	{
	    return -1;
	}
	SMALL_RECT rect = { 0, 0, 160, 48 };
	bret = ::SetConsoleWindowInfo(handle_out, TRUE, &rect);
	if (bret == FALSE)
	{
	    return -1;
	}

	QString error_tips;
	QString allinone_path = "E:/Courage/bullfrog.git/example/001";

	ret = Populous_Read_Config::instance()->init_read_all2(allinone_path,
													       error_tips);
	if (ret != 0)
	{
		return -1;
	}

	ret = Populous_Read_Config::instance()->read_excel(error_tips);
	if (ret != 0)
	{
		return -1;
	}

	return a.exec();
}

