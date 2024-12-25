#ifndef QDICEDITDLG_H
#define QDICEDITDLG_H

#include <QDialog>
#include <QObject>
#include <QStringList>
#include <QTableWidget>

class QDicEditDlg : public QDialog
{
    Q_OBJECT
public:
    QDicEditDlg(QString strTable , QStringList fields , bool bIsRemovedField = false);


    QTableWidget * m_pTableWidget;

    QString m_strTable;
    QStringList m_strlstFields;
    bool m_bIsRemoved;//Есть ли поле Удалено и удаляемо ли соответственно значение (постепенно всех сделаем удаляемыми)


    void UpdateData();
public slots:
    void OnItemDblClk(QTableWidgetItem*);
    void OnAddClicked();
    void OnDelClicked();
};

#endif // QDICEDITDLG_H
