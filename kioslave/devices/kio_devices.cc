/* This file is part of the KDE project
   Copyright (C) 2002,2003 Joseph Wenninger <jowenn@kde.org>
   Copyright (C) 2004 Waldo Bastian <bastian@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

   #include <kio/slavebase.h>
   #include <kapplication.h>
   #include <kdebug.h>
   #include <stdlib.h>
   #include <qtextstream.h>
   #include <klocale.h>
   #include <sys/stat.h>
   #include <dcopref.h>
   #include <dcopclient.h>
   #include <qdatastream.h>
   #include <time.h>
   #include <kprocess.h>

   class DevicesProtocol : public KIO::SlaveBase
   {
   public:
	DevicesProtocol( const QCString& protocol ,const QCString &pool, const QCString &app);
	virtual ~DevicesProtocol();
#if 0
	virtual void get( const KURL& url );
#endif
	virtual void stat(const KURL& url);
	virtual void listDir(const KURL& url);
	virtual void put( const KURL& url, int , bool overwrite, bool );
	virtual void mkdir( const KURL& url, int permissions );
	virtual void rename(KURL const &, KURL const &, bool);
	void listRoot();

   protected:
	void mountAndRedirect(const KURL& url);
	QString deviceNode(uint id);
	bool deviceMounted(const QString dev);
	bool deviceMounted(int);
	QString mountPoint(const QString dev);
	QString mountPoint(int);
	QString deviceType(int);
	QStringList deviceList();
	QStringList deviceInfo(const QString name);

	QStringList kmobile_list( QString deviceName );
   
   private:
	uint mountpointMappingCount();
	bool fullMode;
  };

  extern "C" {
	int kdemain( int argc, char **argv )
	{
		kdDebug(7126)<<"kdemain for devices"<<endl;
		// KApplication is used as we need to use the file:// ioslave
		// which need a valid kapp pointer
		KApplication::disableAutoDcopRegistration();
		KApplication app(argc, argv, "kio_devices", false, true);
		
		DevicesProtocol slave(argv[1],argv[2], argv[3]);
		slave.dispatchLoop();
		return 0;
	}
  }



static void createFileEntry(KIO::UDSEntry& entry, const QString& name, const QString& url, const QString& mime);
static void createDirEntry(KIO::UDSEntry& entry, const QString& name, const QString& url, const QString& mime);

DevicesProtocol::DevicesProtocol( const QCString& protocol, const QCString &pool, const QCString &app): 
		SlaveBase(protocol,  pool, app )
{
	kdDebug(7126)<<"Called with slavename: " << protocol << endl;
	if (protocol=="system") fullMode=true; else fullMode=false;
}

DevicesProtocol::~DevicesProtocol()
{
}

void DevicesProtocol::rename(KURL const &oldURL, KURL const &newURL, bool) {
	kdDebug(7126)<<"DevicesProtocol::rename(): old="<<oldURL<<" new="<<newURL<<endl;
	
	DCOPRef mountwatcher("kded", "mountwatcher");
	DCOPReply reply = mountwatcher.call( "setDisplayName",
	                                     oldURL.path().remove(0,1),
	                                     newURL.path().remove(0,1) );

	bool ok = false;
	
	if ( reply.isValid() )
	{
		ok = reply;
	}
	
	if(ok)
	{
		finished();
	}
	else
	{
		error(KIO::ERR_CANNOT_RENAME,
		      i18n("This device name already exists"));
	}
}


void DevicesProtocol::mountAndRedirect(const KURL& url)
{
	QString device;
	QString path = url.path();
	int i = path.find('/', 1);
	if (i > 0)
	{
		device = path.mid(1, i-1);
		path = path.mid(i+1);
	}
	else
	{
		device = path.mid(1);
		path = QString::null;
	}
	QStringList info = deviceInfo(device);

	if (info.empty())
	{
		error(KIO::ERR_SLAVE_DEFINED,i18n("Unknown device %1").arg(url.fileName()));
		return;
	}

	QStringList::Iterator it=info.begin();
	if (it!=info.end())
	{
		QString device=*it; ++it;
		if (it!=info.end())
		{
			++it; ++it;
			if (it!=info.end())
			{
				QString mp=*it; ++it;++it;
				if (it!=info.end())
				{
					bool mounted=((*it)=="true");
					if (!mounted)
					{
						if (!mp.startsWith("file:/"))
						{
							error(KIO::ERR_SLAVE_DEFINED,i18n("Device not accessible")
									+mp);
							return;
						}
						KProcess *proc = new KProcess;
						*proc << "kio_devices_mounthelper";
						*proc << "-m" << url.url();
						proc->start(KProcess::Block);
						int ec = 0;
						if (proc->normalExit())
							ec = proc->exitStatus();
						delete proc;

						if (ec)
						{
							error(KIO::ERR_SLAVE_DEFINED,i18n("Device not mounted"));
							return;
						}
					}
					KURL newUrl(mp);
					if (!path.isEmpty())
						newUrl.cd(path);
					redirection(newUrl);
					finished();
					return;
				}
			}
		}
	}
	error(KIO::ERR_SLAVE_DEFINED,i18n("Illegal data received"));
	return;
}


void DevicesProtocol::stat(const KURL& url)
{
	kdDebug(7126) << "stat: " << url << endl;
	if (url.path().length() <= 1)
	{
		KIO::UDSEntry   entry;
		if (fullMode)
			createDirEntry(entry, i18n("System"), "system:/", "inode/directory");
		else
			createDirEntry(entry, i18n("Devices"), "devices:/", "inode/directory");
		statEntry(entry);
		finished();
		return;
	}

	mountAndRedirect(url);
}



void DevicesProtocol::listDir(const KURL& url)
{
	kdDebug(7126) << "listdir: " << url << endl;
	if (url.path().length() <= 1)
	{
		listRoot();
		return;
	}

	mountAndRedirect(url);
}

void DevicesProtocol::put( const KURL& url, int /*permissions*/, bool /*overwrite*/, bool /*resume*/ )
{
	kdDebug(7126) << "put: " << url << endl;
	if (url.path().length() <= 1)
	{
		error( KIO::ERR_ACCESS_DENIED, url.prettyURL() );
		return;
	}

	mountAndRedirect(url);
}

