/* This file is part of the KDE libraries
   Copyright (C) 2000 Matej Koss <koss@miesto.sk>
                      David Faure <faure@kde.org>
                 2001 George Staikos <staikos@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
// -*- mode: c++; c-basic-offset: 4 -*-
// TODO:
//
// Use ProgressItem
// Add suspend/resume
// Better icon for systray icon

#include <qtimer.h>
#include <qregexp.h>
#include <q3header.h>
#include <qevent.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>

#include <ksqueezedtextlabel.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kdesu/client.h>
#include <kwin.h>
#include <kdialog.h>
#include <ksystemtrayicon.h>
#include <kmenu.h>
#include <kaction.h>
#include <kdialog.h>
#include <kstdaction.h>

#include "observer.h" // for static methods only
#include "kio/defaultprogress.h"
#include "kio/jobclasses.h"
#include "uiserver.h"
#include "passdlg.h"
#include "kio/renamedlg.h"
#include "kio/skipdlg.h"
#include "slavebase.h" // for QuestionYesNo etc.
#include <ksslinfodlg.h>
#include <ksslcertdlg.h>
#include <ksslcertificate.h>
#include <ksslcertchain.h>

#include "uiserveradaptor_p.h"

// pointer for main instance of UIServer
UIServer* uiserver;

// ToolBar field IDs
enum { TOOL_CANCEL, TOOL_CONFIGURE };

// StatusBar field IDs
enum { ID_TOTAL_FILES = 1, ID_TOTAL_SIZE, ID_TOTAL_TIME, ID_TOTAL_SPEED };

//static
int UIServer::s_jobId = 0;

static const int defaultColumnWidth[] = { 70,  // SIZE_OPERATION
                                    160, // LOCAL_FILENAME
                                    40,  // RESUME
                                    60,  // COUNT
                                    30,  // PROGRESS
                                    65,  // TOTAL
                                    70,  // SPEED
                                    70,  // REMAINING_TIME
                                    450  // URL
};

class ProgressConfigDialog:public KDialog
{
   public:
      ProgressConfigDialog(QWidget* parent);
      ~ProgressConfigDialog()  {}
      void setChecked(int i, bool on);
      bool isChecked(int i) const;
      friend class UIServer;
   private:
      QCheckBox *m_showSystemTrayCb;
      QCheckBox *m_keepOpenCb;
      QCheckBox *m_toolBarCb;
      QCheckBox *m_statusBarCb;
      QCheckBox *m_headerCb;
      QCheckBox *m_fixedWidthCb;
      K3ListView *m_columns;
      Q3CheckListItem *(m_items[ListProgress::TB_MAX]);
};

ProgressConfigDialog::ProgressConfigDialog(QWidget *parent)
:KDialog( parent )
{
  setCaption( i18n("Configure Network Operation Window") );
  setButtons( KDialog::Ok | KDialog::Apply | KDialog::Cancel );
  setDefaultButton( KDialog::Ok );
  setObjectName( "configprog" );
  showButtonSeparator( false );

  QFrame *page = new QFrame( this );
  setMainWidget( page );

   QVBoxLayout *layout=new QVBoxLayout(page);
   layout->setSpacing(spacingHint());
   m_showSystemTrayCb=new QCheckBox(i18n("Show system tray icon"), page);
   m_keepOpenCb=new QCheckBox(i18n("Keep network operation window always open"), page);
   m_headerCb=new QCheckBox(i18n("Show column headers"), page);
   m_toolBarCb=new QCheckBox(i18n("Show toolbar"), page);
   m_statusBarCb=new QCheckBox(i18n("Show statusbar"), page);
   m_fixedWidthCb=new QCheckBox(i18n("Column widths are user adjustable"), page);
   QLabel *label=new QLabel(i18n("Show information:"), page);
   m_columns=new K3ListView(page);

   m_columns->addColumn("info");
   m_columns->setSorting(-1);
   m_columns->header()->hide();

   m_items[ListProgress::TB_ADDRESS]        =new Q3CheckListItem(m_columns, i18n("URL"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_REMAINING_TIME] =new Q3CheckListItem(m_columns, i18nc("Remaining Time", "Rem. Time"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_SPEED]          =new Q3CheckListItem(m_columns, i18n("Speed"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_TOTAL]          =new Q3CheckListItem(m_columns, i18n("Size"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_PROGRESS]       =new Q3CheckListItem(m_columns, i18n("%"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_COUNT]          =new Q3CheckListItem(m_columns, i18n("Count"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_RESUME]         =new Q3CheckListItem(m_columns, i18nc("Resume", "Res."), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_LOCAL_FILENAME] =new Q3CheckListItem(m_columns, i18n("Local Filename"), Q3CheckListItem::CheckBox);
   m_items[ListProgress::TB_OPERATION]      =new Q3CheckListItem(m_columns, i18n("Operation"), Q3CheckListItem::CheckBox);

   layout->addWidget(m_showSystemTrayCb);
   layout->addWidget(m_keepOpenCb);
   layout->addWidget(m_headerCb);
   layout->addWidget(m_toolBarCb);
   layout->addWidget(m_statusBarCb);
   layout->addWidget(m_fixedWidthCb);
   layout->addWidget(label);
   layout->addWidget(m_columns);
}

void ProgressConfigDialog::setChecked(int i, bool on)
{
   if (i>=ListProgress::TB_MAX)
      return;
   m_items[i]->setOn(on);
}

bool ProgressConfigDialog::isChecked(int i) const
{
   if (i>=ListProgress::TB_MAX)
      return false;
   return m_items[i]->isOn();
}

ProgressItem::ProgressItem( ListProgress* view, Q3ListViewItem *after, QByteArray app_id, int job_id,
                            bool showDefault )
  : Q3ListViewItem( view, after ) {

  listProgress = view;

  m_iTotalSize = 0;
  m_iTotalFiles = 0;
  m_iProcessedSize = 0;
  m_iProcessedFiles = 0;
  m_iSpeed = 0;

  m_sAppId = app_id;
  m_iJobId = job_id;
  m_visible = true;
  m_defaultProgressVisible = showDefault;

  // create dialog, but don't show it
  defaultProgress = new KIO::DefaultProgress( false );
  defaultProgress->setOnlyClean( true );
  connect( defaultProgress, SIGNAL( stopped() ), this, SLOT( slotCanceled() ) );
  connect( defaultProgress, SIGNAL( suspend() ), this, SLOT( slotSuspend() ) );
  connect( defaultProgress, SIGNAL( resume() ), this, SLOT( slotResume() ) );
  connect( &m_showTimer, SIGNAL( timeout() ), this, SLOT(slotShowDefaultProgress()) );

  if ( showDefault ) {
    m_showTimer.setSingleShot( true );
    m_showTimer.start( 500 );
  }
}

bool ProgressItem::keepOpen() const
{
    return defaultProgress->keepOpen();
}

void ProgressItem::finished()
{
    defaultProgress->finished();
}

ProgressItem::~ProgressItem() {
    delete defaultProgress;
}


void ProgressItem::setTotalSize( KIO::filesize_t size ) {
  m_iTotalSize = size;

  // It's already in the % column...
  //setText( listProgress->lv_total, KIO::convertSize( m_iTotalSize ) );

  defaultProgress->slotTotalSize( 0, m_iTotalSize );
}


void ProgressItem::setTotalFiles( unsigned long files ) {
  m_iTotalFiles = files;

  defaultProgress->slotTotalFiles( 0, m_iTotalFiles );
}


void ProgressItem::setTotalDirs( unsigned long dirs ) {
  defaultProgress->slotTotalDirs( 0, dirs );
}


void ProgressItem::setProcessedSize( KIO::filesize_t size ) {
  m_iProcessedSize = size;

  setText( ListProgress::TB_TOTAL, KIO::convertSize( size ) );

  defaultProgress->slotProcessedSize( 0, size );
}


void ProgressItem::setProcessedFiles( unsigned long files ) {
  m_iProcessedFiles = files;

  QString tmps = i18n("%1 / %2",  m_iProcessedFiles ,  m_iTotalFiles );
  setText( ListProgress::TB_COUNT, tmps );

  defaultProgress->slotProcessedFiles( 0, m_iProcessedFiles );
}


void ProgressItem::setProcessedDirs( unsigned long dirs ) {
  defaultProgress->slotProcessedDirs( 0, dirs );
}


void ProgressItem::setPercent( unsigned long percent ) {
  const QString tmps = KIO::DefaultProgress::makePercentString( percent, m_iTotalSize, m_iTotalFiles );
  setText( ListProgress::TB_PROGRESS, tmps );

  defaultProgress->slotPercent( 0, percent );
}

void ProgressItem::setInfoMessage( const QString & msg ) {
  QString plainTextMsg(msg);
  plainTextMsg.replace( QRegExp( "</?b>" ), QString() );
  plainTextMsg.replace( QRegExp( "<img.*>" ), QString() );
  setText( ListProgress::TB_PROGRESS, plainTextMsg );

  defaultProgress->slotInfoMessage( 0, msg );
}

void ProgressItem::setSpeed( unsigned long bytes_per_second ) {
  m_iSpeed = bytes_per_second;
  m_remainingSeconds = KIO::calculateRemainingSeconds( m_iTotalSize, m_iProcessedSize, m_iSpeed );

  QString tmps, tmps2;
  if ( m_iSpeed == 0 ) {
    tmps = i18n( "Stalled");
    tmps2 = tmps;
  } else {
    tmps = i18n( "%1/s",  KIO::convertSize( m_iSpeed ));
    tmps2 = KIO::convertSeconds( m_remainingSeconds );
  }
  setText( ListProgress::TB_SPEED, tmps );
  setText( ListProgress::TB_REMAINING_TIME, tmps2 );

  defaultProgress->slotSpeed( 0, m_iSpeed );
}


void ProgressItem::setCopying( const KUrl& from, const KUrl& to ) {
   setText( ListProgress::TB_OPERATION, i18n("Copying") );
   setText( ListProgress::TB_ADDRESS, from.url() );
   setText( ListProgress::TB_LOCAL_FILENAME, to.fileName() );

  defaultProgress->slotCopying( 0, from, to );
}


void ProgressItem::setMoving( const KUrl& from, const KUrl& to ) {
   setText( ListProgress::TB_OPERATION, i18n("Moving") );
   setText( ListProgress::TB_ADDRESS, from.url() );
   setText( ListProgress::TB_LOCAL_FILENAME, to.fileName() );

  defaultProgress->slotMoving( 0, from, to );
}


void ProgressItem::setCreatingDir( const KUrl& dir ) {
   setText( ListProgress::TB_OPERATION, i18n("Creating") );
   setText( ListProgress::TB_ADDRESS, dir.url() );
   setText( ListProgress::TB_LOCAL_FILENAME, dir.fileName() );

  defaultProgress->slotCreatingDir( 0, dir );
}


void ProgressItem::setDeleting( const KUrl& url ) {
   setText( ListProgress::TB_OPERATION, i18n("Deleting") );
   setText( ListProgress::TB_ADDRESS, url.url() );
   setText( ListProgress::TB_LOCAL_FILENAME, url.fileName() );

  defaultProgress->slotDeleting( 0, url );
}

void ProgressItem::setTransferring( const KUrl& url ) {
   setText( ListProgress::TB_OPERATION, i18n("Loading") );
   setText( ListProgress::TB_ADDRESS, url.url() );
   setText( ListProgress::TB_LOCAL_FILENAME, url.fileName() );

  defaultProgress->slotTransferring( 0, url );
}

void ProgressItem::setText(ListProgress::ListProgressFields field, const QString& text)
{
  if (listProgress->m_lpcc[field].enabled)
  {
     QString t=text;
     if ((field==ListProgress::TB_ADDRESS) && (listProgress->m_fixedColumnWidths))
//     if (((field==ListProgress::TB_LOCAL_FILENAME) || (field==ListProgress::TB_ADDRESS)) && (listProgress->m_fixedColumnWidths))
     {
        m_fullLengthAddress=text;
        listProgress->m_squeezer->resize(listProgress->columnWidth(listProgress->m_lpcc[field].index),50);
        listProgress->m_squeezer->setText(t);
        t=listProgress->m_squeezer->text();
     }
     Q3ListViewItem::setText(listProgress->m_lpcc[field].index,t);
  }
}

void ProgressItem::setStating( const KUrl& url ) {
   setText( ListProgress::TB_OPERATION, i18n("Examining") );
   setText( ListProgress::TB_ADDRESS, url.url() );
   setText( ListProgress::TB_LOCAL_FILENAME, url.fileName() );

  defaultProgress->slotStating( 0, url );
}

void ProgressItem::setMounting( const QString& dev, const QString & point ) {
   setText( ListProgress::TB_OPERATION, i18n("Mounting") );
   setText( ListProgress::TB_ADDRESS, point ); // ?
   setText( ListProgress::TB_LOCAL_FILENAME, dev ); // ?

  defaultProgress->slotMounting( 0, dev, point );
}

void ProgressItem::setUnmounting( const QString & point ) {
   setText( ListProgress::TB_OPERATION, i18n("Unmounting") );
   setText( ListProgress::TB_ADDRESS, point ); // ?
   setText( ListProgress::TB_LOCAL_FILENAME, "" ); // ?

  defaultProgress->slotUnmounting( 0, point );
}

void ProgressItem::setCanResume( KIO::filesize_t offset ) {
  /*
  QString tmps;
  // set canResume
  if ( _resume ) {
    tmps = i18n("Yes");
  } else {
    tmps = i18n("No");
  }
  setText( listProgress->lv_resume, tmps );
  */
  defaultProgress->slotCanResume( 0, offset );
}


