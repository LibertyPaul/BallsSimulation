#-------------------------------------------------
#
# Project created by QtCreator 2016-06-17T09:19:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BallsSimulation
TEMPLATE = app


SOURCES += main.cpp\
    Ball.cpp

HEADERS  += MainWindow.hpp \
    Field.hpp \
    Ball.hpp \
    Widget.hpp

CONFIG += c++14
