#include "qpiclabel.h"
#include "qapplication.h"
#include "qevent.h"
#include "qpainter.h"



QPicLabel::QPicLabel(QWidget *parent, Qt::WindowFlags f )
    : QLabel(parent,f)
{
    // const QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    // int iMinWidth = screenGeometry.width()*0.47;
    // int iMinHeight = screenGeometry.height() - 360;/*screenGeometry.height()*0.47*/
    // setMinimumSize(iMinWidth,iMinHeight);

    m_pCurrentImage = NULL;
}

QPicLabel::~QPicLabel()
{
}

void QPicLabel::mousePressEvent(QMouseEvent* event)
{
    qDebug()<<"QPicLabel::mousePressEvent(QMouseEvent* event)"<<event;
    if(event->type() == QEvent::MouseButtonPress)
    {
        const qreal iw = m_pCurrentImage->width();
        const qreal ih = m_pCurrentImage->height();
        const qreal wh = height()-20;
        const qreal ww = width()-20;

        double wStartScaleFactor = ww/iw;
        double hStartScaleFactor = wh/ih;

        double scaleFactor = 1;

        wStartScaleFactor<hStartScaleFactor?scaleFactor=wStartScaleFactor:scaleFactor=hStartScaleFactor;

        int iPicWidth = iw*scaleFactor;
        int iPicHeight = ih*scaleFactor;

        int x = event->position().x();
        int y = event->position().y();

        if( (x<((ww - iPicWidth)/2)) || (x>((ww - iPicWidth)/2) + iPicWidth))
        {
            QLabel::mousePressEvent(event);
            return;
        }
        if( (y<((wh - iPicHeight)/2)) || (y>((wh - iPicHeight)/2) + iPicHeight))
        {
            QLabel::mousePressEvent(event);
            return;
        }
        qDebug()<<"emit clicked();";
        emit clicked();
    }
}

void QPicLabel::SetCurrentImage(QImage * img)
{
    qDebug()<<"PicLabel::SetCurrentImage(QImage * img)";
    m_pCurrentImage = img;
    update();
}

void QPicLabel::paintEvent(QPaintEvent*)
{
    if(!m_pCurrentImage) return;

    QPainter p(this);

    const qreal iw = m_pCurrentImage->width();
    const qreal ih = m_pCurrentImage->height();
    const qreal wh = height()-20;
    const qreal ww = width()-20;

    double wStartScaleFactor = ww/iw;
    double hStartScaleFactor = wh/ih;

    double scaleFactor = 1;

    wStartScaleFactor<hStartScaleFactor?scaleFactor=wStartScaleFactor:scaleFactor=hStartScaleFactor;

    p.translate(ww / 2, wh / 2);

    p.scale(1 * scaleFactor, 1 * scaleFactor);
    p.translate(-iw / 2, -ih / 2);
    p.drawImage(10/scaleFactor, 10/scaleFactor, *m_pCurrentImage);

}
