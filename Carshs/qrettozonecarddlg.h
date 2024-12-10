#ifndef QRETTOZONECARDDLG_H
#define QRETTOZONECARDDLG_H

#include <QDialog>
#include <QObject>
#include "pictures/qpictureswidget.h"

class QRetToZoneCardDlg : public QDialog
{
    Q_OBJECT
public:
    QRetToZoneCardDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
};

#endif // QRETTOZONECARDDLG_H
