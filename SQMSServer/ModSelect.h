#pragma once

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>

class IModSelect
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
};
#if 1
class SQMSTcpSocket : public QTcpSocket
{
	Q_OBJECT
public:
	SQMSTcpSocket();
	~SQMSTcpSocket();
	bool GetConnectState(){return isConnect;}

	private slots:
		void stateChanged(QAbstractSocket::SocketState socketState);
		void errorReceived(QAbstractSocket::SocketError socketError);
		void disconnected();
		void readyRead();

private:
	bool isConnect;
	QTextStream* t;
};

typedef QList<SQMSTcpSocket*> MODCLIENT;

class SQMSModSelect : public QObject, public IModSelect
{
	Q_OBJECT
public:
	void Start();
	void Stop();

	SQMSModSelect();
	~SQMSModSelect();

protected:
	void run();

private slots:
	void newConnect();

private:
	QTcpServer mServer;
	MODCLIENT mClientList;
	bool isRun;
};
#endif
IModSelect* CreateModSelect();
void ReleaseModSelect(IModSelect* pMod);
