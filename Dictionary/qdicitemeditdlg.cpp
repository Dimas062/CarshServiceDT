#include "qdicitemeditdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlQuery>


QDicItemEditDlg::QDicItemEditDlg(QString strTable , QString strId , QString strField)
{
    m_strTable = strTable;
    m_strId = strId;
    m_strField = strField;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    m_pLineEdit = new QLineEdit;

    pVMainLayout->addWidget(m_pLineEdit);

    QString strExec = QString("select \"%1\" from \"%2\" where id='%3'").arg(strField).arg(strTable).arg(strId);
    qDebug()<<strExec;
    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
        m_pLineEdit->setText(query.value(0).toString());

    QHBoxLayout * pHButtonsLayout = new QHBoxLayout;

    QPushButton * pApplyButton = new QPushButton("Применить");
    pHButtonsLayout->addWidget(pApplyButton);
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApply()));

    QPushButton * pCloseButton = new QPushButton("Отменить");
    pHButtonsLayout->addWidget(pCloseButton);
    connect(pCloseButton,SIGNAL(pressed()),this,SLOT(reject()));

    pVMainLayout->addLayout(pHButtonsLayout);
}

void QDicItemEditDlg::OnApply()
{
    QString strExec = QString("update \"%2\" set \"%1\" = '%4' where id='%3'").arg(m_strField).arg(m_strTable).arg(m_strId).arg(m_pLineEdit->text());
    QSqlQuery query;
    query.exec(strExec);
    accept();
}
