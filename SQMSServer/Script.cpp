#include "Script.h"
#include "Log.h"
#include "Device.h"
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QThread>

class SQMSScript : public IScript
{
public:
	int LoadScript(char* fn);
	int LoadScript(QString script);
	int ExecScript();
	int AddDevice(DEVICETYPE type, IDevice* pDev, int pos = 0);
	TValueItemList* GetTestValueList();
	int SetFixValue(const QMSDEVICE& mQdev);

	SQMSScript::SQMSScript();
	SQMSScript::~SQMSScript();

private:
	ILog* pLog;
	IDevice* pSensorBack, *pSensorFront;
	IDevice* pAudioProcessor, *pProjector;
	TScriptItemList listScript;
	TValueItemList listValue;
	QMSDEVICE qDev;
};

IScript* CreateScript()
{
	return new SQMSScript;
}

void ReleaseScript(IScript* pScript)
{
	if (pScript != NULL)
	{
		delete pScript;
	}
}

SQMSScript::SQMSScript()
	:pSensorBack(NULL), pSensorFront(NULL), pAudioProcessor(NULL), pProjector(NULL)
{
	pLog = CreateLog();
}

SQMSScript::~SQMSScript()
{
	ReleaseLog(pLog);
}

int SQMSScript::SetFixValue(const QMSDEVICE& mQdev)
{
	qDev = mQdev;
	return 1;
}

int SQMSScript::AddDevice(DEVICETYPE type, IDevice *pDev, int pos /* = 0 */)
{
	switch(type)
	{
	case DEVICE_LLAS100:
		if (pos == 0)
		{
			pSensorBack = pDev;
		}
		else
			pSensorFront = pDev;
		break;
	case DEVICE_NC2000C:
		pProjector = pDev;
		break;
	case DEVICE_CP850:
		pAudioProcessor = pDev;
		break;
	}
	return 1;
}

int SQMSScript::LoadScript(char* fn)
{
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		pLog->Write(LOG_SCRIPT, QString("Open script file error!"));
		return 0;
	}
	QTextStream in(&f);
	QString buf = in.readAll();
	return 1;
}

int SQMSScript::LoadScript(QString script)
{
	listScript.clear();

	script.remove("\r");
	script.remove("\n");
	QStringList strList = script.split(";");
	for (int i = 0; i < strList.size(); i++)
	{
		QStringList sl = strList.at(i).split("=");
		if(sl.size() == 2)
		{
			LISTITEM item;
			item.name = sl.at(0);
			item.name.remove(" ");
			item.value = sl.at(1);
			item.value.remove(" ");
			listScript.append(item);
		}
	}
	if (listScript.size() > 0)
		return 1;
	else
	{
		pLog->Write(LOG_SCRIPT, QString("Script list is empty."));
		return 0;
	}
}

