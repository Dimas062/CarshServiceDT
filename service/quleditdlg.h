#ifndef QULEDITDLG_H
#define QULEDITDLG_H

#include <QDialog>
#include "../CarshService/service_widgets/qlinetext.h"

class QULEditDlg : public QDialog
{
    Q_OBJECT
public:
    QULEditDlg();
    QString m_strIdUL;

    QLineText * m_pFormaLineText;
    QLineText * m_pNazvanieLineText;
    QLineText * m_pAdressLineText;
    QLineText * m_pINNLineText;
    QLineText * m_pBankLineText;
    QLineText * m_pSchetLineText;
    QLineText * m_pKPPLineText;
    QLineText * m_pBIKLineText;
    QLineText * m_pKorrSchetLineText;
    QLineText * m_pFamDirLineText;
    QLineText * m_pNameDirLineText;
    QLineText * m_pOtchDirLineText;

    void UpdateData();
    QString SaveToBD();
    void LoadFromBD(QString strUuid);


public slots:
    void OnApplyCardPressed();

};

#endif // QULEDITDLG_H
