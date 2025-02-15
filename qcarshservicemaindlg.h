#ifndef QCARSHSERVICEMAINDLG_H
#define QCARSHSERVICEMAINDLG_H

#include <QDialog>
#include <QObject>
#include <QtCore>


class QCarshServiceMainDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshServiceMainDlg();

    //Хардбит работы с базой
    QThread m_thread;
    std::shared_ptr<QTimer> mHeartbeatTimer;
    const int m_iHeartbeatTime = 25000;//msecs

private slots:
    void OnProcessingHeartbeatTimer();

};

#endif // QCARSHSERVICEMAINDLG_H
