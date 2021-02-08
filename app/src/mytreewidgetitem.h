#ifndef MYTREEWIDGETITEM_H
#define MYTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class MyTreeWidgetItem : public QTreeWidgetItem {
public:
//    explicit QTreeWidgetItem(QTreeWidget *treeview, int type = Type);
    explicit MyTreeWidgetItem(QTreeWidget *parent = nullptr, QString name = "", QString path = "")
        : QTreeWidgetItem(parent), Name(name), Path(path) {}
    MyTreeWidgetItem(QTreeWidgetItem *parent, QString name, QString path)
        : QTreeWidgetItem(parent), Name(name), Path(path) {}
    ~MyTreeWidgetItem() = default;

    QString Name;
    QString Path;

    static int addToItemHandler(QTreeWidgetItem* item, QString fileName, QString filePath) {
        if (item) {
            MyTreeWidgetItem *topLevelItem1 =
                    new MyTreeWidgetItem(item, fileName, filePath);
            topLevelItem1->setText(0, fileName);
            return 0;
        }
        return 1;
    }
};

#endif // MYTREEWIDGETITEM_H
