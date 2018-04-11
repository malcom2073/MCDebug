#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_ipc = new MCIPC("mcdebug",this);
	connect(m_ipc,SIGNAL(si_connected()),this,SLOT(ipcConnected()));
	connect(m_ipc,SIGNAL(si_publishMessage(QString,QByteArray)),this,SLOT(publishMessage(QString,QByteArray)));
	connect(m_ipc,SIGNAL(si_disconnected()),this,SLOT(ipcDisconnected()));
	m_ipc->connectToHost("127.0.0.1",12345);
	ui->treeWidget->setColumnCount(2);
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
	m_ipc->subscribeMessage("core/status");
	m_ipc->subscribeMessage("GpsProvider/navdata");
//	m_ipc->subscribeMessage("SysInfoProvider/sysinfo");
}
void MainWindow::ipcDisconnected()
{
	qDebug() << "IPC Disconnected! This means the master has died";
	qDebug() << "TODO: Handle this gracefully, reconnects maybe?";
	exit(-1);
}

void MainWindow::publishMessage(QString name,QByteArray payload)
{
	qDebug() << name << payload;
	if (name == "core/status")
	{
		QJsonDocument doc = QJsonDocument::fromJson(payload);
		QJsonArray top = doc.array();
		for (int i=0;i<top.size();i++)
		{
			QJsonObject subscriber = top.at(i).toObject();
			QString subscribername = subscriber.value("name").toString();
			if (!m_messageCountMap.contains(subscribername))
			{
				m_messageCountMap.insert(subscribername,subscriber.value("count").toInt());
			}
			QList<QTreeWidgetItem*> itemlist = ui->treeWidget->findItems(subscribername,Qt::MatchExactly);
			QTreeWidgetItem *item = 0;
			QJsonArray sublist = subscriber.value("subscribers").toArray();
			if (itemlist.size() == 0)
			{
				//Nothing in the tree, add it!
				item = new QTreeWidgetItem(QStringList() << subscribername);
				ui->treeWidget->addTopLevelItem(item);
				item->addChild(new QTreeWidgetItem(QStringList() << "Count" << "0"));
				item->addChild(new QTreeWidgetItem(QStringList() << "Freq" << "0"));
				item->addChild(new QTreeWidgetItem(QStringList() << "Subscribers" << "0"));
			}
			else
			{
				item = itemlist.at(0);
			}

			for (int j=0;j<item->childCount();j++)
			{
				if (item->child(j)->text(0) == "Count")
				{
					item->child(j)->setText(1,QString::number(subscriber.value("count").toInt()));
				}
				else if (item->child(j)->text(0) == "Freq")
				{
					int messagecount = subscriber.value("count").toInt() - m_messageCountMap.value(subscribername);

					item->child(j)->setText(1,QString::number(messagecount) + "hz");
					m_messageCountMap[subscribername] = subscriber.value("count").toInt();
				}
				else if (item->child(j)->text(0) == "Subscribers")
				{
					item->child(j)->setText(1,QString::number(sublist.size()));
					while (item->child(j)->childCount() > 0)
					{
						item->child(j)->removeChild(item->child(j)->child(0));
					}
					for (int k=0;k<sublist.size();k++)
					{
						item->child(j)->addChild(new QTreeWidgetItem(QStringList() << sublist.at(k).toString() << ""));
					}
				}

			}
		}
	}
	else if (name == "GpsProvider/navdata")
	{
		ui->plainTextEdit->clear();
		QJsonObject top = QJsonDocument::fromJson(payload).object();
		for (QJsonObject::const_iterator i = top.constBegin();i!=top.constEnd();i++)
		{
			ui->plainTextEdit->appendPlainText(i.key() + +":"+i.value().toVariant().toString());
		}
	}
}
