#ifndef QPOINTEDITDLG_H
#define QPOINTEDITDLG_H

#include <QDialog>
#include <QObject>
#include "../CarshService/service_widgets/qlinetext.h"

class QPointEditDlg : public QDialog
{
    Q_OBJECT
public:
    QPointEditDlg();

    QLineText * m_pPointNameLineText;
    QLineText * m_pPointAdressLineText;


    QString SaveUpdateToBD();
    QString SaveNewToBD(QString strPartnerUuid);
    void LoadFromBD(QString strUuid);

    QString m_strIdPoint;

protected:
    QString m_strIdPartner;

public slots:
    void OnApplyPressed();
};

#endif // QPOINTEDITDLG_H
