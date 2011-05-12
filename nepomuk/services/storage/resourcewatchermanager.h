/*
    This file is part of the Nepomuk KDE project.
    Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef RESOURCEWATCHMANAGER_H
#define RESOURCEWATCHMANAGER_H

#include <QtCore/QMultiHash>
#include <QtCore/QSet>

#include <Soprano/FilterModel>
#include <QtDBus/QDBusObjectPath>

namespace Nepomuk {

    class ResourceWatcherConnection;
    
    class ResourceWatcherManager : public QObject
    {
        Q_OBJECT
        Q_CLASSINFO( "D-Bus Interface", "org.kde.nepomuk.ResourceWatcher" )
        
    public:
        ResourceWatcherManager( QObject* parent=0 );

        void addProperty(QList<QUrl> resources, QUrl property, QVariantList values);
        
    public slots:
        Q_SCRIPTABLE QDBusObjectPath watch( const QList<QString> & resources,
                                            const QList<QString> & properties,
                                            const QList<QString> & types );
        Q_SCRIPTABLE void stopWatcher( const QString& objectName );

    private:
        QMultiHash<QUrl, ResourceWatcherConnection*> m_resHash;
        QMultiHash<QUrl, ResourceWatcherConnection*> m_propHash;
        QMultiHash<QUrl, ResourceWatcherConnection*> m_typeHash;
        QSet<QUrl> m_metaProperties; 
        
        friend class ResourceWatcherConnection;
    };

}

#endif // RESOURCEWATCHMANAGER_H
