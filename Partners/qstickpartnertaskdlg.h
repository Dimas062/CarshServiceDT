#ifndef QSTICKPARTNERTASKDLG_H
#define QSTICKPARTNERTASKDLG_H

#include <QDialog>
#include <QObject>
#include "pictures/qpictureswidget.h"

class QStickPartnerTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QStickPartnerTaskDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QSTICKPARTNERTASKDLG_H
