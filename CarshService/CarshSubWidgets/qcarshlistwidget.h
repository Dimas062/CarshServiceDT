#ifndef QCARSHLISTWIDGET_H
#define QCARSHLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

class QCarshListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCarshListWidget(QWidget *parent = nullptr);

    QListWidget * m_pCarshsListWidget;

    void UpdateCarshsList();

public slots:
    void OnAddCarshPressed();
    void OnRemoveCarshPressed();
    void carshDoubleClicked(QListWidgetItem*);
};

#endif // QCARSHLISTWIDGET_H
