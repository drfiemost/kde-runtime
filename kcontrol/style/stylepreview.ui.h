/*
 * $Id$
 *
 * Style Preview Widget
 * Copyright (C) 2002 Karol Szwed <gallium@kde.org>
 * Copyright (C) 2002 Daniel Molkentin <molkentin@kde.org>
 *
 * Portions Copyright (C) 2000 TrollTech AS.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 ***************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you wish to add, delete or rename slots use Qt Designer which will
 ** update this file, preserving your code. Create an init() slot in place of
 ** a constructor, and a destroy() slot in place of a destructor.
 *****************************************************************************/

void StylePreview::init()
{
    // Ensure that the user can't toy with the child widgets.
    // Method borrowed from Qt's qtconfig.
    QObjectList* l = queryList("QWidget");
    QObjectListIt it(*l);
    QObject* obj;
    while ((obj = it.current()) != 0)
    {
        ++it;
        obj->installEventFilter(this);
        ((QWidget*)obj)->setFocusPolicy(NoFocus);
    }
};

bool StylePreview::eventFilter( QObject* obj, QEvent* ev )
{
    switch( ev->type() )
    {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::Wheel:
		case QEvent::ContextMenu:
            return TRUE; // ignore
        default:
            break;
    }
    return FALSE;
}

// vim: set noet ts=4:
