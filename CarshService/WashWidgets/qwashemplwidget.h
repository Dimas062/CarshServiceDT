#ifndef QWASHEMPLWIDGET_H
#define QWASHEMPLWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include "../CarshService/service_widgets/qlinetext.h"

class QWashEmplWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QWashEmplWidget(QWidget *parent = nullptr);

    QListWidget * m_pEmploeeListWidget;

    QComboBox * m_pCarshsCombo;
    QComboBox * m_pActivCombo;
    QLineText * m_pLoginLineText;
    QLineText * m_pPassLineText;
    QLineText * m_p1NameLineText;
    QLineText * m_p2NameLineText;
    QLineText * m_p3NameLineText;
    QLineText * m_pPhoneLineText;

    QString m_strCurUserId;

    void UpdateEmplList();

public slots:
    void OnApplyCardPressed();
    void OnDeleteCardPressed();
    void EmplClicked(QListWidgetItem*);
};

#endif // QWASHEMPLWIDGET_H
