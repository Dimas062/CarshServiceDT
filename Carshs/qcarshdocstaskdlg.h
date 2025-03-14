#ifndef QCARSHDOCSTASKDLG_H
#define QCARSHDOCSTASKDLG_H

#include <QDialog>
#include <QObject>
#include "pictures/qpictureswidget.h"

class QCarshDocsTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshDocsTaskDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QCARSHDOCSTASKDLG_H
