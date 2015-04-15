#include "conexide.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(conexmain);
	QApplication a(argc, argv);
	//ConexMain w;
	ConexIde w;
	w.show();
	return a.exec();
}
