#ifndef QEMPLCOSTDLG_H
#define QEMPLCOSTDLG_H

#include <QDialog>
#include <QString>
#include "pictures/qpictureswidget.h"

class QEmplCostDlg : public QDialog
{
public:
    QEmplCostDlg(QString strCostId);
    QString m_sourceCost;
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QEMPLCOSTDLG_H