void ProgressItem::slotCanceled() {
  emit jobCanceled( this );
}

void ProgressItem::slotSuspend() {
  emit jobSuspended( this );
}

void ProgressItem::slotResume() {
  emit jobResumed( this );
}

// Called 0.5s after the job has been started
void ProgressItem::slotShowDefaultProgress() {
  if (defaultProgress)
  {
    if ( m_visible && m_defaultProgressVisible )
      defaultProgress->show();
    else
      defaultProgress->hide();
  }
}

void ProgressItem::slotToggleDefaultProgress() {
  setDefaultProgressVisible( !m_defaultProgressVisible );
}

// Called when a rename or skip dialog pops up
// We want to prevent someone from killing the job in the uiserver then
void ProgressItem::setVisible( bool visible ) {
  if ( m_visible != visible )
  {
    m_visible = visible;
    updateVisibility();
  }
}

// Can be toggled by the user
void ProgressItem::setDefaultProgressVisible( bool visible ) {
  if ( m_defaultProgressVisible != visible )
  {
    m_defaultProgressVisible = visible;
    updateVisibility();
  }
}

// Update according to state
void ProgressItem::updateVisibility()
{
  if (defaultProgress)
  {
    if ( m_visible && m_defaultProgressVisible )
    {
      m_showTimer.setSingleShot(true);
      m_showTimer.start(250); // Show delayed
    }
    else
    {
      m_showTimer.stop();
      defaultProgress->hide();
    }
  }
}


