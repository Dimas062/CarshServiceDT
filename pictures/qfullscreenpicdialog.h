#ifndef QFULLSCREENPICDIALOG_H
#define QFULLSCREENPICDIALOG_H

#include <QDialog>
#include "imagewidget.h"

class QFullScreenPicDialog : public QDialog
{
    Q_OBJECT
public:
    QFullScreenPicDialog(QImage * pImage, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());


protected:
    ImageWidget * m_pImageWidget;
};

#endif // QFULLSCREENPICDIALOG_H
