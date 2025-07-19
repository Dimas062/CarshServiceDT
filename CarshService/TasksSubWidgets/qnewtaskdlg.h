#ifndef QNEWTASKDLG_H
#define QNEWTASKDLG_H

#include <QDialog>
#include <QObject>
#include <QUuid>

class QNewTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QNewTaskDlg(QUuid uuidEmpl);

protected:
    QUuid m_uuidEmpl;

public slots:
    void OnParkingPressed();
    void OnPenaltyParkingPressed();
    void OnGRZPressed();
    void OnRetToZonePressed();
    void OnDocsPressed();
    void OnSmenaPressed();

};

#endif // QNEWTASKDLG_H
