#include "sfatree.h"
#include "ui_sfatree.h"

namespace xpm {
	//AddMap::AddMap(XpmTermDocControl* xpmControl, QWidget *parent) :QDialog(parent),
	SFATree::SFATree(conex::TermControl* control, sfa_sign* root, QWidget *parent) :QDialog(parent),
		_control(control),
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
			QItemSelectionModel *selectionModel = ui->tvSDFTree->selectionModel();
			connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
				this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

		}
	}

	void SFATree::selectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection) {
		const QModelIndex index = ui->tvSDFTree->selectionModel()->currentIndex();
		if (!index.isValid())
			return;

		sfa_sign *s = static_cast<sfa_sign*>(index.internalPointer());

		if (_control) {
			wstring name = s->name_str() + L"\r\n";
			for (sfa_sign::name_iterator i = s->name_signs_begin(); i != s->name_signs_end(); ++i){
				name += i->word + L':' + _control->GetTermByIid(i->id)->text() + L"\r\n";
			}
			ui->teName->setText(StringCovert::stdWToQString(name));
			wstring value = s->value_as_string() + L"\r\n";
			if (s->type() == SFA_STRING){
				for (sfa_sign::value_iterator i = s->value_signs_begin(); i != s->value_signs_end(); ++i){
					value += i->word + L':' + _control->GetTermByIid(i->id)->text() + L"\r\n";
				}
			}
			ui->teValue->setText(StringCovert::stdWToQString(value));
		}
	}
}

