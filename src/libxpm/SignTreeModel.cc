#include "SignTreeModel.h"

namespace xpm {
	QVariant SignTreeModel::data(const QModelIndex& index, int role) const {
		if (!index.isValid())
			return QVariant();

		if (role == Qt::SizeHintRole) {
			switch (index.column()) {
			case 0:
				return QSize(250, 35);
			case 1:
				return QSize(80, 35);
			default:
				return QSize(100, 35);
			}
		}
		else if (role != Qt::DisplayRole)
			return QVariant();

		sfa_sign *item = static_cast<sfa_sign*>(index.internalPointer());

		int type = item->type();

		switch (index.column()) {
		case 0:
			return StringCovert::stdWToQString(item->name_str());
		case 1:
			return type2str(type);
		case 2:
			return type2value(type, item);
		default:
			return StringCovert::stdWToQString(item->name_str());
		}
	}

	QModelIndex SignTreeModel::index(int row, int column, const QModelIndex &parent) const
	{
		if (parent.isValid() && parent.column() != 0)
			return QModelIndex();

		sfa_sign *parentItem;

		if (!parent.isValid())
			parentItem = _root;
		else
			parentItem = static_cast<sfa_sign*>(parent.internalPointer());

		sfa_sign *childItem = parentItem->at(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}

	QModelIndex SignTreeModel::parent(const QModelIndex &index) const
	{
		if (!index.isValid())
			return QModelIndex();

		sfa_sign *childItem = static_cast<sfa_sign*>(index.internalPointer());
		sfa_sign *parentItem = childItem->parent();

		if (parentItem == _root)
			return QModelIndex();

		int row = 0;
		if (parentItem->parent() != _root) {
			for (int i = 0; i < parentItem->parent()->size(); ++i) {
				if (parentItem->parent()->at(i) == parentItem) {
					row = i;
					break;
				}
			}
		}
		return createIndex(row, 0, parentItem);
	}

	QVariant SignTreeModel::type2value(int type, sfa_sign* item) const {
		switch (type) {
		case SFA_STRING:
			return StringCovert::stdWToQString(item->value_as_string());
		case SFA_NUMBER:
			return QString::number(item->value_as_number());
		case SFA_TRUE:
			return QString("true");
		case SFA_FALSE:
			return tr("false");
		case SFA_NULL:
			return tr("null");
		case SFA_OBJECT:
			return QString::number(item->size());
		case SFA_ARRAY:
			return QString::number(item->size());
		}
	}

	QVariant SignTreeModel::type2str(int type) const {
		switch (type) {
		case SFA_STRING:
			return tr("string");
		case SFA_NUMBER:
			return tr("number");
		case SFA_TRUE:
		case SFA_FALSE:
			return tr("bool");
		case SFA_NULL:
			return tr("null");
		case SFA_OBJECT:
			return tr("object");
		case SFA_ARRAY:
			return tr("array");
		}
	}
}