void DevicesProtocol::mkdir( const KURL& url, int )
{
	kdDebug(7126) << "mkdir: " << url << endl;
	if (url.path().length() <= 1)
	{
		error( KIO::ERR_ACCESS_DENIED, url.prettyURL() );
		return;
	}

	mountAndRedirect(url);
}

uint DevicesProtocol::mountpointMappingCount()
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	uint count=0;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "mountpointMappingCount()", param,retType,data,false ) )
	{
		QDataStream stream1(data,IO_ReadOnly);
		stream1>>count;
	}
	return count;
}

QString DevicesProtocol::deviceNode(uint id)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	QString retVal;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<id;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "devicenode(int)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}

bool DevicesProtocol::deviceMounted(const QString dev)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	bool retVal=false;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<dev;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "mounted(QString)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}


QStringList DevicesProtocol::kmobile_list(const QString deviceName)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	QStringList retVal;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<deviceName;
	if ( dcopClient()->call( "kmobile",
		 "kmobileIface", "kio_devices_deviceInfo(QString)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}


QStringList DevicesProtocol::deviceInfo(QString name)
{
	kdDebug() << "DevicesProtocol::deviceInfo(" << name << ")" << endl;
	QByteArray data;
	QByteArray param;
	QCString retType;
	QStringList retVal;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<name;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "basicDeviceInfo(QString)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	// kmobile support
	if (retVal.isEmpty())
		retVal = kmobile_list(name);

	return retVal;
}


bool DevicesProtocol::deviceMounted(int id)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	bool retVal=false;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<id;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "mounted(int)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}


QStringList DevicesProtocol::deviceList()
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	QStringList retVal;
	QDataStream streamout(param,IO_WriteOnly);
	
	kdDebug(7126)<<"list dir: Fullmode=="<<fullMode<<endl;
	QString dcopFun=fullMode?"basicSystemList()":"basicList()";
	if ( dcopClient()->call( "kded",
		 "mountwatcher", dcopFun.utf8(), param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	else
	{
		retVal.append(QString::fromLatin1("!!!ERROR!!!"));
	}
	// add mobile devices info (kmobile)
	retVal += kmobile_list(QString::null);

	return retVal;
}

QString DevicesProtocol::mountPoint(const QString dev)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	QString retVal;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<dev;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "mountpoint(QString)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}

QString DevicesProtocol::mountPoint(int id)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	QString retVal;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<id;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "mountpoint(int)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}



QString DevicesProtocol::deviceType(int id)
{
	QByteArray data;
	QByteArray param;
	QCString retType;
	QString retVal;
	QDataStream streamout(param,IO_WriteOnly);
	streamout<<id;
	if ( dcopClient()->call( "kded",
		 "mountwatcher", "type(int)", param,retType,data,false ) )
	{
		QDataStream streamin(data,IO_ReadOnly);
		streamin>>retVal;
	}
	return retVal;
}



void DevicesProtocol::listRoot()
{
	KIO::UDSEntry   entry;
	uint count;

	QStringList list=deviceList();
	count=0;
	for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
	{
		if ((*it)=="!!!ERROR!!!")
		{
			error(KIO::ERR_SLAVE_DEFINED,i18n("The KDE mountwatcher is not running. Please activate it in Control Center->KDE Components->Service Manager, if you want to use the devices:/ protocol"));
			return;
		}
// FIXME: look for the real ending
		++it;
		QString url="devices:/"+(*it); //++it;
		QString name=*it; ++it;
		++it; ++it;
		QString type=*it; ++it; ++it;
		createFileEntry(entry,name,url,type);
		listEntry(entry,false);
		count++;
	}
	totalSize(count);
	listEntry(entry, true);


	// Jobs entry

	// finish
	finished();
}

#if 0
 void DevicesProtocol::get( const KURL& url )
 {
/*	mimeType("application/x-desktop");
	QCString output;
	output.sprintf("[Desktop Action Format]\n"
			"Exec=kfloppy\n"
			"Name=Format\n"
			"[Desktop Entry]\n"
			"Actions=Format\n"
			"Dev=/dev/fd0\n"
			"Encoding=UTF-8\n"
			"Icon=3floppy_mount\n"
			"MountPoint=/media/floppy\n"
			"ReadOnly=false\n"
			"Type=FSDevice\n"
			"UnmountIcon=3floppy_unmount\n"
			);
     data(output);
     finished();
 */
  redirection("file:/");
  //finished();
}
#endif

void addAtom(KIO::UDSEntry& entry, unsigned int ID, long l, const QString& s = QString::null)
{
	KIO::UDSAtom    atom;
	atom.m_uds = ID;
	atom.m_long = l;
	atom.m_str = s;
	entry.append(atom);
}

static void createFileEntry(KIO::UDSEntry& entry, const QString& name, const QString& url, const QString& mime)
{
	entry.clear();
	addAtom(entry, KIO::UDS_NAME, 0, name);
	addAtom(entry, KIO::UDS_FILE_TYPE, S_IFDIR);//REG);
	addAtom(entry, KIO::UDS_URL, 0, url);
	addAtom(entry, KIO::UDS_ACCESS, 0500);
	if (mime.startsWith("icon:")) {
		kdDebug(7126)<<"setting prefered icon:"<<mime.right(mime.length()-5)<<endl;
		addAtom(entry,KIO::UDS_ICON_NAME,0,mime.right(mime.length()-5));
		addAtom(entry,KIO::UDS_MIME_TYPE,0,"inode/directory");
	}
	else {
		addAtom(entry, KIO::UDS_MIME_TYPE, 0, mime);
	}
	addAtom(entry, KIO::UDS_SIZE, 0);
	addAtom(entry, KIO::UDS_GUESSED_MIME_TYPE, 0, "inode/directory");
	addAtom(entry, KIO::UDS_CREATION_TIME,1);
	addAtom(entry, KIO::UDS_MODIFICATION_TIME,time(0));
}


static void createDirEntry(KIO::UDSEntry& entry, const QString& name, const QString& url, const QString& mime)
{
	entry.clear();
	addAtom(entry, KIO::UDS_NAME, 0, name);
	addAtom(entry, KIO::UDS_FILE_TYPE, S_IFDIR);
	addAtom(entry, KIO::UDS_ACCESS, 0500);
	kdDebug(7126)<<"Dir Entry: "<<mime<<endl;
	if (mime.startsWith("icon:")) {
		kdDebug(7126)<<"setting prefered icon:"<<mime.right(mime.length()-5)<<endl;
		addAtom(entry,KIO::UDS_ICON_NAME,0,mime.right(mime.length()-5));
		addAtom(entry,KIO::UDS_MIME_TYPE,0,"inode/directory");	
	}
	else {
		addAtom(entry, KIO::UDS_MIME_TYPE, 0, mime);
	}
	addAtom(entry, KIO::UDS_URL, 0, url);
	addAtom(entry, KIO::UDS_SIZE, 0);
	addAtom(entry, KIO::UDS_GUESSED_MIME_TYPE, 0, "inode/directory");

//	addAtom(entry, KIO::UDS_GUESSED_MIME_TYPE, 0, "application/x-desktop");
}
