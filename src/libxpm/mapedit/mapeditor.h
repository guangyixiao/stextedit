#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QWidget>
#include <QtGui>
#include <string>
//#include "control/XpmTermDocControl.h"
#include "XpmTableDocControl.h"
#include "XpmDocModel.h"

using namespace std;

namespace Ui {
	class MapEditor;
}

namespace xpm {
	class XpmDocModel;
	class MapEditor : public QWidget
	{
		Q_OBJECT

	public:
		//explicit MapEditor(XpmTermDocControl* _xpmControl, QWidget *parent = 0);
		explicit MapEditor(XpmTableDocControl* _xpmControl, QWidget *parent = 0);
		~MapEditor();

		//interface for button enable
		void setAddMapEnable(bool enable);
	protected:
		void showEvent(QShowEvent* event);
    signals:
		void highlightSelection(int pos, int length);
		void disHighlightSelection(int pos, int length);
		void selectMapRange(int pos, int length);
	public slots:
		void viewMap();
		void findMapByName();
		void viewDetailMap();
		void addMap();
		void removeMap();
		//a slot for selectionChangedSlot with treeview
		void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);

	private:
		sfa_select_map _range;
		XpmDocModel* model;
		Ui::MapEditor *ui;
		//XpmTermDocControl* _xpmControl;
		XpmTableDocControl* _xpmControl;
	};
}

#endif // MAPEDITOR_H
