#include "qfullscreenpicdialog.h"
#include "qapplication.h"
#include "qscreen.h"
#include <QApplication>

QFullScreenPicDialog::QFullScreenPicDialog(QImage * pImage,QWidget *parent, Qt::WindowFlags f ):QDialog(parent , f)
{
    m_pImageWidget = new ImageWidget(pImage,this);

    const QRect screenGeometry(0, 0, qApp->screens()[0]->size().width() , qApp->screens()[0]->size().height());
    QRect geometry(QPoint(0, 0), QSize(screenGeometry.width() /** 3 / 4*/, screenGeometry.height() /** 3 / 4*/));
    geometry.moveCenter(screenGeometry.center());
    setGeometry(geometry);
    m_pImageWidget->setGeometry(this->geometry());

}
