#ifndef QCARSHLISTWIDGET_H
#define QCARSHLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include "../CarshService/service_widgets/qlinetext.h"

class QCarshListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCarshListWidget(QWidget *parent = nullptr);

    QListWidget * m_pCarshsListWidget;
    QLineText * m_pWashGoogleTableIdLineText;

    void UpdateCarshsList();
    void UpdateSelectedCarshSettings();
protected:
    QString m_strCurrentCarshUuid;

public slots:
    void OnAddCarshPressed();
    void OnRemoveCarshPressed();
    void carshDoubleClicked(QListWidgetItem*);
    void carshClicked(QListWidgetItem*);
    void OnApplySettingsPressed();
};

#endif // QCARSHLISTWIDGET_H
