/*
 * File:   preferences.h
 * Author: Lukasz Marek
 *
 * Created on December 8, 2013, 1:29 PM
 */

#ifndef SRC_PREFERENCES_H
#define SRC_PREFERENCES_H

#include <QObject>
#include <QSettings>
#include <QFont>
#include <QString>

class QVariant;
class QString;

class Preferences : public QObject
{
    Q_OBJECT

    Preferences();
    ~Preferences();
public:

    enum ViewMode {
        VIEW_MODE_NOGUI,     //framebuffer
        VIEW_MODE_WINDOWED,  //only video window
        VIEW_MODE_GUI        //full GUI
    };

    enum RenderingEngine {
        RENDERING_ENGINE_OPENGL,
        RENDERING_ENGINE_X11
    };

    static Preferences& instance();

    void setValue(const QString &key, const QVariant &value, bool apply = true);
    QVariant getValue(const QString &key, const QVariant &defaultValue) const;

    void setLastDir(const QString &dir);
    void setViewMode(ViewMode viewMode);
    ViewMode getViewMode() const;
    RenderingEngine getRenderingEngine() const;
    void setSelectedFile(const QString &file);
    const QString& getSelectedFile() const;
    QFont getDefaultFontEditor() const;
private:
    void createDefaultConfig();

    QSettings *m_settings;
    QFont m_defaultFontEditor;
    ViewMode m_viewMode;
    QString m_selectedFile;

signals:
    void applySettings();
};

#endif /* SRC_PREFERENCES_H */
