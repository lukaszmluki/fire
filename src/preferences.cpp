/*
 * File:   preferences.cpp
 * Author: Lukasz Marek
 *
 * Created on December 8, 2013, 1:29 PM
 */

#include "preferences.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

Preferences& Preferences::instance()
{
    static Preferences instance;
    return instance;
}

Preferences::~Preferences()
{
    delete m_settings;
}

Preferences::Preferences()
{
    QString m_configFile;
    qDebug() << "Loading preferences...";
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    m_configFile = getenv("HOME");
    m_configFile.append( "/.opensubedit/config.ini");
#elif defined(Q_OS_WIN32)
    m_configFile = getenv("USERPROFILE");
    m_configFile.append( "\\Open SubEdit\\config.ini");
#endif
    qDebug() << m_configFile;
    bool needDefault = !QFile::exists(m_configFile);
    m_settings = new QSettings(m_configFile, QSettings::IniFormat);
    if (needDefault) {
        createDefaultConfig();
    }
    m_defaultFontEditor.setFamily(getValue("editor/font_editor_family", "Arial").toString());
    m_defaultFontEditor.setPointSize(getValue("editor/font_editor_size", 12).toInt());
}

QFont Preferences::getDefaultFontEditor() const
{
    return m_defaultFontEditor;
}

void Preferences::setViewMode(ViewMode viewMode)
{
    m_viewMode = viewMode;
}

Preferences::ViewMode Preferences::getViewMode() const
{
    return m_viewMode;
}

void Preferences::createDefaultConfig()
{
    setValue("editor/encoding", "default");
#ifdef Q_WS_WIN
    setValue("editor/end_line_mark", 1);
#elif defined Q_WS_X11
    setValue("editor/end_line_mark", 0);
#else
    setValue("editor/end_line_mark", 2);
#endif
    setValue("editor/spellchecking", false);
}

void Preferences::setLastDir(const QString &dir)
{
    setValue("last_dir", dir.left(dir.lastIndexOf(QDir::separator())));
}

void Preferences::setValue(const QString &key, const QVariant &value, bool apply)
{
    m_settings->setValue(key, value);
    if (apply) {
	m_defaultFontEditor.setFamily(getValue("editor/font_editor_family", "Arial").toString());
	m_defaultFontEditor.setPointSize(getValue("editor/font_editor_size", 12).toInt());
        emit applySettings();
    }
}

QVariant Preferences::getValue(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}
