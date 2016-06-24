#include <QApplication>
#include <QSurfaceFormat>

#include "MainWindow.hpp"

int main(int argc, char *argv[]){
	QApplication a(argc, argv);

	QSurfaceFormat fmt;
	fmt.setSamples(4);
	QSurfaceFormat::setDefaultFormat(fmt);

	MainWindow w;
	w.show();

	return a.exec();
}
