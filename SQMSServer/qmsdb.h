#ifndef QMSDB_H
#define QMSDB_H

#include <QObject>
#include <QSqlDatabase>

class QMSDB : public QObject
{
	Q_OBJECT

public:
	QMSDB(QObject *parent);
	~QMSDB();

private:
	QSqlDatabase db;
};

#endif // QMSDB_H
