//#include "qmaindialog.h"


//#include <QApplication>
#include <QtWidgets/QApplication>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "qlogindlg.h"
#include "qcarshservicemaindlg.h"
#include "Carshs/qcarshmaindlg.h"
#include "common.h"

QRect screenGeometry;

QUuid uuidCurrentUser;

int iButtonHeight = 50;

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    screenGeometry = QRect(0,0,500,500);


    QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("188.243.205.147");
    db.setDatabaseName("CarshService");
    db.setPassword("VthctltcWKR200");
    db.setUserName("postgres");
    db.setPort(5432);

    if(!db.open())
    {
        QMessageBox::information(NULL , "Злобин Каршеринг сервис", QString("Не удаётся соединиться с сервером: %1").arg(db.lastError().text()) , "Закрыть");
        return 0;
    }

    QLoginDlg loginDlg;
    int res = loginDlg.exec();

    if(res == 0) return 0;

    if(res == CarshService)
    {
        QCarshServiceMainDlg dlg;
        dlg.show();

        return a.exec();
    }
    if(res == Carsh)
    {
       QCarshMainDlg dlg;
       dlg.show();

        return a.exec();
    }

    return a.exec();
}
