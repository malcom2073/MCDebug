#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include "mcipc.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	MCIPC *m_ipc;
	QMap<QString,int> m_messageCountMap;
private slots:
	void ipcConnected();
	void ipcDisconnected();
	void publishMessage(QString name,QByteArray payload);
};

#endif // MAINWINDOW_H
