#include "conexide.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(conexmain);
	QApplication a(argc, argv);
	//ConexMain w;
	ConexIde w;
	int width = w.frameGeometry().width();
	int height = w.frameGeometry().height();
	
	QDesktopWidget wid;
	int scr_w = wid.screen()->width();
	int scr_h = wid.screen()->height();

	w.setGeometry(scr_w / 2 - width / 2, scr_h / 2 - height / 2, width, height);
	
	w.show();
	return a.exec();
}
