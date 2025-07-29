#ifndef COMMONDT_H
#define COMMONDT_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSettings>
#include <QStandardPaths>
#include <QTableWidget>
#include <QDateTime>
#include <QDebug>

void saveTableToExcel(QTableWidget* table, const QString& fileName, const QString& title);


#endif // COMMONDT_H
