#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T13:36:36
#
#-------------------------------------------------

QT       += core gui

CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoadEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    #RoadNetwork.cpp \
    #RoadNetworkGenerator.cpp \
    RoadSegment.cpp \
    LaneLimits.cpp \
    DataReader.cpp \
    RoadManager.cpp \
    MarkingLine.cpp \
    Geometry.cpp \
    Intersection.cpp \
    RoadConnection.cpp \
    TestRoadSegments.cpp \
    UnitRoadSegment.cpp \
    OsmXmlReader.cpp \
    OsmGeojsonReader.cpp \
    OsmData.cpp \
    OsmFilter.cpp

HEADERS  += \
    #RoadNetwork.h \
    #RoadNetworkGenerator.h \
    stdafx.h \
    RoadSegment.h \
    LaneLimits.h \
    Constants.h \
    Formulaes.h \
    DataReader.h \
    RoadManager.h \
    Singleton.h \
    mainwindow.h \
    MarkingLine.h \
    Drawable.h \
    Geometry.h \
    Intersection.h \
    RoadConnection.h \
    TestRoadSegments.h \
    UnitRoadSegment.h \
    OsmXmlReader.h \
    OsmGeojsonReader.h \
    BaseDataReaderImpl.h \
    OsmData.h \
    MappedStorage.h \
    RoadSegmentDataImpl.h \
    OsmFilter.h \
    NodeDataImpl.h \
    CrossingDataImpl.h \
    BusStopDataImpl.h \
    TrafficSignalsDataImpl.h \
    GiveWaySignDataImpl.h \
    StopSignDataImpl.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib/ -L/usr/lib -lxml++-3.0 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lglib-2.0  #-ljson_spirit

#-I/usr/include/libxml2
#-I/usr/include/glibmm-2.4
#-I/usr/lib/x86_64-linux-gnu/glibmm-2.4/include
#-I/usr/include/sigc++-2.0
#-I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include
#-I/usr/include/glib-2.0
#-I/usr/lib/x86_64-linux-gnu/glib-2.0/include
#-I/usr/include/libxml++-2.6
#-I/usr/lib/libxml++-2.6/include
#-lxml++-2.6 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lglib-2.0

INCLUDEPATH += /home/eugene/Projects/3rdParty/libxml++
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glibmm-2.4/include
INCLUDEPATH += /usr/include/glibmm-2.4
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/glib-2.0
