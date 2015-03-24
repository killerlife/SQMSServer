#include "ModSelect.h"
#include "Log.h"
#include "ConfigStruct.h"

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QCoreApplication>


SQMSTcpSocket::SQMSTcpSocket():isConnect(true)
{
	t = new QTextStream( this );
	connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(errorReceived(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
	connect(this, SIGNAL(disconnected()), this,SLOT(disconnected()));
}

SQMSTcpSocket::~SQMSTcpSocket()
{
	delete t;
}

void SQMSTcpSocket::stateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == UnconnectedState)
	{
		isConnect = false;
	}
}

void SQMSTcpSocket::errorReceived(QAbstractSocket::SocketError socketError)
{
	abort();
	isConnect = false;
}

void SQMSTcpSocket::disconnected()
{
	isConnect = false;
}

void SQMSTcpSocket::readyRead()
{
	QString responseLine;
	do
	{
		responseLine = this->readLine();
	}
	while ( this->canReadLine() && responseLine[3] != ' ' );
}

static SQMSModSelect staticModSelect;

IModSelect * CreateModSelect()
{
	return &staticModSelect;
}

void ReleaseModSelect(IModSelect* pMod)
{
	return;
}

SQMSModSelect::SQMSModSelect():isRun(false)
{
	mServer.setMaxPendingConnections(30);
	mServer.listen(QHostAddress::Any, 10003);
	connect(&mServer, SIGNAL(newConnection()), this, SLOT(newConnect()));
}

SQMSModSelect::~SQMSModSelect()
{
	mServer.close();
}

void SQMSModSelect::Start()
{
	if(!isRun)
	{
		isRun = true;
		//start();
	}
}

void SQMSModSelect::Stop()
{
	isRun = false;
	for (int i = 0; i < mClientList.size(); i++)
	{
		mClientList.at(i)->abort();
		delete mClientList.at(i);
	}
	mClientList.clear();
}

void SQMSModSelect::newConnect()
{
	SQMSTcpSocket *pConnect = (SQMSTcpSocket*) mServer.nextPendingConnection();
	mClientList.push_back(pConnect);
}

void SQMSModSelect::run()
{
	QList<SQMSTcpSocket*>::iterator itor;
	int i;
	while(isRun)
	{
		if(mClientList.isEmpty())
		{
			if (mServer.hasPendingConnections())
			{
				newConnect();
			}
		}
		else
		{
			for (i = 0; i < mClientList.size(); i++)
			{
				if (!mClientList.at(i)->GetConnectState())
				{
					break;
				}
			}
			delete mClientList.at(i);
			itor = mClientList.begin() + i;
			mClientList.erase(itor);
		}
		//msleep(200);
	}
}