/*  This file is part of the KDE libraries
    Copyright (C) 2000 David Faure <faure@kde.org>

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

#ifndef _ZIP_H
#define _ZIP_H "$Id$"

#include <kio/slavebase.h>
#include <sys/types.h>

class KZipEntry;

class ZIPProtocol : public KIO::SlaveBase
{
public:
    ZIPProtocol( const QCString &pool, const QCString &app );
    virtual ~ZIPProtocol();

    virtual void listDir( const KURL & url );
    virtual void stat( const KURL & url );
    virtual void get( const KURL & url );

protected:
    void createUDSEntry( const KArchiveEntry * archiveEntry, KIO::UDSEntry & entry );
    bool checkNewFile( QString fullPath, QString & path );

    KZip * m_zipFile;
    time_t m_mtime;
};

#endif
