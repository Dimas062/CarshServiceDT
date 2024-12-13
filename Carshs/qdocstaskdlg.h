#ifndef QDOCSTASKDLG_H
#define QDOCSTASKDLG_H

#include <QDialog>
#include <QObject>
#include "pictures/qpictureswidget.h"

class QDocsTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QDocsTaskDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QDOCSTASKDLG_H
