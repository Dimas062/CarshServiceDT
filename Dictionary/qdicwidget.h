#ifndef QDICWIDGET_H
#define QDICWIDGET_H

#include <QWidget>
#include <QListWidget>

class QDicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDicWidget(QWidget *parent = nullptr);

    QListWidget * m_pDicListWidget;

public slots:
    void dicDoubleClicked(QListWidgetItem*);

signals:
};

#endif // QDICWIDGET_H
