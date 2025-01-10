#ifndef QPLATEPARTNERCARDWIDGET_H
#define QPLATEPARTNERCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include "service/quleditdlg.h"

class QPlatePartnerCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPlatePartnerCardWidget(QWidget *parent = nullptr);
    void SetActivPartner(QString strUuid);

    QString m_strActivPartner;
    bool m_bIsAccountActiv;

    QLabel * m_pTopLabel;
    QLabel * m_pActivLabel;
    QLabel * m_pNameLabel;
    QLabel * m_pPostavshikLabel;
    QLabel * m_pAdressLabel;
    QLabel * m_pINNLabel;
    QLabel * m_pBankLabel;
    QLabel * m_pScetLabel;
    QLabel * m_pLoginLabel;
    QLabel * m_pPasswordLabel;

    QPushButton * m_pActivateButton;

    QULEditDlg m_ulDlg;

    QListWidget * m_pPointsListWidget;
signals:

public slots:
    void OnActivAccountPressed();
    void OnULPressed();
};

#endif // QPLATEPARTNERCARDWIDGET_H
