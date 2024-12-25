#ifndef QDICITEMEDITDLG_H
#define QDICITEMEDITDLG_H

#include <QDialog>
#include <QObject>
#include <QString>
#include <QLineEdit>

class QDicItemEditDlg : public QDialog
{
    Q_OBJECT
public:
    QDicItemEditDlg(QString strTable , QString strId , QString strField);
    QLineEdit * m_pLineEdit;
    QString m_strTable , m_strId , m_strField;

public slots:
    void OnApply();
};

#endif // QDICITEMEDITDLG_H
