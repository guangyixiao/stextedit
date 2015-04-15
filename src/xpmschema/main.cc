#include "xpmschema.h"
#include <QApplication>
#include <iostream>
#include <string>

using namespace xpm;
using namespace std;

int main( int argc, char ** argv )
{
    Q_INIT_RESOURCE(textedit);

    QApplication a( argc, argv );

	cout.imbue(locale("chs"));
	wcout.imbue(locale("chs"));
	/*TextEdit mw;
    mw.resize( 800, 800 );
    mw.show();*/
	XpmSchema xpms;
	xpms.resize(1200, 800);
	xpms.show();
    return a.exec();
}
