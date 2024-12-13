#ifndef QPLATEPARTNERCARDWIDGET_H
#define QPLATEPARTNERCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>

class QPlatePartnerCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPlatePartnerCardWidget(QWidget *parent = nullptr);
    void SetActivPartner(QString strUuid);

    QLabel * m_pNameLabel;
    QLabel * m_pPostavshikLabel;
    QLabel * m_pAdressLabel;
    QLabel * m_pINNLabel;
    QLabel * m_pBankLabel;
    QLabel * m_pScetLabel;

    QListWidget * m_pPointsListWidget;
signals:
};

#endif // QPLATEPARTNERCARDWIDGET_H
