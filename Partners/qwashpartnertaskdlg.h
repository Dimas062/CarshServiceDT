#ifndef QWashPARTNERTASKDLG_H
#define QWashPARTNERTASKDLG_H

#include <QDialog>
#include <QObject>
#include "pictures/qpictureswidget.h"

class QWashPartnerTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QWashPartnerTaskDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QWashPARTNERTASKDLG_H
