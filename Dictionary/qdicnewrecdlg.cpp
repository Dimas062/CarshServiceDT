#include "qdicnewrecdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>

QDicNewRecDlg::QDicNewRecDlg(QString strTable , QStringList fields)
{
    m_strTable = strTable;
    m_fields = fields;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);


    QLabel *pTopLabel = new QLabel(QString("<b>%1</b>").arg(m_strTable));
    pVMainLayout->addWidget(pTopLabel);

    foreach(QString field , fields)
    {
        QLineText * pLineText = new QLineText(field);
        lineTexts.push_back(pLineText);
        pVMainLayout->addWidget(pLineText);
    }

    QHBoxLayout * pHButtonsLayout = new QHBoxLayout;

    QPushButton * pApplyButton = new QPushButton("Применить");
    pHButtonsLayout->addWidget(pApplyButton);
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApply()));

    QPushButton * pCloseButton = new QPushButton("Отменить");
    pHButtonsLayout->addWidget(pCloseButton);
    connect(pCloseButton,SIGNAL(pressed()),this,SLOT(reject()));

    pVMainLayout->addLayout(pHButtonsLayout);

    this->setLayout(pVMainLayout);
}

void QDicNewRecDlg::OnApply()
{
    QString strColumns(" id, ");
    QString strValues (" uuid_generate_v4(), ");

    foreach(QLineText * pLineText , lineTexts)
    {
        strColumns.append(" \"");
        strColumns.append(pLineText->getLabelText());
        strColumns.append("\", ");

        strValues.append(" '");
        strValues.append(pLineText->getText());
        strValues.append("', ");
    }

    //Убираем последние пробелы и запятые
    strColumns.chop(2);
    strValues.chop(2);

    QString strQuery = QString("insert into \"%1\" (%2) values (%3)").arg(m_strTable).arg(strColumns).arg(strValues);
    QSqlQuery query;

    query.exec(strQuery);
    accept();
}
