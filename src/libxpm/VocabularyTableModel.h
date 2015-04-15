#ifndef VOCABULARY_TABLE_MODEL_H
#define VOCABULARY_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "model/Term.h"
#include "StringCovert.h"
#include "sfa.h"


using namespace std;

namespace xpm {
class VocabularyTableModel : public QAbstractTableModel{
public:
	VocabularyTableModel(QObject* object) : 
    QAbstractTableModel(object),
	_pagesize(30),
	_pagecount(0),
	_currentpage(1),
	_datasize(0){}
	int rowCount(const QModelIndex& parent) const;
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	int LoadData(vector<conex::Term*>& terms, int pagesize = 30);
	int SetPageSize(int pagesize);
	int PageData();
	void FirstPage();
	void NextPage();
	void PreviewPage();
	void LastPage();
	void GetPage(int currentpage);
	int GetCurrentSize() const;
	int GetCurrentPage();
private:	
	void GetPage();	
	vector<conex::Term*> _terms;
	vector<vector<QString> > _rows;
	int _pagesize;
	int _pagecount;
	int _datasize;
	int _currentpage;
};



inline int VocabularyTableModel::rowCount(const QModelIndex& parent) const {
	return GetCurrentSize();
}

inline int VocabularyTableModel::columnCount(const QModelIndex& parent) const {
	if(_datasize >0)
	    return 4;
	else
		return 0;
}

inline QVariant VocabularyTableModel::data(const QModelIndex& index, int role) const {
	//need to improve the peformance in this function
	if(role == Qt::DisplayRole) {
		return _rows[index.row()][index.column()];
	}
	return QVariant::Invalid;
}

inline QVariant VocabularyTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	const QString headers[] = {"Text", "TextExp", "Pos", "Iid"};
	if(role == Qt::DisplayRole) { 
		if(orientation == Qt::Horizontal)		
			return headers[section];
	}
	return QVariant::Invalid;
}

inline int VocabularyTableModel::LoadData(vector<conex::Term*>& terms, int pagesize) {
	_pagesize = pagesize;
	_terms.swap(terms);
	_rows.clear();	
	return PageData();
	//LoadData();
}

inline int VocabularyTableModel::SetPageSize(int pagesize) {
	_pagesize = pagesize;
	return PageData();
}

inline int VocabularyTableModel::PageData() {
	_datasize = _terms.size();
	if(_pagesize > 0 ) {
		_pagecount = 1 + _datasize / _pagesize;
		if(_pagecount > 0)
			FirstPage();
	}
	return _pagecount;
}

inline void VocabularyTableModel::FirstPage() {
	if(_pagecount > 0) {
		_currentpage = 1;
		GetPage();
	}
}

inline void VocabularyTableModel::LastPage() {
	if(_pagecount > 0) {
		_currentpage = _pagecount;
		GetPage();
	}
}

inline void VocabularyTableModel::NextPage() {
	if(_pagecount > 0) {
		if(_currentpage == _pagecount) {
			return;
		}else {
			++_currentpage;
			GetPage();
		}
	}
}

inline void VocabularyTableModel::PreviewPage() {
	if(_pagecount > 0) {
		if(_currentpage == 1) {
			return;
		}else {
			--_currentpage;
			GetPage();
		}
	}
}

inline void VocabularyTableModel::GetPage(int currentpage) {
	if(currentpage != _currentpage) {
		_currentpage = currentpage;
		GetPage();
	}
}

inline int VocabularyTableModel::GetCurrentPage() {
	return _currentpage;
}

inline void VocabularyTableModel::GetPage() {	
	_rows.clear();	
	QModelIndex index;
	if(_pagecount > 0 && _currentpage > 0) {
		int count = 0;
		int start = (_currentpage - 1) * _pagesize;
		if(start < _datasize) {
			sfa s;
			for(vector<conex::Term*>::iterator it = _terms.begin() + start; it != _terms.end()&&count != GetCurrentSize() ; ++it, ++count) {
				vector<QString> row;
				conex::Term* term = *it;
				row.push_back(StringCovert::stdWToQString(term->indexWord()));
				string source = StringCovert::stdWToQString(term->text()).toUtf8();
				QString t = QString::fromUtf8(s.parse_source(source).content.c_str());
				row.push_back(t);
				row.push_back(QString(term->pos().c_str()));
				row.push_back(QString(term->id()->iid().c_str()));
				_rows.push_back(row);
			}
		}
	}
	//endRemoveRows();
}

inline int VocabularyTableModel::GetCurrentSize() const {
	if(_currentpage == _pagecount && _pagesize > 0)
		return _datasize % _pagesize;
	else
		return _pagesize;	
}

}
#endif
