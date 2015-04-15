#ifndef CONEXIDE_H
#define CONEXIDE_H

#include <vector>
#include <string>
#include <QApplication>
#include <QMainWindow>
#include "ui_conexide.h"
#include "model/Vocabulary.h"
#include "control/VocabularyXmlControl.h"
#include "control/TermControl.h"
#include "control/TermFactory.h"
#include "VocabularyTableModel.h"
//#include "viewmodel/ModifiedTableModel.h"
#include "FindDialog.h"
#include "XpmTableDocControl.h"
#include "mapedit/mapeditor.h"


using namespace std;
//using namespace conex;


class ConexIde : public QMainWindow
{
	Q_OBJECT

public:
	ConexIde(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ConexIde();
	// create iids
	void CreateIids();
private slots:
   // void NewVoc();
	//vocabulary maangement
	void OpenVoc();
	void OpenEn();
	void OpenChs();
	void OpenBoth();
	bool SaveVoc();
	void SaveAs();	
	void CloseVoc();
	//when a table view clicked
	//term management
	void ViewClicked(const QModelIndex& index);
	bool SaveTerm();
	void RemoveTerm();
	void NewTerm();
	void FindTerms();
	void FindModifiesTerms();
	//term paging
	void FirstPage();
	void NextPage();
	void PreviewPage();
	void LastPage();
	void SetPageCount(int pagecount);
	void GetPage(int index);
	void SetPagesize(const QString & text);
	void ChangePageIndex();
	//tool management
	void Backup();
	void Restore();
	//context management
	void AddSynonyms();
	void AddNamespace();
	void AddAuthor();
	void AddHypernyms();
	void AddClassifier();
	void AddAntonym();
	void AddSimilar();
	void RemoveAnyFromContext();
	//remove rels management
	void SynonymViewClicked(const QModelIndex& index);
	void HypernymViewClicked(const QModelIndex& index);
	void ClassifierViewClicked(const QModelIndex& index);
	void AntonymViewClicked(const QModelIndex& index);
	void SimilarViewClicked(const QModelIndex& index);
	void PosIndexChanged ( int index );
private:
	void ShowStatistics();
	conex::TermFactory* factory();
	void ShowContext(bool isTermContext);
	void OpenVocFromFile(QString filename = QApplication::applicationDirPath() + tr("/data/Vocabulary_1.0.xml"));
	bool MaybeSave();
	void DisplayTerm(conex::Term *term);
	void DisplayClear();
	void LoadTerm(conex::Term *term);
	std::wstring qToStdWString(const QString &str);
    QString stdWToQString(const std::wstring &str);
	Ui::ConexIdeClass ui;
	conex::Vocabulary* _voc;
	conex::VocabularyXmlControl _vocControl;
	conex::TermControl* _termControl;
	conex::Term* _currentTerm;
	conex::Term* _currentTermChs;
	conex::Term* _relatedTerm;
	conex::VocabularyTableModel* _model;
	conex::VocabularyTableModel* _modifymodel;
	conex::VocabularyTableModel* _namespacemodel;
	conex::VocabularyTableModel* _authormodel;
	conex::VocabularyTableModel* _hypernymmodel;
	conex::VocabularyTableModel* _classifiermodel;
	conex::VocabularyTableModel* _antonymmodel;
	conex::VocabularyTableModel* _similarmodel;
	conex::VocabularyTableModel* _synonymmodel;	
	//paging
	int _pagesize;
	FindDialog _findDialog;
	FindDialog _clsDialog;
	// for new term
	bool _be_new_term;
	// for both
	bool _be_single;
	conex::TermFactory* _factory;
	// for sfasfa
	xpm::XpmTableDocControl* _xpmControl;
	xpm::XpmTableDocControl* _chsXpmControl;
	xpm::MapEditor* _enMapEditor;
	xpm::MapEditor* _chsMapEditor;
};

#endif // CONEXMAIN_H
