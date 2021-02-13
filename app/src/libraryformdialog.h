#ifndef LIBRARYFORMDIALOG_H
#define LIBRARYFORMDIALOG_H

#include <QDialog>
#include "QMenu"
#include "database.h"

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

    void setDataBase(DataBase &db) { m_db = &db; }

    void loadPlaylists();

private slots:
    void on_pushButtonOpenDir_clicked();

    void on_pushButtonAddTracksFromQueue_clicked();

    void on_pushButtonAddToQueue_clicked();

    void on_pushButtonNewPlaylist_clicked();

    void on_pushButtonSavePlaylist_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonRenamePlaylist_clicked();

    void on_pushButtonImportPlaylist_clicked();

private:
    Ui::libraryformdialog *ui;

    MainWindow* m_main_window = nullptr;
    QMenu m_menu;
    DataBase *m_db;

private:
    void setupMenu();
};

#endif // LIBRARYFORMDIALOG_H
