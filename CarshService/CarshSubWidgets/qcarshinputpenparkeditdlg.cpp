#include "qcarshinputpenparkeditdlg.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QSqlQuery>
#include "qsetemplforpenparktaskdlg.h"

QCarshInputPenParkEditDlg::QCarshInputPenParkEditDlg(QUuid penTaskId, QString strGrz, QString strTime , QString strPenPark)
{
    m_uuidTask = penTaskId;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel(QString("<h2>Задача от %1 | %2 | %3<\h2>").arg(strTime , strGrz , strPenPark));
    pVMainLayout->addWidget(pTopLabel);

    QPushButton * pSetEmplButton = new QPushButton("Назначить");
    connect(pSetEmplButton,SIGNAL(pressed()),this,SLOT(OnSetEmplPressed()));
    pVMainLayout->addWidget(pSetEmplButton);

    QPushButton * pRemTaskButton = new QPushButton("Удалить задачу");
    connect(pRemTaskButton,SIGNAL(pressed()),this,SLOT(OnRemTaskPressed()));
    pVMainLayout->addWidget(pRemTaskButton);
}

void QCarshInputPenParkEditDlg::OnSetEmplPressed()
{
    QSetEmplForPenParkTaskDlg dlg(m_uuidTask);
    if(dlg.exec()==QDialog::Accepted)
        accept();
}

void QCarshInputPenParkEditDlg::OnRemTaskPressed()
{
    QString strExec= QString("update ЗадачиЗаказчикаШС set ПереведенаВЗадачу = '11111111-1111-1111-1111-111111111111' where id = '%1'").arg(m_uuidTask.toString());
    QSqlQuery Query;
    Query.exec(strExec);
    accept();
}
