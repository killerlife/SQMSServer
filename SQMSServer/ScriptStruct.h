#pragma once

#include <QList>
#include <QString>

typedef struct {
	QString name;
	QString value;
} LISTITEM;

typedef QList<LISTITEM> TScriptItemList;
typedef QList<LISTITEM> TValueItemList;