//-----------------------------------------------------------------------------
ListProgress::ListProgress (QWidget *parent)
    : K3ListView (parent)
{

  // enable selection of more than one item
  setMultiSelection( true );

  setAllColumnsShowFocus( true );

  m_lpcc[TB_OPERATION].title=i18n("Operation");
  m_lpcc[TB_LOCAL_FILENAME].title=i18n("Local Filename");
  m_lpcc[TB_RESUME].title=i18nc("Resume", "Res.");
  m_lpcc[TB_COUNT].title=i18n("Count");
  m_lpcc[TB_PROGRESS].title=i18n("%");
  m_lpcc[TB_TOTAL].title=i18n("Size");
  m_lpcc[TB_SPEED].title=i18n("Speed");
  m_lpcc[TB_REMAINING_TIME].title=i18nc("Remaining Time", "Rem. Time");
  m_lpcc[TB_ADDRESS].title=i18n("URL");
  readSettings();

  applySettings();

  //used for squeezing the text in local file name and url
  m_squeezer=new KSqueezedTextLabel(this);
  m_squeezer->hide();
  connect(header(),SIGNAL(sizeChange(int,int,int)),this,SLOT(columnWidthChanged(int)));
}


ListProgress::~ListProgress() {
}

void ListProgress::applySettings()
{
  int iEnabledCols=0;

  // Update listcolumns to show
  for (int i=0; i<TB_MAX; i++)
  {
    if ( !m_lpcc[i].enabled )
      continue;

    iEnabledCols++;

    // Add new or reuse existing column
    if ( iEnabledCols > columns() )
      m_lpcc[i].index=addColumn(m_lpcc[i].title, m_fixedColumnWidths?m_lpcc[i].width:-1);
    else
    {
      m_lpcc[i].index = iEnabledCols - 1;
      setColumnText(m_lpcc[i].index, m_lpcc[i].title);
    }

    setColumnWidth(m_lpcc[i].index, m_lpcc[i].width); //yes, this is required here, alexxx
    if (m_fixedColumnWidths)
        setColumnWidthMode(m_lpcc[i].index, Manual);
  }

  // Remove unused columns. However we must keep one column left
  // Otherwise the listview will be emptied
  while( iEnabledCols < columns() && columns() > 1 )
     removeColumn( columns() - 1 );

  if ( columns() == 0 )
    addColumn( "" );

  if ( !m_showHeader || iEnabledCols == 0 )
    header()->hide();
  else
    header()->show();
}

