#ifndef REMOVE_MAP_H
#define REMOVE_MAP_H

#include <QtGui>
#include <QDialog>
#include <string>
//#include "control/XpmTermDocControl.h"
#include "XpmTableDocControl.h"
#include "XpmDocModel.h"

using namespace std;

namespace Ui {
	class RemoveMap;
}

namespace xpm {
	class RemoveMap : public QDialog
	{
		Q_OBJECT

	public:
		//explicit RemoveMap(XpmTermDocControl* _xpmControl, SelectionRange* range, QWidget *parent = 0);
		explicit RemoveMap(XpmTableDocControl* _xpmControl, sfa_map_select* range, QWidget *parent = 0);
		~RemoveMap();

		void FindTermByRange();
	public slots:
	    void RemoveMapAccept();
		void RemoveMapCancel();
	private:
		Ui::RemoveMap *ui;
		sfa_map_select *_range;
		//XpmTermDocControl* _xpmControl;
		XpmTableDocControl* _xpmControl;
	};
}

#endif // MAPEDITOR_H
