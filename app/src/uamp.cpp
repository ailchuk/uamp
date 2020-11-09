    #include "uamp.h"
#include "./ui_uamp.h"

UAmp::UAmp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UAmp)
{
    ui->setupUi(this);
}

UAmp::~UAmp()
{
    delete ui;
}

