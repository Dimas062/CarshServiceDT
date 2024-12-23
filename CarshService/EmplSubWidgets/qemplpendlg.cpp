#include "qemplpendlg.h"
#include <QUuid>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QUuid>
#include <QDateTime>

QEmplPenDlg::QEmplPenDlg(QUuid uuidPen , QString strUserId)
{
    m_soursePenId = uuidPen;
    m_strUserId = strUserId;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);


    m_pLineComment = new QLineText("Комментарий");
    pVMainLayout->addWidget(m_pLineComment);

    m_pRadioButtonP3 = new QRadioButton("+3");
    pVMainLayout->addWidget(m_pRadioButtonP3);
    m_pRadioButtonP2 = new QRadioButton("+2");
    pVMainLayout->addWidget(m_pRadioButtonP2);
    m_pRadioButtonP1 = new QRadioButton("+1");
    pVMainLayout->addWidget(m_pRadioButtonP1);
    m_pRadioButtonM1 = new QRadioButton("-1");
    pVMainLayout->addWidget(m_pRadioButtonM1);
    m_pRadioButtonM2 = new QRadioButton("-2");
    pVMainLayout->addWidget(m_pRadioButtonM2);
    m_pRadioButtonM3 = new QRadioButton("-3");
    pVMainLayout->addWidget(m_pRadioButtonM3);

    if(uuidPen!=QUuid())
    {
        QString strExec = QString("select Значение, Комментарий from ПоощренияНаказания where id='%1'").arg(uuidPen.toString());
        QSqlQuery query;
        query.exec(strExec);
        while(query.next())
        {
            m_pLineComment->setText(query.value(1).toString());
            int iVal = query.value(0).toInt();
            if(iVal == 3) m_pRadioButtonP3->setChecked(true);
            if(iVal == 2) m_pRadioButtonP2->setChecked(true);
            if(iVal == 1) m_pRadioButtonP1->setChecked(true);
            if(iVal == -1) m_pRadioButtonM1->setChecked(true);
            if(iVal == -2) m_pRadioButtonM2->setChecked(true);
            if(iVal == -3) m_pRadioButtonM3->setChecked(true);
        }
    }

    QPushButton * pApplyButton = new QPushButton("Применить");
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApplyPressed()));
    pVMainLayout->addWidget(pApplyButton);

    this->setLayout(pVMainLayout);
}

void QEmplPenDlg::OnApplyPressed()
{
    QUuid newPen = QUuid::createUuid();
    int iVal = 0;
    if(m_pRadioButtonP3->isChecked()) iVal = 3;
    if(m_pRadioButtonP2->isChecked()) iVal = 2;
    if(m_pRadioButtonP1->isChecked()) iVal = 1;
    if(m_pRadioButtonM1->isChecked()) iVal = -1;
    if(m_pRadioButtonM2->isChecked()) iVal = -2;
    if(m_pRadioButtonM3->isChecked()) iVal = -3;

    QString strExec;
    if(m_soursePenId == QUuid() )
        strExec = QString("insert into ПоощренияНаказания (id, ДатаВремя, Значение, Комментарий, Сотрудник) values ('%1' , '%2' ,'%3' ,'%4' ,'%5')").arg(newPen.toString()).arg(QDateTime::currentDateTime().toSecsSinceEpoch()).arg(iVal).arg(m_pLineComment->getText()).arg(m_strUserId);
    else
        strExec = QString("update ПоощренияНаказания set ДатаВремя = '%1' , Значение = '%2', Комментарий ='%3', Сотрудник='%4' where id='%5'").arg(QDateTime::currentDateTime().toSecsSinceEpoch()).arg(iVal).arg(m_pLineComment->getText()).arg(m_strUserId).arg(m_soursePenId.toString());
    QSqlQuery query;

    query.exec(strExec);
    accept();
}