void ListProgress::readSettings() {
  KConfig config("uiserverrc");

  // read listview geometry properties
  config.setGroup( "ProgressList" );
  for ( int i = 0; i < TB_MAX; i++ ) {
     QString tmps="Col"+QString::number(i);
     m_lpcc[i].width=config.readEntry( tmps, 0 );
     if (m_lpcc[i].width==0) m_lpcc[i].width=defaultColumnWidth[i];

     tmps="Enabled"+QString::number(i);
     m_lpcc[i].enabled=config.readEntry(tmps, true);
  }
  m_showHeader=config.readEntry("ShowListHeader", true);
  m_fixedColumnWidths=config.readEntry("FixedColumnWidths", false);

  m_lpcc[TB_RESUME].enabled=false;
}

void ListProgress::columnWidthChanged(int column)
{
   //resqueeze if necessary
   if ((m_lpcc[TB_ADDRESS].enabled) && (column==m_lpcc[TB_ADDRESS].index))
   {
      for (Q3ListViewItem* lvi=firstChild(); lvi!=0; lvi=lvi->nextSibling())
      {
         ProgressItem *pi=(ProgressItem*)lvi;
         pi->setText(TB_ADDRESS,pi->fullLengthAddress());
      }
   }
   writeSettings();
}

void ListProgress::writeSettings() {
   KConfig config("uiserverrc");

   // write listview geometry properties
   config.setGroup( "ProgressList" );
   for ( int i = 0; i < TB_MAX; i++ ) {
      if (!m_lpcc[i].enabled) {
         QString tmps= "Enabled" + QString::number(i);
         config.writeEntry( tmps, false );
         continue;
      }
      m_lpcc[i].width=columnWidth(m_lpcc[i].index);
      QString tmps="Col"+QString::number(i);
      config.writeEntry( tmps, m_lpcc[i].width);
   }
   config.writeEntry("ShowListHeader", m_showHeader);
   config.writeEntry("FixedColumnWidths", m_fixedColumnWidths);
   config.sync();
}


//------------------------------------------------------------


UIServer::UIServer()
:KMainWindow(0)
,m_shuttingDown(false)
,m_configDialog(0)
,m_contextMenu(0)
,m_systemTray(0)
{

  readSettings();

  // setup toolbar
  m_toolCancel = toolBar()->addAction(i18n("Cancel"));
  connect(m_toolCancel, SIGNAL(triggered(bool)), SLOT(slotCancelCurrent()));

  m_toolConfigure = toolBar()->addAction(i18n("Settings..."));
  connect(m_toolConfigure, SIGNAL(triggered(bool)), SLOT(slotConfigure()));

  addToolBar(Qt::LeftToolBarArea, toolBar());

  // setup statusbar
  statusBar()->insertItem( i18n(" Files: %1 ",  0 ), ID_TOTAL_FILES);
  statusBar()->insertItem( i18nc("Remaining Size", " Rem. Size: %1 kB ",  0 ), ID_TOTAL_SIZE);
  statusBar()->insertItem( i18nc("Remaining Time", " Rem. Time: 00:00:00 "), ID_TOTAL_TIME);
  statusBar()->insertItem( i18n(" %1 kB/s ", 0), ID_TOTAL_SPEED);

  // setup listview
  listProgress = new ListProgress( this );
  listProgress->setObjectName( "progresslist" );

  setCentralWidget( listProgress );

  connect( listProgress, SIGNAL( selectionChanged() ),
           SLOT( slotSelection() ) );
  connect( listProgress, SIGNAL( executed( Q3ListViewItem* ) ),
           SLOT( slotToggleDefaultProgress( Q3ListViewItem* ) ) );
  connect( listProgress, SIGNAL( contextMenu( K3ListView*, Q3ListViewItem *, const QPoint &)),
           SLOT(slotShowContextMenu(K3ListView*, Q3ListViewItem *, const QPoint&)));


  // setup animation timer
  updateTimer = new QTimer( this );
  connect( updateTimer, SIGNAL( timeout() ),
           SLOT( slotUpdate() ) );
  m_bUpdateNewJob=false;

  setCaption(i18n("Progress Dialog"));
  setMinimumSize( 150, 50 );
  resize( m_initWidth, m_initHeight);

  applySettings();

/*  if ((m_bShowList) && (m_keepListOpen))
  {
     cerr<<"show() !"<<endl;
     show();
  }
  else*/
  hide();

  (void)new UIServerAdaptor(this);
  QDBusConnection::sessionBus().registerObject(QLatin1String("/UIServer"), this);
}

UIServer::~UIServer() {
  updateTimer->stop();
}

void UIServer::applySettings()
{
  if (m_showSystemTray && m_systemTray == 0)
  {
     m_systemTray = new KSystemTrayIcon(this);
     QMenu* pop = m_systemTray->contextMenu();
     pop->addAction(i18n("Settings..."), this, SLOT(slotConfigure()));
     pop->addAction(i18n("Remove"), this, SLOT(slotRemoveSystemTrayIcon()));
     m_systemTray->setIcon(KSystemTrayIcon::loadIcon("filesave"));
     m_systemTray->show();
  }
  else if (m_showSystemTray == false && m_systemTray != 0)
  {
     delete m_systemTray;
     m_systemTray = 0;
  }

  if (m_showStatusBar==false)
     statusBar()->hide();
  else
     statusBar()->show();
  if (m_showToolBar==false)
     toolBar()->hide();
  else
     toolBar()->show();
}

