/*
   Copyright (C) 2005-2006 by Olivier Goffart <ogoffart at kde.org>


   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 */

#include "knotifyconfig.h"

#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kdebug.h>
#include <QCache>


static KSharedConfig::Ptr retrieve_from_cache(const QString& filename, const char *resourceType="config") 
{
	static QCache<QString, KSharedConfig::Ptr> cache(25);
	if (cache.contains(filename)) 
		return *cache[filename];
	KSharedConfig::Ptr m = KSharedConfig::openConfig (filename , KConfig::NoGlobals, resourceType );
	cache.insert(filename, new KSharedConfig::Ptr(m));
	return m;
}


KNotifyConfig::KNotifyConfig( const QString & _appname, const ContextList & _contexts, const QString & _eventid )
	: appname (_appname),
	eventsfile(retrieve_from_cache(_appname+'/'+_appname + ".notifyrc" , "data" )),
	configfile(retrieve_from_cache(_appname+QString::fromAscii( ".notifyrc" ))),
	contexts(_contexts) , eventid(_eventid)
{
//	kDebug(300) << appname << " , " << eventid;
}

KNotifyConfig::~KNotifyConfig()
{
}

QString KNotifyConfig::readEntry( const QString & entry, bool path )
{
	QPair<QString , QString> context;
	foreach(  context , contexts )
	{
		const QString group="Event/" + eventid + '/' + context.first + '/' + context.second;
		if( configfile->hasGroup( group ) )
		{
			KConfigGroup cg(configfile, group);
			QString p=path ?  cg.readPathEntry(entry, QString()) : cg.readEntry(entry,QString());
			if(!p.isNull())
				return p;
		}
	}
//	kDebug(300) << entry << " not found in contexts ";
	const QString group="Event/" + eventid ;
	if(configfile->hasGroup( group ) )
	{
		KConfigGroup cg(configfile, group);
		QString p=path ?  cg.readPathEntry(entry, QString()) : cg.readEntry(entry,QString());
		if(!p.isNull())
			return p;
	}
//	kDebug(300) << entry << " not found in config ";
	if(eventsfile->hasGroup( group ) )
	{
            KConfigGroup cg( eventsfile, group);
		QString p=path ?  cg.readPathEntry(entry, QString()) : cg.readEntry(entry, QString());
		if(!p.isNull())
			return p;
	}
//	kDebug(300) << entry << " not found !!! ";

	return QString();
}

