QT += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#LIBS += -L/usr/local/lib -lpq

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icons\CarshService.ico

SOURCES += \
    ../CarshService/service_widgets/qlinetext.cpp \
    BDPatterns.cpp \
    CarshService/CarshSubWidgets/qcarshemplwidget.cpp \
    CarshService/CarshSubWidgets/qcarshlistwidget.cpp \
    CarshService/CarshSubWidgets/qcarshdlg.cpp \
    CarshService/EmplSubWidgets/qemplcostdlg.cpp \
    CarshService/EmplSubWidgets/qemplpendlg.cpp \
    CarshService/EmplSubWidgets/qemplpenwidget.cpp \
    CarshService/GraphicSubWidgets/qemplgraphwidget.cpp \#
    CarshService/GraphicSubWidgets/qpenparkgraphwidget.cpp \
    CarshService/ProviderSubWidgets/qproviderdlg.cpp \
    CarshService/ProviderSubWidgets/qproviderempllistwidget.cpp \
    CarshService/ProviderSubWidgets/qproviderslistwidget.cpp \
    CarshService/TasksSubWidgets/qpenalparktaskfilterwidget.cpp \
    CarshService/qcarshswidget.cpp \
    CarshService/EmplSubWidgets/qemplcardwidget.cpp \
    CarshService/qcostswidget.cpp \
    CarshService/qemploeeswidget.cpp \
    CarshService/EmplSubWidgets/qempltaskswidget.cpp \
    CarshService/EmplSubWidgets/qemplworkdaywidget.cpp \
    CarshService/QCSGraphicsWidget.cpp \
    CarshService/qpartnerswidget.cpp \
    CarshService/qproviderswidget.cpp \
    CarshService/qtaskswidget.cpp \
    Carshs/qcarshempltaskswidget.cpp \
    Carshs/qcarshmaindlg.cpp \
    Carshs/qcarshparkingtaskcarddlg.cpp \
    Carshs/qcarshpenaltyparktaskcarddlg.cpp \
    Carshs/qcarshplatetaskcarddlg.cpp \
    Carshs/qdocstaskdlg.cpp \
    Carshs/qregistercarshuserdlg.cpp \
    Carshs/qrettozonecarddlg.cpp \
    Carshs/qsmenataskdlg.cpp \
    Dictionary/qdicitemeditdlg.cpp \
    Dictionary/qdicnewrecdlg.cpp \
    Dictionary/qdicwidget.cpp \
    Partners/qplatepartnercardwidget.cpp \
    Partners/qplatepartnerswidget.cpp \
    Partners/qplatepartnertaskwidget.cpp \
    Partners/qstickpartnercardwidget.cpp \
    Partners/qstickpartnerswidget.cpp \
    Partners/qstickpartnertaskdlg.cpp \
    Partners/qstickpartnertaskwidget.cpp \
    Partners/qwashpartnercardwidget.cpp \
    Partners/qwashpartnerpenaltydlg.cpp \
    Partners/qwashpartnerswidget.cpp \
    Partners/qwashpartnertaskdlg.cpp \
    Partners/qwashpartnertaskwidget.cpp \
    common.cpp \
    main.cpp \
    pictures/imagewidget.cpp \
    pictures/qfullscreenpicdialog.cpp \
    pictures/qpiclabel.cpp \
    pictures/qpictureswidget.cpp \
    qcarshservicemaindlg.cpp \
    qlogindlg.cpp \
    qregisterdlg.cpp \
    service/qdelopenpendlg.cpp \
    Dictionary/qdiceditdlg.cpp \
    service/qpointeditdlg.cpp \
    service/qselectavailablezakazs.cpp \
    service/qselfrombddlg.cpp \
    service/qtaskhistorydlg.cpp \
    service/quleditdlg.cpp \
    service/xlspatterns.cpp

HEADERS += \
     ../CarshService/service_widgets/qlinetext.h \
    #BDPatterns.h \
    BDPatterns.h \
    CarshService/CarshSubWidgets/qcarshemplwidget.h \
    CarshService/CarshSubWidgets/qcarshlistwidget.h \
    CarshService/CarshSubWidgets/qcarshdlg.h \
    CarshService/EmplSubWidgets/qemplcostdlg.h \
    CarshService/EmplSubWidgets/qemplpendlg.h \
    CarshService/EmplSubWidgets/qemplpenwidget.h \
    CarshService/GraphicSubWidgets/qemplgraphwidget.h \
    CarshService/GraphicSubWidgets/qpenparkgraphwidget.h \
    CarshService/ProviderSubWidgets/qproviderdlg.h \
    CarshService/ProviderSubWidgets/qproviderempllistwidget.h \
    CarshService/ProviderSubWidgets/qproviderslistwidget.h \
    CarshService/TasksSubWidgets/qpenalparktaskfilterwidget.h \
    CarshService/qcarshswidget.h \
    CarshService/EmplSubWidgets/qemplcardwidget.h \
    CarshService/qcostswidget.h \
    CarshService/qemploeeswidget.h \
    CarshService/EmplSubWidgets/qempltaskswidget.h \
    CarshService/EmplSubWidgets/qemplworkdaywidget.h \
    CarshService/QCSGraphicsWidget.h \
    CarshService/qpartnerswidget.h \
    CarshService/qproviderswidget.h \
    CarshService/qtaskswidget.h \
    Carshs/qcarshempltaskswidget.h \
    Carshs/qcarshmaindlg.h \
    Carshs/qcarshparkingtaskcarddlg.h \
    Carshs/qcarshpenaltyparktaskcarddlg.h \
    Carshs/qcarshplatetaskcarddlg.h \
    Carshs/qdocstaskdlg.h \
    Carshs/qregistercarshuserdlg.h \
    Carshs/qrettozonecarddlg.h \
    Carshs/qsmenataskdlg.h \
    Dictionary/qdicitemeditdlg.h \
    Dictionary/qdicnewrecdlg.h \
    Dictionary/qdicwidget.h \
    Partners/qplatepartnercardwidget.h \
    Partners/qplatepartnerswidget.h \
    Partners/qplatepartnertaskwidget.h \
    Partners/qstickpartnercardwidget.h \
    Partners/qstickpartnerswidget.h \
    Partners/qstickpartnertaskdlg.h \
    Partners/qstickpartnertaskwidget.h \
    Partners/qwashpartnercardwidget.h \
    Partners/qwashpartnerpenaltydlg.h \
    Partners/qwashpartnerswidget.h \
    Partners/qwashpartnertaskdlg.h \
    Partners/qwashpartnertaskwidget.h \
    common.h \
    pictures/imagewidget.h \
    pictures/qfullscreenpicdialog.h \
    pictures/qpiclabel.h \
    pictures/qpictureswidget.h \
    qcarshservicemaindlg.h \
    qlogindlg.h \
    qregisterdlg.h \
    service/qdelopenpendlg.h \
    Dictionary/qdiceditdlg.h \
    service/qpointeditdlg.h \
    service/qselectavailablezakazs.h \
    service/qselfrombddlg.h \
    service/qtaskhistorydlg.h \
    service/quleditdlg.h \
    service/xlspatterns.h

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./QXlsx/        # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/
include(./QXlsx/QXlsx.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx{
    # ICON = Resources/icons/icon.icns
    ICON = Mac/icon.icns
    QMAKE_INFO_PLIST = Mac/Info.plist
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    templates/schet.xlsx
