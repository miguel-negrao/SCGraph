#include "main_window.h"

#include <QtGui/QApplication>


void MainWindow::start() {

}

void MainWindow::stop() {

}

int main(int argc, char *argv[]) {
	QApplication qapp(argc, argv);

	MainWindow main_window;
	main_window.show();
	qapp->exec();
}
