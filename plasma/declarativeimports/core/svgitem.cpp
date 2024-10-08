/*
 *   Copyright 2010 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "svgitem.h"

#include <QPainter>

#include "kdebug.h"
#include "plasma/svg.h"

namespace Plasma
{

SvgItem::SvgItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_smooth(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}


SvgItem::~SvgItem()
{
}

void SvgItem::setElementId(const QString &elementID)
{
    if (elementID == m_elementID) {
        return;
    }

    if (implicitWidth() <= 0) {
        setImplicitWidth(naturalSize().width());
    }
    if (implicitHeight() <= 0) {
        setImplicitHeight(naturalSize().height());
    }

    m_elementID = elementID;
    emit elementIdChanged();
    emit naturalSizeChanged();
    update();
}

QString SvgItem::elementId() const
{
    return m_elementID;
}

QSizeF SvgItem::naturalSize() const
{
    if (!m_svg) {
        return QSizeF();
    } else if (!m_elementID.isEmpty()) {
        return m_svg.data()->elementSize(m_elementID);
    }

    return m_svg.data()->size();
}


void SvgItem::setSvg(Plasma::Svg *svg)
{
    if (m_svg) {
        disconnect(m_svg.data(), 0, this, 0);
    }
    m_svg = svg;
    if (svg) {
        connect(svg, SIGNAL(repaintNeeded()), this, SLOT(updateNeeded()));
        connect(svg, SIGNAL(repaintNeeded()), this, SIGNAL(naturalSizeChanged()));
        connect(svg, SIGNAL(sizeChanged()), this, SIGNAL(naturalSizeChanged()));
    }

    if (implicitWidth() <= 0) {
        setImplicitWidth(naturalSize().width());
    }
    if (implicitHeight() <= 0) {
        setImplicitHeight(naturalSize().height());
    }

    emit svgChanged();
    emit naturalSizeChanged();
}

Plasma::Svg *SvgItem::svg() const
{
    return m_svg.data();
}

void SvgItem::setSmooth(const bool smooth)
{
    if (smooth == m_smooth) {
        return;
    }
    m_smooth = smooth;
    emit smoothChanged();
    update();
}

bool SvgItem::smooth() const
{
    return m_smooth;
}

void SvgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_svg) {
        return;
    }
    //do without painter save, faster and the support can be compiled out
    const bool wasAntiAlias = painter->testRenderHint(QPainter::Antialiasing);
    const bool wasSmoothTransform = painter->testRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing, m_smooth);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, m_smooth);

    //setContainsMultipleImages has to be done there since m_frameSvg can be shared with somebody else
    m_svg.data()->setContainsMultipleImages(!m_elementID.isEmpty());
    m_svg.data()->paint(painter, boundingRect(), m_elementID);
    painter->setRenderHint(QPainter::Antialiasing, wasAntiAlias);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, wasSmoothTransform);
}

void SvgItem::updateNeeded()
{
    if (implicitWidth() <= 0) {
        setImplicitWidth(naturalSize().width());
    }
    if (implicitHeight() <= 0) {
        setImplicitHeight(naturalSize().height());
    }
    update();
}

void SvgItem::setImplicitWidth(qreal width)
{
    if (implicitWidth() == width) {
        return;
    }

    QDeclarativeItem::setImplicitWidth(width);

    emit implicitWidthChanged();
}

qreal SvgItem::implicitWidth() const
{
    return QDeclarativeItem::implicitWidth();
}

void SvgItem::setImplicitHeight(qreal height)
{
    if (implicitHeight() == height) {
        return;
    }

    QDeclarativeItem::setImplicitHeight(height);

    emit implicitHeightChanged();
}

qreal SvgItem::implicitHeight() const
{
    return QDeclarativeItem::implicitHeight();
}

} // Plasma namespace

#include "moc_svgitem.cpp"

