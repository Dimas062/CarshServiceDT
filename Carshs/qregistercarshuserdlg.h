#ifndef QREGISTERCARSHUSERDLG_H
#define QREGISTERCARSHUSERDLG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include "../CarshService/service_widgets/qlinetext.h"

class QRegisterCarshUserDlg : public QDialog
{
    Q_OBJECT
public:
    QRegisterCarshUserDlg();
    QLabel * m_pLogoLabel;
    //QComboBox * m_pCarshsCombo;
    QLineText * m_pLoginLineText;
    QLineText * m_pPassLineText;
    QLineText * m_p1NameLineText;
    QLineText * m_p2NameLineText;
    QLineText * m_p3NameLineText;
    QLineText * m_pPositionLineText;
    QLineText * m_pMailLineText;
    QLineText * m_pTelegrammLineText;
    QLineText * m_pPhoneLineText;

public slots:
    void OnRegisterPressed();
};

#endif // QREGISTERCARSHUSERDLG_H
