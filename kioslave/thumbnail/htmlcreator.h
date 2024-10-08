/*  This file is part of the KDE libraries
    Copyright (C) 2000 Malte Starostik <malte@kde.org>
    Copyright (C) 2006 Roberto Cappuccio <roberto.cappuccio@gmail.com>
    Copyright (C) 2011 Dawit Alemayehu <adawit@kde.org>

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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef HTMLCREATOR_H
#define HTMLCREATOR_H

#include <QtCore/QObject>
#include <QtCore/QEventLoop>

#include <kio/thumbcreator.h>

class QWebPage;

class HTMLCreator : public QObject, public ThumbCreator
{
    Q_OBJECT
public:
    HTMLCreator();
    virtual ~HTMLCreator();
    virtual bool create(const QString &path, int width, int height, QImage &img);
    virtual Flags flags() const;

protected:
    virtual void timerEvent (QTimerEvent*);

private Q_SLOTS:
    void slotFinished(bool);

private:
    bool m_loadedOk;
    QWebPage *m_page;
    QEventLoop m_eventLoop;
};

#endif
