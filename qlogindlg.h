#ifndef QLOGINDLG_H
#define QLOGINDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class QLoginDlg : public QDialog
{
    Q_OBJECT
public:
    QLoginDlg();
    QString m_strLastLoginedCarshId;

protected:
    QLineEdit * m_pLoginLineEdit;
    QLineEdit * m_pPasswordLineEdit;

    QLabel * m_pStatusLabel;

public slots:
    void OnSettingsModePressed();
    void OnLoginPressed();
    void OnRegisterPressed();
};

#endif // QLOGINDLG_H
