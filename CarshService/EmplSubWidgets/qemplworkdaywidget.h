#ifndef QEMPLWORKDAYWIDGET_H
#define QEMPLWORKDAYWIDGET_H

#include <QWidget>

class QEmplWorkDayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEmplWorkDayWidget(QWidget *parent = nullptr);
    void SetActivEmpl(QString strUuid);

signals:
};

#endif // QEMPLWORKDAYWIDGET_H
