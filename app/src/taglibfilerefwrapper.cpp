#include "taglibfilerefwrapper.h"

#include <QFileInfo>

TagLibFileRefWrapper::TagLibFileRefWrapper(QString fp) : filePath(fp), fileRef(fp.toStdWString().c_str())
{
}


bool TagLibFileRefWrapper::fileRefIsNull() const
{
    return fileRef.isNull();
}


QString TagLibFileRefWrapper::getTitle() const
{
    QString title = fileRef.tag()->title().toCString(true);
    if (title == "")
    {
        title = QFileInfo(filePath).completeBaseName();
    }

    return title;
}


QString TagLibFileRefWrapper::getArtist() const
{
    return fileRef.tag()->artist().toCString(true);
}


QString TagLibFileRefWrapper::getAlbum() const
{
    return fileRef.tag()->album().toCString(true);
}


unsigned int TagLibFileRefWrapper::getTrackNum() const
{
    return fileRef.tag()->track();
}


unsigned int TagLibFileRefWrapper::getYear() const
{
    return fileRef.tag()->year();
}


QString TagLibFileRefWrapper::getGenre() const
{
    return fileRef.tag()->genre().toCString(true);
}


int TagLibFileRefWrapper::getDurationInMilliseconds() const
{
    return fileRef.audioProperties()->lengthInMilliseconds();
}
