#include "sfatree.h"
#include "ui_sfatree.h"


namespace xpm {
	//AddMap::AddMap(XpmTermDocControl* xpmControl, QWidget *parent) :QDialog(parent),
	SDFTree::SDFTree(SDFSign* root, QWidget *parent) :QDialog(parent),
		_model(0),
		ui(new Ui::SDFTreeView){
			ui->setupUi(this);
			setRoot(root);
	}

	SDFTree::~SDFTree(){
		delete ui;
		if(_model != 0) {
			delete _model;
			_model = 0;
		}
	}

	void SDFTree::setRoot(SDFSign* root){ 
		_root = root;
		if(_model != 0) {
			delete _model;
			_model = 0;
		}
		if(root != 0 && !root->empty()) {
			_model = new SignTreeModel(this, root);
			ui->tvSDFTree->setModel(_model);
			QModelIndex index;
			//ui->tvSDFTree->expand(index);
			ui->tvSDFTree->expandAll();
			for (int column = 0; column < Column_Size; ++column)
				ui->tvSDFTree->resizeColumnToContents(column);
			
		}
	}

}
