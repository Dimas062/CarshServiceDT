#ifndef QCARSHINPUTPENPARKEDITDLG_H
#define QCARSHINPUTPENPARKEDITDLG_H

#include <QDialog>
#include <QObject>
#include <QUuid>

class QCarshInputPenParkEditDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshInputPenParkEditDlg(QUuid penTaskId, QString strGrz, QString strTime , QString strPenPark);

public slots:
    void OnSetEmplPressed();
    void OnRemTaskPressed();

protected:
    QUuid m_uuidTask;
};

#endif // QCARSHINPUTPENPARKEDITDLG_H
