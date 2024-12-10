#ifndef QCARSHPARKINGTASKCARDDLG_H
#define QCARSHPARKINGTASKCARDDLG_H

#include <QDialog>
#include "pictures/qpictureswidget.h"

class QCarshParkingTaskCardDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshParkingTaskCardDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QCARSHPARKINGTASKCARDDLG_H
