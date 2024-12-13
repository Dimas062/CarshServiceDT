#ifndef QPLATEPARTNERTASKWIDGET_H
#define QPLATEPARTNERTASKWIDGET_H

#include <QWidget>

class QPlatePartnerTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPlatePartnerTaskWidget(QWidget *parent = nullptr);
    void SetActivPartner(QString strUuid);
signals:
};

#endif // QPLATEPARTNERTASKWIDGET_H
