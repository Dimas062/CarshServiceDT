#include "qdelopenpendlg.h"
#include <QVBoxLayout>
#include <QPushButton>

QDelOpenPenDlg::QDelOpenPenDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QPushButton * pOpenButton = new QPushButton("Открыть/править рекламацию");
    pVMainLayout->addWidget(pOpenButton);
    connect(pOpenButton , SIGNAL(clicked()) , this , SLOT(OnOpenClicked()));

    QPushButton * pDelButton = new QPushButton("Удалить рекламацию");
    pVMainLayout->addWidget(pDelButton);
    connect(pDelButton , SIGNAL(clicked()) , this , SLOT(OnDelClicked()));

    QPushButton * pExitButton = new QPushButton("Закрыть");
    pVMainLayout->addWidget(pExitButton);
    connect(pExitButton , SIGNAL(clicked()) , this , SLOT(reject()));


}


void QDelOpenPenDlg::OnOpenClicked()
{
    done(Open);
}

void QDelOpenPenDlg::OnDelClicked()
{
    done(Del);
}
