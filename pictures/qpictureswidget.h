#ifndef QPICTURESWIDGET_H
#define QPICTURESWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>
#include "qpiclabel.h"
#include <QPushButton>
#include <QImage>

class QPicturesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPicturesWidget(QWidget *parent = nullptr , bool bViewOnly = false, bool bDesktop = false);
    //QVector<QString> m_PicturePaths;
    QVector<QImage> m_Pictures;
    QPicLabel * m_pCurrentPicLabel;
    QPushButton * m_pLeftButton;
    QPushButton * m_pRightButton;
    QPushButton * m_pRemoveButton;
    QVector<QImage>::iterator m_currentPictureIt;

    bool AddPicturePath(QString path, bool bUpdatePicture = true);
    bool AddImage(QImage&);
    bool RemovePicture();
    void UpdatePictures();
   // void ShowCurrentPic();
    bool m_bIsDesktop;
    bool m_bViewOnly;

private:
    //QImage m_CurrentImage;

public slots:
    void OnLeftButton();
    void OnRightButton();
    void OnPictureClik();
    void OnRemoveCurrFotoPressedSlot();


signals:

};

#endif // QPICTURESWIDGET_H
