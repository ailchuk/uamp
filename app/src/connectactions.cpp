#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::connectActions() {
    connect(ui->actionAdd_track, &QAction::triggered, this, &MainWindow::on_actionAdd_track_triggered);
    connect(ui->actionDelete_tracks, &QAction::triggered, this, [this]() {
        QModelIndexList indexes;
        qDebug() << "Delete songs";

        bool output = true;
        for (auto row : indexes = ui->playlistView->selectionModel()->selectedIndexes()) {
            if (output == true) {
                qDebug() << "Song name";
                output = false;
            }
            else {
                qDebug() << "Song path";
                output = true;
            }
            qDebug() << (m_playListModel->itemFromIndex(row))->text();
        }

        while((indexes = ui->playlistView->selectionModel()->selectedIndexes()).size()) {
            m_playListModel->removeRow(indexes.first().row());
            m_playlist->removeMedia(indexes.first().row());
        }
    });
}
