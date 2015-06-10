#ifndef ADD_MAP_H
#define ADD_MAP_H

#include <QWidget>
#include <QDialog>
#include <QtGui>
#include <string>
//#include "control/XpmTermDocControl.h"
#include "XpmTableDocControl.h"
#include "XpmLineEdit.h"

using namespace std;

namespace Ui {
	class AddMap;
}

namespace xpm {
	class AddMap : public QDialog
	{
		Q_OBJECT

	public:
		//explicit AddMap(XpmTermDocControl* _xpmControl, QWidget *parent = 0);
		explicit AddMap(XpmTableDocControl* _xpmControl, sfa_map_select* _select, QWidget *parent = 0);
		~AddMap();
		conex::Term* term();
	public slots:
		void SelectedTerm(conex::Term* term);
		void AcceptMap();
	private:
		XpmLineEdit *termLineEdit;
		Ui::AddMap *ui;
		//XpmTermDocControl* _xpmControl;
		XpmTableDocControl* _xpmControl;
		conex::Term*  _term;
		sfa_map_select* _select;
	};

	inline conex::Term *AddMap::term() {
		return _term;
	}
}

#endif // MAPEDITOR_H
