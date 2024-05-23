// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWayland.Compositor
import Qt.labs.platform 1.1 as LP

import org.deepin.ds 1.0
import org.deepin.dtk 1.0 as D
import org.deepin.ds.dock 1.0

AppletItem {
    id: tray

    property bool useColumnLayout: Panel.position % 2
    property int dockOrder: 25
    implicitWidth: useColumnLayout ? Panel.rootObject.dockSize : trayContainter.suggestedImplicitWidth + overflowBtn.implicitWidth * 2
    implicitHeight: useColumnLayout ? trayContainter.suggestedImplicitHeight + overflowBtn.implicitHeight * 2 : Panel.rootObject.dockSize

    PanelPopup {
        id: popup
        width: popupContent.width
        height: popupContent.height
        
        property int defaultX: popup.x
        property int defaultY: popup.y

        onHeightChanged: {
            popup.y = popup.defaultY - popup.height
        }

        onWidthChanged: {
            popup.x = popup.defaultX - popup.width / 2
        }

        ShellSurfaceItem {
            id: popupContent
            onSurfaceDestroyed: {
                popup.close()
            }
        }
    }

    GridLayout {
        id: overflowId
        columns: 1
        rows: 1
        anchors.centerIn: parent
        flow: useColumnLayout ? GridLayout.LeftToRight : GridLayout.TopToBottom
        property bool trayVisible: true
        columnSpacing: 10
        rowSpacing: 10
        OverflowContainer {
            id: trayContainter
            useColumnLayout: tray.useColumnLayout
            model: DockCompositor.trayPluginSurfaces
            spacing: 10
            delegate: Item {
                id: pluginItem
                property var plugin: modelData
                property var popupShow: false
                implicitHeight: 16
                implicitWidth: 16

                ShellSurfaceItem {
                    anchors.centerIn: parent
                    width: 16
                    height: 16
                    shellSurface: plugin
                }

                Component.onCompleted: {
                    var pos = pluginItem.mapToItem(null, 0, 0)
                    plugin.updatePluginGeometry(Qt.rect(pos.x, pos.y, 16, 16))
                }
            }
        }
        D.ActionButton {
            id: overflowBtn
            implicitWidth: 16
            implicitHeight: 16
            icon.name: "dock-control-panel"
            icon.height: 16
            icon.width: 16
            onClicked: {
                var itemPos = tray.getItemPopupPosition(overflowBtn, popup)
                popup.x = itemPos.x
                popup.y = itemPos.y
                popup.open()
            }
        }
    }

    Connections {
        target: DockCompositor
        function onTooltipCreated(tooltip)
        {
            popupContent.shellSurface = tooltip
            popup.defaultX = tooltip.x - (popup.width / 2)
            popup.defaultY = - 10
            popup.open()
        }
    }

    WaylandOutput {
        compositor: DockCompositor.compositor
        window: Panel.rootObject
        sizeFollowsWindow: true
    }

    WaylandOutput {
        compositor: DockCompositor.compositor
        window: Panel.popupWindow
        sizeFollowsWindow: false
    }

    function getItemPopupPosition(item, popupItem) {
        var itemPos = item.mapToItem(null,0,0)

        switch (Panel.position) {
        case Dock.Top:
            itemPos.x -= popupItem.width / 2
            itemPos.y += Panel.dockSize
            break
        case Dock.Right:
            itemPos.x -= (popupItem.width + 10)
            itemPos.y -= popupItem.height / 2
            break
        case Dock.Bottom:
            itemPos.x -= popupItem.width / 2
            itemPos.y -= (popupItem.height + 10)
            break
        case Dock.Left:
            itemPos.x += Panel.dockSize
            itemPos.y -= popupItem.height / 2
            break
        }

        return itemPos
    }
}