#include "playercontrolsslider.h"

#include <QMouseEvent>
#include <QStyleOption>

PlayerControlsSlider::PlayerControlsSlider(QWidget *parent) : QSlider(parent)
{
}


void PlayerControlsSlider::mousePressEvent(QMouseEvent *event)
{
    /* Move the slider handle directly to position clicked on slider.
     * https://stackoverflow.com/questions/11132597/qslider-mouse-direct-jump
    */
    if (maximum() == minimum())
    {
        return;
    }

    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sliderHandle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    if (event->button() == Qt::LeftButton && !sliderHandle.contains(event->pos()))
    {
        int adaptedPosX = event->x();
        double halfHandleWidth = (0.5 * sliderHandle.width()) + 0.5;
        if (adaptedPosX < halfHandleWidth)
        {
            adaptedPosX = halfHandleWidth;
        }
        if (adaptedPosX > width() - halfHandleWidth)
        {
            adaptedPosX = width() - halfHandleWidth;
        }

        // Get new dimensions accounting for slider handle width.
        double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
        double normalizedPosition = (adaptedPosX - halfHandleWidth) / newWidth;
        setValue(minimum() + ((maximum() - minimum()) * normalizedPosition));

        event->accept();
        emit sliderMoved(value());
    }
    QSlider::mousePressEvent(event); // This allows the slider handle to remain slideable.
}
