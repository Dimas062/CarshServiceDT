#ifndef QEMPLCARDWIDGET_H
#define QEMPLCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include "pictures/qpiclabel.h"

class QEmplCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEmplCardWidget(QWidget *parent = nullptr);
    void SetActivEmpl(QString strUuid);

    QLabel * m_pFIOLabel;
    QPicLabel * m_pFotoPicLabel;
    QLabel * m_pLabelActive;
    QImage m_FotoImage;
    QPushButton * m_pActivationButton;
    QString m_strCurUserId;
    bool m_IsActive;

signals:
public slots:
    void OnDriverCardPressed();
    void OnPassportPressed();
    void OnSertPressed();
    void OnActivPressed();
    void OnFotoPictureClik();
    void OnRemovePressed();
    void OnZakazsPressed();
};

#endif // QEMPLCARDWIDGET_H
