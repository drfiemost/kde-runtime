#include <stdio.h>
#include <stdlib.h>

#include <kdebug.h>
#include <kprocess.h>
#include <kstddirs.h>
#include <kinstance.h>

#include "info.h"

using namespace KIO;

InfoProtocol::InfoProtocol( const QCString &pool, const QCString &app )
    : SlaveBase( "info", pool, app )
    , m_page( "" )
    , m_node( "" )
{
    kDebugInfo( 7108, "InfoProtocol::InfoProtocol" );

    m_infoScript = locate( "data", "kio_info/kde-info2html" );

    m_perl = KGlobal::dirs()->findExe( "perl" );
    
    if( m_infoScript.isEmpty() )
	kDebugFatal( 7108, "Critical error: Cannot locate 'kde-info2html' for HTML-conversion" );
    
    kDebugInfo( 7108, "InfoProtocol::InfoProtocol - done" );
}

InfoProtocol::~InfoProtocol()
{
    kDebugInfo( 7108, "InfoProtocol::~InfoProtocol" );

    kDebugInfo( 7108, "InfoProtocol::~InfoProtocol - done" );
}

void InfoProtocol::get( const QString& path, const QString& /*query*/, bool /*reload*/ )
{
    kDebugInfo( 7108, "InfoProtocol::get" );
    kDebugInfo( 7108, path.data() );

    decodePath( path );
    /*
    if( m_page.isEmpty() )
    {
	//error( 1, "Syntax error in URL" );
	
	QByteArray array = errorMessage();
	
	data( array );
	finished();

	return;
    }
    */
    if ( m_page.isEmpty() )
      m_page = "dir";

    QString cmds("%1 %2 %3 %4 \"%5\" \"%6\"");
    QCString cmd = cmds.arg(m_perl).arg(m_infoScript).arg(locate("data", "kio_info/kde-info2html.conf")).arg(KGlobal::dirs()->findResourceDir("icon", "hicolor/22x22/actions/up.png")).arg(m_page).arg(m_node).latin1();
    
    FILE *fd = popen( cmd.data(), "r" );
    
    char buffer[ 4090 ];
    QByteArray array;
    
    while ( !feof( fd ) )
    {
      int n = fread( buffer, 1, 2048, fd );
      if ( n == -1 )
      {
        // ERROR
        pclose( fd );
	return;
      }
      array.setRawData( buffer, n );
      data( array );
      array.resetRawData( buffer, n );
    }
    
    pclose( fd );

    finished();
    
    kDebugInfo( 7108, "InfoProtocol::get - done" );
}

void InfoProtocol::mimetype( const QString& /*path*/, const QString& /*query*/ )
{
    kDebugInfo( 7108, "InfoProtocol::mimetype" );

    // to get rid of those "Open with" dialogs...
    mimeType( "text/html" );

    // finish action
    finished();

    kDebugInfo( 7108, "InfoProtocol::mimetype - done" );
}

void InfoProtocol::decodePath( QString path )
{
    kDebugInfo( 7108, "InfoProtocol::decodePath" );

    m_page = "";
    m_node = "";

    // remove leading slash
    path = path.right( path.length() - 1 );

    int slashPos = path.find( "/" );

    if( slashPos < 0 )
    {
	m_page = path;
	m_node = "Top";
	return;
    }

    m_page = path.left( slashPos );
    m_node = path.right( path.length() - slashPos - 1);

    kDebugInfo( 7108, "InfoProtocol::decodePath - done" );
}

QCString InfoProtocol::errorMessage()
{
    kDebugInfo( 7108, "InfoProtocol::errorMessage" );

    // i18n !!!!!!!!!!!!!!!!!!
    return QCString( "<html><body bgcolor=\"#FFFFFF\">An error occured during converting an info-page to HTML</body></html>" );
}

extern "C" { int kdemain( int argc, char **argv ); }

int kdemain( int argc, char **argv )
{
  KInstance instance( "kio_info" );

  kdDebug() << "kio_info starting " << getpid() << endl;

  if (argc != 4)
  {
     fprintf(stderr, "Usage: kio_file protocol domain-socket1 domain-socket2\n");
     exit(-1);
  }

  InfoProtocol slave( argv[2], argv[3] );
  slave.dispatchLoop();

  return 0;
}
