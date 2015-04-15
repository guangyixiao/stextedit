#include "ConexIde.h"
#include <QtGui>
#include <QString>
#include <QFileDialog>
#include <qmessagebox.h>
#include <map>
#include <string>
#include "control/VocabularyControl.h"
#include "control/VocabularyXmlControl.h"
#include "model/Term.h"
#include "mapedit/mapeditor.h"
#include "XpmTextEdit.h"

ConexIde::ConexIde(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags), 
	_voc(NULL), 
	_model(NULL),
	_modifymodel(NULL),
	_namespacemodel(NULL),
	_authormodel(NULL),
	_hypernymmodel(NULL),
	_classifiermodel(NULL),
	_antonymmodel(NULL),
	_similarmodel(NULL),
	_synonymmodel(NULL),
	_currentTerm(NULL),
	_currentTermChs(NULL),
	_relatedTerm(0),
	_pagesize(30),
	_findDialog(this),
	_clsDialog(this),
	_vocControl(conex::VocabularyControl::XML_SAX2),
	_termControl(0),
	_be_new_term(0),
	_be_single(1),
	_factory(0),
	_xpmControl(0),
	_enMapEditor(0),
	_chsMapEditor(0)
{
	ui.setupUi(this);
	_pagesize = ui.pageSizeComboBox->currentText().toInt();	
	//
	ui.actionSave_Term->setEnabled(false);
	conex::VocabularyXmlControl _vocControl(conex::VocabularyControl::XML_SAX2);
	// construct the facotry
	factory();
	//connect(ui.actionOpenVocFile, SIGAINL(trigged()), 
	connect(ui.actionOpenVocFile, SIGNAL(triggered()), this, SLOT(OpenVoc()));
	connect(ui.actionOpenEn, SIGNAL(triggered()), this, SLOT(OpenEn()));
	connect(ui.actionOpenChs, SIGNAL(triggered()), this, SLOT(OpenChs()));
	connect(ui.actionSave_Voc, SIGNAL(triggered()), this, SLOT(SaveVoc()));
	connect(ui.actionSave_As, SIGNAL(triggered()), this, SLOT(SaveAs()));
	//term maangement
	connect(ui.tableView, SIGNAL(activated (const QModelIndex &)), this, SLOT(ViewClicked(const QModelIndex&)));
	connect(ui.actionAdd_Term, SIGNAL(triggered()), this, SLOT(NewTerm()));
	connect(ui.actionSave_Term, SIGNAL(triggered()), this, SLOT(SaveTerm()));
	connect(ui.actionRemove_Term, SIGNAL(triggered()), this, SLOT(RemoveTerm()));
	connect(ui.addTermButton, SIGNAL(clicked()), this, SLOT(NewTerm()));
	connect(ui.saveTermButton, SIGNAL(clicked()), this, SLOT(SaveTerm()));
	connect(ui.removeTermButton, SIGNAL(clicked()), this, SLOT(RemoveTerm()));
	// tool management
	connect(ui.actionBackup, SIGNAL(triggered()), this, SLOT(Backup()));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(Restore()));
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
	//context management
	connect(ui.addSynonymsButton, SIGNAL(clicked()), this, SLOT(AddSynonyms()));
	connect(ui.addNamespacesButton, SIGNAL(clicked()), this, SLOT(AddNamespace()));
	connect(ui.addAuthorButton, SIGNAL(clicked()), this, SLOT(AddAuthor()));
	connect(ui.addHypernymButton, SIGNAL(clicked()), this, SLOT(AddHypernyms()));
	connect(ui.addClassifierButton, SIGNAL(clicked()), this, SLOT(AddClassifier()));
	connect(ui.addAntonymButton, SIGNAL(clicked()), this, SLOT(AddAntonym()));
	connect(ui.addSimilarButton, SIGNAL(clicked()), this, SLOT(AddSimilar()));
	ui.pageComboBox->setInsertPolicy(QComboBox::InsertAtBottom);
	_findDialog.hide();
	_clsDialog.hide();
	ShowContext(true);
	//OpenVocFromFile();
	//remove relation maangement
	connect(ui.synonymsView, SIGNAL(activated (const QModelIndex &)), this, SLOT(SynonymViewClicked(const QModelIndex&)));
	connect(ui.hypernymView, SIGNAL(activated (const QModelIndex &)), this, SLOT(HypernymViewClicked(const QModelIndex&)));
	connect(ui.similarView, SIGNAL(activated (const QModelIndex &)), this, SLOT(SimilarViewClicked(const QModelIndex&)));
	connect(ui.antonymView, SIGNAL(activated (const QModelIndex &)), this, SLOT(AntonymViewClicked(const QModelIndex&)));
	connect(ui.classifierView, SIGNAL(activated (const QModelIndex &)), this, SLOT(ClassifierViewClicked(const QModelIndex&)));
	connect(ui.removeButton, SIGNAL(clicked()), this, SLOT(RemoveAnyFromContext()));
	connect(ui.actionClose_Voc, SIGNAL(triggered()), this, SLOT(CloseVoc()));
	connect(ui.actionOpen_Both, SIGNAL(triggered()), this, SLOT(OpenBoth()));
	//connect(ui.cbPos, SIGNAL(currentIndexChanged (int)), this, SLOT(PosIndexChanged(int)));
	// for sfa
	_xpmControl = new xpm::XpmTableDocControl(_factory);		
	_chsXpmControl = new xpm::XpmTableDocControl(_factory);	
	_chsXpmControl->safe_model(_chsXpmControl->model())->set_lang("chs");
	CreateIids();
	QWidget* p = ui.teTextEn->parentWidget();
	QGridLayout* l = static_cast<QGridLayout*>( p->layout());
	delete ui.teTextEn;
	ui.teTextEn = new xpm::XpmTextEdit(_xpmControl, p);	
	l->addWidget(ui.teTextEn, 0, 1);
	connect(ui.teTextEn, SIGNAL(refreshMap()), _enMapEditor, SLOT(viewMap()));
	delete ui.teTextCh;
	ui.teTextCh = new xpm::XpmTextEdit(_chsXpmControl, p);	
	l->addWidget(ui.teTextCh, 0, 2);
	connect(ui.teTextCh, SIGNAL(refreshMap()), _chsMapEditor, SLOT(viewMap()));
	//p->setLayout(Qt::LayoutDirection::);
}

