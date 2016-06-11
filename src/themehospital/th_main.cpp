#include "th_predefine.h"


int main(int argc, char *argv[])
{
	int ret = 0;
	QCoreApplication a(argc, argv);

	QString error_tips;
	QString allinone_path = "E:/Courage/bullfrog.git/example/001";

	ret = Populous_Read_Config::instance()->init_read_all2(allinone_path,
													 error_tips);
	if (ret != 0)
	{
		return -1;
	}

	return a.exec();
}
