#ifndef QStickPARTNERCARDWIDGET_H
#define QStickPARTNERCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include "service/quleditdlg.h"

class QStickPartnerCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QStickPartnerCardWidget(QWidget *parent = nullptr);
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

    QULEditDlg m_ulDlg;

    QPushButton * m_pActivateButton;

    QListWidget * m_pPointsListWidget;
signals:

public slots:
    void OnActivAccountPressed();
    void OnULPressed();
};

#endif // QStickPARTNERCARDWIDGET_H
