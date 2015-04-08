#include "Device.h"
#include <QTcpSocket>
#include "Log.h"
#include "soapSystemManagementBinding_USCOREv1_USCORE0Proxy.h"
#include "SystemManagementBinding_USCOREv1_USCORE0.nsmap"

class SQMSDevice : public IDevice
{
public:
	QString Read(DATACLASS dataClass);
	int Write(DATACLASS dataClass, const QString& strParam){ return 1;};
	int SetIp(QString strIp, int nPort = 10001);
	DEVICETYPE IdType(){return DEVICE_LLAS100;};

	SQMSDevice();
	~SQMSDevice();

protected:
	int Connect();
	QString ReadDevice(char* s);

private:
	QString mIp;
	QTcpSocket mSocket;
	int nPort;
	ILog* pLog;
	bool isConnected;
};

class Dolby850 : public IDevice
{
public:
	QString Read(DATACLASS dataClass);
	int Write(DATACLASS dataClass, const QString& strParam);
	int SetIp(QString strIp, int nPort);
	DEVICETYPE IdType(){return DEVICE_CP850;};

	Dolby850();
	~Dolby850();

private:
	QString mIp;
	ILog* pLog;
};

IDevice* CreateDevice(DEVICETYPE type)
{
	switch(type)
	{
	case DEVICE_LLAS100:
		return new SQMSDevice;
	case DEVICE_CP850:
		return new Dolby850;
	default:
		return NULL;
	}
}

void ReleaseDevice(IDevice *pDevice)
{
	if(pDevice != NULL)
		delete pDevice;
}

SQMSDevice::SQMSDevice():isConnected(false)
{
	pLog = CreateLog();
}

SQMSDevice::~SQMSDevice()
{
	if (isConnected)
	{
		mSocket.abort();
		isConnected = false;
	}
	ReleaseLog(pLog);
}

int SQMSDevice::Connect()
{
	QString msg;
	mSocket.connectToHost(mIp, nPort);
	if(!mSocket.waitForConnected(3000))
	{
		msg.sprintf("Connect to %s:%d error!", mIp.toStdString().c_str(), nPort);
		pLog->Write(LOG_DEVICE, msg);
		isConnected = false;
		return 0;
	}
	isConnected = true;
	return 1;
}

int SQMSDevice::SetIp(QString strIp, int nPort)
{
	mIp = strIp;
	this->nPort = nPort;
	return Connect();
}

QString SQMSDevice::Read(DATACLASS dataClass)
{
	QString result;
	if (!isConnected)
	{
		Connect();
	}
	if (isConnected)
	{
		try{
			QString cmd;
			switch(dataClass)
			{
			case DATA_FTL:
				cmd = "lss100.sys.ftl\r";
				break;
			case DATA_CMD2:
				cmd = "lss100.sys.cdm2\r";
				break;
			case DATA_XY:
				cmd = "lss100.sys.xy\r";
				break;
			case DATA_ZYZ:
				cmd = "getxyz\r";
				break;
			case DATA_SPL:
				cmd = "lss100.sys.spl\r";
				break;
			case DATA_TEMP:
				cmd = "lss100.sys.temperature\r";
				break;
			case DATA_COLORTEMP:
				cmd = "lss100.sys.cct\r";
				break;
			case DATA_THEATER:
				cmd = "lss100.sys.theater_name\r";
				break;
			case DATA_HALL:
				cmd = "lss100.sys.theater_number\r";
				break;
			case CTRL_STANDBY:
				cmd = "standby\r";
				break;
			case CTRL_POWERON:
				cmd = "poweron\r";
				break;
			}
			result = ReadDevice((char*)cmd.toStdString().c_str());
// 			pLog->Write(LOG_DEVICE, mIp + ":" + result);
		}
		catch(QString m)
		{
			pLog->Write(LOG_DEVICE, mIp + ": " + m);
			mSocket.abort();
			isConnected = false;
		}
	}
	return result;
}

QString SQMSDevice::ReadDevice(char* s)
{
	char buf[50];
	mSocket.write(s, strlen(s));
	if(!mSocket.waitForBytesWritten(5000))
		throw mSocket.errorString();
	memset(buf, 0, 50);
	if (!mSocket.waitForReadyRead(5000))
		throw mSocket.errorString();
	mSocket.read(buf, 50);
	return QString(buf);
}

Dolby850::Dolby850()
{
	pLog = CreateLog();
}

Dolby850::~Dolby850()
{
	ReleaseLog(pLog);
}

int Dolby850::SetIp(QString strIp, int nPort)
{
	mIp = strIp;
	return 1;
}

