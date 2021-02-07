#include "libraryformdialog.h"
#include "ui_libraryformdialog.h"
#include "mainwindow.h"
#include "mytreewidgetitem.h"

libraryformdialog::libraryformdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::libraryformdialog)
{
    ui->setupUi(this);
    m_main_window = qobject_cast<MainWindow*>(parent);

    setupMenu();
}

void libraryformdialog::setupMenu() {
//    m_menu.addAction("", this, []() {

//    });
    MyTreeWidgetItem *topLevelItem = new MyTreeWidgetItem(ui->treeWidget, "playlist", "playlist path");
    MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "playlist2", "playlist path2");
    ui->treeWidget->addTopLevelItem(topLevelItem);
    ui->treeWidget->addTopLevelItem(topLevelItem1);
    topLevelItem->setText(0, "playlist text of top level item");

    connect(ui->pushButtonOpenDir, &QPushButton::clicked, this, [this](){
        QStringList trackPath = QFileDialog::getOpenFileNames(this,
                                tr("Open files"),
                                QString(),
                                tr("Audio Files (*.mp3; *.flac)"));

        foreach (QString filePath, trackPath) {
            if (true/* add SQL querry here */) {
                QString fileName = QDir(filePath).dirName();
                MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget->currentItem(), fileName, filePath);
                topLevelItem1->setText(0, fileName);
            }
        }
    });

    connect(ui->pushButtonAddTracksFromQueue, &QPushButton::clicked, this, [this](){

    });

    connect(ui->pushButtonAddToQueue, &QPushButton::clicked, this, [this](){

    });

    connect(ui->pushButtonSavePlaylist, &QPushButton::clicked, this, [this](){

    });

    connect(ui->pushButtonDeletePlaylist, &QPushButton::clicked, this, [this](){

    });

    connect(ui->pushButtonDeleteSongFromPlaylist, &QPushButton::clicked, this, [this](){

    });
}

libraryformdialog::~libraryformdialog()
{
    delete ui;
}
