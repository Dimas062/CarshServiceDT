#ifndef QPROVIDERDLG_H
#define QPROVIDERDLG_H

#include <QDialog>
#include <QUuid>
#include "../CarshService/service_widgets/qlinetext.h"
#include "service/quleditdlg.h"

class QProviderDlg : public QDialog
{
    Q_OBJECT
public:
    QProviderDlg(QUuid uuidProvider = QUuid());

    QULEditDlg ulDlg;

public slots:
    void OnApplyPressed();
    void OnULPressed();

protected:
    QUuid m_uuidSourceProvider;
    QLineText * m_pNameLineText;
    void LoadFromBd();
};

#endif // QPROVIDERDLG_H
