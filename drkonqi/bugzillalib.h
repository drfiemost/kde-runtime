/*******************************************************************
* bugzillalib.h
* Copyright 2000-2003 Hans Petter Bieker <bieker@kde.org>     
*           2009    Dario Andres Rodriguez <andresbajotierra@gmail.com>
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of 
* the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
******************************************************************/

#ifndef BUGZILLALIB__H
#define BUGZILLALIB__H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QList>

#include <QtXml/QDomDocument>

#include <kio/job.h>

class KJob;
class QString;
class QByteArray;

//Typedefs for Bug Report Listing
typedef QMap<QString,QString>   BugMap; //Report basic fields map
typedef QList<BugMap>           BugMapList; //List of reports

//Main bug report data, full fields + comments
class BugReport
{
    public:
    
        BugReport();
        
        void setBugNumber( QString value ) { setData("bug_id", value); }
        QString bugNumber() { return getData("bug_id"); }
        
        void setShortDescription( QString value ) { setData("short_desc", value); }
        QString shortDescription() { return getData("short_desc"); }
        
        void setProduct( QString value ) { setData("product", value); }
        QString product() { return getData("product"); }
        
        void setComponent( QString value ) { setData("component", value); }
        QString component() { return getData("component"); }
        
        void setVersion( QString value ) { setData("version", value); }
        QString version() { return getData("version"); }
        
        void setOperatingSystem( QString value ) { setData("op_sys", value); }
        QString operatingSystem() { return getData("op_sys"); }
        
        void setBugStatus( QString value ) { setData("bug_status", value); }
        QString bugStatus() { return getData("bug_status"); }
        
        void setResolution( QString value ) { setData("resolution", value); }
        QString resolution() { return getData("resolution"); }
        
        void setPriority( QString value ) { setData("priority", value); }
        QString priority() { return getData("priority"); }
        
        void setBugSeverity( QString value ) { setData("bug_severity", value); }
        QString bugSeverity() { return getData("bug_severity"); }
        
        void setDescription( QString desc ) { m_commentList.insert(0, desc); }
        QString description() { return m_commentList.at(0); }
        
        void setComments( QStringList comm) { m_commentList.append( comm ); }
        QStringList comments() { return m_commentList.mid(1); }
        
        void setValid( bool valid ) { m_isValid = valid; }
        bool isValid() { return m_isValid; }
        
        QString toHtml();
        
    private:
        
        bool        m_isValid;
        
        BugMap      m_dataMap;
        QStringList m_commentList;
        
        void setData( QString key, QString val ) { m_dataMap.insert(key,val); }
        QString getData( QString key ) { return m_dataMap.value(key); }
        
        
};

//XML parser that creates a BugReport object
class BugReportXMLParser
{
    public:
        BugReportXMLParser( QByteArray );
        BugReport * parse();
        bool isValid() { return m_valid; }
        
    private:
        bool            m_valid;
        QDomDocument    m_xml;
        QString getSimpleValue( QString );
    
};

class BugListCSVParser
{
    public:
        BugListCSVParser( QByteArray );
        bool isValid() { return m_isValid; }
        BugMapList parse();
        
    private:
        bool m_isValid;
        QByteArray  m_data;
};

class BugzillaManager : public QObject
{
    Q_OBJECT
    
    public:
        BugzillaManager();
        
        void setLoginData( QString, QString );
        void tryLogin();
        
        void fetchBugReport( int );
        void searchBugs( QString words, QString product, QString severity, QString date_start, QString date_end , QString comment);
        
        void commitReport( BugReport * );
        
        bool getLogged() { return m_logged; }
        QString getUsername() { return m_username; }
        
        QString urlForBug( int bug_number );
        
    private Q_SLOTS:
        void loginDone(KJob*);
        void fetchBugReportDone(KJob*);
        void searchBugsDone(KJob*);
        void commitReportDone(KJob*);
        
    Q_SIGNALS:
        void loginFinished( bool );
        void bugReportFetched( BugReport* );
        void searchFinished( BugMapList );
        void reportCommited( int ); //???RESULT ?
        
        void loginError( QString );
        void searchError( QString );
        void bugReportError( QString );
        void commitReportError( QString );

    private:
    
        QString     m_username;
        QString     m_password;
        bool        m_logged;
        
        KJob *  fetchBugJob;

};

#endif
