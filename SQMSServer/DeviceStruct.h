#ifndef DEVICESTRUCT_H
#define DEVICESTRUCT_H

typedef enum {
	//----------------------------------------------------------------------
	//                       LLAS-100 function
	//               Use IDevice->Read to access device
	DATA_FTL = 0, //IDevice->Read, return "ftl"
	DATA_CMD2, //IDevice->Read, return "ccd"
	DATA_XY, //IDevice->Read, return "x\ty"
	DATA_ZYZ, //IDevice->Read, return "X\tY\tZ"
	DATA_SPL, //IDevice->Read, return "SPL"
	DATA_TEMP, //IDevice->Read, return "temperature_C\ttemperature_F"
	DATA_COLORTEMP, //IDevice->Read, return "color_temperature"
	DATA_THEATER, //IDevice->Read, return cinema name
	DATA_HALL, //IDevice->Read, return hall name
	CTRL_STANDBY, //IDevice->Read, don't care return
	CTRL_POWERON, //IDevice->Read, don't care return
	//End of LLAS-100 function
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	//                        CP850 function
	//              Use IDevice->Read to access device
	CP850_GETCONFIG, //IDevice->Read, return XML content 
	CP850_GETMUTE, //IDevice->Read, return "ON" or "OFF"
	//Use IDevice->Write to access device
	CP850_PINKNOISE, //IDevice->Write, strParam = "ON" or "OFF"
	CP850_SETCHANNEL, //IDevice->Write, strParam = "0" ... "64"
	CP850_CLEARCHANNEL, //IDevice->Write, don't care strParam
	CP850_SETMUTE, //IDevice->Write, strParm = "ON" or "OFF"
	//End of CP850 device
	//----------------------------------------------------------------------
	DATA_END
} DATACLASS;

typedef enum{
	DEVICE_LLAS100 = 0,
	DEVICE_CP850,
	DEVICE_NC2000C,
	DEVICE_END
}DEVICETYPE;

#endif