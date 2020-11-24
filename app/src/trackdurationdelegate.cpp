#include "trackdurationdelegate.h"

#include <QTime>


TrackDurationDelegate::TrackDurationDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}


QString TrackDurationDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(locale)

    int durationInMillisec = value.toInt();
    int remainingMillisec = durationInMillisec;
    int hours = durationInMillisec / 1000 / 60 / 60;
    remainingMillisec = durationInMillisec - (hours * 1000 * 60 * 60);
    int minutes = remainingMillisec / 1000 / 60;
    remainingMillisec = durationInMillisec - (minutes * 1000 * 60);
    int seconds = remainingMillisec / 1000;
    remainingMillisec = remainingMillisec - (seconds * 1000);

    QString format;
    if (hours)
    {
        format = "h:mm:ss";
    }
    else
    {
        format = "m:ss";
    }

    QTime time(hours, minutes, seconds);
    return time.toString(format);
}
