#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>

#include "Widget.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	static constexpr size_t fieldHeight = 800;
	static constexpr size_t fieldWidth	= 800;

	Widget *widget;


public:
	explicit MainWindow(QWidget *parent = 0): QMainWindow(parent), widget(new Widget(this, fieldHeight, fieldWidth)){
		this->setCentralWidget(this->widget);

		QTimer *timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this->widget, &Widget::render);
		timer->start(6);

		this->widget->start();
	}

	~MainWindow(){}

};

#endif // MAINWINDOW_HPP
