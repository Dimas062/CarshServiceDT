#ifndef QWashPARTNERTASKDLG_H
#define QWashPARTNERTASKDLG_H

#include <QDialog>
#include <QObject>
#include "pictures/qpictureswidget.h"
#include <QTableWidget>

class QWashPartnerTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QWashPartnerTaskDlg(QString strTaskUuid);
    QPicturesWidget * m_pPicturesWidget;
    QTableWidget * m_pTypesTableWidget;
    QString m_strTaskId;

    void UpdateTable();

public slots:
    void typesCellDoubleClicked(int, int);
};

#endif // QWashPARTNERTASKDLG_H