void UIServer::slotShowContextMenu(K3ListView*, Q3ListViewItem* item, const QPoint& pos)
{
   if (m_contextMenu==0)
   {
      m_contextMenu=new QMenu(this);
      m_cancelAction = m_contextMenu->addAction(i18n("Cancel Job"), this, SLOT(slotCancelCurrent()));
//      m_contextMenu->insertItem(i18n("Toggle Progress"), this, SLOT(slotToggleDefaultProgress()));
      m_contextMenu->addSeparator();
      m_contextMenu->addAction(i18n("Settings..."), this, SLOT(slotConfigure()));
   }
   if ( item )
       item->setSelected( true );
   bool enabled = false;
   Q3ListViewItemIterator it( listProgress );
   for ( ; it.current(); ++it ) {
     if ( it.current()->isSelected() ) {
       enabled = true;
       break;
     }
   }
   m_cancelAction->setEnabled(enabled);

   m_contextMenu->popup(pos);
}

void UIServer::slotRemoveSystemTrayIcon()
{
   m_showSystemTray=false;
   applySettings();
   writeSettings();
}

void UIServer::slotConfigure()
{
   if (m_configDialog==0)
   {
      m_configDialog=new ProgressConfigDialog(0);
//      connect(m_configDialog,SIGNAL(cancelClicked()), this, SLOT(slotCancelConfig()));
      connect(m_configDialog,SIGNAL(okClicked()), this, SLOT(slotApplyConfig()));
      connect(m_configDialog,SIGNAL(applyClicked()), this, SLOT(slotApplyConfig()));
   }
   m_configDialog->m_showSystemTrayCb->setChecked(m_showSystemTray);
   m_configDialog->m_keepOpenCb->setChecked(m_keepListOpen);
   m_configDialog->m_toolBarCb->setChecked(m_showToolBar);
   m_configDialog->m_statusBarCb->setChecked(m_showStatusBar);
   m_configDialog->m_headerCb->setChecked(listProgress->m_showHeader);
   m_configDialog->m_fixedWidthCb->setChecked(listProgress->m_fixedColumnWidths);
   for (int i=0; i<ListProgress::TB_MAX; i++)
   {
      m_configDialog->setChecked(i, listProgress->m_lpcc[i].enabled);
   }
   m_configDialog->show();
}

void UIServer::slotApplyConfig()
{
   m_showSystemTray=m_configDialog->m_showSystemTrayCb->isChecked();
   m_keepListOpen=m_configDialog->m_keepOpenCb->isChecked();
   m_showToolBar=m_configDialog->m_toolBarCb->isChecked();
   m_showStatusBar=m_configDialog->m_statusBarCb->isChecked();
   listProgress->m_showHeader=m_configDialog->m_headerCb->isChecked();
   listProgress->m_fixedColumnWidths=m_configDialog->m_fixedWidthCb->isChecked();
   for (int i=0; i<ListProgress::TB_MAX; i++)
      listProgress->m_lpcc[i].enabled=m_configDialog->isChecked(i);


   applySettings();
   listProgress->applySettings();
   writeSettings();
   listProgress->writeSettings();
}

int UIServer::newJob( const QString &observerAppId, bool showProgress )
{
  kDebug(7024) << "UIServer::newJob observerAppId=" << observerAppId << ". "
            << "Giving id=" << s_jobId+1 << endl;

  Q3ListViewItemIterator it( listProgress );
  for ( ; it.current(); ++it ) {
    if ( it.current()->itemBelow() == 0L ) { // this will find the end of list
      break;
    }
  }

  // increment counter
  s_jobId++;

  bool show = !m_bShowList && showProgress;

  ProgressItem *item = new ProgressItem( listProgress, it.current(), observerAppId.toLatin1(), s_jobId, show );
  connect( item, SIGNAL( jobCanceled( ProgressItem* ) ),
           SLOT( slotJobCanceled( ProgressItem* ) ) );
  connect( item, SIGNAL( jobSuspended( ProgressItem* ) ),
           SLOT( slotJobSuspended( ProgressItem* ) ) );
  connect( item, SIGNAL( jobResumed( ProgressItem* ) ),
           SLOT( slotJobResumed( ProgressItem* ) ) );

  if ( m_bShowList && !updateTimer->isActive() )
    updateTimer->start( 1000 );

  m_bUpdateNewJob=true;

  return s_jobId;
}


ProgressItem* UIServer::findItem( int id )
{
  Q3ListViewItemIterator it( listProgress );

  ProgressItem *item;

  for ( ; it.current(); ++it ) {
    item = (ProgressItem*) it.current();
    if ( item->jobId() == id ) {
      return item;
    }
  }

  return 0L;
}


void UIServer::setItemVisible( ProgressItem * item, bool visible )
{
  item->setVisible( visible );
  // Check if we were the last one to be visible
  // or the first one -> hide/show the list in that case
  // (Note that the user could have hidden the listview by hand yet, no time)
  if ( m_bShowList ) {
      m_bUpdateNewJob = true;
      slotUpdate();
  }
}

// Called by Observer when opening a skip or rename dialog
void UIServer::setJobVisible( int id, bool visible )
{
  kDebug(7024) << "UIServer::setJobVisible id=" << id << " visible=" << visible << endl;
  ProgressItem *item = findItem( id );
  Q_ASSERT( item );
  if ( item )
      setItemVisible( item, visible );
}

void UIServer::jobFinished( int id )
{
  kDebug(7024) << "UIServer::jobFinished id=" << id << endl;
  ProgressItem *item = findItem( id );

  // remove item from the list and delete the corresponding defaultprogress
  if ( item ) {
    if ( item->keepOpen() )
      item->finished();
    else
    delete item;
  }
}


