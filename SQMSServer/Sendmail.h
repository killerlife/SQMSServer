#pragma once

#include <QString>
#include <QStringList>
#include <QObject>
#include <QAbstractSocket>
#include <QSslSocket>

#include "ConfigStruct.h"
#include "Log.h"
#include "Report.h"

class ISendmail
{
public:
	virtual int Send(const QString &subject, const QString &body,
		QStringList files = QStringList()) = 0;
	virtual void SetSmtpSetting(SMTPSETTING ss, IReport *pRep) = 0;
};

class Sendmail : public QObject, public ISendmail
{
	Q_OBJECT
public:
	int Send(const QString &subject, const QString &body,
		QStringList files = QStringList());
	void SetSmtpSetting(SMTPSETTING ss, IReport *pRep);
	Sendmail(int timeout = 30000);
	~Sendmail();

signals:
	void status( const QString &);
	void finished();

private slots:
	void stateChanged(QAbstractSocket::SocketState socketState);
	void errorReceived(QAbstractSocket::SocketError socketError);
	void disconnected();
	void connected();
	void readyRead();

private:
	SMTPSETTING mSS;
	ILog *pLog;
	int timeout;
	QString message;
	QTextStream *t;
	QSslSocket *socket;
	QStringList rcpt;
	QString response;
	enum states {Tls, HandShake ,Auth,User,Pass,Rcpt,Mail,Data,Init,Body,Quit,Close};
	int state;
	int recvCout;
	IReport* pRep;
	int nSend;
};

ISendmail * CreateSendmail();
void ReleaseSendmail(ISendmail* pMail);

