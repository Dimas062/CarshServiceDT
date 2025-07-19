#ifndef QSETEMPLFORPENPARKTASKDLG_H
#define QSETEMPLFORPENPARKTASKDLG_H

#include <QDialog>
#include <QObject>
#include <QUuid>
#include <QListWidget>

class QSetEmplForPenParkTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QSetEmplForPenParkTaskDlg(QUuid penTaskId);

protected:
    QUuid m_uuidPenTask;
    QListWidget * m_pEmploeeListWidget;
    void SetForEmpl(QUuid uuidEmpl);

public slots:
    void OnEmplDblClicked(QListWidgetItem*);
    void OnSetEmplPressed();
};

#endif // QSETEMPLFORPENPARKTASKDLG_H
