#ifndef QSELEMPLDLG_H
#define QSELEMPLDLG_H

#include <QDialog>
#include <QObject>
#include <QObject>
#include <QUuid>
#include <QListWidget>

class QSelEmplDlg : public QDialog
{
    Q_OBJECT
public:
    QSelEmplDlg();
    QUuid m_uuidSelectedEmpl;

protected:
    QListWidget * m_pEmploeeListWidget;

public slots:
    void OnEmplDblClicked(QListWidgetItem*);
    void OnSetEmplPressed();
};

#endif // QSELEMPLDLG_H