int SQMSScript::ExecScript()
{
	QThread delay;
	listValue.clear();
	float t;

	for (int i = 0; i < listScript.size(); i++)
	{
		LISTITEM item = listScript.at(i);

		//---------------------------------------------------------------------------------
		//Sleep script processor
		//item.name = item.name.toLower();
		if (item.name.contains("sleep"))
		{
			delay.msleep(item.value.toInt());
		}
		//End of sleep script processor
		//---------------------------------------------------------------------------------

		//---------------------------------------------------------------------------------
		//LLAS-100 Sensor script processor
		if (item.name.contains("sensor"))
		{
			if(item.name.contains("ftl"))
			{
				LISTITEM valueItem;
				switch(item.value.toInt())
				{
				case 0:
					valueItem.value = pSensorBack->Read(DATA_FTL);
					if(valueItem.value.isEmpty() || valueItem.value == "")
					{
						valueItem.value = "0";
					}
					if (item.name.contains("white"))
					{
						valueItem.name = "white_ftl";
						t = valueItem.value.toFloat() + qDev.fix_ftl;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else if (item.name.contains("red"))
					{
						valueItem.name = "red_ftl";
					}
					else if (item.name.contains("green"))
					{
						valueItem.name = "green_ftl";
					}
					else if (item.name.contains("blue"))
					{
						valueItem.name = "blue_ftl";
					}
					else
					{
						valueItem.name = "unsupport";
					}
					listValue.append(valueItem);
					break;
				default:
					break;
				}
			}
			if(item.name.contains("xy"))
			{
				LISTITEM valueItem;
				QStringList listXY;
				switch(item.value.toInt())
				{
				case 0:
					listXY = pSensorBack->Read(DATA_XY).split("\t"); 
					if(listXY.size() < 2)
					{
						listXY<<"0";
						listXY<<"0";
					}
					if (item.name.contains("white"))
					{
						valueItem.name = "wx";
						valueItem.value = listXY.at(0);
						listValue.append(valueItem);
						valueItem.name = "wy";
						valueItem.value = listXY.at(1);
						listValue.append(valueItem);
					}
					else if (item.name.contains("red"))
					{
						valueItem.name = "rx";
						valueItem.value = listXY.at(0);
						listValue.append(valueItem);
						valueItem.name = "ry";
						valueItem.value = listXY.at(1);
						listValue.append(valueItem);						
					}
					else if (item.name.contains("green"))
					{
						valueItem.name = "gx";
						valueItem.value = listXY.at(0);
						listValue.append(valueItem);
						valueItem.name = "gy";
						valueItem.value = listXY.at(1);
						listValue.append(valueItem);
					}
					else if (item.name.contains("blue"))
					{
						valueItem.name = "bluex";
						valueItem.value = listXY.at(0);
						listValue.append(valueItem);
						valueItem.name = "bluey";
						valueItem.value = listXY.at(1);
						listValue.append(valueItem);
					}
					else
						valueItem.name = "unsupport";
					break;
				default:
					break;
				}
			}
			if(item.name.contains("spl"))
			{
				LISTITEM valueItem;
				switch(item.value.toInt())
				{
				case 0:
					valueItem.value = pSensorBack->Read(DATA_SPL);
					if(valueItem.value.isEmpty() || valueItem.value == "")
					{
						valueItem.value = "0";
					}
					if (item.name.contains("left"))
					{
						valueItem.name = "lc";
						t = valueItem.value.toFloat() + qDev.fix_lc;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else if (item.name.contains("right"))
					{
						valueItem.name = "rc";
						t = valueItem.value.toFloat() + qDev.fix_rc;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else if (item.name.contains("center"))
					{
						valueItem.name = "cc";
						t = valueItem.value.toFloat() + qDev.fix_cc;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else if (item.name.contains("lfe"))
					{
						valueItem.name = "hb";
						t = valueItem.value.toFloat() + qDev.fix_hb;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else if (item.name.contains("ls"))
					{
						valueItem.name = "la";
						t = valueItem.value.toFloat() + qDev.fix_la;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else if (item.name.contains("rs"))
					{
						valueItem.name = "ra";
						t = valueItem.value.toFloat() + qDev.fix_ra;
						if (t > 0)
						{
							valueItem.value = QString::number(t);
						}
					}
					else
					{
						valueItem.name = "unsupport";
					}
					listValue.append(valueItem);
					break;
				default:
					break;
				}
			}
		}
		//End of LLAS-100 script processor
		//---------------------------------------------------------------------------------

		//---------------------------------------------------------------------------------
		//CP850 script processor
		if(item.name.contains("audio") && pAudioProcessor != NULL)
		{
			if (pAudioProcessor->IdType() == DEVICE_CP850)
			{
				if (item.name.contains("cp850.speaker"))
				{
					LISTITEM valueItem;
					pAudioProcessor->Write(CP850_SETCHANNEL, item.value);
					pAudioProcessor->Write(CP850_PINKNOISE, "ON");
					delay.msleep(5000);
					valueItem.value = pSensorBack->Read(DATA_SPL);
					pAudioProcessor->Write(CP850_PINKNOISE, "OFF");
					pAudioProcessor->Write(CP850_CLEARCHANNEL, "");
					valueItem.name = "speaker-" + item.value;
					listValue.append(valueItem);
				}
			}
		}
		//End of CP850 script processor
		//---------------------------------------------------------------------------------	
	}
	return 1;
}

TValueItemList* SQMSScript::GetTestValueList()
{
	return &listValue;
}