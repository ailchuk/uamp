#include "libraryformdialog.h"
#include "ui_libraryformdialog.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mytreewidgetitem.h"
#include <QTableWidget>
#include "QInputDialog"

libraryformdialog::libraryformdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::libraryformdialog)
{
    ui->setupUi(this);
    m_main_window = qobject_cast<MainWindow*>(parent);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);

    setupMenu();
}

void libraryformdialog::loadPlaylists()
{
//    m_db->PrintPlaylist();
    auto save = m_db->loadPlaylists();

    qDebug() << save.size();
    if (save.size() <= 0)
        return ;

    for (const auto &it : save)
    {
        if (ui->treeWidget->findItems(it.first, Qt::MatchContains|Qt::MatchRecursive, 0).size() > 0)
        {
            qDebug() << "find a copy, skipped";
            continue;
        }

        if (it.second != nullptr)
        {
            QFile file(it.second);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&file);
            QString line = in.readLine();
            MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "Item", "");

            topLevelItem1->setText(0, QFileInfo(it.second).baseName());
            ui->treeWidget->addTopLevelItem(topLevelItem1);
            ui->treeWidget->setCurrentItem(topLevelItem1);

            while (!line.isNull())
            {
                MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), QDir(line).dirName(), line);

                line = in.readLine();
            }
            file.close();
        }
        qDebug() << ">>>>>>>>>>>> Load from db playlists\n";
    }
}


void libraryformdialog::setupMenu() {
    m_menu.addAction("Rename playlist", this, []() {

    });
}

libraryformdialog::~libraryformdialog()
{
    delete ui;
}

void libraryformdialog::on_pushButtonOpenDir_clicked()
{
    QStringList trackPath = QFileDialog::getOpenFileNames(this,
                            tr("Open files"),
                            QString(),
                            tr("Audio Files (*.mp3; *.flac;)"));

    foreach (QString filePath, trackPath) {
        if (true/* add SQL querry here */) {
            MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), QDir(filePath).dirName(), filePath);
        }
    }
}

void libraryformdialog::on_pushButtonAddTracksFromQueue_clicked()
{
    auto mainPlaylistModel = this->m_main_window->m_playListModel;

    for (auto row : this->m_main_window->ui->playlistView->selectionModel()->selectedRows()) {
        QStandardItem* itemName = mainPlaylistModel->item(row.row());
        QStandardItem* itemPath = mainPlaylistModel->item(row.row(), 1);
        QString trackName = itemName->data(0).toString();
        QString trackPath = itemPath->data(0).toString();

        MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), trackName, trackPath);
    }
}

void libraryformdialog::on_pushButtonAddToQueue_clicked()
{
    for (auto row : this->ui->treeWidget->selectedItems()) {
        MyTreeWidgetItem *item = dynamic_cast<MyTreeWidgetItem *>(row);

        QList<QStandardItem *> items;
        items.append(new QStandardItem(item->Name));
        items.append(new QStandardItem(item->Path));

        if (item->Path.begin() != item->Path.end()) {
            this->m_main_window->m_playListModel->appendRow(items);
            this->m_main_window->m_playlist->addMedia(QUrl(item->Path));
            this->m_main_window->m_db->addToQueue(item->Path);
        }
    }
}

void libraryformdialog::on_pushButtonNewPlaylist_clicked()
{
    MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "New playlist", "");
    ui->treeWidget->addTopLevelItem(topLevelItem1);
    topLevelItem1->setText(0, "New playlist");
}

void libraryformdialog::on_pushButtonSavePlaylist_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    QMediaPlaylist *playlist = new QMediaPlaylist;
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "*.m3u");

    if (filename != nullptr)
    {
        for (int i = 0; i < item->childCount(); ++i)
        {
            MyTreeWidgetItem *items = dynamic_cast<MyTreeWidgetItem *>(item->child(i));

            playlist->addMedia(QUrl(items->Path));
            qDebug() << QString(items->Path);
        }
        playlist->save(QUrl::fromLocalFile(filename), "m3u");
    }

    bool save = m_db->SavePlaylist(filename, item->text(0));

    if (!save)
        QMessageBox::warning(this, "Warning!","Can't import playlist!");

    delete playlist;
}

void libraryformdialog::on_pushButtonDelete_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();

    if (item)
    {
        QString name = item->text(ui->treeWidget->currentColumn());

        item->~QTreeWidgetItem();
        bool is = m_db->DeletePlaylist(name);
        qDebug() << "--->>>> Delete " << name << "from DataBase" << is;
    }
}

void libraryformdialog::on_pushButtonRenamePlaylist_clicked()
{

    MyTreeWidgetItem *item = dynamic_cast<MyTreeWidgetItem *>(ui->treeWidget->currentItem());
    QString name = nullptr;

    if (!item || item->Path.begin() != item->Path.end())
        return;

    if (item)
        name = item->text(ui->treeWidget->currentColumn());
    else
        return;

    QString defaultText("new name");
    bool ok;

    qDebug() << "Playlist before renaming";
    m_db->PrintPlaylist();

    QString caseInput = QInputDialog::getText(this, tr("Rename"), ui->treeWidget->currentItem()->text(0), QLineEdit::Normal, defaultText, &ok);
    if (caseInput != nullptr) {
        if (!name.isEmpty()) {
        QString path = m_db->getPathPlaylist(name);
        if (!path.isEmpty()) {
            std::string new_path = QFileInfo(path).absolutePath().toStdString() + '/' + caseInput.toStdString() + ".m3u";
            QFile::rename(path, QString::fromStdString(new_path));
            m_db->DeletePlaylist(name);
            m_db->SavePlaylist(QString::fromStdString(new_path), caseInput);
            qDebug() << "new path: " << QString::fromStdString(new_path);
        }

        ui->treeWidget->currentItem()->setText(0, caseInput);

        qDebug() << "Rename playlist \n oldname: " << name << "new name: " << caseInput;
        }
    }
    qDebug() << "Playlist after renaming";
    m_db->PrintPlaylist();
}

void libraryformdialog::on_pushButtonImportPlaylist_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open m3u"), "", tr("(*.m3u)"));

    if (path != nullptr)
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        QString line = in.readLine();
        bool added = m_db->SavePlaylist(path, QFileInfo(path).baseName());

        if (added)
        {
            MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "Item", "");

            topLevelItem1->setText(0, QFileInfo(path).baseName());
            ui->treeWidget->addTopLevelItem(topLevelItem1);
            ui->treeWidget->setCurrentItem(topLevelItem1);

            while (!line.isNull())
            {
                MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), QDir(line).dirName(), line);

                line = in.readLine();
            }
            qDebug() << ">>>>>>>>>>>> Import playlist\n";
        }
        else
        {
            QMessageBox::warning(this, "Warning!","Can't import playlist!");
        }
    }
}
