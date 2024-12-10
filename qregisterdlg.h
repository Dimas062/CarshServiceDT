#ifndef QREGISTERDLG_H
#define QREGISTERDLG_H

#include <QDialog>

class QRegisterDlg : public QDialog
{
    Q_OBJECT
public:
    QRegisterDlg();

public slots:
    void OnRegisterCarshUserPressed();
};

#endif // QREGISTERDLG_H