conex::TermFactory* ConexIde::factory() {
	if(_factory == 0) {
		// construct the facotry
		QString enfile = QApplication::applicationDirPath() + "/data/Vocabulary_en_1.0.0.xml";
		QString chfile = QApplication::applicationDirPath() + "/data/Vocabulary_zh_1.0.0.xml";
		QString enCls = QApplication::applicationDirPath() + "/data/Classifiers_en_1.0.0.xml";
		QString chCls = QApplication::applicationDirPath() + "/data/Classifiers_zh_1.0.0.xml";
		_factory = new conex::TermFactory(enfile.toStdString(), chfile.toStdString(), enCls.toStdString(), chCls.toStdString());
	}
	return _factory;
}

ConexIde::~ConexIde()
{
	SAFE_DELETE(_model);
	SAFE_DELETE(_modifymodel);
	SAFE_DELETE(_namespacemodel);
	SAFE_DELETE(_authormodel);
	SAFE_DELETE(_hypernymmodel);
	SAFE_DELETE(_classifiermodel);
	SAFE_DELETE(_antonymmodel);
	SAFE_DELETE(_similarmodel);
	SAFE_DELETE(_synonymmodel);	
}

void ConexIde::CreateIids() {
	_enMapEditor = new xpm::MapEditor(_xpmControl, ui.wdIidsEn);
	QGridLayout* l = new QGridLayout(ui.wdIidsEn);
	l->addWidget(_enMapEditor);
	_chsMapEditor = new xpm::MapEditor(_chsXpmControl, ui.wdIidsChs);
	l = new QGridLayout(ui.wdIidsChs);
	l->addWidget(_chsMapEditor);
}

void ConexIde::OpenVoc() {
	if (MaybeSave()) {
		QString filename =	QFileDialog::getOpenFileName(this);
		OpenVocFromFile(filename);
	}

}

void ConexIde::Backup() {
	if (MaybeSave()) {
		QString folder = QFileDialog::getExistingDirectory(this);
		if(folder.size() > 0 && folder != QApplication::applicationDirPath()) {
			QDir(folder).mkdir("data");
			QString enfile = QApplication::applicationDirPath() + "/data/Vocabulary_en_1.0.0.xml";
		    QString chfile = QApplication::applicationDirPath() + "/data/Vocabulary_zh_1.0.0.xml";
			QFile::copy(enfile, folder + "/data/Vocabulary_en_1.0.0.xml");
			QFile::copy(chfile, folder + "/data/Vocabulary_zh_1.0.0.xml");
			ui.statusBar->showMessage(tr("backuped success to folder :") + folder);
		}
	}
}

void ConexIde::Restore() {
	if (MaybeSave()) {
		CloseVoc();
		QString folder = QFileDialog::getExistingDirectory(this);
		if(folder.size() > 0 && folder != QApplication::applicationDirPath() + "/data") {
			QString enfile = QApplication::applicationDirPath() + "/data/Vocabulary_en_1.0.0.xml";
		    QString chfile = QApplication::applicationDirPath() + "/data/Vocabulary_zh_1.0.0.xml";
			if(QFile(folder + "/Vocabulary_en_1.0.0.xml").exists())
			    QFile::copy(folder + "/Vocabulary_en_1.0.0.xml", enfile);
			if(QFile(folder + "/Vocabulary_zh_1.0.0.xml").exists())
				QFile::copy(folder + "/Vocabulary_zh_1.0.0.xml", chfile);
			ui.statusBar->showMessage(tr("restored success from folder :") + folder);
		}
	}
}

void ConexIde::CloseVoc() {	
	if(MaybeSave()) {
		if(!_be_single) {
			delete _factory;
			_factory = 0;
			_termControl = 0;
			_voc = 0;
		}
		if(_termControl != 0) {
			delete _termControl;
			_termControl = 0;
		}
		if(_voc != 0) {
			_voc->ClearTerms();
			delete _voc;
			_voc = 0;
		}
		if(_model != 0)
			_model->clear();
		_vocControl.SetFile("");
		_be_new_term = 0;
		DisplayClear();
		ui.statusBar->showMessage(tr("Voc is closed!"));
	}
}

