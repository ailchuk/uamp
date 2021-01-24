#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::on_actionAdd_track_triggered()
{
    // Using the file selection dialog to make multiple selections of mp3 files
        QStringList files = QFileDialog::getOpenFileNames(this,
                                                          tr("Open files"),
                                                          QString(),
                                                          tr("Audio Files (*.mp3; *.flac)"));



        // Next, set the data names and file paths
        // into the playlist and table displaying the playlist
        foreach (QString filePath, files) {
            QList<QStandardItem *> items;
            items.append(new QStandardItem(QDir(filePath).dirName()));
            items.append(new QStandardItem(filePath));
            m_playListModel->appendRow(items);
            m_playlist->addMedia(QUrl(filePath));

        }
    qDebug() << "=>>>>>>>>>> open send";
}

