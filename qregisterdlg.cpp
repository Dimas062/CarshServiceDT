#include "qregisterdlg.h"
#include <QPushButton>
#include "Carshs/qregistercarshuserdlg.h"
#include <QVBoxLayout>

extern int iButtonHeight;

QRegisterDlg::QRegisterDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    int iUnderButtonSpace = 15;

    pVMainLayout->addSpacing(5);

    pVMainLayout->setContentsMargins(20,20,20,20);

    this->setMinimumWidth(300);

    pVMainLayout->addSpacing(iUnderButtonSpace);
    QPushButton * pRegisterCarshUserButton = new QPushButton("Пользователь службы каршеринга");
    connect(pRegisterCarshUserButton,SIGNAL(pressed()),this,SLOT(OnRegisterCarshUserPressed()));
    pRegisterCarshUserButton->setMaximumHeight(iButtonHeight);
    pRegisterCarshUserButton->setMinimumHeight(iButtonHeight);
    pVMainLayout->addWidget(pRegisterCarshUserButton);
}


void QRegisterDlg::OnRegisterCarshUserPressed()
{
    QRegisterCarshUserDlg dlg;
    dlg.exec();
}
