#ifndef QSMENATASKDLG_H
#define QSMENATASKDLG_H

#include <QDialog>
#include <QObject>

class QSmenaTaskDlg : public QDialog
{
    Q_OBJECT
public:
    QSmenaTaskDlg(QString strTaskUuid);
};

#endif // QSMENATASKDLG_H
