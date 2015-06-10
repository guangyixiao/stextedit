#include "XpmCompleter.h"
#include <iostream>
#include <QAbstractItemView>
#include <QAbstractTableModel>
#include "StringCovert.h"

namespace xpm {
	XpmCompleter::XpmCompleter(conex::TermControl* control, QObject * parent) : QCompleter(parent),
		_termControl(control),
		_model(NULL)
		 { 
	}
	XpmCompleter::~XpmCompleter() {
		if(_model) {
			delete _model;
			_model = NULL;
		}
	}

	void XpmCompleter::setCompletionPrefix(const QString & prefix) {
		_prefix = prefix;
		vector<conex::Term*> terms;
		if(_termControl != 0 && !prefix.isEmpty()) {		
			wstring sprefix(StringCovert::qToStdWString(prefix));
			_termControl->PrefixSearch(sprefix, terms);	
			if (terms.size() == 0 && prefix.size() > 1){
				_prefix = prefix.right(1);
				_termControl->PrefixSearch(_prefix.toStdWString(), terms);
			}
			if(_model) {
				delete _model;
				_model = NULL;
			}
			_model = new VocabularyTableModel(this);
			int pagecount = _model->LoadData(terms);	
			setModel(_model);
		}
	}

	void XpmCompleter::selectedIndex(const QModelIndex& index) {
		//cout << "begining emit the term signal !" << endl;
		if(_termControl != 0 ) {
			int row = index.row();
			string id(_model->data(_model->index(row, 3), Qt::DisplayRole).toByteArray().constData());
			conex::Term* term = _termControl->GetTermByIid(id);
			if(term) {
				emit activatedTerm(term);
				//cout << "finished emit the term signal !" << endl;
			}
		}
	}
}