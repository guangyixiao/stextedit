#ifndef XPM_DOC_GUI_MAPEDIT_XPMDOCMODEL_H
#define XPM_DOC_GUI_MAPEDIT_XPMDOCMODEL_H

#include <QtGui>
#include <qabstractitemmodel.h>
#include "sfa.h"
#include "TreeItem.h"

namespace xpm {
	class XpmDocModel : public QAbstractItemModel {
		Q_OBJECT
	public:
		XpmDocModel(sfa_model* doc, QObject* parent = 0);
		~XpmDocModel();

		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

		virtual QModelIndex parent(const QModelIndex &child) const;		
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		TreeItem* GetRootItem();
	private:
		void setupModelData(string filter = "");
		sfa_model* _doc;		
		TreeItem *rootItem;
	};

	inline TreeItem* XpmDocModel::GetRootItem() {
		return rootItem;
	}
}

#endif