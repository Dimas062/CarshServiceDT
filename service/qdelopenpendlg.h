#ifndef QDELOPENPENDLG_H
#define QDELOPENPENDLG_H

#include <QDialog>
#include <QObject>



class QDelOpenPenDlg : public QDialog
{
    Q_OBJECT
public:
    QDelOpenPenDlg();

    static const int Del = 3;
    static const int Open = 4;

public slots:
    void OnOpenClicked();
    void OnDelClicked();
};

#endif // QDELOPENPENDLG_H