void UIServer::totalSize( int id, KIO::filesize_t size )
{
//  kDebug(7024) << "UIServer::totalSize " << id << " " << KIO::number(size) << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setTotalSize( size );
  }
}

void UIServer::totalFiles( int id, unsigned long files )
{
  kDebug(7024) << "UIServer::totalFiles " << id << " " << (unsigned int) files << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setTotalFiles( files );
  }
}

void UIServer::totalDirs( int id, unsigned long dirs )
{
  kDebug(7024) << "UIServer::totalDirs " << id << " " << (unsigned int) dirs << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setTotalDirs( dirs );
  }
}

void UIServer::processedSize( int id, KIO::filesize_t size )
{
  //kDebug(7024) << "UIServer::processedSize " << id << " " << KIO::number(size) << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setProcessedSize( size );
  }
}

void UIServer::processedFiles( int id, unsigned long files )
{
  //kDebug(7024) << "UIServer::processedFiles " << id << " " << (unsigned int) files << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setProcessedFiles( files );
  }
}

void UIServer::processedDirs( int id, unsigned long dirs )
{
  kDebug(7024) << "UIServer::processedDirs " << id << " " << (unsigned int) dirs << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setProcessedDirs( dirs );
  }
}

void UIServer::percent( int id, unsigned long ipercent )
{
  //kDebug(7024) << "UIServer::percent " << id << " " << (unsigned int) ipercent << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setPercent( ipercent );
  }
}

void UIServer::speed( int id, unsigned long bytes_per_second )
{
  //kDebug(7024) << "UIServer::speed " << id << " " << (unsigned int) bytes_per_second << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setSpeed( bytes_per_second );
  }
}

void UIServer::infoMessage( int id, const QString & msg )
{
  //kDebug(7024) << "UIServer::infoMessage " << id << " " << msg << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setInfoMessage( msg );
  }
}

void UIServer::canResume( int id, KIO::filesize_t offset )
{
  //kDebug(7024) << "UIServer::canResume " << id << " " << offset << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setCanResume( offset );
  }
}

void UIServer::copying( int id, KUrl from, KUrl to )
{
  //kDebug(7024) << "UIServer::copying " << id << " " << from.url() << "  " << to.url() << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setCopying( from, to );
  }
}

void UIServer::moving( int id, KUrl from, KUrl to )
{
  //kDebug(7024) << "UIServer::moving " << id << " " << from.url() << "  " << to.url() << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setMoving( from, to );
  }
}

void UIServer::deleting( int id, KUrl url )
{
  //kDebug(7024) << "UIServer::deleting " << id << " " << url.url() << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setDeleting( url );
  }
}

void UIServer::transferring( int id, KUrl url )
{
  //kDebug(7024) << "UIServer::transferring " << id << " " << url.url() << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setTransferring( url );
  }
}

void UIServer::creatingDir( int id, KUrl dir )
{
  kDebug(7024) << "UIServer::creatingDir " << id << " " << dir.url() << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setCreatingDir( dir );
  }
}

void UIServer::stating( int id, KUrl url )
{
  kDebug(7024) << "UIServer::stating " << id << " " << url.url() << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setStating( url );
  }
}

void UIServer::mounting( int id, QString dev, QString point )
{
  kDebug(7024) << "UIServer::mounting " << id << " " << dev << " " << point << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setMounting( dev, point );
  }
}

void UIServer::unmounting( int id, QString point )
{
  kDebug(7024) << "UIServer::unmounting " << id << " " << point << endl;

  ProgressItem *item = findItem( id );
  if ( item ) {
    item->setUnmounting( point );
  }
}

void UIServer::callObserver( const QString &observerAppId, int progressId, const char* method )
{
    // Contact the object "KIO::Observer" in the application <observerAppId>
    QDBusInterface observer( observerAppId, "/KIO/Observer", "org.kde.KIO.Observer" );
    // Call this method
    observer.call( method, progressId );
}

void UIServer::slotJobCanceled( ProgressItem *item ) {
  // kill the corresponding job
  callObserver( item->appId(), item->jobId(), "killJob" );

  // KIO::Job, when killed, should call back jobFinished(), but we can't
  // really rely on that - the app may have crashed
  delete item;
}

void UIServer::slotJobSuspended( ProgressItem* item ) {
  callObserver( item->appId(), item->jobId(), "suspend" );
}

void UIServer::slotJobResumed( ProgressItem* item ) {
  callObserver( item->appId(), item->jobId(), "resume" );
}

void UIServer::slotQuit()
{
  m_shuttingDown = true;
  qApp->quit();
}

