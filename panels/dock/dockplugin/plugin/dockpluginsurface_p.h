// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dockplugin.h"
#include "qwayland-dock-plugin-manager-v1.h"

#include <QtWaylandClient/private/qwaylandshellsurface_p.h>

namespace dock {

class DockPluginManager;
class DockPluginSurface : public QtWaylandClient::QWaylandShellSurface, public QtWayland::dock_plugin_surface
{
    Q_OBJECT

public:
    DockPluginSurface(DockPluginManager *manager, QtWaylandClient::QWaylandWindow *window);
    ~DockPluginSurface() override;

public Q_SLOTS:
    void onRequestSetAppletVisible(const QString &itemKey, uint32_t visible);
    void onContextMenuCreated(const QString &contextMenu);
    void onDCCIconChanged(const QString &dccIcon);
    void onPluginFlags(int32_t flags);

protected:
    void dock_plugin_surface_configure(int32_t x, int32_t y, int32_t width, int32_t height) override;

private:
    DockPlugin* m_plugin;
    QWindow* m_window;
};

class DockToolTipSurface : public QtWaylandClient::QWaylandShellSurface, public QtWayland::dock_plugin_tooltip_surface
{
    Q_OBJECT

public:
    DockToolTipSurface(DockPluginManager *manager, QtWaylandClient::QWaylandWindow *window);
    ~DockToolTipSurface() override;

protected:
    void dock_plugin_tooltip_surface_close() override;

private:
    QWindow* m_window;
};
}