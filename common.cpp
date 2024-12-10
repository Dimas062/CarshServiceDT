#include "common.h"
#include <QBuffer>
QString PictureFileToBase64(QString strPath)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QImage qp(strPath);

    qp.save(&buffer, "JPG");

    return buffer.data().toBase64();
}

QString ImageToBase64(const QImage & img)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    img.save(&buffer, "JPG");

    return buffer.data().toBase64();
}

QImage Base64ToImage(QString & base64Str)
{
    //TODO:  дописать

    QByteArray arr = QByteArray::fromBase64(base64Str.toLatin1());

    return QImage::fromData(arr, "JPG");

}
