QT += core
QT -= gui

CONFIG += c++11

TARGET = timer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    foo.cpp

HEADERS += \
    foo.h
