#include "qdicwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVariant>
#include "Dictionary/qdiceditdlg.h"

QDicWidget::QDicWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    m_pDicListWidget = new QListWidget;
    connect(m_pDicListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(dicDoubleClicked(QListWidgetItem*)));

    QListWidgetItem *  pItem = new QListWidgetItem("Товары");
    QStringList fields;
    fields.clear();
    fields<<"Товар";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Штрафстоянки");
    fields.clear();
    fields<<"Название"<<"Адрес";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Типы задач Оклейка");
    fields.clear();
    fields<<"Тип"<<"Цена";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, true);
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Типы задач Мойка");
    fields.clear();
    fields<<"Тип"<<"Цена";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, true);
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Статьи расходов");
    fields.clear();
    fields<<"Статья расходов";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, true);
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Причины задержания");
    fields.clear();
    fields<<"Название";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Отделы ГИБДД");
    fields.clear();
    fields<<"Название"<<"Адрес";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Количество часов в смене");
    fields.clear();
    fields<<"Количество";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);
    m_pDicListWidget->addItem(pItem);

    pVMainLayout->addWidget(m_pDicListWidget);
}

void QDicWidget::dicDoubleClicked(QListWidgetItem* item)
{
    QDicEditDlg dlg(item->text() , item->data(Qt::UserRole).toStringList() , item->data(Qt::UserRole +1).toBool());
    dlg.exec();
}
