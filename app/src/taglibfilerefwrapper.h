#ifndef TAGLIBFILEREFWRAPPER_H
#define TAGLIBFILEREFWRAPPER_H

#include <taglib/fileref.h>

#include <QString>


class TagLibFileRefWrapper
{
/* Wrapper class for TagLib::FileRef to get the metadata
 * of a file located at the @filepath passed to constructor
*/
public:
    TagLibFileRefWrapper(QString filepath);
    bool fileRefIsNull() const;
    QString getTitle() const;
    QString getArtist() const;
    QString getAlbum() const;
    unsigned int getTrackNum() const;
    unsigned int getYear() const;
    QString getGenre() const;
    int getDurationInMilliseconds() const;
private:
    QString filePath;
    TagLib::FileRef fileRef;
};

#endif // TAGLIBFILEREFWRAPPER_H
