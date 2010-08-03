/* This file is part of the KDE Project
   Copyright (c) 2008-2009 Sebastian Trueg <trueg@kde.org>

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

#include "systray.h"
#include "indexscheduler.h"
#include "strigiservice.h"

#include <KMenu>
#include <KToggleAction>
#include <KLocale>
#include <KIcon>
#include <KToolInvocation>
#include <KActionCollection>
#include <KStandardAction>



Nepomuk::SystemTray::SystemTray( StrigiService* service, QWidget* parent )
    : KStatusNotifierItem( parent ),
      m_service( service ),
      m_suspendedManually( false )
{
    setCategory( SystemServices );
    setStatus( Passive );
    setIconByName( "nepomuk" );
    setTitle( i18n( "Search Service" ) );

    KMenu* menu = new KMenu;
    menu->addTitle( i18n( "Search Service" ) );

    m_suspendResumeAction = new KToggleAction( i18n( "Suspend File Indexing" ), menu );
    m_suspendResumeAction->setCheckedState( KGuiItem( i18n( "Suspend File Indexing" ) ) );
    m_suspendResumeAction->setToolTip( i18n( "Suspend or resume the file indexer manually" ) );
    connect( m_suspendResumeAction, SIGNAL( toggled( bool ) ),
             this, SLOT( slotSuspend( bool ) ) );

    KAction* configAction = new KAction( menu );
    configAction->setText( i18n( "Configure File Indexer" ) );
    configAction->setIcon( KIcon( "configure" ) );
    connect( configAction, SIGNAL( triggered() ),
             this, SLOT( slotConfigure() ) );

    menu->addAction( m_suspendResumeAction );
    menu->addAction( configAction );

    connect( m_service, SIGNAL( statusStringChanged() ), this, SLOT( slotUpdateStrigiStatus() ) );

    // we do not want autogenerated quit and restore actions
    setStandardActionsEnabled( false );

    setContextMenu( menu );
}


Nepomuk::SystemTray::~SystemTray()
{
}


void Nepomuk::SystemTray::slotUpdateStrigiStatus()
{
    // make sure we do not update the systray icon all the time

    // a manually suspended service should not be passive
    ItemStatus newStatus = m_service->isIdle() && !m_service->isSuspended() ? Passive : Active;
    if ( newStatus != status() ) {
        setStatus( newStatus );
    }

    QString statusString = m_service->simpleUserStatusString();
    if ( statusString != m_prevStatus ) {
        m_prevStatus = statusString;
        setToolTip("nepomuk", i18n("Search Service"), statusString );
    }

    m_suspendResumeAction->setChecked( m_service->indexScheduler()->isSuspended() );
}


void Nepomuk::SystemTray::slotConfigure()
{
    QStringList args;
    args << "kcm_nepomuk";
    KToolInvocation::kdeinitExec("kcmshell4", args);
}


void Nepomuk::SystemTray::slotSuspend( bool suspended )
{
    m_suspendedManually = suspended;
    m_service->setSuspended( suspended );
}

#include "systray.moc"