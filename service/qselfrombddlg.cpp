#include "qselfrombddlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QPushButton>

QSelFromBdDlg::QSelFromBdDlg(QString strTable , QString strShowField , bool bIsDeleteble)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);
    
    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText(QString("<b>%1<\b>").arg(strTable));
    pTopLabel->setStyleSheet("font-size: 20px;");
    
    pVMainLayout->addSpacing(5);
    
    QString strFilter;
    
    if(bIsDeleteble) strFilter = "where Удалено<>true";
    
    QString strExec = QString("select id , \"%1\" from \"%2\" %3").arg(strShowField).arg(strTable).arg(strFilter);
    
    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
    {
        QPushButton * pButton = new QPushButton(query.value(1).toString());
        pButton->setProperty("id",QVariant(QString(query.value(0).toString())));
        m_vButtons.push_back(pButton);
        pButton->setCheckable(true);
        connect(pButton , SIGNAL(toggled(bool)) , this , SLOT(OnButtonToggled(bool)));
        pVMainLayout->addWidget(pButton);
    }

    QHBoxLayout * pHExitButtonsLayout = new QHBoxLayout(this);

    QPushButton * pApplyButton = new QPushButton("Выбрать");
    connect(pApplyButton , SIGNAL(clicked()) , this , SLOT(accept()));
    pHExitButtonsLayout->addWidget(pApplyButton);
    pApplyButton->setDefault(true);

    QPushButton * pRejectButton = new QPushButton("Отменить");
    connect(pRejectButton , SIGNAL(clicked()) , this , SLOT(reject()));
    pHExitButtonsLayout->addWidget(pRejectButton);

    pVMainLayout->addLayout(pHExitButtonsLayout);
}


void QSelFromBdDlg::OnButtonToggled(bool isSelected)
{
    if(isSelected)
    {
        QPushButton * pButton = (QPushButton *)sender();
        m_strRetId = (pButton->property("id")).value<QString>();

        foreach (QPushButton * pAllButton, m_vButtons) {
            if(pAllButton != pButton)pAllButton->setChecked(false);
        }
    }
}
