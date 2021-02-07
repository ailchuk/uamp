#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include "libraryformdialog.h"

#define COLUMN_COUNT 2

void MainWindow::connectActions() {
    connect(ui->actionAdd_track, &QAction::triggered, this, &MainWindow::on_actionAdd_track_triggered);
    connect(ui->actionDelete_tracks, &QAction::triggered, this, [this]() {
        QModelIndexList indexes;
        qDebug() << "Delete songs";



        for (auto row : ui->playlistView->selectionModel()->selectedIndexes()) {
            QStandardItem* item = m_playListModel->itemFromIndex(row);
            QString path = item->data(1).toString();

            m_db->deleteTrack(path);
            qDebug() << path;
        }

        while((indexes = ui->playlistView->selectionModel()->selectedIndexes()).size()) {
            m_playListModel->removeRow(indexes.first().row());
            m_playlist->removeMedia(indexes.first().row());
        }
    });

    connect(ui->actionOpenLibrary, &QAction::triggered, this, [this]() {
        this->m_libraryform->show();
    });
}
