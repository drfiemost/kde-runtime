// object to handle network manager events
#ifndef WILL_NM_OBJECT
#define WILL_NM_OBJECT

#include <QCoreApplication>
#include <qdbusextratypes.h>

class QDBusInterface;

Q_DECLARE_METATYPE(QList<QDBusObjectPath>)

class NMObject : public QCoreApplication
{
Q_OBJECT
	public:
	NMObject( int argc, char ** argv );
	~NMObject();
	
	void showDevices();
	public slots:
	void updateNetwork(QDBusObjectPath,QDBusObjectPath);
	void deviceStrengthChanged(QDBusObjectPath,int);
	void netStrengthChanged(QDBusObjectPath,QDBusObjectPath,int);
	void wirelessNetworkAppeared(QDBusObjectPath,QDBusObjectPath);
	void wirelessNetworkDisappeared(QDBusObjectPath,QDBusObjectPath);
	private:
	QDBusInterface * nmIface;
	QDBusInterface * nmDeviceIface;
};

#endif
	
