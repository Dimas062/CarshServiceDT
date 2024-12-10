#ifndef QPROVIDERDLG_H
#define QPROVIDERDLG_H

#include <QDialog>
#include <QUuid>
#include "../CarshService/service_widgets/qlinetext.h"

class QProviderDlg : public QDialog
{
    Q_OBJECT
public:
    QProviderDlg(QUuid uuidProvider = QUuid());

public slots:
    void OnApplyPressed();

protected:
    QUuid m_uuidSourceProvider;
    QLineText * m_pNameLineText;
    void LoadFromBd();
};

#endif // QPROVIDERDLG_H
