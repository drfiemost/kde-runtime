/*
 *   Copyright (C) 2011 Ivan Cukic <ivan.cukic(at)kde.org>
 *   Copyright (c) 2011 Sebastian Trueg <trueg@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef NEPOMUK_EVENT_BACKEND_H_
#define NEPOMUK_EVENT_BACKEND_H_

#include "EventBackend.h"

#include <Nepomuk/Resource>

class KUrl;
class QDateTime;

/**
 *
 */
class NepomukEventBackend: public EventBackend {
public:
    NepomukEventBackend(QObject * parent = 0);

    virtual void addEvents(const EventList & events);

private:
    Nepomuk::Resource createDesktopEvent(const KUrl& uri, const QDateTime& startTime, const QString& app);

    Nepomuk::Resource m_currentActivity;
};

#endif // NEPOMUK_EVENT_BACKEND_H_

