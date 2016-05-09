#-------------------------------------------------
#
# Project created by QtCreator 2016-05-06T08:09:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HikDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#LIBS += ./libhcnetsdk.so\
#    ./libhpr.so\
#    ./libHCCore.so\
#    ./HCNetSDKCom/libStreamTransClient.so\
#    ./HCNetSDKCom/libHCPreview.so\
#    ./HCNetSDKCom/libSystemTransform.so\
#    ./HCNetSDKCom/libHCAlarm.so\
#    ./HCNetSDKCom/libHCPlayBack.so\
#    ./HCNetSDKCom/libiconv2.so\
#    ./HCNetSDKCom/libHCIndustry.so\
#    ./HCNetSDKCom/libHCDisplay.so\
#    ./HCNetSDKCom/libHCVoiceTalk.so\
#    ./HCNetSDKCom/libHCCoreDevCfg.so\
#    ./HCNetSDKCom/libHCGeneralCfgMgr.so\
#    ./HCNetSDKCom/libanalyzedata.so
#LIBS += -L./ -lhcnetsdk -L./HCNetSDKCom/ -lHCP
LIBS += -L./ -lhcnetsdk -lhpr -lHCCore\
    -L./HCNetSDKCom/ -lStreamTransClient -lHCPreview -lSystemTransform -lHCAlarm -lHCPlayBack -liconv2 -lHCIndustry -lHCDisplay -lHCVoiceTalk -lHCCoreDevCfg -lHCGeneralCfgMgr -lanalyzedata
