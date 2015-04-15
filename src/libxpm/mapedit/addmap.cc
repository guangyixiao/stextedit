#include "addmap.h"
#include "ui_addmap.h"
#include "StringCovert.h"


namespace xpm {
	//AddMap::AddMap(XpmTermDocControl* xpmControl, QWidget *parent) :QDialog(parent),
	AddMap::AddMap(XpmTableDocControl* xpmControl, QWidget *parent) :QDialog(parent),
		_xpmControl(xpmControl),
		_term(0),
		ui(new Ui::AddMap){
			ui->setupUi(this);
			termLineEdit = new XpmLineEdit(_xpmControl->termControl(), this);
			termLineEdit->setObjectName(QString::fromUtf8("termLineEdit"));			
			connect(termLineEdit->completer(), SIGNAL(activatedTerm(conex::Term*)), this, SLOT(SelectedTerm(conex::Term*)));
            ui->horizontalLayout_5->addWidget(termLineEdit);
			ui->lableSelectedText->setText( StringCovert::stdWToQString(_xpmControl->select().word));
			termLineEdit->setFocus();
			connect(ui->okButton, SIGNAL(clicked()), this, SLOT(AcceptMap()));
	}

	AddMap::~AddMap(){
		delete ui;
	}

	void AddMap::SelectedTerm(conex::Term *term) {
		_term = term;
		QString text(StringCovert::stdWToQString( term->id()->word()));
		ui->labelText->setText(text);
		ui->labelIid->setText(term->id()->iid().c_str());
		ui->labelDefinition->setText(StringCovert::stdWToQString(term->text()));
		termLineEdit->setText(StringCovert::stdWToQString(term->indexWord()));
		ui->okButton->setFocus();
	}

	void AddMap::AcceptMap() {
		sfa_select range = _xpmControl->select();
		if(_term && range.word.length() > 0)
			_xpmControl->add_map(range.word_ix, range.word, _term->id()->iid(), range.block_ix);
	}
}
