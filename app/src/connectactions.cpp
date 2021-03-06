#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include "libraryformdialog.h"
#include "dtagmusic.h"

#define COLUMN_COUNT 2

void MainWindow::connectActions() {
    connect(ui->actionAdd_track, &QAction::triggered, this, &MainWindow::on_actionAdd_track_triggered);
    connect(ui->actionDelete_tracks, &QAction::triggered, this, [this]() {
        qDebug() << "Delete songs";

        for (auto row : ui->playlistView->selectionModel()->selectedRows()) {
            QStandardItem* itemName = m_playListModel->item(row.row());
            QStandardItem* itemPath = m_playListModel->item(row.row(), 1);
            QString trackName = itemName->data(0).toString();
            QString trackPath = itemPath->data(0).toString();

            m_db->deleteTrack(trackPath);
            qDebug() << trackPath;
            qDebug() << trackName;
        }

        QModelIndexList indexes;
        while((indexes = ui->playlistView->selectionModel()->selectedIndexes()).size()) {
            m_playListModel->removeRow(indexes.first().row());
            m_playlist->removeMedia(indexes.first().row());
        }
    });

    connect(ui->actionOpenLibrary, &QAction::triggered, this, [this]() {
        this->m_libraryform->show();
        m_libraryform->loadPlaylists();
    });

    connect(ui->actionOpenDtag, &QAction::triggered, this, [this]() {
        this->m_dtag->show();
    });
}

