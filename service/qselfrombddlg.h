#ifndef QSELFROMBDDLG_H
#define QSELFROMBDDLG_H

#include <QDialog>
#include <QObject>
#include <QString>
#include <QVector>

class QSelFromBdDlg : public QDialog
{
    Q_OBJECT
public:
    QSelFromBdDlg(QString strTable , QString strShowField , bool bIsDeleteble = true);
    QString m_strRetId; //uuid выбранного элемента
    QVector<QPushButton *> m_vButtons;

public slots:
    void OnButtonToggled(bool);
};

#endif // QSELFROMBDDLG_H
