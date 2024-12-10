/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "imagewidget.h"

#include <QDir>
#include <QImageReader>
#include <QGestureEvent>
#include <QPainter>
#include <QTime>
#include "common.h"

//Q_LOGGING_CATEGORY(lcExample, "qt.examples.imagegestures")

//! [constructor]
ImageWidget::ImageWidget(/*QVector<QString> & picturesPaths ,QVector<QString>::const_iterator&  currentPictureIt ,*/const QImage * pImage , QWidget *parent)
    : QWidget(parent)/*,  m_currentPictureIt(currentPictureIt), m_picturesPaths(picturesPaths)*/, horizontalOffset(0)
    , verticalOffset(0), rotationAngle(0), scaleFactor(1), currentStepScaleFactor(1)
{
   setMinimumSize(QSize(200, 200));

   currentImage = *pImage;

   m_bIsFirstPaint = true;

   m_iLastClicMsecs = -3000;

   update();
}
//! [constructor]


void ImageWidget::grabGestures(const QList<Qt::GestureType> &gestures)
{
    //! [enable gestures]
    for (Qt::GestureType gesture : gestures)
        grabGesture(gesture);
    //! [enable gestures]
}


bool ImageWidget::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}



void ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    const qreal iw = currentImage.width();
    const qreal ih = currentImage.height();
    const qreal wh = height();
    const qreal ww = width();

    if(m_bIsFirstPaint)
    {
        double wStartScaleFactor = ww/iw;
        double hStartScaleFactor = wh/ih;

        wStartScaleFactor<hStartScaleFactor?scaleFactor=wStartScaleFactor:scaleFactor=hStartScaleFactor;
        m_bIsFirstPaint = false;
    }

    p.translate(ww / 2, wh / 2);
    p.translate(horizontalOffset, verticalOffset);
    p.rotate(rotationAngle);
    p.scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
    p.translate(-iw / 2, -ih / 2);
    p.drawImage(0, 0, currentImage);
}


void ImageWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    // rotationAngle = 0;
    // scaleFactor = 1;
    // currentStepScaleFactor = 1;
    // verticalOffset = 0;
    // horizontalOffset = 0;
    // update();
}

void ImageWidget::mousePressEvent(QMouseEvent *)
{
     // int curClicMsecs = QTime::currentTime().msecsSinceStartOfDay();
     // if(curClicMsecs - 500 < m_iLastClicMsecs)
     // {
     //     const qreal iw = currentImage.width();
     //     const qreal ih = currentImage.height();
     //     const qreal wh = height();
     //     const qreal ww = width();
     //     double wStartScaleFactor = ww/iw;
     //     double hStartScaleFactor = wh/ih;
     //     wStartScaleFactor<hStartScaleFactor?scaleFactor=wStartScaleFactor:scaleFactor=hStartScaleFactor;
     //     rotationAngle = 0;
     //     currentStepScaleFactor = 1;
     //     verticalOffset = 0;
     //     horizontalOffset = 0;
     //     update();
     // }


     // m_iLastClicMsecs = curClicMsecs;
}


bool ImageWidget::gestureEvent(QGestureEvent *event)
{
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    else if (QGesture *pan = event->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}


void ImageWidget::panTriggered(QPanGesture *gesture)
{
#ifndef QT_NO_CURSOR
    switch (gesture->state()) {
        case Qt::GestureStarted:
        case Qt::GestureUpdated:
            setCursor(Qt::SizeAllCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
    }
#endif
    QPointF delta = gesture->delta();
    horizontalOffset += delta.x();
    verticalOffset += delta.y();
    update();
}

//! [pinch function]
void ImageWidget::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::RotationAngleChanged) {
        qreal rotationDelta = gesture->rotationAngle() - gesture->lastRotationAngle();
        rotationAngle += rotationDelta;
      //      rotationDelta << "->" << rotationAngle;
    }
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        currentStepScaleFactor = gesture->totalScaleFactor();
        //    gesture->scaleFactor() << "->" << currentStepScaleFactor;
    }
    if (gesture->state() == Qt::GestureFinished) {
        scaleFactor *= currentStepScaleFactor;
        currentStepScaleFactor = 1;
    }
    update();
}
//! [pinch function]

//! [swipe function]
void ImageWidget::swipeTriggered(QSwipeGesture */*gesture*/)
{
    /*Пока нормальный свайп не настроен (не тремя пальцами) обрабатывать не будем - сбивается связь с превью на окне записи qtodoeditwidget*/
    return;
//    if (gesture->state() == Qt::GestureFinished) {
//        if (gesture->horizontalDirection() == QSwipeGesture::Left
//            || gesture->verticalDirection() == QSwipeGesture::Up) {
//            goPrevImage();
//        } else {
//            goNextImage();
//        }
//        update();
//    }
}
//! [swipe function]

void ImageWidget::resizeEvent(QResizeEvent*)
{
    update();
}


QImage ImageWidget::loadImage(const QString &fileName) const
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    if (!reader.canRead()) {
        return QImage();
    }

    QImage image;
    if (!reader.read(&image)) {
        return QImage();
    }
    const QSize maximumSize(2000, 2000); // Reduce in case someone has large photo images.
    if (image.size().width() > maximumSize.width() || image.height() > maximumSize.height())
        image = image.scaled(maximumSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return image;
}

void ImageWidget::goNextImage()
{
//    if (m_picturesPaths.isEmpty())
//        return;

//    if (m_currentPictureIt != m_picturesPaths.end()-2) {
//        ++m_currentPictureIt;
//        prevImage = currentImage;
//        currentImage = nextImage;
//        if (m_currentPictureIt+1 != m_picturesPaths.end())
//            nextImage = loadImage(*(m_currentPictureIt+1));
//        else
//            nextImage = QImage();
//    }
//    update();
}

void ImageWidget::goPrevImage()
{
//    if (m_picturesPaths.isEmpty())
//        return;

//    if (m_currentPictureIt != m_picturesPaths.begin()) {
//        --m_currentPictureIt;
//        nextImage = currentImage;
//        currentImage = prevImage;
//        if (m_currentPictureIt != m_picturesPaths.begin())
//            prevImage = loadImage(*(m_currentPictureIt-1));
//        else
//            prevImage = QImage();
//    }
//    update();
}

