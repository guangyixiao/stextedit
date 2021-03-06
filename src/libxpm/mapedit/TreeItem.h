#ifndef XPMDOC_GUI_MAPEDIT_TREEITEM_H
#define XPMDOC_GUI_MAPEDIT_TREEITEM_H

#include <qlist.h>
#include <qvariant.h>
#include "sfa.h"

namespace xpm {
	class TreeItem
 {
 public:
     TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
     ~TreeItem();

     void appendChild(TreeItem *child);

     TreeItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     int row() const;
     TreeItem *parent();
	 sfa_map_select *range();

 private:
     QList<TreeItem*> childItems;
     QList<QVariant> itemData;
     TreeItem *parentItem;
	 sfa_map_select _range;
 };

	inline sfa_map_select* TreeItem::range() {
		return &_range;
	}

}
#endif