#include "sfatree.h"
#include "ui_sfatree.h"

namespace xpm {
	//AddMap::AddMap(XpmTermDocControl* xpmControl, QWidget *parent) :QDialog(parent),
	SFATree::SFATree(sfa_sign* root, QWidget *parent) :QDialog(parent),
		_model(0),
		ui(new Ui::SFATreeView){
		ui->setupUi(this);
		setRoot(root);
	}

	SFATree::~SFATree(){
		delete ui;
		if (_model != 0) {
			delete _model;
			_model = 0;
		}
	}

	void SFATree::setRoot(sfa_sign* root){
		_root = root;
		if (_model != 0) {
			delete _model;
			_model = 0;
		}
		if (root != 0 && !root->empty()) {
			_model = new SignTreeModel(this, root);
			ui->tvSDFTree->setModel(_model);
			QModelIndex index;
			//ui->tvSFATree->expand(index);
			ui->tvSDFTree->expandAll();
			for (int column = 0; column < Column_Size; ++column)
				ui->tvSDFTree->resizeColumnToContents(column);

		}
	}
}

