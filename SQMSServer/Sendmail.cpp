#include "Sendmail.h"
#include <QFile>
#include <QFileInfo>

ISendmail* CreateSendmail()
{
	return new Sendmail;
}

void ReleaseSendmail(ISendmail* pMail)
{
	return;
// 	Don't do this, because QObject is auto delete.
// 	if (pMail)
// 	{
// 		delete pMail;
// 	}
}

Sendmail::Sendmail(int timeout):socket(NULL), t(NULL), nSend(0)
{
	socket = new QSslSocket(this);

	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(errorReceived(QAbstractSocket::SocketError)));
	connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
	connect(socket, SIGNAL(disconnected()), this,SLOT(disconnected()));

	this->timeout = timeout;
	pLog = CreateLog();
}

Sendmail::~Sendmail()
{
	ReleaseLog(pLog);
	if (t)
		delete t;
	pRep->Exit(nSend);
}

void Sendmail::SetSmtpSetting(SMTPSETTING ss, IReport* pRep)
{
	mSS = ss;
	this->pRep = pRep;
}

int Sendmail::Send(const QString &subject, const QString &body, QStringList files)
{
	mSS.strRecvList.remove("\r");
	mSS.strRecvList.remove("\n");
	mSS.strSender.remove("\r");
	mSS.strSender.remove("\n");

	QString listRecv = mSS.strRecvList;

	listRecv.replace(" ", ", ");
	message.clear();
	message.append("From: LEONIS S-QMS<" + mSS.strSender + ">\n");
 	//message.append("To: " + listRecv + "\n");
	message.append("To: S-QMS List<s-qms-list@leoniscinema.com>\n");
	message.append("Subject: " + subject + "\n");
	//Let's intitiate multipart MIME with cutting boundary "frontier"
	message.append("MIME-Version: 1.0\n");
	message.append("Content-Type: multipart/mixed; boundary=frontier\n\n");

	message.append( "--frontier\n" );
	message.append( "Content-Type: text/html\n\n" );  //Uncomment this for HTML formating, coment the line below
	//    message.append( "Content-Type: text/plain\n\n" );
	message.append(body);
	message.append("\n\n");

	if(!files.isEmpty())
	{
		foreach(QString filePath, files)
		{
			QFile file(filePath);
			if(file.exists())
			{
				if (!file.open(QIODevice::ReadOnly))
				{
					pLog->Write(LOG_SENDMAIL, tr("Couldn't open the attachment file"));
					return 0;
				}
				pLog->Write(LOG_SENDMAIL, tr("Attach file: ") + filePath);
				QByteArray bytes = file.readAll();
				message.append(QString::fromLocal8Bit( "--frontier\n" ));
				message.append(QString::fromLocal8Bit( "Content-Type: application/octet-stream\nContent-Disposition: attachment; filename=")+ QFileInfo(file.fileName()).fileName() + QString::fromLocal8Bit(";\nContent-Transfer-Encoding: base64\n\n" ));
				message.append(bytes.toBase64());
				message.append(QString::fromLocal8Bit("\n"));
			}
			else
			{
				pLog->Write(LOG_SENDMAIL, tr("No File exists: ") + filePath);
			}
		}
	}
	else
		pLog->Write(LOG_SENDMAIL, tr("No attachments found"));

	message.append(QString::fromLocal8Bit( "--frontier--\n" ) );
	message.replace( QString::fromLocal8Bit("\n" ), QString::fromLocal8Bit( "\r\n" ) );
	message.replace( QString::fromLocal8Bit( "\r\n.\r\n" ),QString::fromLocal8Bit( "\r\n..\r\n" ) );

	rcpt = mSS.strRecvList.split(" ");
	state = Init;
	recvCout = 0;
	socket->connectToHostEncrypted(mSS.strServer, mSS.nPort); //"smtp.gmail.com" and 465 for gmail TLS
//    socket->connectToHost(host, port);
	if (!socket->waitForConnected(timeout)) 
	{
		pLog->Write(LOG_SENDMAIL, socket->errorString());
		return 0;
	}
	else
	{
	t = new QTextStream( socket );
		return 1;
	}
}

