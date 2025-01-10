#ifndef QCARSHDLG_H
#define QCARSHDLG_H

#include <QUuid>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QRgb>
#include "../CarshService/service_widgets/qlinetext.h"
#include "service/quleditdlg.h"

class QCarshDlg:public QDialog
{
    Q_OBJECT
public:
    QCarshDlg(QUuid uuidCarh = QUuid());

public slots:
    void OnAddLogoPressed();
    void OnApplyPressed();
    void OnColorPressed();
    void OnULPressed();

protected:
    QUuid m_uuidSourceCarsh;
    QLineText * m_pNameLineText;
    QLabel * m_pLogoLabel;
    QString m_strLogoPath;
    QRgb m_currentRGB;
    QPushButton * m_pColorButton;
    QULEditDlg m_ulDlg;
    void LoadFromBd();
};

#endif // QCARSHDLG_H
