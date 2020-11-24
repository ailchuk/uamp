#ifndef PLAYERCONTROLSSLIDER_H
#define PLAYERCONTROLSSLIDER_H

#include <QSlider>

class PlayerControlsSlider : public QSlider
{
public:
    PlayerControlsSlider(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLAYERCONTROLSSLIDER_H
