#ifndef __smb_h__
#define __smb_h__ "$Id"

#include <qstring.h>

#include <kio_interface.h>
#include <kio_base.h>

#include <smb.h>

#include <string.h>
#include <list>

#include <sys/types.h>


class SmbProtocol : public IOProtocol
{
public:
	SmbProtocol( Connection *_conn );
//	virtual ~SmbProtocol();
  
	virtual void slotGet( const char *_url );
	virtual void slotGetSize( const char *_url );

	virtual void slotPut( const char *_url, int _mode,
				bool _overwrite, bool _resume, int _size );

	virtual void slotMkdir( const char *_url, int _mode );

	virtual void slotCopy( const char *_source, const char *_dest );
	virtual void slotCopy( QStringList& _source, const char *_dest );

	virtual void slotMove( const char *_source, const char *_dest );
	virtual void slotMove( QStringList& _source, const char *_dest );

	virtual void slotDel( QStringList& _source );

	virtual void slotListDir( const char *_url );
	virtual void slotTestDir( const char *_url );

	virtual void slotData( void *_p, int _len );
	virtual void slotDataEnd();

	Connection* connection() { return ConnectionSignals::m_pConnection; }

	void jobError( int _errid, const char *_txt );
  
protected:

	SMBIO *smbio;

	struct Copy
	{
		QString m_strAbsSource;
		QString m_strRelDest;
		mode_t m_mode;
		off_t m_size;
	};

	struct CopyDir
	{
		QString m_strAbsSource;
		QString m_strRelDest;
		mode_t m_mode;
		ino_t m_ino;
	};

	struct Del
	{
		QString m_strAbsSource;
		QString m_strRelDest;
		mode_t m_mode;
		off_t m_size;
	};

	int m_cmd;
	bool m_bIgnoreJobErrors;

	long listRecursive( const char *smbURL, const char *dest,
		QValueList<Copy>& _files, QValueList<CopyDir>& _dirs, bool dirChecked=false);
	void doCopy( QStringList& _source, const char *_dest, bool _rename, bool _move = false );
	int m_fPut;
};

class SmbIOJob : public IOJob
{
public:
	SmbIOJob( Connection *_conn, SmbProtocol *_Smb );

	virtual void slotError( int _errid, const char *_txt );

protected:
	SmbProtocol* m_pSmb;
};

#endif // __smb_h__
