/*
 *   Copyright 2010 Marco Martin <notmart@gmail.com>
 *   Copyright 2011 Daker Fernandes Pinheiro <dakerfp@gmail.com>
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
import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: listItem

    // Plasma API
    property Item view: null
    property alias hoverEnabled: mouse.hoverEnabled
    property alias hover: mouse.containsMouse
    property alias pressed: mouse.pressed
    signal selected(int index)

    implicitWidth: view.width
    implicitHeight: childrenRect.height + 4

    MouseArea {
        id: mouse

        anchors.fill: parent
        hoverEnabled: false
        onEntered: { view.currentIndex = index; }
        onClicked: {
            view.currentIndex = index;
            listItem.selected(index);
        }
    }
}
