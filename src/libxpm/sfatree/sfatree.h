#ifndef SDFTREE_H
#define SDFTREE_H

#include <QWidget>
#include <QtGui>
#include <string>
//#include "control/XpmTermDocControl.h"
#include "control/XpmJsonDocControl.h"
#include "model/sdf/SDFSign.h"
#include "viewmodel/SignTreeModel.h"

using namespace std;

namespace Ui {
	class SDFTreeView;
}

namespace xpm {
	class SDFTree : public QDialog
	{
		Q_OBJECT

	public:
		//explicit AddMap(XpmTermDocControl* _xpmControl, QWidget *parent = 0);
		explicit SDFTree(SDFSign* root, QWidget *parent = 0);
		~SDFTree();
		SDFSign* root();
		void setRoot(SDFSign* root);
	public slots:
	private:
		Ui::SDFTreeView *ui;
		SDFSign* _root;
		SignTreeModel* _model;
		//XpmTermDocControl* _xpmControl;
	};
	inline SDFSign* SDFTree::root() {
		return _root;
	}	
}

#endif // MAPEDITOR_H
