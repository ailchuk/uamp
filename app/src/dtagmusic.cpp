#include "dtagmusic.h"
#include "ui_dtagmusic.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QCheckBox>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include "unsynchronizedlyricsframe.h"
#include "synchronizedlyricsframe.h"

static void setNewImg(const QString& pathFile, const QString& pathImg) {
    TagLib::MPEG::File file(pathFile.toUtf8().constData());
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);
    if (tag) {
        TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;
        frame->setMimeType(pathImg.toUtf8().data());
        QImage image(pathImg);
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
            if (pathImg.right(3).toUpper() == "PEG") {
                if (!image.save(&buffer, "JPEG")) {
                    buffer.close();
                    return;
                }
            }
            else if (pathImg.right(3).toUpper() == "PNG"){
                if (!image.save(&buffer, "PNG")) {
                    buffer.close();
                    return;
                }
            }
            else {
                if (!image.save(&buffer, "IMG")) {
                    buffer.close();
                    return;
                }
            }
        buffer.close();
        frame->setPicture(TagLib::ByteVector( bytes.data(), bytes.count() ));
            tag->removeFrames("APIC");
        tag->addFrame(frame);
        file.save();
    }
}

DTagMusic::DTagMusic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DTagMusic)
{
    ui->setupUi(this);
    ui->m_artist->setReadOnly(false);
    ui->m_Albom->setReadOnly(false);
    ui->m_Genre->setReadOnly(false);
    ui->m_Title->setReadOnly(false);
    if (ui->m_Path->text().right(3).toUpper() == "MP3")
        ui->m_Lyric->setReadOnly(false);
}

static QString getLyric(TagLib::ID3v2::Tag *tag) {
    TagLib::ID3v2::FrameList frames = tag->frameListMap()["USLT"];
    TagLib::ID3v2::UnsynchronizedLyricsFrame *frame = NULL;
    QString lyric;

    if (!frames.isEmpty()) {
        frame = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frames.front());
        if (frame)
            lyric = frame->text().toCString();
    }
    return lyric;
}

void DTagMusic::setTagWindow(const QString& path) {
    ui->m_Path->setText(path);
    m_coverImage.load(":/cover.jpg");
    {
        ui->ButtonChengeImg->setEnabled(true);
        m_name.clear();
        TagLib::FileRef f(path.toUtf8().constData());
        QString fileType = path.right(3).toUpper();

        if (fileType == "MP3") {
            TagLib::MPEG::File file(path.toUtf8().constData());
            TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

            if(tag) {
                TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];

                if (!frameList.isEmpty()) {
                    TagLib::ID3v2::AttachedPictureFrame *coverImg = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
                    m_coverImage.loadFromData((const uchar *) coverImg->picture().data(), coverImg->picture().size());
                }
                ui->m_Lyric->setText(getLyric(tag));
            }
        }
        else {
            ui->ButtonChengeImg->setEnabled(false);
        }
        ui->m_artist->setText(f.tag()->artist().toCString());
        ui->m_Albom->setText(f.tag()->album().toCString());
        ui->m_Genre->setText(f.tag()->genre().toCString());
        ui->m_Title->setText(f.tag()->title().toCString());
    }
}

DTagMusic::~DTagMusic()
{
    delete ui;
}

void DTagMusic::on_ButtonChengeImg_clicked()
{
    if (ui->m_Path->text().size()) {
        QString path = QFileDialog::getOpenFileName(this, tr("Open Track"), QDir::currentPath(),
                                    tr("Images(*.png *.img *.jpeg)"));
        if (path.size()) {
            QFile file(path);
            if (!file.open(QIODevice::ReadWrite))
                return;
            setNewImg(ui->m_Path->text(), path);
            file.close();
        }
    }
}
void DTagMusic::on_ButtonSaveTags_clicked()
{

    TagLib::FileRef f(ui->m_Path->text().toUtf8().constData());
    if(f.isNull())
        return;
    f.tag()->setArtist(ui->m_artist->text().toStdString());
    f.tag()->setAlbum(ui->m_Albom->text().toStdString());
    f.tag()->setGenre(ui->m_Genre->text().toStdString());
    f.tag()->setTitle(ui->m_Title->text().toStdString());
    f.file()->save();

    if (ui->m_Path->text().right(3).toUpper() == "MP3") {
        TagLib::MPEG::File file(ui->m_Path->text().toUtf8().data());
        TagLib::ID3v2::FrameList frames = file.ID3v2Tag()->frameListMap()["USLT"];
        TagLib::ID3v2::UnsynchronizedLyricsFrame *frame = new TagLib::ID3v2::UnsynchronizedLyricsFrame;
        if (!file.ID3v2Tag()->frameListMap()["USLT"].isEmpty())
            file.ID3v2Tag()->removeFrames(file.ID3v2Tag()->frameListMap()["USLT"].front()->frameID());
        frame->setText(ui->m_Lyric->toPlainText().toUtf8().data());
        file.ID3v2Tag()->addFrame(frame);
        file.save();
    }
}
