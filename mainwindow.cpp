#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_ipc = new MCIPC("mcdebug",this);
	connect(m_ipc,SIGNAL(si_connected()),this,SLOT(ipcConnected()));
	connect(m_ipc,SIGNAL(si_publishMessage(QString,QByteArray)),this,SLOT(publishMessage(QString,QByteArray)));
	m_ipc->connectToHost("127.0.0.1",12345);
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::ipcConnected()
{
	//m_ipc->sendM
	qDebug() << "Sending test-advert";
//	m_ipc->advertizeMessage("test-advert");
	QByteArray payload;
	payload.append(0x01);
	payload.append(0x02);
	payload.append(0x03);
	m_ipc->publishMessage("MyMessage",payload);
	m_ipc->subscribeMessage("SysInfoProvider/sysinfo");
}
void MainWindow::publishMessage(QString name,QByteArray payload)
{
	qDebug() << name << payload;
}
