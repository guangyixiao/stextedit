#ifndef SIGNTREE_TABLE_MODEL_H
#define SIGNTREE_TABLE_MODEL_H

#include <QAbstractItemModel>
#include <QSize>
#include <vector>
#include "sfa_sign.h"
#include "StringCovert.h"


using namespace std;

namespace xpm {
	const int Column_Size = 3;
	class SignTreeModel : public QAbstractItemModel{
	public:
		SignTreeModel(QObject* object, sfa_sign* root = 0) :
			QAbstractItemModel(object), _root(root){}
		sfa_sign* root();
		void setRoot(sfa_sign* root);
		int rowCount(const QModelIndex& parent) const;
		int columnCount(const QModelIndex& parent) const;
		QVariant data(const QModelIndex& index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
	private:
		QVariant type2str(int type) const;
		QVariant type2value(int type, sfa_sign *item) const;
		void GetPage();
		sfa_sign* _root;
		vector<vector<QString> > _rows;
	};

	inline sfa_sign* SignTreeModel::root() {
		return _root;
	}

	inline void SignTreeModel::setRoot(sfa_sign* root) {
		_root = root;
	}
	inline int SignTreeModel::rowCount(const QModelIndex& parent) const {
		sfa_sign *parentItem;

		if (!parent.isValid())
			parentItem = _root;
		else
			parentItem = static_cast<sfa_sign*>(parent.internalPointer());

		int rowCount = parentItem->size();
		return rowCount;
	}

	inline int SignTreeModel::columnCount(const QModelIndex& parent) const {
		return Column_Size;
	}


	inline QVariant SignTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
		const QString headers[] = { "Name", "Type", "Value" };
		if (role == Qt::DisplayRole) {
			if (orientation == Qt::Horizontal)
				return headers[section];
		}
		return QVariant::Invalid;
	}

	inline Qt::ItemFlags SignTreeModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return 0;

		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
}
#endif
