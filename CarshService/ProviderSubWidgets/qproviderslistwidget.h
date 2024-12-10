#ifndef QPROVIDERSLISTWIDGET_H
#define QPROVIDERSLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

class QProvidersListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QProvidersListWidget(QWidget *parent = nullptr);


    QListWidget * m_pProvidersListWidget;

    void UpdateProvidersList();

public slots:
    void OnAddProviderPressed();
    void OnRemoveProviderPressed();
    void providerDoubleClicked(QListWidgetItem*);

signals:
};

#endif // QPROVIDERSLISTWIDGET_H
