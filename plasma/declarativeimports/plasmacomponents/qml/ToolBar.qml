/*
*   Copyright (C) 2011 by Marco Martin <mart@kde.org>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU Library General Public License as
*   published by the Free Software Foundation; either version 2, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Library General Public License for more details
*
*   You should have received a copy of the GNU Library General Public
*   License along with this program; if not, write to the
*   Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item{
    id: toolBar
    width: parent.width
    height: 48 + frameSvg.margins.top + frameSvg.margins.bottom

    // The current set of tools; null if none.
    property Item tools

    // The transition type. One of the following:
    //      set         an instantaneous change (default)
    //      push        follows page stack push animation
    //      pop         follows page stack pop animation
    //      replace     follows page stack replace animation
    property string transition: "set"

    // Sets the tools with a transition.
    function setTools(tools, transition)
    {
        if (toolBar.tools == tools) {
            return
        }

        toolBar.transition = transition
        toolBar.tools = tools
    }
    onToolsChanged: {
        var newContainer
        var oldContainer
        if (containerA.current) {
            newContainer = containerB
            oldContainer = containerA
        } else {
            newContainer = containerA
            oldContainer = containerB
        }
        containerA.current = !containerA.current

        tools.parent = newContainer
        tools.visible = true
        tools.anchors.left = newContainer.left
        tools.anchors.right = newContainer.right
        tools.anchors.verticalCenter = newContainer.verticalCenter

        switch (transition) {
        case "push":
            containerA.animationsEnabled = true
            oldContainer.x = -oldContainer.width/2

            containerA.animationsEnabled = false
            newContainer.x = newContainer.width/2
            newContainer.y = 0
            containerA.animationsEnabled = true
            newContainer.x = 0
            break
        case "pop":
            containerA.animationsEnabled = true
            oldContainer.x = oldContainer.width/2

            containerA.animationsEnabled = false
            newContainer.x = -newContainer.width/2
            newContainer.y = 0
            containerA.animationsEnabled = true
            newContainer.x = 0
            break
        case "replace":
            containerA.animationsEnabled = true
            oldContainer.y = oldContainer.height

            containerA.animationsEnabled = false
            newContainer.x = 0
            newContainer.y = -newContainer.height
            containerA.animationsEnabled = true
            newContainer.y = 0
            break
        case "set":
        default:
            containerA.animationsEnabled = false
            containerA.animationsEnabled = false
            oldContainer.x = -oldContainer.width/2
            newContainer.x = 0
            break
        }

        newContainer.opacity = 1
        oldContainer.opacity = 0
    }

    PlasmaCore.FrameSvgItem {
        id: frameSvg
        imagePath: "widgets/frame"
        prefix: "raised"
        anchors {
            fill: parent
            leftMargin: -margins.left
            rightMargin: -margins.right
            topMargin: toolBar.y <= 0 ? -margins.top : 0
            bottomMargin: toolBar.y >= toolBar.parent.height - toolBar.height ? -margins.bottom : 0
        }
    }

    Item {
        anchors {
            fill: parent
            leftMargin: parent.margins.left
            topMargin: parent.margins.top
            rightMargin: parent.margins.right
            bottomMargin: parent.margins.bottom
        }

        Item {
            id: containerA
            width: parent.width
            height: parent.height
            property bool animationsEnabled: false
            opacity: 0
            //this asymmetry just to not export a property
            property bool current: false
            Behavior on opacity {
                PropertyAnimation { duration: 250 }
            }
            Behavior on x {
                enabled: containerA.animationsEnabled
                PropertyAnimation {
                    duration: 250
                }
            }
            Behavior on y {
                enabled: containerA.animationsEnabled
                PropertyAnimation {
                    duration: 250
                }
            }
        }
        Item {
            id: containerB
            width: parent.width
            height: parent.height
            opacity: 0
            Behavior on opacity {
                PropertyAnimation { duration: 250 }
            }
            Behavior on x {
                enabled: containerA.animationsEnabled
                PropertyAnimation {
                    duration: 250
                }
            }
            Behavior on y {
                enabled: containerA.animationsEnabled
                PropertyAnimation {
                    duration: 250
                }
            }
        }
    }
}
