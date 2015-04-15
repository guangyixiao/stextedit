#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <vector>
#include <qdialog.h>
#include "ui_FindDialog.h"
#include "model/Vocabulary.h"
#include "control/VocabularyXmlControl.h"
#include "control/TermControl.h"
#include "VocabularyTableModel.h"
//#include "viewmodel/ModifiedTableModel.h"

using namespace std;
//using namespace conex;

class FindDialog : public QDialog
{
	Q_OBJECT

public:
	FindDialog(QWidget *parent = 0);
	~FindDialog();
	void SetVocabularyAndTermControl(conex::Vocabulary* voc, conex::TermControl* termControl);
	conex::Term* GetSelectedTerm();
	conex::TermControl* termControl();
private slots:   	
	//when a table view clicked
	//term management	
	void ViewClicked(const QModelIndex& index);	
	void FindTerms();
	//term paging
	void FirstPage();
	void NextPage();
	void PreviewPage();
	void LastPage();
	void SetPageCount(int pagecount);
	void GetPage(int index);
	void SetPagesize(const QString & text);
	void ChangePageIndex();
private:
	std::wstring qToStdWString(const QString &str);
    Ui::Dialog ui;
	conex::Vocabulary* _voc;
	conex::Term* _currentTerm;
	conex::VocabularyTableModel* _model;
	conex::TermControl* _termControl;
	int _pagesize;
};

inline conex::TermControl* FindDialog::termControl() {
	return _termControl;
}

#endif // CONEXMAIN_H
