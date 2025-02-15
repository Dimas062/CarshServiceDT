#ifndef QSELECTAVAILABLEZAKAZS_H
#define QSELECTAVAILABLEZAKAZS_H

#include <QDialog>
#include <QObject>
#include <QTableWidget>

class QSelectAvailableZakazs : public QDialog
{
    Q_OBJECT
public:
    QSelectAvailableZakazs(QString strEmplPartnerUUid);

protected:
    QString m_strEmplPartnerUUid;
    QTableWidget * m_pZakazsTable;
    void LoadZakazs();

public slots:
    void OnAcceptPressed();
};

#endif // QSELECTAVAILABLEZAKAZS_H
