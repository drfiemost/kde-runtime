// $Id$

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <kurl.h>
#include <kprotocolmanager.h>

#include "tar.h"

int main(int , char **)
{
  signal(SIGCHLD, IOProtocol::sigchld_handler);
  signal(SIGSEGV, IOProtocol::sigsegv_handler);

  Connection parent( 0, 1 );

  TARProtocol tar( &parent );
  tar.dispatchLoop();
}

TARProtocol::TARProtocol(KIOConnection *_conn) : KIOProtocol(_conn)
{
  m_cmd = CMD_NONE;
  m_pFilter = 0L;
  m_pJob = 0L;
}

void TARProtocol::slotGet(const char *_url)
{
  assert(m_cmd == CMD_NONE);

  m_cmd = CMD_GET;

  KURL::List lst = KURL::split(_url);
  if (lst.isEmpty())  {
    error(ERR_MALFORMED_URL, _url);
    m_cmd = CMD_NONE;
    return;
  }

  if ((*lst.begin()).protocol() != "tar")  {
    error( ERR_INTERNAL, "kio_tar got a URL which does not contain the tar protocol");
    m_cmd = CMD_NONE;
    return;
  }

  if (lst.count() < 2) {
    error(ERR_NO_SOURCE_PROTOCOL, "tar");
    m_cmd = CMD_NONE;
    return;
  }

  QString path=(*lst.begin()).path();

  // Remove tar protocol
  lst.remove(lst.begin());

  QString exec = KProtocolManager::self().executable((*lst.begin()).protocol());

  if (exec.isEmpty()) {
    error(ERR_UNSUPPORTED_PROTOCOL, (*lst.begin()).protocol());
    m_cmd = CMD_NONE;
    return;
  }

  // Start the file protcol
  KIOSlave slave(exec);
  if (slave.pid() == -1) {
    error(ERR_CANNOT_LAUNCH_PROCESS, exec);
    return;
  }

  QString tar_cmd = "tar";
  // x eXtract
  // O standard Output
  // f from a file
  // - standard input
  const char *argv[4] = {"-xOf","-", path.ascii()+1, 0};

  // Start the TAR process
  TARFilter filter(this, tar_cmd, argv);
  m_pFilter = &filter;
  if ( filter.pid() == -1 ) {
    error(ERR_CANNOT_LAUNCH_PROCESS, tar_cmd);
    finished();
    return;
  }

  TARIOJob job(&slave, this);
  QString src = KURL::join( lst );
  debug("kio_gzip : Nested fetching %s", src.ascii());
  
  job.get(src);
  while(!job.isReady() && !job.hasFinished())
    job.dispatch();

  if( job.hasFinished()) {
    finished();
    return;
  }
  
  while(!job.hasFinished())
    job.dispatch();

  finished();

  m_cmd = CMD_NONE;
}

void TARProtocol::slotPut(const char *, int , bool ,
			  bool , unsigned int)
{
}

void TARProtocol::slotCopy(const char *, const char *)
{
  fprintf(stderr, "TARProtocol::slotCopy\n");
  fflush(stderr);
}

void TARProtocol::slotData(void *_p, int _len)
{
  switch (m_cmd) {
    case CMD_PUT:
      assert(m_pFilter);
      m_pFilter->send(_p, _len);
      break;
    default:
      abort();
      break;
    }
}

void TARProtocol::slotDataEnd()
{
  switch (m_cmd) {
    case CMD_PUT:
      assert(m_pFilter && m_pJob);
      m_pFilter->finish();
      m_pJob->dataEnd();
      m_cmd = CMD_NONE;
      break;
    default:
      abort();
      break;
    }
}

void TARProtocol::jobData(void *_p, int _len)
{
  switch (m_cmd) {
  case CMD_GET:
    assert(m_pFilter);
    m_pFilter->send(_p, _len);
    break;
  case CMD_COPY:
    assert(m_pFilter);
    m_pFilter->send(_p, _len);
    break;
  default:
    abort();
  }
}

void TARProtocol::jobError(int _errid, const char *_text)
{
  error(_errid, _text);
}

void TARProtocol::jobDataEnd()
{
  switch (m_cmd) {
  case CMD_GET:
    assert(m_pFilter);
    m_pFilter->finish();
    dataEnd();
    break;
  case CMD_COPY:
    assert(m_pFilter);
    m_pFilter->finish();
    m_pJob->dataEnd();
    break;
  default:
    abort();
  }
}

void TARProtocol::filterData(void *_p, int _len)
{
  switch (m_cmd) {
  case CMD_GET:
    data(_p, _len);
    break;
  case CMD_PUT:
    assert (m_pJob);
    m_pJob->data(_p, _len);
    break;
  case CMD_COPY:
    assert(m_pJob);
    m_pJob->data(_p, _len);
    break;
  default:
    abort();
  }
}


/*************************************
 *
 * TARIOJob
 *
 *************************************/

TARIOJob::TARIOJob(Connection *_conn, TARProtocol *_tar) :
	IOJob(_conn)
{
  m_pTAR = _tar;
}
  
void TARIOJob::slotData(void *_p, int _len)
{
  m_pTAR->jobData( _p, _len );
}

void TARIOJob::slotDataEnd()
{
  m_pTAR->jobDataEnd();
}

void TARIOJob::slotError(int _errid, const char *_txt)
{
  m_pTAR->jobError(_errid, _txt );
}


/*************************************
 *
 * TARFilter
 *
 *************************************/

TARFilter::TARFilter(TARProtocol *_tar, const char *_prg, const char **_argv)
  : KIOFilter(_prg, _argv)
{
  m_pTAR = _tar;
}

void TARFilter::emitData(void *_p, int _len)
{
  m_pTAR->filterData(_p, _len);
}
