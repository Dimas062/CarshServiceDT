#ifndef QWashPARTNERCARDWIDGET_H
#define QWashPARTNERCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>

class QWashPartnerCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QWashPartnerCardWidget(QWidget *parent = nullptr);
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

    QListWidget * m_pPointsListWidget;
signals:

public slots:
    void OnActivAccountPressed();
};

#endif // QWashPARTNERCARDWIDGET_H