void ConexIde::OpenVocFromFile(QString filename) {


	//connect(ui.
	if(_vocControl.GetFile() != filename.toStdString()) {	
		CloseVoc();
		_vocControl.SetFile(filename.toStdString());
		ui.statusBar->showMessage(tr("Loading Vocabulary From XML!"));
		ui.leWordCh->hide();
		ui.lePoneCh->hide();
		ui.lePronCh->hide();
		ui.teTextCh->hide();
		ui.wdIidsChs->hide();
		ui.lbEn->hide();
		ui.lbCh->hide();
		_be_single = true;
		_voc = new conex::Vocabulary();
		_vocControl.Load(*_voc);
		ui.statusBar->showMessage(tr("Loaded Vocabulary From XML!"));

		_termControl = new conex::TermControl(_voc);
		_findDialog.SetVocabularyAndTermControl(_voc, _termControl);		
		ui.statusBar->showMessage(tr("Indexed Vocabulary From XML!"));
	}	
	FindTerms();
	QString title = tr("Vocabulary Management for Conex - ") + filename;
	setWindowTitle(title);
	ui.statusBar->showMessage(tr("Loaded and Indexed Vocabulary From XML!"));
	ShowStatistics();
}

void ConexIde::OpenEn() {
	if ( MaybeSave()) {
		QString filename = QApplication::applicationDirPath() + "/data/Vocabulary_en_1.0.0.xml";
		OpenVocFromFile(filename);

	}
}

void ConexIde::OpenChs() {
	if ( MaybeSave()) {
		QString filename = QApplication::applicationDirPath() + "/data/Vocabulary_zh_1.0.0.xml";
		OpenVocFromFile(filename);		
	}
}

void ConexIde::OpenBoth() {
	if( MaybeSave()) {
		CloseVoc();
		ui.leWordCh->show();
		ui.lePoneCh->show();
		ui.lePronCh->show();
		ui.teTextCh->show();
		ui.wdIidsChs->show();
		ui.lbEn->show();
		ui.lbCh->show();
		_be_single = false;		
		ui.statusBar->showMessage(tr("Loading Both Vocabulary From XML!"));
		factory();
		_termControl = _factory->enControl();
		_factory->chsControl();		
		_voc = _factory->enVoc();
		_findDialog.SetVocabularyAndTermControl(_voc, _termControl);
		ui.statusBar->showMessage(tr("Loaded Both Vocabulary From XML!"));
		QString title = tr("Vocabulary Management for Conex - Vocabulary_en_1.0.0.xml and Vocabulary_zh_1.0.0.xml");
		setWindowTitle(title);
		ShowStatistics();
	}
}

bool ConexIde::MaybeSave() {
	if (!_termControl || (_termControl->SizeofModified() == 0 && _be_new_term == 0))
		return true;
	/*if (fileName.startsWith(QLatin1String(":/")))
		return true;*/
	// save the new term
	QMessageBox::StandardButton ret;
	if(_be_new_term ) {		
		ret = QMessageBox::warning(this, tr("Application"),
			tr("The new term has been created.\n"
			"Do you want to save your new term?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			if(!SaveTerm())
				return false;
		}
		else if (ret == QMessageBox::Discard)
			RemoveTerm();
		else
			return false;
	}

	if(_termControl->SizeofModified() == 0)
		return true;
	
	// save the voc
	//QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("Application"),
		tr("The vocabulary has been modified.\n"
		"Do you want to save your changes?"),
		QMessageBox::Save | QMessageBox::Discard
		| QMessageBox::Cancel);
	if (ret == QMessageBox::Save)
		return SaveVoc();
	else if (ret == QMessageBox::Cancel)
		return false;
	return true;
}

bool ConexIde::SaveVoc() {		
	if(!_be_single) {
		string message = "Both Saved " + _factory->enFile() + " | " + _factory->chsFile();	
		_factory->saveBoth();
		ui.statusBar->showMessage(tr(message.c_str()));
		FindModifiesTerms();
		return true;
	}
	if(_voc) {		
		string message = "Saving " + _vocControl.GetFile();
		ui.statusBar->showMessage(tr(message.c_str()));
		_vocControl.Save(*_voc);
		_termControl->ClearModifyTerms();
		message = "Saved " + _vocControl.GetFile();			
		ui.statusBar->showMessage(tr(message.c_str()));
		FindModifiesTerms();
		return true;
	}
	return true;
}

void ConexIde::SaveAs() {
	if(MaybeSave()) {
		QString filename =	QFileDialog::getSaveFileName(this);		
		if(_voc) {			
			_vocControl.SetFile(filename.toStdString());
			string message = "Saving " + _vocControl.GetFile();
			ui.statusBar->showMessage(tr(message.c_str()));
			_vocControl.Save(*_voc);
			message = "Saved " + _vocControl.GetFile();
			ui.statusBar->showMessage(tr(message.c_str()));
			QString title = tr("Vocabulary Management for CONEX - ") + filename;
			setWindowTitle(title);
		}		
	}
}

void ConexIde::ViewClicked(const QModelIndex& index) {
	int row = index.row();
	int i = 3;
	if(!_be_single)
		i = 5;
	string id(ui.tableView->model()->data(ui.tableView->model()->index(row, i)).toByteArray().constData());
	_currentTerm = _voc->GetTermByIid(id);
	if(!_be_single) 
		_currentTermChs = _factory->chsVoc()->GetTermByIid(id);
	if(_currentTerm) {
		DisplayTerm(_currentTerm);		
		ShowContext(true);
		ui.termTab->setCurrentIndex(0);

	}
}

