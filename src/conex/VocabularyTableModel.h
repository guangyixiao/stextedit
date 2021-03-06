#ifndef VOCABULARY_TABLE_MODEL_H
#define VOCABULARY_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "model/Term.h"
#include "sfa.h"


using namespace std;

namespace conex {
class VocabularyTableModel : public QAbstractTableModel{
public:
	VocabularyTableModel(QObject* object) : 
    QAbstractTableModel(object),
	_pagesize(30),
	_pagecount(0),
	_currentpage(1),
	_be_single(true),
	_datasize(0){}
	int rowCount(const QModelIndex& parent) const;
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	int LoadData(vector<conex::Term*>& terms, int pagesize = 30);
	int LoadData(vector<conex::Term*>& terms, vector<conex::Term*>& t2, int pagesize = 30);
	int LoadData(vector<pair<wstring, conex::Term*>>& terms, int pagesize = 30);
	int SetPageSize(int pagesize);
	int PageData();
	void FirstPage();
	void NextPage();
	void PreviewPage();
	void LastPage();
	void GetPage(int currentpage);
	int GetCurrentSize() const;
	int GetCurrentPage();
	void clear();
private:	
	QString stdWToQString(const std::wstring &str);
	void GetPage();	
	vector<conex::Term*> _terms;
	vector<conex::Term*> _terms2;
	vector<vector<QString> > _rows;
	int _pagesize;
	int _pagecount;
	int _datasize;
	int _currentpage;
	int _be_single;
};

inline void VocabularyTableModel::clear() {
	beginResetModel();
	_terms.clear();
	_rows.clear();
	endResetModel();
	_pagesize= 30;
	_pagecount= 0;
	_currentpage = 1;
	_datasize = 0;
}

inline int VocabularyTableModel::rowCount(const QModelIndex& parent) const {
	return GetCurrentSize();
}

inline int VocabularyTableModel::columnCount(const QModelIndex& parent) const {
	if(_datasize >0) {
	    if(_be_single)
			return 4;
		else
			return 6;
	}else
		return 0;
}

inline QVariant VocabularyTableModel::data(const QModelIndex& index, int role) const {
	if(role == Qt::DisplayRole) {
		return _rows[index.row()][index.column()];
	}
	return QVariant::Invalid;
}

inline QVariant VocabularyTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	const static QString headers[] = {"Word", "Text", "Pos", "Iid"};
	const static QString headers2[] = {"Word", "Word", "Text", "Text", "Pos", "Iid"};
	if(role == Qt::DisplayRole) { 
		if(orientation == Qt::Horizontal) {	
			if(_be_single)
			    return headers[section];
			else
				return headers2[section];
		}
	}
	return QVariant::Invalid;
}

inline int VocabularyTableModel::LoadData(vector<conex::Term*>& terms, int pagesize) {
	if(pagesize == 0 || terms.size() == 0)
		return 0;
	_pagesize = pagesize;
	_terms.swap(terms);
	_rows.clear();	
	return PageData();
	//LoadData();
}

inline int VocabularyTableModel::LoadData(vector<conex::Term*>& terms, vector<conex::Term*>& t2, int pagesize) {
	if(pagesize == 0 || terms.size() == 0 || t2.size() == 0)
		return 0;
	_pagesize = pagesize;
	_terms.swap(terms);
	_terms2.swap(t2);
	_rows.clear();	
	_be_single = false;
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
	// need to debug
	_rows.clear();	
	QModelIndex index;
	if(_pagecount > 0 && _currentpage > 0) {
		int count = 0;
		int start = (_currentpage - 1) * _pagesize;
		if(start < _datasize) {
			sfa s;
			if(_be_single) {
				for(vector<conex::Term*>::iterator it = _terms.begin() + start; it != _terms.end()&&count != GetCurrentSize() ; ++it, ++count) {
					vector<QString> row;
					conex::Term* term = *it;
					row.push_back(stdWToQString(term->indexWord()));
					string source = stdWToQString(term->text()).toUtf8();
					QString t = QString::fromUtf8( s.parse_source(source).content.c_str());
					row.push_back(t);
					row.push_back(QString(term->pos().c_str()));
					row.push_back(QString(term->id()->iid().c_str()));
					_rows.push_back(row);
				}
			}else {
				vector<conex::Term*>::iterator j = _terms2.begin() + start;
				for(vector<conex::Term*>::iterator it = _terms.begin() + start; it != _terms.end()&&count != GetCurrentSize() ; ++it, ++j, ++count) {
					vector<QString> row;
					conex::Term* term = *it;
					conex::Term* t2 = *j;
					row.push_back(stdWToQString(term->indexWord()));
					row.push_back(stdWToQString(t2->indexWord()));
					string source = stdWToQString(term->text()).toUtf8();
					QString t = QString::fromUtf8( s.parse_source(source).content.c_str());
					row.push_back(t);
					source = stdWToQString(t2->text()).toUtf8();
					t = QString::fromUtf8( s.parse_source(source).content.c_str());
					row.push_back(t);
					row.push_back(QString(term->pos().c_str()));
					row.push_back(QString(term->id()->iid().c_str()));
					_rows.push_back(row);
				}
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

/*! Convert an std::wstring to a QString */
inline QString VocabularyTableModel::stdWToQString(const std::wstring &str)
{
#ifdef _MSC_VER
	return QString::fromUtf16((const ushort *)str.c_str());
#else
	return QString::fromStdWString(str);
#endif
}

}
#endif
