######################################################################
# Automatically generated by qmake (2.01a) ?? ????. 12 12:14:21 2012
######################################################################

TEMPLATE = app
TARGET =
DEPENDPATH += . \
              resources \
              src/common \
              src/dataCreater \
              src/debug \
              src/htm \
              src/main \
              src/options

INCLUDEPATH += . src/common src/dataCreater src/htm src/options src/main

# Input
HEADERS += src/common/Random.h \
           src/dataCreater/ImageDataCreator.h \
           src/debug/debughelper.h \
           src/htm/DataSample.h \
           src/htm/Net.h \
           src/htm/NetParams.h \
           src/htm/Sensor.h \
           src/htm/Synapse.h \
           src/main/MainWindow.h \
           src/options/Options.h

FORMS += src/main/MainWindow.ui

SOURCES += src/common/Random.cpp \
           src/dataCreater/ImageDataCreator.cpp \
           src/debug/debughelper.cpp \
           src/htm/Net.cpp \
           src/htm/NetParams.cpp \
           src/htm/Sensor.cpp \
           src/htm/Synapse.cpp \
           src/main/main.cpp \
           src/main/MainWindow.cpp

RESOURCES += resources/resources.qrc