void ConexIde::SynonymViewClicked(const QModelIndex& index) {
	int row = index.row();
	string id(ui.synonymsView->model()->data(ui.tableView->model()->index(row, 3)).toByteArray().constData());
	_relatedTerm = _voc->GetTermByIid(id);
	if(_relatedTerm) {
		ui.removeButton->show();
	}
}

void ConexIde::HypernymViewClicked(const QModelIndex& index) {
	int row = index.row();
	string id(ui.hypernymView->model()->data(ui.tableView->model()->index(row, 3)).toByteArray().constData());
	_relatedTerm = _voc->GetTermByIid(id);
	if(_relatedTerm) {
		ui.removeButton->show();
	}
}
void ConexIde::ClassifierViewClicked(const QModelIndex& index) {
	int row = index.row();
	string id(ui.classifierView->model()->data(ui.tableView->model()->index(row, 3)).toByteArray().constData());
	_relatedTerm = _factory->clsControl(_voc->lang())->GetTermByIid(id);
	if(_relatedTerm) {
		ui.removeButton->show();
	}
}
void ConexIde::AntonymViewClicked(const QModelIndex& index) {
	int row = index.row();
	string id(ui.antonymView->model()->data(ui.tableView->model()->index(row, 3)).toByteArray().constData());
	_relatedTerm = _voc->GetTermByIid(id);
	if(_relatedTerm) {
		ui.removeButton->show();
	}
}
void ConexIde::SimilarViewClicked(const QModelIndex& index) {
	int row = index.row();
	string id(ui.similarView->model()->data(ui.tableView->model()->index(row, 3)).toByteArray().constData());
	_relatedTerm = _voc->GetTermByIid(id);
	if(_relatedTerm) {
		ui.removeButton->show();
	}
}

void ConexIde::DisplayTerm(conex::Term* term) {
	if(!_be_new_term) {
		QString iid(term->id()->iid().c_str());
		QString word = stdWToQString(term->id()->word());
		QString pos(term->pos().c_str());
		QString ponetines(term->phonetic().c_str());
		QString textExp = stdWToQString(term->text());
		QString iidExp(term->structure()->iids().c_str());
		QString prons(term->id()->pronunciation().c_str());
		// iid
		ui.iidValueLabel->setText(iid);
		// set year
		bool ok;
		int year = iid.mid(4, 4).toInt(&ok, 16);
		ui.cbYearTag->setCurrentIndex(0);
		ui.cbYear->setCurrentIndex(5);
		if(ok) {
			if(year >> 15)
				ui.cbYearTag->setCurrentIndex(1);
			int year_int = year & 32767;
			if(year_int >= 0 && year_int <= 32767)
				ui.cbYear->setEditText(QString::number(year_int));
		}
		// word
		ui.leWordEn->setText(word);
		// pron.
		ui.lePronEn->setText(prons);
		// pos
		ui.cbPos->setCurrentIndex(_termControl->posToViewIndex(term->pos()));
		// state
		ui.cbState->setCurrentIndex(_termControl->stateToViewIndex(term->state()));	
		// text
		ui.teTextEn->clear();
		string source = textExp.toUtf8();
		sfa_pair pair = _xpmControl->parse_source(source);
		ui.teTextEn->setText(QString::fromUtf8(pair.content.c_str()));
		if(pair.maps.size() > 10) {
		    _xpmControl->parse_model(pair.maps);
			static_cast<xpm::XpmTextEdit*>(ui.teTextEn)->formatMaps();
		}
		// pone.
		ui.lePoneEn->setText(ponetines);
		// iids
		//ui.txIids->setText(iidExp);
		ui.actionSave_Term->setEnabled(true);
		//ui.textValueEdit-->
		// set chs
		if(!_be_single && _currentTermChs) {
			QString word_chs = stdWToQString(_currentTermChs->id()->word());
			ui.leWordCh->setText(word_chs);
			ui.teTextCh->clear();
			QString textExp_chs = stdWToQString(_currentTermChs->text());
			string source = textExp_chs.toUtf8();
			sfa_pair pair = _chsXpmControl->parse_source(source);
			ui.teTextCh->setText(QString::fromUtf8(pair.content.c_str()));
			if(pair.maps.size() > 10) {
				_chsXpmControl->parse_model(pair.maps);
				static_cast<xpm::XpmTextEdit*>(ui.teTextCh)->formatMaps();
			}
			//ui.teTextCh->setText(textExp_chs);
			QString ponetines_chs(_currentTermChs->phonetic().c_str());
			ui.lePoneCh->setText(ponetines_chs);
			QString prons_chs(_currentTermChs->id()->pronunciation().c_str());
			ui.lePronCh->setText(prons_chs);
		}
	}
}

void ConexIde::DisplayClear() {
	ui.iidValueLabel->setText(tr(""));	
	ui.cbPos->setCurrentIndex(0);
	ui.cbState->setCurrentIndex(1);
	ui.leWordEn->setText(tr(""));
	ui.lePronEn->setText(tr(""));
	ui.lePoneEn->setText(tr(""));	
	ui.teTextEn->clear();
	ui.leWordCh->setText(tr(""));
	ui.lePronCh->setText(tr(""));
	ui.lePoneCh->setText(tr(""));	
	ui.teTextCh->clear();
	//ui.txIids->setText(tr(""));
	ui.lbWords->setText(tr("0"));
	ui.lbSenses->setText(tr("0"));	
	//ui.textValueEdit-->
}

