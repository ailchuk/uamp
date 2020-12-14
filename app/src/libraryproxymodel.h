#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include <QSortFilterProxyModel>


class LibraryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit LibraryProxyModel(QObject *parent = nullptr);

    void setTrackIdColumn(int columnIndex) { trackIdColumn = columnIndex; }
    void setTitleColumn(int columnIndex) { titleColumn = columnIndex; }
    void setArtistColumn(int columnIndex) { artistColumn = columnIndex; }
    void setAlbumColumn(int columnIndex) { albumColumn = columnIndex; }
    void setTrackNumColumn(int columnIndex) { trackNumColumn = columnIndex; }
    void setYearColumn(int columnIndex) { yearColumn = columnIndex; }
    void setGenreColumn(int columnIndex) { genreColumn = columnIndex; }
    void setDurationColumn(int columnIndex) { durationColumn = columnIndex; }
    void setLocationColumn(int columnIndex) { locationColumn = columnIndex; }

    int getTrackIdColumn() const { return trackIdColumn; }
    int getTitleColumn() const { return titleColumn; }
    int getArtistColumn() const { return artistColumn; }
    int getAlbumColumn() const { return albumColumn; }
    int getTrackNumColumn() const { return trackNumColumn; }
    int getYearColumn() const { return yearColumn; }
    int getGenreColumn() const { return genreColumn; }
    int getDurationColumn() const { return durationColumn; }
    int getLocationColumn() const { return locationColumn; }

private:
    int trackIdColumn;
    int titleColumn;
    int artistColumn;
    int albumColumn;
    int trackNumColumn;
    int yearColumn;
    int genreColumn;
    int durationColumn;
    int locationColumn;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // LIBRARYMODEL_H
