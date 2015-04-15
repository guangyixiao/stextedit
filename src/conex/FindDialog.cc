#include "FindDialog.h"
#include <QtGui>
#include <map>
#include <string>
#include "control/VocabularyControl.h"
#include "control/VocabularyXmlControl.h"
#include "model/Term.h"

FindDialog::FindDialog(QWidget *parent)
	: QDialog(parent), 
	_voc(NULL), 
	_model(NULL),
	_currentTerm(NULL),
	_pagesize(30),
	_termControl(0)
{
	ui.setupUi(this);
	_pagesize = ui.pageSizeComboBox->currentText().toInt();	
	//
	conex::VocabularyXmlControl _vocControl(conex::VocabularyControl::XML_SAX2);
	//connect(ui.actionOpenVocFile, SIGAINL(trigged()), 
	
	//term maangement
	connect(ui.tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(ViewClicked(const QModelIndex&)));	
	//find terms
	connect(ui.findButton, SIGNAL(clicked()), this, SLOT(FindTerms()));
	connect(ui.findLineEdit, SIGNAL(returnPressed ()), this, SLOT(FindTerms()));
	//bind pageing
	connect(ui.firstPageButton, SIGNAL(clicked()), this, SLOT(FirstPage()));
	connect(ui.nextButton, SIGNAL(clicked()), this, SLOT(NextPage()));
	connect(ui.previewButton, SIGNAL(clicked()), this, SLOT(PreviewPage()));
	connect(ui.lastButton, SIGNAL(clicked()), this, SLOT(LastPage()));
	connect(ui.pageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(GetPage(int)));
	connect(ui.pageSizeComboBox, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(SetPagesize(const QString & )));
}

FindDialog::~FindDialog()
{
	SAFE_DELETE(_model);
}

void FindDialog::SetVocabularyAndTermControl(conex::Vocabulary* voc, conex::TermControl* termControl) {
	SAFE_DELETE(_model);
	_voc = voc;
	_termControl = termControl;
}

void FindDialog::ViewClicked(const QModelIndex& index) {
	int row = index.row();
	string id(ui.tableView->model()->data(ui.tableView->model()->index(row, 3)).toByteArray().constData());
	_currentTerm = _termControl->GetTermByIid(id);
}

conex::Term* FindDialog::GetSelectedTerm() {
	return _currentTerm;
}

void FindDialog::FindTerms() {
	QString text = ui.findLineEdit->text();
	
	SAFE_DELETE(_model);
	_model = new conex::VocabularyTableModel(this);
	vector<conex::Term*> terms;
	int pagecount = 0;
	if(!text.isEmpty()) {		
		wstring prefix(qToStdWString( text));
		_termControl->PrefixSearch(prefix, terms);		
		
	}else {
		vector<conex::Term*> terms;
		_voc->GetTopkTerms(0, terms);
	}
	pagecount = _model->LoadData(terms);
	
	SetPageCount(pagecount);
	ui.tableView->setModel(_model);
	ui.tableView->resizeColumnsToContents();
	ui.findLineEdit->setFocus();
	QModelIndex index = _model->index(0,0);
	ui.tableView->setCurrentIndex(index);
}

//term paging
void FindDialog::FirstPage() {
	if(_model) {
		_model->FirstPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void FindDialog::NextPage() {
	if(_model) {
		_model->NextPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void FindDialog::PreviewPage() {
	if(_model) {
		_model->PreviewPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void FindDialog::LastPage() {
	if(_model) {
		_model->LastPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void FindDialog::SetPageCount(int pagecount) {
	QStringList items;
	for(int i = 0; i < pagecount; i++) {
		int page = i+1;
		QString page_str = QString::number(page, 10);
		items << page_str;
	}
	ui.pageComboBox->setInsertPolicy(QComboBox::InsertAtBottom);
	ui.pageComboBox->addItems(items);
	ui.pageComboBox->setCurrentIndex(0);
}
void FindDialog::GetPage(int index) {
	if(_model) {
		int page = index + 1;
		_model->GetPage(page);
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
	}
}
void FindDialog::SetPagesize(const QString & text) {
	if(_model) {
		_pagesize = text.toInt();
		int pagecount = _model->SetPageSize(_pagesize);
		SetPageCount(pagecount);
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
	}
}

void FindDialog::ChangePageIndex() {
	if(_model) {
		int currentpage = _model->GetCurrentPage();
		ui.pageComboBox->setCurrentIndex(currentpage -1);
	}
}

/*! Convert a QString to an std::wstring */
std::wstring FindDialog::qToStdWString(const QString &str)
{
#ifdef _MSC_VER
	return std::wstring((const wchar_t *)str.utf16());
#else
	return str.toStdWString();
#endif
}