QString Dolby850::Read(DATACLASS dataClass)
{
	SystemManagementBinding_USCOREv1_USCORE0Proxy proxy;
	proxy.userid = "admin";
	proxy.passwd = "admin";
	QString result;
	QString endpoint;
	endpoint.sprintf("http://%s:9090/cp/ws/smi/v1/services/SystemManagement", mIp.toStdString().c_str());
	
	_ns3__getAtmosConfigRequest confReq;
	_ns3__getAtmosConfigResponse confResp;
	_ns3__getMuteRequest muteReq;
	_ns3__getMuteResponse muteRep;

	switch(dataClass)
	{
	case CP850_GETCONFIG:
		confReq.soap = &proxy;
		if(proxy.getAtmosConfig(endpoint.toStdString().c_str(), NULL, &confReq, confResp) != SOAP_OK)
		{
			pLog->Write(LOG_DEVICE, "CP850: getAtomsConfig failure");
			break;
		}
		result = QString::fromStdString(confResp.config);
		break;
	case CP850_GETMUTE:
		muteReq.soap = &proxy;
		if (proxy.getMute(endpoint.toStdString().c_str(), NULL, &muteReq, muteRep) != SOAP_OK)
		{
			pLog->Write(LOG_DEVICE, "CP850: getMute failure");
			break;
		}
		result = (muteRep.isMuted?"ON":"OFF");
		break;
	default:
		pLog->Write(LOG_DEVICE, "CP850: Unsupport command - " + QString::number(dataClass));
		break;
	}
	return result;
}

int Dolby850::Write(DATACLASS dataClass, const QString& strParam)
{
	SystemManagementBinding_USCOREv1_USCORE0Proxy proxy;
	proxy.userid = "admin";
	proxy.passwd = "admin";
	
	_ns3__setTestSignalOutputRequest reqOutput;
	_ns3__setTestSignalOutputResponse repOutput;
	ns2__testSignalObjectPath path;

	_ns3__setTestSignalModeRequest reqMode;
	_ns3__setTestSignalModeResponse repMode;

	_ns3__setMuteRequest reqMute;
	_ns3__setMuteResponse repMute;

	QString s;
	QString endpoint;
	endpoint.sprintf("http://%s:9090/cp/ws/smi/v1/services/SystemManagement", mIp.toStdString().c_str());

	switch(dataClass)
	{
	case CP850_SETCHANNEL:
		reqOutput.outputMode = ns2__testSignalOutputMode__speaker;
		reqOutput.speakerIndex.push_back(strParam.toStdString().c_str());
		path = ns2__testSignalObjectPath__allSpeakersSnap;
		reqOutput.objectPath = &path;
		reqOutput.soap = &proxy;
		if(proxy.setTestSignalOutput(endpoint.toStdString().c_str(), NULL, &reqOutput, repOutput) != SOAP_OK)
		{
			pLog->Write(LOG_DEVICE, "CP850: setTestSignalOutput failure - " + strParam);
			return 0;
		}
		break;
	case CP850_CLEARCHANNEL:
		reqOutput.outputMode = ns2__testSignalOutputMode__speaker;
		path = ns2__testSignalObjectPath__allSpeakersSnap;
		reqOutput.objectPath = &path;
		reqOutput.soap = &proxy;
		if(proxy.setTestSignalOutput(endpoint.toStdString().c_str(), NULL, &reqOutput, repOutput) != SOAP_OK)
		{
			pLog->Write(LOG_DEVICE, "CP850: setTestSignalOutput failure - clear");
			return 0;
		}
		break;
	case CP850_PINKNOISE:
		reqMode.soap = &proxy;
		reqMode.signalType = ((strParam == "ON") ? ns2__testSignalType__pinkNoise : ns2__testSignalType__none);
		if(proxy.setTestSignalMode(endpoint.toStdString().c_str(), NULL, &reqMode, repMode) != SOAP_OK)
		{
			pLog->Write(LOG_DEVICE, "CP850: setTestSignalMode failure - " + strParam);
			return 0;
		}
		break;
	case CP850_SETMUTE:
		reqMute.soap = &proxy;
		reqMute.shouldBeMuted = ((strParam == "ON")?true:false);
		if (proxy.setMute(endpoint.toStdString().c_str(), NULL, &reqMute, repMute) != SOAP_OK)
		{
			pLog->Write(LOG_DEVICE, "CP850: setMute - " + strParam);
			return 0;
		}
		break;
	default:
		pLog->Write(LOG_DEVICE, "CP850: Unsupport command - " + QString::number(dataClass));
		return 0;
	}

	return 1;
}

#include <QDomDocument>
#include <QDomElement>
QStringList Dolby850ParseConfig(const QString& config)
{
	QDomDocument doc;
	QString errStr;
	int errLine;
	int errColumn;
	QStringList speakerList;

	ILog* pLog = CreateLog();
	if (!doc.setContent(config, true, &errStr, &errLine, &errColumn))
	{
		pLog->Write(LOG_DEVICE, "CP850: ParseConfig error - " + errStr + " Line - " + QString::number(errLine) + " Col - " + QString::number(errColumn));
	}
	else
	{
		QDomElement root = doc.documentElement();
		QDomNodeList list = root.elementsByTagName("speakerEndpoint");
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement elment = list.at(i).toElement();
			QString id = elment.attribute("id").remove("SPKREP-");
			QString name = elment.firstChildElement("name").text();
			speakerList<<name;
		}
	}
	return speakerList;
}