void Sendmail::stateChanged(QAbstractSocket::SocketState socketState)
{
	QString msg = tr("State change: ");
	switch(socketState)
	{
	case 4:
		msg += tr("BoundState");
		break;
	case 6:
		msg += tr("ClosingState");
		break;
	case 3:
		msg += tr("ConnectedState");
		break;
	case 5:
		msg += tr("ConnectingState");
		break;
	case 1:
		msg += tr("HostLookupState");
		break;
	case 2:
		msg += tr("ListeningState");
		break;
	case 0:
		msg += tr("UnconnectedState");
		break;
	default:
		msg += tr("UnknowState");
		break;
	}
	pLog->Write(LOG_SENDMAIL, msg);
}

void Sendmail::errorReceived(QAbstractSocket::SocketError socketError)
{
	pLog->Write(LOG_SENDMAIL, tr("Socket error"));
	socket->abort();
}

void Sendmail::disconnected()
{
	pLog->Write(LOG_SENDMAIL, tr("Disconnected"));
	pLog->Write(LOG_SENDMAIL, socket->errorString());
	socket->abort();
}

void Sendmail::connected()
{
	pLog->Write(LOG_SENDMAIL, tr("Connected"));
}

void Sendmail::readyRead()
{
    // SMTP is line-oriented
    QString responseLine;
    do
    {
        responseLine = socket->readLine();
        response += responseLine;
    }
    while ( socket->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate( 3 );

    if ( state == Init && responseLine == "220" )
    {
        // banner was okay, let's go on
        *t << "EHLO localhost" <<"\r\n";
        t->flush();

        state = HandShake;
    }
   //No need, because I'm using socket->startClienEncryption() which makes the SSL handshake for you
    /*else if (state == Tls && responseLine == "250")
    {
        // Trying AUTH
        qDebug() << "STarting Tls";
        *t << "STARTTLS" << "\r\n";
        t->flush();
        state = HandShake;
    }*/
    else if (state == HandShake && responseLine == "250")
    {
        socket->startClientEncryption();
        if(!socket->waitForEncrypted(timeout))
        {
			pLog->Write(LOG_SENDMAIL, socket->errorString());
            state = Close;
        }
        //Send EHLO once again but now encrypted

        *t << "EHLO localhost" << "\r\n";
        t->flush();
        state = Auth;
    }
    else if (state == Auth && responseLine == "250")
    {
        // Trying AUTH
        *t << "AUTH LOGIN" << "\r\n";
        t->flush();
        state = User;
    }
    else if (state == User && responseLine == "334")
    {
       //Trying User
        //GMAIL is using XOAUTH2 protocol, which basically means that password and username has to be sent in base64 coding
        //https://developers.google.com/gmail/xoauth2_protocol
        *t << QByteArray().append(mSS.strSender).toBase64()  << "\r\n";
        t->flush();

        state = Pass;
    }
    else if (state == Pass && responseLine == "334")
    {
        //Trying pass
        *t << QByteArray().append(mSS.strPwd).toBase64() << "\r\n";
        t->flush();

        state = Mail;
    }
    else if ( state == Mail && responseLine == "235" )
    {
        // HELO response was okay (well, it has to be)
	
        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
        *t << "MAIL FROM:<" << mSS.strSender << ">\r\n";
        t->flush();
        state = Rcpt;
    }
    else if ( state == Rcpt && responseLine == "250" )
    {
        if(recvCout < rcpt.size())
        {
			QString s = rcpt.at(recvCout++);
			s.remove("\r");
			s.remove("\n");
            //Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
            *t << "RCPT TO:<" << s << ">\r\n"; //r
            t->flush();
            if(recvCout == rcpt.size())
                state = Data;
            else
                state = Rcpt;
        }
    }
    else if ( state == Data && responseLine == "250" )
    {

        *t << "DATA\r\n";
        t->flush();
        state = Body;
    }
    else if ( state == Body && responseLine == "354" )
    {

        *t << message << "\r\n.\r\n";
        t->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine == "250" )
    {

        *t << "QUIT\r\n";
        t->flush();
        // here, we just close.
        state = Close;
    }
    else if ( state == Close )
    {
		nSend = 1;
        deleteLater();
        return;
    }
    else
    {
 // something broke.
//        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        state = Close;
		pLog->Write(LOG_SENDMAIL, tr("Failed to send message: ") + response);
    }
    response = "";
}