void ConexIde::LoadTerm(conex::Term* term) {
	if(_currentTerm && _termControl) {
		//word
		QString word = ui.leWordEn->text();
		_termControl->SetWord(qToStdWString( word), _currentTerm);
		//state
		string state = ui.cbState->currentText().toStdString();
		_currentTerm->setState(state);
		// prons
		string prons = ui.lePoneEn->text().toStdString();
		_currentTerm->id()->setPronunciation(prons);
		//text
		QString textExp = ui.teTextEn->toPlainText();
		if(_xpmControl->total_maps() > 0) {
			string content = textExp.toUtf8();
			string r = _xpmControl->write_target(content);
			textExp = QString::fromUtf8(r.c_str());
		}
		_termControl->SetTextExplanation(qToStdWString(textExp), _currentTerm);
		//ponetenis
		string ponetenis = ui.lePoneEn->text().toStdString();
		_termControl->SetPonetenics(ponetenis, _currentTerm);		
		//iids
		string iidExp = ""; // ui.txIids->toPlainText().toAscii().constData();
		_termControl->SetIidExplanation(iidExp, _currentTerm);
		ui.statusBar->showMessage(tr("Saved Term : ") + word + tr(" Iid: ") + ui.iidValueLabel->text() );
	}
	// set chs
	if(!_be_single && _currentTerm && _currentTermChs) {
		_currentTermChs->structure()->setIids(_currentTerm->structure()->iids());
		_currentTermChs->setState(_currentTerm->state());
		QString word_chs = ui.leWordCh->text();
		_currentTermChs->id()->setWord(qToStdWString(word_chs));
		QString textExp_chs = ui.teTextCh->toPlainText();
		if(_chsXpmControl->total_maps() > 0) {
			string content = textExp_chs.toUtf8();
			string r = _chsXpmControl->write_target(content);
			textExp_chs = QString::fromUtf8(r.c_str());
		}
		_currentTermChs->setText(qToStdWString(textExp_chs));
		_currentTermChs->setPhonetic(ui.lePoneCh->text().toStdString());
		_currentTermChs->id()->setPronunciation(ui.lePronCh->text().toStdString());
	}

	//ui.textValueEdit-->
}

bool ConexIde::SaveTerm() {
	if(_currentTerm && _termControl ) {
		// 1. evaluate the data format of cbYear
		bool ok;
		int year = ui.cbYear->currentText().toInt(&ok);
		if(!ok || year > 32767 || year < 0) {
			QString message = tr("Year in Combo Box is not a number or it is out of range (0 - 32768) for ") + ui.cbYear->currentText();
			ui.statusBar->showMessage(message);
			return 0;
		}
		int yearWithTag = ui.cbYearTag->currentIndex() == 1 ? (1 << 15 | year) : year;
		// verify 
		string yearToHex = QString("%1").arg(yearWithTag, 4, 16, QChar('0')).toStdString();
		// 2. evaluate the word of term
		if(ui.leWordEn->text().isEmpty()) {
			QString message = tr("The word of current term can'b empty!");
			ui.statusBar->showMessage(message);
			return 0;
		}
		if(!_be_single && ui.leWordCh->text().isEmpty()) {
			QString message = tr("The word of current Chinese term can'b empty!");
			ui.statusBar->showMessage(message);
			return 0;
		}
		// 3. evaluate the text of term

		//it is important to remove term index before load the term
		_termControl->RemoveTermIndex(_currentTerm);
		if(!_be_single && _currentTermChs) {
			_factory->chsControl()->RemoveTermIndex(_currentTermChs);
		}
		LoadTerm(_currentTerm); // will load both two terms
		string pos = ui.cbPos->currentText().toStdString();
		_termControl->SaveTerm(_currentTerm, pos, yearToHex);
		if(!_be_single && _currentTermChs) {
			_factory->chsControl()->SaveTerm(_currentTermChs, pos, yearToHex);
		}
		//_model.LoadData(_voc);
		ui.termViewTab->setCurrentIndex(1);
		ui.actionSave_Term->setDisabled(true);
		FindTerms();
		FindModifiesTerms();
		DisplayTerm(_currentTerm);
		ShowStatistics();
		ui.actionAdd_Term->setEnabled(true);
		ui.addTermButton->setEnabled(true);
		_be_new_term = 0;
		return 1;
	}
}

void ConexIde::RemoveTerm() {
	if(_currentTerm && _termControl) {
		QString message = tr("Removed Current Term !");
		_termControl->RemoveTerm(_currentTerm);
		_currentTerm = 0;
		if(!_be_single && _currentTermChs) {
			_factory->chsControl()->RemoveTerm(_currentTermChs);
			_currentTermChs = 0;
		}
		DisplayClear();
		ui.statusBar->showMessage(message );
		ui.actionSave_Term->setDisabled(true);
		FindTerms();
		FindModifiesTerms();
		ShowStatistics();
		ui.actionAdd_Term->setEnabled(true);
		ui.addTermButton->setEnabled(true);
		_be_new_term = 0;
	}	
}

