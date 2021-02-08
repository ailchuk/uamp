#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include "libraryformdialog.h"

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
    });
}

void MainWindow::on_actionLight_triggered()
{
    ui->actionLight->setChecked(true);
    ui->actionDark->setChecked(false);

    this->setStyleSheet("color: black; background-color: #dde1e5;");
    ui->playlistView->setStyleSheet("QHeaderView::section { background-color:white } color: black; background-color: #dde1e5;");
}

void MainWindow::on_actionDark_triggered()
{
    ui->actionLight->setChecked(false);
    ui->actionDark->setChecked(true);

    this->setStyleSheet("color: white; background-color: #323336;");
    ui->playlistView->setStyleSheet("QHeaderView::section { background-color:#323336 } color: white; background-color: #323336;");
}
