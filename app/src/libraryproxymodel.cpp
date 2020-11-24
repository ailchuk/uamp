#include "libraryproxymodel.h"


LibraryProxyModel::LibraryProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}


Qt::ItemFlags LibraryProxyModel::flags(const QModelIndex &index) const
{
/* Make trackId, duration, and location columns read-only. */
    Qt::ItemFlags flags = QAbstractProxyModel::flags(index);
    if (index.column() == trackIdColumn || index.column() == durationColumn || index.column() == locationColumn)
    {
        flags &= ~Qt::ItemIsEditable;
    }
    return flags;
}