void ConexIde::NewTerm() {
	if(_voc && _termControl) {
		DisplayClear(); 
		ui.termViewTab->setCurrentIndex(1);
		ui.leWordEn->setFocus();
		_currentTerm = _termControl->CreateTerm();
		if(!_be_single) {
			_currentTermChs = _factory->chsControl()->CreateTermWithIid(_currentTerm->id()->iid());
		}
		QString iid(_currentTerm->id()->iid().c_str());
		ui.iidValueLabel->setText(iid);
		ui.actionSave_Term->setEnabled(true);
		ui.statusBar->showMessage(tr("Please fill the data in the term!!!!") );	 
		ShowStatistics();
		ui.actionAdd_Term->setEnabled(false);
		ui.addTermButton->setEnabled(false);
		_be_new_term = 1;
		ui.termTab->setCurrentIndex(0);
	}
}

void ConexIde::FindTerms() {
	if(_voc == 0 || _termControl == 0)
		return;
	QString text = ui.findLineEdit->text();
	ui.statusBar->showMessage(tr("Finding terms from the prefix of '") + text + tr("'"));
	vector<conex::Term*> terms;
	vector<conex::Term*> t2;
	if(!text.isEmpty()) {		
		wstring prefix(qToStdWString( text));
		if(_be_single)
			_termControl->PrefixSearch(prefix, terms);		
		else
			_factory->PrefixSearch(prefix, terms, t2);

	}else {
		if(_be_single)
		    _voc->GetTopkTerms(0, terms);
		else
			_factory->PrefixSearch(L"##", terms, t2);
	}
	SAFE_DELETE(_model);
	_model = new conex::VocabularyTableModel(this);	
	int pagecount = 0;
	int total = terms.size();
	if(_be_single)
		pagecount = _model->LoadData(terms);	
	else
		pagecount = _model->LoadData(terms, t2);
	SetPageCount(pagecount);	
	ui.lbTotal->setText(tr("Total Size :") + QString::number(total));
	ui.tableView->setModel(_model);
	ui.tableView->resizeColumnsToContents();
	ui.findLineEdit->setFocus();
	QModelIndex index = _model->index(0,0);
	ui.tableView->setCurrentIndex(index);
	ui.statusBar->showMessage(tr("Finded terms from the prefix of '") + text + tr("'"));
}

void ConexIde::FindModifiesTerms() {
	vector<conex::Term*> terms;
	_termControl->GetModifyTerms(terms);
	SAFE_DELETE(_modifymodel);
	_modifymodel = new conex::VocabularyTableModel(this);
	_modifymodel->LoadData(terms);
	ui.modifyTableView->setModel(_modifymodel);
	ui.modifyTableView->resizeColumnsToContents();	
	QModelIndex index = _modifymodel->index(0,0);
	ui.modifyTableView->setCurrentIndex(index);
}

