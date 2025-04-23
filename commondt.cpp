#include "commondt.h"
#include "xlsxdocument.h"

#include "xlsxdocument.h"
#include <QTableWidget>
#include <QHeaderView>

#include "xlsxdocument.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>

void saveTableToExcel(QTableWidget* table, const QString& fileName, const QString& title = "Report") {
    QXlsx::Document xlsx;

    // Стили оформления
    QXlsx::Format headerFormat;
    headerFormat.setFontBold(true);
    headerFormat.setFontColor(Qt::white);
    headerFormat.setFillPattern(QXlsx::Format::PatternSolid);
    headerFormat.setPatternBackgroundColor(Qt::blue);
    headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);


    // Заголовок таблицы
    xlsx.write(1, 1, title, headerFormat);
    xlsx.mergeCells(QXlsx::CellRange(1, 1, 1, table->columnCount()), headerFormat);

    // Заголовки столбцов
    int headerRow = 2;
    for(int col = 0; col < table->columnCount(); ++col) {
        QString header = table->horizontalHeaderItem(col) ?
                             table->horizontalHeaderItem(col)->text() :
                             QString("Column %1").arg(col+1);

        QXlsx::Format colHeaderFormat;
        if(table->horizontalHeaderItem(col)) {
            QColor bgColor = table->horizontalHeaderItem(col)->background().color();
            if(bgColor.alpha() > 0 && bgColor != Qt::white && bgColor != Qt::black) {
                colHeaderFormat.setFillPattern(QXlsx::Format::PatternSolid);
                colHeaderFormat.setPatternBackgroundColor(bgColor);
            }
        }
        colHeaderFormat.setFontBold(true);

        xlsx.write(headerRow, col + 1, header, colHeaderFormat);
    }

    // Обработка данных
    for(int row = 0; row < table->rowCount(); ++row) {
        for(int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem* item = table->item(row, col);
            if(!item) continue;

            QXlsx::Format cellFormat;

            cellFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);


            // Установка цвета фона только если он явно задан
            QColor bgColor = item->background().color();
            if(bgColor.alpha() > 0 && bgColor != Qt::white && bgColor != Qt::black) {
                cellFormat.setFillPattern(QXlsx::Format::PatternSolid);
                cellFormat.setPatternBackgroundColor(bgColor);
            }

            // Обработка объединения ячеек
            int rowSpan = table->rowSpan(row, col);
            int colSpan = table->columnSpan(row, col);

            if(rowSpan > 1 || colSpan > 1) {
                QXlsx::CellRange range(
                    headerRow + 1 + row,
                    col + 1,
                    headerRow + 1 + row + rowSpan - 1,
                    col + 1 + colSpan - 1
                    );
                xlsx.mergeCells(range, cellFormat);
            }

            // Определение типа данных
            int excelRow = headerRow + 1 + row;
            int excelCol = col + 1;
            QString text = item->text();

            xlsx.write(excelRow, excelCol, text, cellFormat);
        }
    }

    // Автоподбор ширины столбцов
    for(int col = 0; col < table->columnCount(); ++col) {
        xlsx.autosizeColumnWidth(col + 1);
        //xlsx.setColumnWidth(col + 1, xlsx.columnWidth(col + 1) + 2);
    }

    if(!xlsx.saveAs(fileName)) {
        qDebug() << "Ошибка сохранения файла!";
    }
}
