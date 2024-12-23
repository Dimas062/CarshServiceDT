#ifndef QEMPLPENDLG_H
#define QEMPLPENDLG_H

#include <QDialog>
#include <QUuid>
#include <QString>
#include "../CarshService/service_widgets/qlinetext.h"
#include <QRadioButton>

class QEmplPenDlg : public QDialog
{
    Q_OBJECT
public:
    QEmplPenDlg(QUuid uuidPen , QString strIdUser);

    QUuid m_soursePenId;
    QString m_strUserId;

    QRadioButton * m_pRadioButtonP3;
    QRadioButton * m_pRadioButtonP2;
    QRadioButton * m_pRadioButtonP1;
    QRadioButton * m_pRadioButtonM3;
    QRadioButton * m_pRadioButtonM2;
    QRadioButton * m_pRadioButtonM1;

    QLineText * m_pLineComment;

public slots:
    void OnApplyPressed();
};

#endif // QEMPLPENDLG_H
