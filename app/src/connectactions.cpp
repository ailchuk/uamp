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
                m_db->deleteTrack(m_playListModel->itemFromIndex(row)->text());
            }
            qDebug() << (m_playListModel->itemFromIndex(row))->text();
        }

        while((indexes = ui->playlistView->selectionModel()->selectedIndexes()).size()) {
            m_playListModel->removeRow(indexes.first().row());
            m_playlist->removeMedia(indexes.first().row());
        }
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