void UIServer::slotUpdate() {
  // don't do anything if we don't have any inserted progress item
  // or if they're all hidden
   Q3ListViewItemIterator lvit( listProgress );
   bool visible = false;
   for ( ; lvit.current(); ++lvit )
      if ( ((ProgressItem*)lvit.current())->isVisible() ) {
         visible = true;
         break;
      }

   if ( !visible || !m_bShowList ) {
      if (!m_keepListOpen) hide();
      updateTimer->stop();
      return;
   }

  // Calling show() is conditional, so that users can close the window
  // and it only pops up back when a new job is started
  if (m_bUpdateNewJob)
  {
    m_bUpdateNewJob=false;
    show();

    // Make sure we'll be called back
    if ( m_bShowList && !updateTimer->isActive() )
      updateTimer->start( 1000 );
  }

  int iTotalFiles = 0;
  KIO::filesize_t iTotalSize = 0;
  int iTotalSpeed = 0;
  unsigned int totalRemTime = 0; // in seconds

  ProgressItem *item;

  // count totals for statusbar
  Q3ListViewItemIterator it( listProgress );

  for ( ; it.current(); ++it ) {
    item = (ProgressItem*) it.current();
    if ( item->totalSize() != 0 ) {
      iTotalSize += ( item->totalSize() - item->processedSize() );
    }
    iTotalFiles += ( item->totalFiles() - item->processedFiles() );
    iTotalSpeed += item->speed();

    if ( item->remainingSeconds() > totalRemTime ) {
      totalRemTime = item->remainingSeconds();
    }
  }

  // update statusbar
  statusBar()->changeItem( i18n( " Files: %1 ",  iTotalFiles ), ID_TOTAL_FILES);
  statusBar()->changeItem( i18nc( "Remaining Size", " Rem. Size: %1 ",  KIO::convertSize( iTotalSize ) ),
                           ID_TOTAL_SIZE);
  statusBar()->changeItem( i18nc( "Remaining Time", " Rem. Time: %1 ",  KIO::convertSeconds( totalRemTime ) ),
                           ID_TOTAL_TIME);
  statusBar()->changeItem( i18n( " %1/s ",  KIO::convertSize( iTotalSpeed ) ),
                           ID_TOTAL_SPEED);

}

void UIServer::setListMode( bool list )
{
  m_bShowList = list;
  Q3ListViewItemIterator it( listProgress );
  for ( ; it.current(); ++it ) {
    // When going to list mode -> hide all progress dialogs
    // When going back to separate dialogs -> show them all
    ((ProgressItem*) it.current())->setDefaultProgressVisible( !list );
  }

  if (m_bShowList)
  {
    show();
    updateTimer->start( 1000 );
  }
  else
  {
    hide();
    updateTimer->stop();
  }
}

void UIServer::slotToggleDefaultProgress( Q3ListViewItem *item ) {
  ((ProgressItem*) item )->slotToggleDefaultProgress();
}


void UIServer::slotSelection() {
  Q3ListViewItemIterator it( listProgress );

  for ( ; it.current(); ++it ) {
    if ( it.current()->isSelected() ) {
      m_toolCancel->setEnabled(true);
      return;
    }
  }
  m_toolCancel->setEnabled(false);
}

int UIServer::messageBox( int progressId, int type, const QString &text, const QString &caption, const QString &buttonYes, const QString &buttonNo )
{
    return Observer::messageBox( progressId, type, text, caption, buttonYes, buttonNo );
}

void UIServer::showSSLInfoDialog(const QString &url, const KIO::MetaData &meta)
{
    return showSSLInfoDialog(url,meta,0);
}

void UIServer::showSSLInfoDialog(const QString &url, const KIO::MetaData &meta, int mainwindow)
{
   KSSLInfoDlg *kid = new KSSLInfoDlg(meta["ssl_in_use"].toUpper()=="TRUE", 0L /*parent?*/, 0L, true);
   KSSLCertificate *x = KSSLCertificate::fromString(meta["ssl_peer_certificate"].toLocal8Bit());
   if (x) {
      // Set the chain back onto the certificate
      QStringList cl =
                      meta["ssl_peer_chain"].split(QString("\n"), QString::SkipEmptyParts);
      Q3PtrList<KSSLCertificate> ncl;

      ncl.setAutoDelete(true);
      for (QStringList::Iterator it = cl.begin(); it != cl.end(); ++it) {
         KSSLCertificate *y = KSSLCertificate::fromString((*it).toLocal8Bit());
         if (y) ncl.append(y);
      }

      if (ncl.count() > 0)
         x->chain().setChain(ncl);

      kDebug(7024) << "ssl_cert_errors=" << meta["ssl_cert_errors"] << endl;
      kid->setCertState(meta["ssl_cert_errors"]);
      QString ip = meta.contains("ssl_proxied") ? "" : meta["ssl_peer_ip"];
      kid->setup( x,
                  ip,
                  url, // the URL
                  meta["ssl_cipher"],
                  meta["ssl_cipher_desc"],
                  meta["ssl_cipher_version"],
                  meta["ssl_cipher_used_bits"].toInt(),
                  meta["ssl_cipher_bits"].toInt(),
                  KSSLCertificate::KSSLValidation(meta["ssl_cert_state"].toInt()));
      kDebug(7024) << "Showing SSL Info dialog" << endl;
#ifndef Q_WS_WIN
      if( mainwindow != 0 )
          KWin::setMainWindow( kid, mainwindow );
#endif
      kid->exec();
      delete x;
      kDebug(7024) << "SSL Info dialog closed" << endl;
   } else {
      KMessageBox::information( 0L, // parent ?
                              i18n("The peer SSL certificate appears to be corrupt."), i18n("SSL") );
   }
   // Don't delete kid!!
}

KSSLCertDlgRet UIServer::showSSLCertDialog(const QString& host, const QStringList& certList)
{
    return showSSLCertDialog( host, certList, 0 );
}

KSSLCertDlgRet UIServer::showSSLCertDialog(const QString& host, const QStringList& certList, int mainwindow)
{
   KSSLCertDlgRet rc;
   rc.ok = false;
   if (!certList.isEmpty()) {
      KSSLCertDlg *kcd = new KSSLCertDlg(0L, 0L, true);
      kcd->setupDialog(certList);
      kcd->setHost(host);
      kDebug(7024) << "Showing SSL certificate dialog" << endl;
#ifndef Q_WS_WIN
      if( mainwindow != 0 )
          KWin::setMainWindow( kcd, mainwindow );
#endif
      kcd->exec();
      rc.ok = true;
      rc.choice = kcd->getChoice();
      rc.save = kcd->saveChoice();
      rc.send = kcd->wantsToSend();
      kDebug(7024) << "SSL certificate dialog closed" << endl;
      delete kcd;
   }
   return rc;
}


