#include "removemap.h"
#include "ui_removemap.h"
#include "StringCovert.h"


namespace xpm {
	//RemoveMap::RemoveMap(XpmTermDocControl* xpmControl, SelectionRange *range, QWidget *parent) :QDialog(parent),
	RemoveMap::RemoveMap(XpmTableDocControl* xpmControl, sfa_map_select *range, QWidget *parent) :QDialog(parent),
		_range(range),
		_xpmControl(xpmControl),
		ui(new Ui::RemoveMap){
			ui->setupUi(this);
			connect(ui->okButton, SIGNAL(clicked()), this, SLOT(RemoveMapAccept()));
			if(range) {
				ui->lableSelectedText->setText(StringCovert::stdWToQString(range->word));
				FindTermByRange();
			}
	}

	RemoveMap::~RemoveMap(){
		delete ui;
	}

	void RemoveMap::FindTermByRange() {
		//_xpmControl->FindByRange(_range, block, term);
		string iid = _range->id;
		conex::Term* term = _xpmControl->termControl()->GetTermByIid(iid);
		if(term) {
			ui->labelText->setText(StringCovert::stdWToQString(term->id()->word()));
			ui->labelIid->setText(iid.c_str());
			ui->labelDefinition->setText(StringCovert::stdWToQString(term->text()));
		}
	}

	void RemoveMap::RemoveMapAccept() {
		cout << "block_ix :" << _range->block_ix << ", word_ix" << _range->word_ix << endl;
		_xpmControl->remove_map(_range->word_ix, _range->block_ix);
		accept();
	}

	void RemoveMap::RemoveMapCancel() {
		reject();
	}

}
