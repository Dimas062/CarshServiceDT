#ifndef QDICEDITDLG_H
#define QDICEDITDLG_H

#include <QDialog>
#include <QObject>
#include <QStringList>
#include <QTableWidget>
#include <QVector>


class QDicEditDlg : public QDialog
{
    Q_OBJECT
public:
    QDicEditDlg(QString strTable , QStringList fields , bool bIsRemovedField = false , bool bIsCarshsPrices = false , bool bIsAddedField = true);


    QTableWidget * m_pTableWidget;

    QString m_strTable;
    QStringList m_strlstFields;
    bool m_bIsRemoved;//Есть ли поле Удалено и удаляемо ли соответственно значение (постепенно всех сделаем удаляемыми)
    bool m_bIsCarshsPrices;//Выводим ли мы цены для заказчиков в этом словаре (естественно, этот словарь должен быть про цены)

    void UpdateData();

    //Заказчики, для отображения цен по ним
    struct SCarshs
    {
        QString strUuid;
        QString strName;
    };
    QVector<SCarshs> m_vCarshs;

    enum ECellType{Standart , CarshPrice};

protected:
    int m_iCarshsCount;//Количество заказчиков для вывода доп. столбцов с ценами

public slots:
    void OnItemDblClk(QTableWidgetItem*);
    void OnAddClicked();
    void OnDelClicked();
};

#endif // QDICEDITDLG_H
