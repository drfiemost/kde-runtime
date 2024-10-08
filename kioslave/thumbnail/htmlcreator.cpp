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

#include "htmlcreator.h"

#include <QImage>
#include <QPainter>
#include <QtWebKit/QWebFrame>

#include <kapplication.h>
#include <kwebpage.h>
#include <kurl.h>
#include <kdebug.h>

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new HTMLCreator;
    }
}

HTMLCreator::HTMLCreator()
    : m_loadedOk(true), m_page(0)
{
}

HTMLCreator::~HTMLCreator()
{
    delete m_page;
}

bool HTMLCreator::create(const QString &path, int width, int height, QImage &img)
{
    if (!m_page)
    {
        m_page = new QWebPage;
        connect(m_page, SIGNAL(loadFinished(bool)), SLOT(slotFinished(bool)));
        m_page->settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
        m_page->settings()->setAttribute(QWebSettings::JavaEnabled, false);
        m_page->settings()->setAttribute(QWebSettings::PluginsEnabled, false);
        m_page->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, false);
        m_page->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
    }

    KUrl url(path);
    m_loadedOk = false;
    m_page->mainFrame()->load(url);

    const int t = startTimer((url.isLocalFile()?5000:30000));
    m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    killTimer(t);

    if (m_page->mainFrame()->contentsSize().isEmpty()) {
        m_loadedOk = false;
    }

    if (!m_loadedOk) {
        return false;
    }

    QPixmap pix;
    if (width > 400 || height > 600) {
        if (height * 3 > width * 4) {
            pix = QPixmap(width, width * 4 / 3);
        } else {
            pix = QPixmap(height * 3 / 4, height);
        }
    } else {
        pix = QPixmap(400, 600);
    }
    pix.fill(Qt::transparent);
    const int borderX = pix.width() / width;
    const int borderY = pix.height() / height;
    QRect clip (borderX, borderY, pix.width() - borderX * 2, pix.height() - borderY * 2);
    QPainter p (&pix);
    m_page->setViewportSize(m_page->mainFrame()->contentsSize());
    m_page->mainFrame()->render(&p, QWebFrame::ContentsLayer, clip);

    img = pix.toImage();
    return true;
}

void HTMLCreator::timerEvent(QTimerEvent *)
{
    m_eventLoop.quit();
}

void HTMLCreator::slotFinished(bool ok)
{
    m_loadedOk = ok;
    m_eventLoop.quit();
}

ThumbCreator::Flags HTMLCreator::flags() const
{
    return DrawFrame;
}

#include "htmlcreator.moc"

