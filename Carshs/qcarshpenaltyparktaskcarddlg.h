#ifndef QCARSHPENALTYPARKTASKCARDDLG_H
#define QCARSHPENALTYPARKTASKCARDDLG_H

#include <QDialog>
#include "pictures/qpictureswidget.h"


class QCarshPenaltyParkTaskCardDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshPenaltyParkTaskCardDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QCARSHPENALTYPARKTASKCARDDLG_H
