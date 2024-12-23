#ifndef QWASHPARTNERPENALTYDLG_H
#define QWASHPARTNERPENALTYDLG_H

#include <QString>
#include <QDialog>
#include <QLineEdit>
#include "pictures/qpictureswidget.h"

class QWashPartnerPenaltyDlg: public QDialog
{
    Q_OBJECT
public:
    QWashPartnerPenaltyDlg(QString strIdTask , QString strIdType, bool bNigth);

    QLineEdit * m_pCountLineEdit;
    QPicturesWidget * m_pPicturesWidget;
    QString m_strTaskId;
    QString m_strIdType;
    QString m_strSourcePenId;
    bool m_bNigth;

public slots:
    void OnAddPicPressed();
    void OnApplyPressed();
    void OnDelPicPressed();
};

#endif // QWASHPARTNERPENALTYDLG_H
