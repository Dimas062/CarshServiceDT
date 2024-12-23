#ifndef QEMPLOEESWIDGET_H
#define QEMPLOEESWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include "EmplSubWidgets/qempltaskswidget.h"
#include "EmplSubWidgets/qemplworkdaywidget.h"
#include "EmplSubWidgets/qemplcardwidget.h"
#include "EmplSubWidgets/qemplpenwidget.h"//

class QEmploeesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEmploeesWidget(QWidget *parent = nullptr);

    QListWidget * m_pEmploeeListWidget;
    QTabWidget * m_pEmplTabWidget;

    QEmplTasksWidget * m_pEmplTasksWidget;
    QEmplWorkDayWidget * m_pEmplWorkDayWidget;
    QEmplCardWidget * m_pEmplCardWidget;
    QEmplPenWidget * m_pEmplPenWidget;

public slots:
    void EmplClicked(QListWidgetItem*);

signals:
};

#endif // QEMPLOEESWIDGET_H
