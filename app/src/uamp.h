#ifndef UAMP_H
#define UAMP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UAmp; }
QT_END_NAMESPACE

class UAmp : public QMainWindow
{
    Q_OBJECT

public:
    UAmp(QWidget *parent = nullptr);
    ~UAmp();

private:
    Ui::UAmp *ui;
};
#endif // UAMP_H
