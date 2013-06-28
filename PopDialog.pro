#-------------------------------------------------
#
# Project created by QtCreator 2013-05-28T17:52:05
#
#-------------------------------------------------

QT       += core gui widgets opengl

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PopDialog
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

CONFIG += static
#CONFIG +=    debug_and_release

#LIBS += scmfc18.lib \
#    sclib18.lib

SOURCES += main.cpp\
    tree.cpp \
    layouttree.cpp \
    popdialog.cpp \
    qslider2.cpp \
    painter.cpp \
    tinyxml/tinyxml2.cpp

HEADERS  += \
    tree.h \
    layouttree.h \
    popdialog.h \
    node.h \
    qslider2.h \
    painter.h \
    tinyxml/tinyxml2.h


FORMS    += popdialog.ui

OTHER_FILES += \
    android/version.xml \
    android/libjava.so \
    android/AndroidManifest.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-et/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-nl/strings.xml \
    android/java.pro \
    android/Makefile \
    android/READ-THIS-BEFORE-MANUALLY-ADDING-FILES-TO-PACKAGE.txt \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java
