QT += core gui network sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/../CarshService

#LIBS += -L/usr/local/lib -lpq

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icons\CarshService.ico

SOURCES += \
    ../CarshService/carsh_service_widgets/qcarshserviceemploeecarddlg.cpp \
    ../CarshService/common.cpp \
    ../CarshService/emploee_widgets/QEmploeeTasksDlg.cpp \
    ../CarshService/emploee_widgets/QSelectTaskEmploeer.cpp \
    ../CarshService/qsocketbd.cpp \
    ../CarshService/qsocketbdnetclient.cpp \
    ../CarshService/service_widgets/QCSButton.cpp \
    ../CarshService/service_widgets/qcalendardataselectdlg.cpp \
    ../CarshService/service_widgets/qclicablelabel.cpp \
    ../CarshService/service_widgets/qcsbasedialog.cpp \
    ../CarshService/service_widgets/qcsbasedlgscrollwidget.cpp \
    ../CarshService/service_widgets/qcsbaselistitemdelegate.cpp \
    ../CarshService/service_widgets/qcsbaselistwidget.cpp \
    ../CarshService/service_widgets/qcslineedit.cpp \
    ../CarshService/service_widgets/qcspaybasedialog.cpp \
    ../CarshService/service_widgets/qcsscrollarea.cpp \
    ../CarshService/service_widgets/qcsselectdialog.cpp \
    ../CarshService/service_widgets/qcsselectdlgbuttonswidget.cpp \
    ../CarshService/service_widgets/qfinddlg.cpp \
    ../CarshService/service_widgets/qinputtextdlg.cpp \
    ../CarshService/service_widgets/qlinetext.cpp \
    ../CarshService/service_widgets/qloaddoclinewidget.cpp \
    ../CarshService/service_widgets/qloaddocsdlg.cpp \
    ../CarshService/service_widgets/qnotecalendarwidget.cpp \
    ../CarshService/service_widgets/qselprovidercarshwidget.cpp \
    ../CarshService/service_widgets/qstringlistdlg.cpp \
    ../CarshService/service_widgets/qyesnodlg.cpp \
    ../CarshService/tasks/qcostsdialog.cpp \
    ../CarshService/tasks/qdocstaskdlg.cpp \
    ../CarshService/tasks/qparkingtaskdialog.cpp \
    ../CarshService/tasks/qpenaltyparkingdialog.cpp \
    ../CarshService/tasks/qplatetaskdialog.cpp \
    ../CarshService/tasks/qrettozonedialog.cpp \
    ../CarshService/tasks/qsmenadlg.cpp \
    BDPatterns.cpp \
    CarshService/CarshSubWidgets/qcarshemplwidget.cpp \
    CarshService/CarshSubWidgets/qcarshinputpenparkeditdlg.cpp \
    CarshService/CarshSubWidgets/qcarshinputpenparktaskswidget.cpp \
    CarshService/CarshSubWidgets/qcarshlistwidget.cpp \
    CarshService/CarshSubWidgets/qcarshdlg.cpp \
    CarshService/CarshSubWidgets/qsetemplforpenparktaskdlg.cpp \
    CarshService/EmplSubWidgets/qemplcostdlg.cpp \
    CarshService/EmplSubWidgets/qemplpendlg.cpp \
    CarshService/EmplSubWidgets/qemplpenwidget.cpp \
    CarshService/GraphicSubWidgets/qemplgraphwidget.cpp \#
    CarshService/GraphicSubWidgets/qpenparkgraphwidget.cpp \
    CarshService/ProviderSubWidgets/qproviderdlg.cpp \
    CarshService/ProviderSubWidgets/qproviderempllistwidget.cpp \
    CarshService/ProviderSubWidgets/qproviderslistwidget.cpp \
    CarshService/TasksSubWidgets/qnewtaskdlg.cpp \
    CarshService/TasksSubWidgets/qpenalparktaskfilterwidget.cpp \
    CarshService/WashWidgets/qwashemplwidget.cpp \
    CarshService/WashWidgets/qwashtaskwidget.cpp \
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
    CarshService/qwashwidget.cpp \
    Carshs/qcarshdocstaskdlg.cpp \
    Carshs/qcarshempltaskswidget.cpp \
    Carshs/qcarshmaindlg.cpp \
    Carshs/qcarshparkingtaskcarddlg.cpp \
    Carshs/qcarshpenaltyparktaskcarddlg.cpp \
    Carshs/qcarshplatetaskcarddlg.cpp \
  #  Carshs/qdocstaskdlg.cpp \
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
    #common.cpp \
    commondt.cpp \
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
    service/qselempldlg.cpp \
    service/qselfrombddlg.cpp \
    service/qtaskhistorydlg.cpp \
    service/quleditdlg.cpp \
    service/xlspatterns.cpp

