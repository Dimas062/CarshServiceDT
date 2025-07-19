#include "qnewtaskdlg.h"
#include "../CarshService/tasks/qdocstaskdlg.h"
#include "../CarshService/tasks/qpenaltyparkingdialog.h"
#include "../CarshService/tasks/qparkingtaskdialog.h"
#include "../CarshService/tasks/qrettozonedialog.h"
#include "../CarshService/tasks/qsmenadlg.h"
#include "../CarshService/tasks/qplatetaskdialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

extern QUuid uuidCurrentUser;

QNewTaskDlg::QNewTaskDlg(QUuid uuidEmpl)
{
    m_uuidEmpl = uuidEmpl;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel("<h2>Новая задача<\h2>");
    pVMainLayout->addWidget(pTopLabel);

    QPushButton * pParkingButton = new QPushButton("Закрытая территория");
    connect(pParkingButton,SIGNAL(pressed()),this,SLOT(OnParkingPressed()));
    pVMainLayout->addWidget(pParkingButton);

    QPushButton * pPenaltyParkingButton = new QPushButton("Штрафстоянка");
    connect(pPenaltyParkingButton,SIGNAL(pressed()),this,SLOT(OnPenaltyParkingPressed()));
    pVMainLayout->addWidget(pPenaltyParkingButton);

    QPushButton * pGRZButton = new QPushButton("Номера");
    connect(pGRZButton,SIGNAL(pressed()),this,SLOT(OnGRZPressed()));
    pVMainLayout->addWidget(pGRZButton);

    QPushButton * pRetToZoneButton = new QPushButton("Возврат в зону");
    connect(pRetToZoneButton,SIGNAL(pressed()),this,SLOT(OnRetToZonePressed()));
    pVMainLayout->addWidget(pRetToZoneButton);

    QPushButton * pDocsButton = new QPushButton("Документы");
    connect(pDocsButton,SIGNAL(pressed()),this,SLOT(OnDocsPressed()));
    pVMainLayout->addWidget(pDocsButton);

    QPushButton * pSmenaButton = new QPushButton("Смена");
    connect(pSmenaButton,SIGNAL(pressed()),this,SLOT(OnSmenaPressed()));
    pVMainLayout->addWidget(pSmenaButton);
}


void QNewTaskDlg::OnParkingPressed()
{
    QUuid uuidOrigUser = uuidCurrentUser;
    uuidCurrentUser = m_uuidEmpl;
    QParkingTaskDialog dlg;
    dlg.exec();
    uuidCurrentUser = uuidOrigUser;
    accept();
}
void QNewTaskDlg::OnPenaltyParkingPressed()
{
    QUuid uuidOrigUser = uuidCurrentUser;
    uuidCurrentUser = m_uuidEmpl;
    QPenaltyParkingDialog dlg;
    dlg.exec();
    uuidCurrentUser = uuidOrigUser;
    accept();
}

void QNewTaskDlg::OnGRZPressed()
{
    QUuid uuidOrigUser = uuidCurrentUser;
    uuidCurrentUser = m_uuidEmpl;
    QPlateTaskDialog dlg;
    dlg.exec();
    uuidCurrentUser = uuidOrigUser;
    accept();
}

void QNewTaskDlg::OnRetToZonePressed()
{
    QUuid uuidOrigUser = uuidCurrentUser;
    uuidCurrentUser = m_uuidEmpl;
    QRetToZoneDialog dlg;
    dlg.exec();
    uuidCurrentUser = uuidOrigUser;
    accept();
}

void QNewTaskDlg::OnDocsPressed()
{
    QUuid uuidOrigUser = uuidCurrentUser;
    uuidCurrentUser = m_uuidEmpl;
    QDocsTaskDlg dlg;
    dlg.exec();
    uuidCurrentUser = uuidOrigUser;
    accept();
}

void QNewTaskDlg::OnSmenaPressed()
{
    QUuid uuidOrigUser = uuidCurrentUser;
    uuidCurrentUser = m_uuidEmpl;
    QSmenaDlg dlg;
    dlg.exec();
    uuidCurrentUser = uuidOrigUser;
    accept();
}

