#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::connectActions() {
    connect(ui->actionAdd_track, &QAction::triggered, this, &MainWindow::on_actionAdd_track_triggered);

}
