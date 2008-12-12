#ifndef SCGRAPH_MAINWINDOW_HH
#define SCGRAPH_MAINWINDOW_HH

#include <QtGui/QMainWindow>
#include <QtGui/QTextEntry>

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

	public slots:
		void start();
		void stop();
};

#endif
