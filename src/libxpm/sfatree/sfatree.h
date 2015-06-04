#ifndef SDFTREE_H
#define SDFTREE_H

#include <QWidget>
#include <QtGui>
#include <QDialog>
#include <string>
#include "sfa_sign.h"
#include "SignTreeModel.h"

using namespace std;

namespace Ui {
	class SFATreeView;
}

namespace xpm {
	class SFATree : public QDialog
	{
		Q_OBJECT

	public:
		explicit SFATree(sfa_sign* root, QWidget *parent = 0);
		~SFATree();
		sfa_sign* root();
		void setRoot(sfa_sign* root);
		public slots:
	private:
		Ui::SFATreeView *ui;
		sfa_sign* _root;
		SignTreeModel* _model;
	};
	inline sfa_sign* SFATree::root() {
		return _root;
	}
}
#endif // MAPEDITOR_H