QByteArray UIServer::open_RenameDlg( int id,
                                     const QString & caption,
                                     const QString& src, const QString & dest,
                                     int mode,
                                     unsigned long sizeSrc,
                                     unsigned long sizeDest,
                                     unsigned long ctimeSrc,
                                     unsigned long ctimeDest,
                                     unsigned long mtimeSrc,
                                     unsigned long mtimeDest
                                     )
{ return open_RenameDlg64(id, caption, src, dest, mode, sizeSrc, sizeDest,
                          ctimeSrc, ctimeDest, mtimeSrc, mtimeDest); }


QByteArray UIServer::open_RenameDlg64( int id,
                                     const QString & caption,
                                     const QString& src, const QString & dest,
                                     int mode,
                                     KIO::filesize_t sizeSrc,
                                     KIO::filesize_t sizeDest,
                                     unsigned long ctimeSrc,
                                     unsigned long ctimeDest,
                                     unsigned long mtimeSrc,
                                     unsigned long mtimeDest
                                     )
{
  // Hide existing dialog box if any
  ProgressItem *item = findItem( id );
  if ( item )
    setItemVisible( item, false );
  QString newDest;
  kDebug(7024) << "Calling KIO::open_RenameDlg" << endl;
  KIO::RenameDlg_Result result = KIO::open_RenameDlg( caption, src, dest,
                                                      (KIO::RenameDlg_Mode) mode, newDest,
                                                      sizeSrc, sizeDest,
                                                      (time_t)ctimeSrc, (time_t)ctimeDest,
                                                      (time_t)mtimeSrc, (time_t)mtimeDest );
  kDebug(7024) << "KIO::open_RenameDlg done" << endl;
  QByteArray data;
  QDataStream stream( &data, QIODevice::WriteOnly );
  stream << quint8(result) << newDest;
  if ( item && result != KIO::R_CANCEL )
    setItemVisible( item, true );
  return data;
}

int UIServer::open_SkipDlg( int id,
                            int /*bool*/ multi,
                            const QString & error_text )
{
  // Hide existing dialog box if any
  ProgressItem *item = findItem( id );
  if ( item )
    setItemVisible( item, false );
  kDebug(7024) << "Calling KIO::open_SkipDlg" << endl;
  KIO::SkipDlg_Result result = KIO::open_SkipDlg( (bool)multi, error_text );
  if ( item && result != KIO::S_CANCEL )
    setItemVisible( item, true );
  return (KIO::SkipDlg_Result) result;
}


void UIServer::readSettings() {
  KConfig config("uiserverrc");
  config.setGroup( "UIServer" );
  m_showStatusBar=config.readEntry("ShowStatusBar", false);
  m_showToolBar=config.readEntry("ShowToolBar", true);
  m_keepListOpen=config.readEntry("KeepListOpen", false);
  m_initWidth=config.readEntry("InitialWidth", 460);
  m_initHeight=config.readEntry("InitialHeight", 150);
  m_bShowList = config.readEntry("ShowList", false);
  m_showSystemTray=config.readEntry("ShowSystemTray", false);
}

void UIServer::writeSettings() {
  KConfig config("uiserverrc");
  config.setGroup( "UIServer" );
  config.writeEntry("InitialWidth",width());
  config.writeEntry("InitialHeight",height());
  config.writeEntry("ShowStatusBar", m_showStatusBar);
  config.writeEntry("ShowToolBar", m_showToolBar);
  config.writeEntry("KeepListOpen", m_keepListOpen);
  config.writeEntry("ShowList", m_bShowList);
  config.writeEntry("ShowSystemTray", m_showSystemTray);
}


void UIServer::slotCancelCurrent() {
  Q3ListViewItemIterator it( listProgress );
  ProgressItem *item;

  // kill selected jobs
  for ( ; it.current() ; ++it )
  {
    if ( it.current()->isSelected() ) {
      item = (ProgressItem*) it.current();
      callObserver( item->appId(), item->jobId(), "killJob" );
      return;
    }
  }
}

void UIServer::resizeEvent(QResizeEvent* e)
{
   KMainWindow::resizeEvent(e);
   writeSettings();
}

bool UIServer::queryClose()
{
  if (( !m_shuttingDown ) && !kapp->sessionSaving()) {
    hide();
    return false;
  }
  return true;
}

UIServer* UIServer::createInstance()
{
    return new UIServer;
}

//------------------------------------------------------------

extern "C" KDE_EXPORT int kdemain(int argc, char **argv)
{
    KLocale::setMainCatalog("kdelibs");
    //  GS 5/2001 - I changed the name to "KDE" to make it look better
    //              in the titles of dialogs which are displayed.
    KAboutData aboutdata("kio_uiserver", I18N_NOOP("KDE"),
                         "0.8", I18N_NOOP("KDE Progress Information UI Server"),
                         KAboutData::License_GPL, "(C) 2000-2005, David Faure & Matt Koss");
    aboutdata.addAuthor("David Faure",I18N_NOOP("Maintainer"),"faure@kde.org");
    aboutdata.addAuthor("Matej Koss",I18N_NOOP("Developer"),"koss@miesto.sk");

    KCmdLineArgs::init( argc, argv, &aboutdata );
    // KCmdLineArgs::addCmdLineOptions( options );
    KUniqueApplication::addCmdLineOptions();

    if (!KUniqueApplication::start())
    {
      kDebug(7024) << "kio_uiserver is already running!" << endl;
      return (0);
    }

    KUniqueApplication app;

    // This app is started automatically, no need for session management
    app.disableSessionManagement();
    app.setQuitOnLastWindowClosed( false );
    //app.dcopClient()->setDaemonMode( true );

    uiserver = UIServer::createInstance();

    return app.exec();
}

#include "uiserver.moc"
