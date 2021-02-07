#ifndef LIBRARYFORMDIALOG_H
#define LIBRARYFORMDIALOG_H

#include <QDialog>
#include "QMenu"

namespace Ui {
class libraryformdialog;
}

class MainWindow;

class libraryformdialog : public QDialog
{
    Q_OBJECT

public:
    explicit libraryformdialog(QWidget *parent = nullptr);
    ~libraryformdialog();

private:
    Ui::libraryformdialog *ui;
    MainWindow* m_main_window = nullptr;
    QMenu m_menu;

private:
    void setupMenu();
};

#endif // LIBRARYFORMDIALOG_H
