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
    CarshService/CarshSubWidgets/qcarshemplwidget.cpp \
    CarshService/CarshSubWidgets/qcarshlistwidget.cpp \
    CarshService/CarshSubWidgets/qcarshdlg.cpp \
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
    Carshs/qcarshmaindlg.cpp \
    Carshs/qcarshparkingtaskcarddlg.cpp \
    Carshs/qcarshpenaltyparktaskcarddlg.cpp \
    Carshs/qcarshplatetaskcarddlg.cpp \
    Carshs/qregistercarshuserdlg.cpp \
    Carshs/qrettozonecarddlg.cpp \
    common.cpp \
    main.cpp \
    pictures/imagewidget.cpp \
    pictures/qfullscreenpicdialog.cpp \
    pictures/qpiclabel.cpp \
    pictures/qpictureswidget.cpp \
    qcarshservicemaindlg.cpp \
    qlogindlg.cpp \
    qregisterdlg.cpp

HEADERS += \
     ../CarshService/service_widgets/qlinetext.h \
    #BDPatterns.h \
    CarshService/CarshSubWidgets/qcarshemplwidget.h \
    CarshService/CarshSubWidgets/qcarshlistwidget.h \
    CarshService/CarshSubWidgets/qcarshdlg.h \
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
    Carshs/qcarshmaindlg.h \
    Carshs/qcarshparkingtaskcarddlg.h \
    Carshs/qcarshpenaltyparktaskcarddlg.h \
    Carshs/qcarshplatetaskcarddlg.h \
    Carshs/qregistercarshuserdlg.h \
    Carshs/qrettozonecarddlg.h \
    common.h \
    pictures/imagewidget.h \
    pictures/qfullscreenpicdialog.h \
    pictures/qpiclabel.h \
    pictures/qpictureswidget.h \
    qcarshservicemaindlg.h \
    qlogindlg.h \
    qregisterdlg.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
