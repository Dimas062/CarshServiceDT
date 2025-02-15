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
    pItem->setData(Qt::UserRole + 2, false);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Товары");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Штрафстоянки");
    fields.clear();
    fields<<"Название"<<"Адрес";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, false);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Штрафстоянки");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Типы задач партнеров 'Оклейка'");
    fields.clear();
    fields<<"Тип"<<"Цена";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, true);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, true);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Типы задач Оклейка");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Типы задач партнеров 'Мойка'");
    fields.clear();
    fields<<"Тип"<<"Цена";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, true);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, true);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Типы задач Мойка");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Типы задач партнеров 'Номера'");
    fields.clear();
    fields<<"Тип"<<"Цена";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, true);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, false);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Типы задач Номера");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Статьи расходов");
    fields.clear();
    fields<<"Статья расходов";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, true);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, false);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Статьи расходов");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Причины задержания");
    fields.clear();
    fields<<"Название";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, false);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Причины задержания");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Отделы ГИБДД");
    fields.clear();
    fields<<"Название"<<"Адрес";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, false);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Отделы ГИБДД");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Количество часов в смене");
    fields.clear();
    fields<<"Количество";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, false);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, true);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Количество часов в смене");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pItem = new QListWidgetItem("Типы задач сотрудников");
    fields.clear();
    fields<<"Тип"<<"Цена";
    pItem->setData(Qt::UserRole , QVariant(fields));
    pItem->setData(Qt::UserRole + 1, false);//Возможно ли удаение (есть поле Удалено)
    pItem->setData(Qt::UserRole + 2, true);//Является ли справочником с ценами для заказчиков
    pItem->setData(Qt::UserRole + 3, false);//Возможно ли добавление
    pItem->setData(Qt::UserRole + 4, "Типы задач");//Название таблицы в базе
    m_pDicListWidget->addItem(pItem);

    pVMainLayout->addWidget(m_pDicListWidget);
}

void QDicWidget::dicDoubleClicked(QListWidgetItem* item)
{
    QDicEditDlg dlg(item->data(Qt::UserRole + 4).toString() , item->data(Qt::UserRole).toStringList() , item->data(Qt::UserRole + 1).toBool(), item->data(Qt::UserRole + 2).toBool(), item->data(Qt::UserRole + 3).toBool());
    dlg.exec();
}
