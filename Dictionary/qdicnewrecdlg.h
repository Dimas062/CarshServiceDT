#ifndef QDICNEWRECDLG_H
#define QDICNEWRECDLG_H

#include <QDialog>
#include <QObject>
#include <QStringList>
#include "../CarshService/service_widgets/qlinetext.h"
#include <QVector>

class QDicNewRecDlg : public QDialog
{
    Q_OBJECT
public:
    QDicNewRecDlg(QString strTable , QStringList fields);

    QString m_strTable;
    QStringList m_fields;

    QVector<QLineText *> lineTexts;

public slots:
    void OnApply();
};

#endif // QDICNEWRECDLG_H
