// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "pluginproxyinterface.h"

#include <QLabel>
#include <QObject>
#include <QWindow>
#include <QScopedPointer>

enum class DockPart {
    QuickShow = 0,    // 快捷插件显示区域
    QuickPanel,       // 快捷面板区域
    SystemPanel,      // 系统插件显示区域
    DCCSetting        // 显示在控制中心个性化设置的图标
};

enum PluginFlag {
    Type_NoneFlag = 0x1,                 // 插件类型-没有任何的属性，不在任何地方显示
    Type_Common = 0x2,                   // 插件类型-快捷插件区
    Type_Tool = 0x4,                     // 插件类型-工具插件，例如回收站
    Type_System = 0x8,                   // 插件类型-系统插件，例如关机插件
    Type_Tray = 0x10,                    // 插件类型-托盘区，例如U盘插件
    Type_Fixed = 0x20,                   // 插件类型-固定区域,例如多任务视图和显示桌面

    Quick_Single = 0x40,                 // 当插件类型为Common时,快捷插件区域只有一列的那种插件
    Quick_Multi = 0x80,                  // 当插件类型为Common时,快捷插件区占两列的那种插件
    Quick_Full = 0x100,                  // 当插件类型为Common时,快捷插件区占用4列的那种插件，例如声音、亮度设置和音乐等

    Attribute_CanDrag = 0x200,           // 插件属性-是否支持拖动
    Attribute_CanInsert = 0x400,         // 插件属性-是否支持在其前面插入其他的插件，普通的快捷插件是支持的
    Attribute_CanSetting = 0x800,        // 插件属性-是否可以在控制中心设置显示或隐藏
    Attribute_ForceDock = 0x1000,        // 插件属性-强制显示在任务栏上

    FlagMask = 0xffffffff                // 掩码
};

Q_DECLARE_FLAGS(PluginFlags, PluginFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(PluginFlags)
  
#define QUICK_ITEM_KEY "quick_item_key"

namespace dock {
class DockPlugin;
class TrayIconWidget;
class WidgetPlugin : public QObject, public PluginProxyInterface
{
    Q_OBJECT
    Q_PROPERTY(QString pluginId READ pluginName)

public:
    WidgetPlugin(PluginsItemInterface* pluginItem);
    ~WidgetPlugin();

    // proxy interface
    void itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    void itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    void itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    void requestWindowAutoHide(PluginsItemInterface * const itemInter, const QString &itemKey, const bool autoHide) override;
    void requestRefreshWindowVisible(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    void requestSetAppletVisible(PluginsItemInterface * const itemInter, const QString &itemKey, const bool visible) override;
    void saveValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &value) override;
    const QVariant getValue(PluginsItemInterface *const itemInter, const QString &key, const QVariant& fallback = QVariant()) override;
    void removeValue(PluginsItemInterface *const itemInter, const QStringList &keyList) override;
    void updateDockInfo(PluginsItemInterface *const, const DockPart &);

    const QString pluginName() const;
    const QString itemCommand(const QString &itemKey);
    const QString itemContextMenu(const QString &itemKey);

public Q_SLOTS:
    void onDockPositionChanged(uint32_t position);
    void onDockDisplayModeChanged(uint32_t displayMode);

private:
    QWidget* getQucikPluginTrayWidget(const QString &itemKey);
    DockPlugin* getPlugin(QWidget*);

private:
    PluginsItemInterface* m_pluginItem;

    QScopedPointer<TrayIconWidget> m_widget;
    bool m_isPluginInterface_v2 = false;
};

class TrayIconWidget : public QWidget
{
    Q_OBJECT
public:
    TrayIconWidget(PluginsItemInterface* m_pluginItem, QString m_itemKey, QWidget* parent = nullptr);
    ~TrayIconWidget();

    void paintEvent(QPaintEvent *event) override;

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    PluginsItemInterface* m_pluginItem;
    QString m_itemKey;
};
}
