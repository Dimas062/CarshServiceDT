#ifndef QCARSHEMPLWIDGET_H
#define QCARSHEMPLWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>

#include "../CarshService/service_widgets/qlinetext.h"

class QCarshEmplWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCarshEmplWidget(QWidget *parent = nullptr);
    QListWidget * m_pEmploeeListWidget;

    QComboBox * m_pCarshsCombo;
    QComboBox * m_pActivCombo;
    QLineText * m_pLoginLineText;
    QLineText * m_pPassLineText;
    QLineText * m_p1NameLineText;
    QLineText * m_p2NameLineText;
    QLineText * m_p3NameLineText;
    QLineText * m_pPositionLineText;
    QLineText * m_pMailLineText;
    QLineText * m_pTelegrammLineText;
    QLineText * m_pPhoneLineText;

    QString m_strCurUserId;

    void UpdateEmplList();

public slots:
    void OnApplyCardPressed();
    void OnDeleteCardPressed();
    void EmplClicked(QListWidgetItem*);
};

#endif // QCARSHEMPLWIDGET_H
