#include "XpmDocModel.h"
#include "StringCovert.h"
#include <qvector.h>

namespace xpm {
	XpmDocModel::XpmDocModel(sfa_model* doc, QObject* parent) : QAbstractItemModel(parent),
		_doc(doc) {
			QList<QVariant> rootData;
			rootData << "Text" << "Pos" << "Iid";
			rootItem = new TreeItem(rootData);
			setupModelData("");
	}

	XpmDocModel::~XpmDocModel()
	{
		delete rootItem;
	}

	int XpmDocModel::columnCount(const QModelIndex &parent) const
	{
		if (parent.isValid())
			return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
		else
			return rootItem->columnCount();
	}

	QVariant XpmDocModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role != Qt::DisplayRole)
			return QVariant();

		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

		return item->data(index.column());
	}

	Qt::ItemFlags XpmDocModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return 0;

		/*TreeItem *currentItem;
		currentItem = static_cast<TreeItem*>(index.internalPointer());
		if(currentItem->parent() == rootItem)
			return Qt::ItemIsEditable;
		else
			return Qt::ItemIsEditable | Qt::ItemIsSelectable;*/

		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}

	QVariant XpmDocModel::headerData(int section, Qt::Orientation orientation,
		int role) const
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
			return rootItem->data(section);

		return QVariant();
	}

	QModelIndex XpmDocModel::index(int row, int column, const QModelIndex &parent)
		const
	{
		if (!hasIndex(row, column, parent))
			return QModelIndex();

		TreeItem *parentItem;

		if (!parent.isValid())
			parentItem = rootItem;
		else
			parentItem = static_cast<TreeItem*>(parent.internalPointer());

		TreeItem *childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}

	QModelIndex XpmDocModel::parent(const QModelIndex &index) const
	{
		if (!index.isValid())
			return QModelIndex();

		TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
		TreeItem *parentItem = childItem->parent();

		if (parentItem == rootItem)
			return QModelIndex();

		return createIndex(parentItem->row(), 0, parentItem);
	}

	int XpmDocModel::rowCount(const QModelIndex &parent) const
	{
		TreeItem *parentItem;
		if (parent.column() > 0)
			return 0;

		if (!parent.isValid())
			parentItem = rootItem;
		else
			parentItem = static_cast<TreeItem*>(parent.internalPointer());

		return parentItem->childCount();
	}

	void XpmDocModel::setupModelData(string filter)
	{
		sfa_blocks termblocks = _doc->blocks();
		//append the block items
		for(sfa_blocks::iterator it = termblocks.begin(); it != termblocks.end(); ++it) {
			QString pos; 
			pos.setNum((*it)->ix());
			QString  blockStr("block:");
			QList<QVariant> blockColumnData;
			blockColumnData << blockStr << pos << tr("");
			TreeItem *blockItem = new TreeItem(blockColumnData, rootItem);
			rootItem->appendChild(blockItem);
			//append the term items
			for(sfa_maps::iterator j = (*it)->maps().begin(); j != (*it)->maps().end(); ++j) {
				QString text(StringCovert::stdWToQString(j->word));
				//QString definition(StringCovert::stdWToQString(j->term()->text()));
				QString iid(j->id.c_str());
				QList<QVariant> termColumnData;
				termColumnData << text;
				termColumnData << j->word_ix;
				termColumnData << iid;
				TreeItem *termItem = new TreeItem(termColumnData, blockItem);
				blockItem->appendChild(termItem);
				sfa_select_map* range  = termItem->range();
				range->block_ix = (*it)->ix();
				range->word_ix = j->word_ix + (*it)->ix();
				range->word = j->word;
				range->id = j->id;
			}
		}
	}
}