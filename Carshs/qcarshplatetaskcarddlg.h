#ifndef QCARSHPLATETASKCARDDLG_H
#define QCARSHPLATETASKCARDDLG_H

#include <QDialog>
#include "pictures/qpictureswidget.h"

class QCarshPlateTaskCardDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshPlateTaskCardDlg(QString strTaskUuid);
   QPicturesWidget * m_pPicturesWidget;
};

#endif // QCARSHPLATETASKCARDDLG_H