HEADERS += \
    ../CarshService/carsh_service_widgets/qcarshserviceemploeecarddlg.h \
    ../CarshService/common.h \
    ../CarshService/emploee_widgets/QEmploeeTasksDlg.h \
    ../CarshService/emploee_widgets/QSelectTaskEmploeer.h \
    ../CarshService/qsocketbd.h \
    ../CarshService/qsocketbdnetclient.h \
    ../CarshService/service_widgets/QCSButton.h \
    ../CarshService/service_widgets/qcalendardataselectdlg.h \
    ../CarshService/service_widgets/qclicablelabel.h \
    ../CarshService/service_widgets/qcsbasedialog.h \
    ../CarshService/service_widgets/qcsbasedlgscrollwidget.h \
    ../CarshService/service_widgets/qcsbaselistitemdelegate.h \
    ../CarshService/service_widgets/qcsbaselistwidget.h \
    ../CarshService/service_widgets/qcslineedit.h \
    ../CarshService/service_widgets/qcspaybasedialog.h \
    ../CarshService/service_widgets/qcsscrollarea.h \
    ../CarshService/service_widgets/qcsselectdialog.h \
    ../CarshService/service_widgets/qcsselectdlgbuttonswidget.h \
    ../CarshService/service_widgets/qfinddlg.h \
    ../CarshService/service_widgets/qinputtextdlg.h \
     ../CarshService/service_widgets/qlinetext.h \
    #BDPatterns.h \
    ../CarshService/service_widgets/qloaddoclinewidget.h \
    ../CarshService/service_widgets/qloaddocsdlg.h \
    ../CarshService/service_widgets/qnotecalendarwidget.h \
    ../CarshService/service_widgets/qselprovidercarshwidget.h \
    ../CarshService/service_widgets/qstringlistdlg.h \
    ../CarshService/service_widgets/qyesnodlg.h \
    ../CarshService/tasks/qcostsdialog.h \
    ../CarshService/tasks/qdocstaskdlg.h \
    ../CarshService/tasks/qparkingtaskdialog.h \
    ../CarshService/tasks/qpenaltyparkingdialog.h \
    ../CarshService/tasks/qplatetaskdialog.h \
    ../CarshService/tasks/qrettozonedialog.h \
    ../CarshService/tasks/qsmenadlg.h \
    BDPatterns.h \
    CarshService/CarshSubWidgets/qcarshemplwidget.h \
    CarshService/CarshSubWidgets/qcarshinputpenparkeditdlg.h \
    CarshService/CarshSubWidgets/qcarshinputpenparktaskswidget.h \
    CarshService/CarshSubWidgets/qcarshlistwidget.h \
    CarshService/CarshSubWidgets/qcarshdlg.h \
    CarshService/CarshSubWidgets/qsetemplforpenparktaskdlg.h \
    CarshService/EmplSubWidgets/qemplcostdlg.h \
    CarshService/EmplSubWidgets/qemplpendlg.h \
    CarshService/EmplSubWidgets/qemplpenwidget.h \
    CarshService/GraphicSubWidgets/qemplgraphwidget.h \
    CarshService/GraphicSubWidgets/qpenparkgraphwidget.h \
    CarshService/ProviderSubWidgets/qproviderdlg.h \
    CarshService/ProviderSubWidgets/qproviderempllistwidget.h \
    CarshService/ProviderSubWidgets/qproviderslistwidget.h \
    CarshService/TasksSubWidgets/qnewtaskdlg.h \
    CarshService/TasksSubWidgets/qpenalparktaskfilterwidget.h \
    CarshService/WashWidgets/qwashemplwidget.h \
    CarshService/WashWidgets/qwashtaskwidget.h \
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
    CarshService/qwashwidget.h \
    Carshs/qcarshdocstaskdlg.h \
    Carshs/qcarshempltaskswidget.h \
    Carshs/qcarshmaindlg.h \
    Carshs/qcarshparkingtaskcarddlg.h \
    Carshs/qcarshpenaltyparktaskcarddlg.h \
    Carshs/qcarshplatetaskcarddlg.h \
    #Carshs/qdocstaskdlg.h \
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
    #common.h \
    commondt.h \
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
    service/qselempldlg.h \
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

win32 {
    DEFINES += WINVER=0x0601 _WIN32_WINNT=0x0601  # Windows 7
    QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x0601 -DWINVER=0x0601
}

macx{
    # ICON = Resources/icons/icon.icns
    ICON = Mac/icon.icns
    QMAKE_INFO_PLIST = Mac/Info.plist
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    templates/schet.xlsx