//term paging
void ConexIde::FirstPage() {
	if(_model) {
		_model->FirstPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void ConexIde::NextPage() {
	if(_model) {
		_model->NextPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void ConexIde::PreviewPage() {
	if(_model) {
		_model->PreviewPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void ConexIde::LastPage() {
	if(_model) {
		_model->LastPage();
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
		ChangePageIndex();
	}
}
void ConexIde::SetPageCount(int pagecount) {
	QStringList items;
	for(int i = 0; i < pagecount; i++) {
		int page = i+1;
		QString page_str = QString::number(page, 10);
		items << page_str;
	}	
	if(!ui.pageComboBox->isVisible())
		ui.pageComboBox->setVisible(true);
	disconnect(ui.pageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(GetPage(int)));
	ui.pageComboBox->clear();
	connect(ui.pageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(GetPage(int)));
	ui.pageComboBox->setInsertPolicy(QComboBox::InsertAtBottom);
	ui.pageComboBox->addItems(items);
	ui.pageComboBox->setCurrentIndex(0);
}
void ConexIde::GetPage(int index) {
	if(_model) {
		int page = index + 1;
		_model->GetPage(page);
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
	}
}
void ConexIde::SetPagesize(const QString & text) {
	if(_model) {
		_pagesize = text.toInt();
		int pagecount = _model->SetPageSize(_pagesize);
		SetPageCount(pagecount);
		ui.tableView->setModel(_model);
		ui.tableView->resizeColumnsToContents();
	}
}

void ConexIde::ChangePageIndex() {
	if(_model) {
		int currentpage = _model->GetCurrentPage();
		ui.pageComboBox->setCurrentIndex(currentpage -1);
	}
}

void ConexIde::AddSynonyms() {
	if(_currentTerm) {
		if( _findDialog.exec() == QDialog::Accepted) {
			_relatedTerm = _findDialog.GetSelectedTerm();
			if(_currentTerm && _relatedTerm && _currentTerm != _relatedTerm) {
				_currentTerm->context()->AddRelation(conex::SYNONYM, _relatedTerm);
				if(!_be_single && _currentTermChs) {
					_currentTermChs->context()->AddRelation(conex::SYNONYM, _relatedTerm->id()->iid());
				}
				wstring message = L"Term: " + _currentTerm->id()->word()+ L" Added Synonym of Term: " + _relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(true);
				_relatedTerm = 0;
			}
		}
	}

}

//context management
void ConexIde::AddNamespace() {
	if(_voc) {
		if( _findDialog.exec() == QDialog::Accepted) {
			conex::Term* relatedTerm = _findDialog.GetSelectedTerm();
			if(_voc && relatedTerm) {
				//_voc->GetVocabularyContext()->AddNamespace()->RelatedTo(relatedTerm);
				wstring message = L"Vocabulary:  Added Namespace of Term: " + relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(false);
			}
		}
	}
}
void ConexIde::AddAuthor() {
	if(_voc) {
		if( _findDialog.exec() == QDialog::Accepted) {
			conex::Term* relatedTerm = _findDialog.GetSelectedTerm();
			if(_voc && relatedTerm) {
				//_voc->GetVocabularyContext()->AddAuthor()->RelatedTo(relatedTerm);
				wstring message = L"Vocabulary: Added Author of Term: " + relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(false);
			}
		}
	}
}
void ConexIde::AddHypernyms() {
	if(_currentTerm) {
		if( _findDialog.exec() == QDialog::Accepted) {
			_relatedTerm = _findDialog.GetSelectedTerm();
			if(_currentTerm && _relatedTerm && _currentTerm != _relatedTerm) {
				_currentTerm->context()->AddRelation(conex::HYPERNYM, _relatedTerm);
				wstring message = L"Term: " + _currentTerm->id()->word() + L" Added Hypernym of Term: " + _relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(true);
				_relatedTerm = 0;
			}
		}
	}
}
void ConexIde::AddClassifier() {
	if(_clsDialog.termControl() == 0) {
		if(_voc->lang() == "eng")
			_clsDialog.SetVocabularyAndTermControl(_factory->enCls(), _factory->enClsControl());
		else
			_clsDialog.SetVocabularyAndTermControl(_factory->chsCls(), _factory->chsClsControl());
	}

	if(_currentTerm) {
		if( _clsDialog.exec() == QDialog::Accepted) {
			_relatedTerm = _clsDialog.GetSelectedTerm();
			if(_currentTerm && _relatedTerm && _currentTerm != _relatedTerm) {
				_currentTerm->context()->AddRelation(conex::CATEGORY, _relatedTerm);
				if(!_be_single && _currentTermChs) {
					_currentTermChs->context()->AddRelation(conex::CATEGORY, _relatedTerm->id()->iid());
				}
				wstring message = L"Term: " + _currentTerm->id()->word() + L" Added Classifier of Term: " + _relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(true);
			}
		}
	}
}
void ConexIde::AddAntonym() {
	if(_currentTerm) {
		if( _findDialog.exec() == QDialog::Accepted) {
			_relatedTerm = _findDialog.GetSelectedTerm();
			if(_currentTerm && _relatedTerm && _currentTerm != _relatedTerm) {
				_currentTerm->context()->AddRelation(conex::ANTONYM, _relatedTerm);
				wstring message = L"Term: " + _currentTerm->id()->word() + L" Added Antonym of Term: " + _relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(true);
				_relatedTerm = 0;
			}
		}
	}
}
void ConexIde::AddSimilar() {
	if(_currentTerm) {
		if( _findDialog.exec() == QDialog::Accepted) {
			_relatedTerm = _findDialog.GetSelectedTerm();
			if(_currentTerm && _relatedTerm && _currentTerm != _relatedTerm) {
				_currentTerm->context()->AddRelation(conex::SIMINYM, _relatedTerm);
				wstring message = L"Term: " + _currentTerm->id()->word() + L" Added Similar of Term: " + _relatedTerm->id()->word();
				ui.statusBar->showMessage(stdWToQString(message.c_str()));
				ShowContext(true);
				_relatedTerm = 0;
			}
		}
	}
}
void ConexIde::RemoveAnyFromContext() {
	if(_currentTerm && _relatedTerm && _currentTerm != _relatedTerm) {
		_currentTerm->context()->DeleteRelation(_relatedTerm);
		ui.removeButton->hide();
		ShowContext(true);
	}
}

void ConexIde::PosIndexChanged(int index) {
	/*string pos = _termControl->posIndexToStr(index);
	string iid = ui.iidValueLabel->text().toAscii().data();
	if(iid.size() == 14) {
	iid = _termControl->changePos(pos, iid);
	ui.iidValueLabel->setText(iid.c_str());
	}*/
}

void ConexIde::ShowStatistics() {
	if( _voc && _termControl ) {
		QFont f("Times", 24, QFont::Bold);
		ui.lbSenses->setFont(f);
		ui.lbWords->setFont(f);
		ui.lbChsWords->setFont(f);

		char buffer[33];
		int size = _voc->GetTerms().size();
		itoa( size, buffer, 10 );
		string str_size = buffer;
		if(size > 0)
			ui.lbSenses->setText(tr(str_size.c_str()));
		int words = _termControl->WordsSize();
		itoa(words, buffer, 10);
		string str_words = buffer;
		if(words > 0)
			ui.lbWords->setText(tr(str_words.c_str()));

		if(!_be_single) {
			words = _factory->chsClsControl()->WordsSize();
			itoa(words, buffer, 10);
			string str_words = buffer;
			if(words > 0)
				ui.lbChsWords->setText(tr(str_words.c_str()));
			ui.lbChsWords->show();
			ui.lbChsWordsHint->show();
		}else {
			ui.lbChsWords->hide();
			ui.lbChsWordsHint->hide();
		}
	}
}

void ConexIde::ShowContext(bool isTermEdit) {
	if(isTermEdit) {
		ui.addAuthorButton->hide();
		ui.authorGroup->hide();
		ui.addNamespacesButton->hide();
		ui.namespaceGroup->hide();		
		ui.addSimilarButton->show();
		ui.similarGroup->show();
		ui.addHypernymButton->show();
		ui.hypernymGroup->show();
		ui.addClassifierButton->show();
		ui.classifierGroup->show();
		ui.addAntonymButton->show();
		ui.antonymGroup->show();
		if(_currentTerm) {
			//synonyms
			vector<conex::Term*> synonyms;
			_termControl->GetRelationTerms(_currentTerm, conex::SYNONYM, synonyms);
			SAFE_DELETE(_synonymmodel);
			_synonymmodel = new conex::VocabularyTableModel(this);
			_synonymmodel->LoadData(synonyms);			
			//ui.statusBar->showMessage(tr("size :" + synonyms.size()));
			ui.synonymsView->setModel(_synonymmodel);
			ui.synonymsView->resizeColumnsToContents();
			//similars
			vector<conex::Term*> similars;
			_termControl->GetRelationTerms(_currentTerm, conex::SIMINYM, similars);
			SAFE_DELETE(_similarmodel);
			_similarmodel = new conex::VocabularyTableModel(this);
			_similarmodel->LoadData(similars);
			ui.similarView->setModel(_similarmodel);
			ui.similarView->resizeColumnsToContents();
			//hypernym
			vector<conex::Term*> hypernyms;
			_termControl->GetRelationTerms(_currentTerm, conex::HYPERNYM, hypernyms);
			SAFE_DELETE(_hypernymmodel);
			_hypernymmodel = new conex::VocabularyTableModel(this);
			_hypernymmodel->LoadData(hypernyms);
			ui.hypernymView->setModel(_hypernymmodel);
			ui.hypernymView->resizeColumnsToContents();
			//classifier
			if(_factory != 0) {
				vector<conex::Term*> classifiers;
				_factory->clsControl(_voc->lang())->GetRelationTerms(_currentTerm, conex::CATEGORY, classifiers);
				SAFE_DELETE(_classifiermodel);
				_classifiermodel = new conex::VocabularyTableModel(this);
				_classifiermodel->LoadData(classifiers);
				ui.classifierView->setModel(_classifiermodel);
				ui.classifierView->resizeColumnsToContents();
			}
			//antonym
			vector<conex::Term*> antonyms;
			_termControl->GetRelationTerms(_currentTerm, conex::ANTONYM, antonyms);
			SAFE_DELETE(_antonymmodel);
			_antonymmodel = new conex::VocabularyTableModel(this);
			_antonymmodel->LoadData(antonyms);
			ui.antonymView->setModel(_antonymmodel);
			ui.antonymView->resizeColumnsToContents();
			//show them
			ui.termTab->setCurrentIndex(1);

		}
	}else {
		ui.addAuthorButton->show();
		ui.authorGroup->hide();
		ui.addNamespacesButton->show();
		ui.namespaceGroup->show();
		ui.addSimilarButton->hide();
		ui.similarGroup->hide();
		ui.addHypernymButton->hide();
		ui.hypernymGroup->hide();
		ui.addClassifierButton->hide();
		ui.classifierGroup->hide();
		ui.addAntonymButton->hide();
		ui.antonymGroup->hide();
		if(_voc) {
			//synonyms
			//conex::VocabularyContext* context = _voc->GetVocabularyContext();
			vector<conex::Term*> synonyms;
			//context->GetSynonyms(synonyms);
			SAFE_DELETE(_synonymmodel);
			_synonymmodel = new conex::VocabularyTableModel(this);
			_synonymmodel->LoadData(synonyms);
			ui.synonymsView->setModel(_synonymmodel);
			ui.synonymsView->resizeColumnsToContents();
			//author
			vector<conex::Term*> authors;
			//context->GetAuthor(authors);
			SAFE_DELETE(_authormodel);
			_authormodel = new conex::VocabularyTableModel(this);
			_authormodel->LoadData(authors);
			ui.authorView->setModel(_authormodel);
			ui.authorView->resizeColumnsToContents();
			//hypernym
			vector<conex::Term*> namespaces;
			//context->GetNamespace(namespaces);
			SAFE_DELETE(_namespacemodel);
			_namespacemodel = new conex::VocabularyTableModel(this);
			_namespacemodel->LoadData(namespaces);
			ui.namespaceView->setModel(_namespacemodel);
			ui.namespaceView->resizeColumnsToContents();
			//show them
			ui.termTab->setCurrentIndex(1);
		}
	}
	ui.removeButton->hide();
}

/*! Convert a QString to an std::wstring */
std::wstring ConexIde::qToStdWString(const QString &str)
{
#ifdef _MSC_VER
	return std::wstring((const wchar_t *)str.utf16());
#else
	return str.toStdWString();
#endif
}

/*! Convert an std::wstring to a QString */
QString ConexIde::stdWToQString(const std::wstring &str)
{
#ifdef _MSC_VER
	return QString::fromUtf16((const ushort *)str.c_str());
#else
	return QString::fromStdWString(str);
#endif
}


