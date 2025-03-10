#ifndef QTASKHISTORYDLG_H
#define QTASKHISTORYDLG_H

#include <QObject>
#include <QDialog>
#include <QTableWidget>

class QTaskHistoryDlg: public QDialog
{
    Q_OBJECT
public:
    QTaskHistoryDlg(QString strTaskUuid);
protected:
    QString m_strTaskUuid;
    QTableWidget * m_pHistoryTable;

    void UpdateHistory();
};

#endif // QTASKHISTORYDLG_H
