/*
 * File:   mainwindow.cpp
 * Author: Lukasz Marek
 *
 * Created on December 10, 2013, 10:08 PM
 */

#include "mainwindow.h"
#include <QResizeEvent>
#include <QMoveEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDebug>
#include <QLabel>
#include <QActionGroup>
#include <QAction>
#include <QToolButton>
#include <QToolBar>
#include <QSlider>
#include <QFileDialog>
#include <QDir>
#include "common.h"
#include "preferences.h"
#include "openglwidget.h"
#include "x11widget.h"
#include "subtitleseditor.h"
#include "playermanager.h"
#include "ffengine.h"
#include "playlist/playlist_view.h"

void MainWindow::createCentralWidget()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    m_centralWidget = new QWidget();
    m_centralWidget->setLayout(layout);
}

void MainWindow::createMovieLine()
{
    QHBoxLayout *hlayout = new QHBoxLayout();
    m_movieLine = new QWidget();
    m_movieLine->setLayout(hlayout);
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 1, 0, 0);
    m_moviePosition = new QLabel();
    hlayout->addWidget(m_moviePosition);
    hlayout->addStretch(1);
    m_movieInfo = new QLabel();
    hlayout->addWidget(m_movieInfo);
}

void MainWindow::createVideoNavigator()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(5);
    layout->setMargin(2);
    m_navigationPanel = new QWidget();
    m_navigationPanel->setLayout(layout);
    m_navigationPanel->setMaximumHeight(30);

    m_playButton = addNavigationButton("navigation/play.bmp", SLOT(togglePause()), layout);
    addNavigationButton("navigation/stop.bmp", SLOT(stop()), layout);
    addNavigationButton("navigation/backward.bmp", SLOT(backward()), layout);
    addNavigationButton("navigation/forward.bmp", SLOT(forward()), layout);
    addNavigationButton("navigation/previous.bmp", SLOT(previous()), layout);
    addNavigationButton("navigation/next.bmp", SLOT(next()), layout);
    layout->addSpacing(20);

    QToolButton *mute = new QToolButton();
    mute->setText("M");
    mute->setMaximumWidth(20);
    connect(mute, SIGNAL(clicked()), this, SLOT(muteButtonClicked()));
    layout->addWidget(mute);
    layout->addSpacing(10);

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setFocusPolicy(Qt::NoFocus);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setSliderPosition(Preferences::instance().getValue("Player/volume", 100).toInt());
    m_volumeSlider->setMaximumWidth(100);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeSliderChanged(int)));
    layout->addWidget(m_volumeSlider);
    layout->addSpacing(20);

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setTracking(false);
    m_positionSlider->setFocusPolicy(Qt::NoFocus);
    m_positionSlider->setRange(0, 0);
    m_positionSlider->setSliderPosition(0);
    connect(m_positionSlider, SIGNAL(valueChanged(int)), this, SLOT(positionSliderChanged(int)));
    layout->addWidget(m_positionSlider);
}

void MainWindow::createPlaylistArea()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);

    m_playlistArea = new QWidget();
    m_playlistArea->setLayout(layout);

    layout->addWidget(new PlaylistView());
}

void MainWindow::createVideoArea()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(2);
    layout->setMargin(0);

    m_videoArea = new QWidget();
    m_videoArea->setLayout(layout);
    m_videoArea->setMinimumSize(100,100);
    m_videoArea->setStyleSheet("background-color:black;");

    Preferences::RenderingEngine renderingEngine = Preferences::instance().getRenderingEngine();

    if (renderingEngine == Preferences::RENDERING_ENGINE_OPENGL) {
        m_videoWidget = new OpenGLWidget();
    } else if (renderingEngine == Preferences::RENDERING_ENGINE_X11) {
        m_videoWidget = new X11Widget();
    }

    if (m_videoWidget)
        qDebug() << "using engine: " << m_videoWidget->engineName();
    else
        qCritical() << "No rendering engine available";

    QWidget *videoWidget = dynamic_cast<QWidget *>(m_videoWidget);
    if (videoWidget) {
        layout->addWidget(videoWidget);
    } else {
        qDebug() << "Cannot cast m_videoArea to QWidget";
    }
}

void MainWindow::createSubtitlesEditorArea()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(2);
    layout->setMargin(0);

    m_subtitlesEditorArea = new QWidget();
    m_subtitlesEditorArea->setLayout(layout);
    m_subtitlesEditorArea->setMinimumSize(100,100);

    m_editorFileName = new QLabel("");
    layout->addWidget(m_editorFileName);
    m_subtitlesEditor = new SubtitlesEditor();
    m_subtitlesEditor->setLineWrapMode(QTextEdit::NoWrap);
//    highlighter = new QSubtitlesHighlighter(subtitlesEditor);
    layout->addWidget(m_subtitlesEditor);
    m_editorInfoLine = new QLabel("");
    layout->addWidget(m_editorInfoLine);
}

void MainWindow::createMenu()
{
//    QAction *action;
    QActionGroup *group;
    QMenu *menu;

    //TOOL BARS
//    m_fileToolBar = addToolBar(tr("File"));
//    m_stuffToolBar = addToolBar(tr("Stuff"));

    //MENU
    menu = menuBar()->addMenu(tr("&File"));
    /*action =*/ addMenuAction(menu, Utils::imagePath("menu/open_video.bmp"), tr("&Open video file"),
                           this, SLOT(loadVideo()), QKeySequence(Qt::CTRL + Qt::Key_O), QVariant(0));
//    m_fileToolBar->addAction(action);
    /*action =*/ addMenuAction(menu, Utils::imagePath("menu/close_video.bmp"), tr("&Close video file"),
                           this, SLOT(closeVideo()), QKeySequence(Qt::Key_F4 + Qt::CTRL), QVariant(0));
//    m_fileToolBar->addAction(action);
//    menu->addSeparator();
//    action = addMenuAction(menu, imagePath("menu/open_sub.bmp"), tr("Open &subtitles"), this, SLOT(loadSubtitles()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O), QVariant(0));
//    m_fileToolBar->addAction(action);
//    action = addMenuAction(menu, imagePath("menu/save_sub.bmp"), tr("Save subtitles"), this, SLOT(saveSubtitles()), QKeySequence(Qt::CTRL + Qt::Key_S), QVariant(0));
//    m_fileToolBar->addAction(action);
//    action = addMenuAction(menu, imagePath("menu/save_sub.bmp"), tr("Save subtitles as..."), this, SLOT(saveSubtitlesAs()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), QVariant(0));
//    action = addMenuAction(menu, imagePath("menu/close_sub.bmp"), tr("Close subtitles"), this, SLOT(closeSubtitles()), QKeySequence(Qt::Key_F4 + Qt::SHIFT + Qt::CTRL), QVariant(0));
//    m_fileToolBar->addAction(action);
    menu->addSeparator();
    /*action =*/ addMenuAction(menu, Utils::imagePath("menu/exit.bmp"), tr("&Exit"), this, SLOT(quit()), QKeySequence(Qt::Key_F4 + Qt::ALT), QVariant(0));

    menu = menuBar()->addMenu(tr("&Video"));
    m_videoStreamsSubMenu = menu->addMenu(tr("Streams"));
    m_videoStreamsSubMenu->setEnabled(false);
    m_videoStreamsGroup = new QActionGroup(m_videoStreamsSubMenu);
    QMenu *aspectSubMenu = menu->addMenu(tr("Ascect Ratio"));
        group = new QActionGroup(aspectSubMenu);
        addMenuAction(aspectSubMenu, NULL, tr("Oryginal"), this, SLOT(changeAspectRatio()), QKeySequence(), QVariant(0.0), group)->setChecked(true);
        addMenuAction(aspectSubMenu, NULL, "4:3", this, SLOT(changeAspectRatio()), QKeySequence(), QVariant((double)4/ double(3)), group);
        addMenuAction(aspectSubMenu, NULL, "16:9", this, SLOT(changeAspectRatio()), QKeySequence(), QVariant((double)16/ double(9)), group);

    menu = menuBar()->addMenu(tr("&Audio"));
    m_audioStreamsSubMenu = menu->addMenu(tr("Streams"));
    m_audioStreamsSubMenu->setEnabled(false);
    m_audioStreamsGroup = new QActionGroup(m_audioStreamsSubMenu);

//        m_subtitlesConvertersMenu = menuBar()->addMenu(tr("Subtitles converters"));

        menu = menuBar()->addMenu(tr("&Editor"));
        /*action =*/ addMenuAction(menu, Utils::imagePath("menu/editor.bmp"), tr("Show/hide subtitles &editor"), this, SLOT(toggleEditor()), QKeySequence(Qt::CTRL + Qt::Key_E), QVariant(0));
//        m_stuffToolBar->addAction(action);
//        menu->addSeparator();
//        QMenu *encodingSubMenu = menu->addMenu(tr("En&coding"));
//        group = new QActionGroup(encodingSubMenu);
//        for(int i=0; Encodings[i].comment != NULL; i++) {
//            action = add_menu_action(encodingSubMenu, NULL, tr(Encodings[i].comment), this, SLOT(changeEncoding()), 0, QVariant(Encodings[i].name), group);
//            action->setChecked(Encodings[i].name == pref.getValue("editor/encoding", "default").toByteArray());
//        }
//        m_endLineSubMenu = menu->addMenu(tr("End line &mark"));
//        group = new QActionGroup(m_endLineSubMenu);
//        action = addMenuAction(m_endLineSubMenu, NULL, "Unix", this, SLOT(changeEndLine()), 0, QVariant(0), group);
//        action->setChecked(Preferences::instance().getValue("editor/end_line_mark", 0) == 0);
//        action = addMenuAction(m_endLineSubMenu, NULL, "Windows", this, SLOT(changeEndLine()), 0, QVariant(1), group);
//        action->setChecked(Preferences::instance().getValue("editor/end_line_mark", 0) == 1);
//        action = addMenuAction(m_endLineSubMenu, NULL, "Mac", this, SLOT(changeEndLine()), 0, QVariant(2), group);
//        action->setChecked(Preferences::instance().getValue("editor/end_line_mark", 0) == 2);
//        menu->addSeparator();

//        //incorrectTimeHighlighter = add_menu_action(subMenu, NULL, tr("Mark as red lines with incorrect time format"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
//        //incorrectTimeHighlighter->setCheckable(true);
//        //incorrectTimeHighlighter->setChecked(pref.getValue("editor/highlight_time_format", true).toBool());
//        //sublinesHighlighter = add_menu_action(subMenu, NULL, tr("Mark as ping lines with to many sublines"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
//        //sublinesHighlighter->setCheckable(true);
//        //sublinesHighlighter->setChecked(pref.getValue("editor/highlight_sublines", true).toBool());
//        //charsHighlighter = add_menu_action(subMenu, NULL, tr("Mark as ping sublines with to many characters"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
//        //charsHighlighter->setCheckable(true);
//        //charsHighlighter->setChecked(pref.getValue("editor/highlight_chars", true).toBool());
//        //grayHighlighter = add_menu_action(subMenu, NULL, tr("Mark as gray short-displayed lines"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
//        //grayHighlighter->setCheckable(true);
//        //grayHighlighter->setChecked(pref.getValue("editor/highlight_gray", true).toBool());
//        addMenuAction(menu, imagePath("menu/shortcuts.bmp"), tr("Load &shortcuts"), m_subtitlesEditor, SLOT(loadShortCuts()), 0, QVariant(1));

//        menu = menuBar()->addMenu(tr("&Options"));
//        action = addMenuAction(menu, imagePath("menu/pref.bmp"), tr("&Preferences"), optionsWindow, SLOT(exec()), QKeySequence(Qt::Key_U + Qt::CTRL), QVariant(0));
//        m_stuffToolBar->addAction(action);

    menu = menuBar()->addMenu(tr("&View"));
    addMenuAction(menu, NULL, tr("&Playlist"),
                  this, SLOT(togglePlaylist()), QKeySequence(Qt::CTRL + Qt::Key_L), QVariant(0));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createCentralWidget();
    createMovieLine();
    createVideoArea();
    createSubtitlesEditorArea();
    createPlaylistArea();
    createMenu();
    createVideoNavigator();

    m_splitterVideoPlaylistArea = new QSplitter(Qt::Horizontal);
    m_splitterVideoPlaylistArea->addWidget(m_playlistArea);
    m_splitterVideoPlaylistArea->addWidget(m_videoArea);

    m_splitterVideoSubtitlesArea = new QSplitter(Qt::Vertical);
    m_splitterVideoSubtitlesArea->addWidget(new QWidget());
    m_splitterVideoSubtitlesArea->widget(0)->setLayout(new QVBoxLayout());
    m_splitterVideoSubtitlesArea->widget(0)->layout()->setMargin(0);
    m_splitterVideoSubtitlesArea->widget(0)->layout()->setSpacing(0);
    m_splitterVideoSubtitlesArea->widget(0)->layout()->addWidget(m_splitterVideoPlaylistArea);
    m_splitterVideoSubtitlesArea->widget(0)->layout()->addWidget(m_navigationPanel);
    m_splitterVideoSubtitlesArea->addWidget(m_subtitlesEditorArea);
    m_splitterVideoSubtitlesArea->setCollapsible(0, false);
    m_splitterVideoSubtitlesArea->setCollapsible(1, true);

    m_centralWidget->layout()->addWidget(m_splitterVideoSubtitlesArea);
    //m_centralWidget->layout()->addWidget(m_movieLine);
    setCentralWidget(m_centralWidget);

    //restore geometry of the window
    const QByteArray &geometry = Preferences::instance().getValue("Window/geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        setGeometry(0, 0, 640, 480);
        m_splitterVideoPlaylistArea->setSizes(QList<int>() << 140 << 500);
    } else {
        restoreGeometry(geometry);
        m_splitterVideoPlaylistArea->restoreState(
            Preferences::instance().getValue("Window/splitter_video_playlist_state", QByteArray()).toByteArray());
        m_splitterVideoSubtitlesArea->restoreState(
            Preferences::instance().getValue("Window/splitter_video_subtitles_state", QByteArray()).toByteArray());
    }
    updateEditorVisibility();
    updatePlaylistVisibility();
    connect(m_splitterVideoPlaylistArea, SIGNAL(splitterMoved(int, int)), this, SLOT(saveSplitterState(int, int)));
    connect(m_splitterVideoSubtitlesArea, SIGNAL(splitterMoved(int, int)), this, SLOT(saveSplitterState(int, int)));

    setWindowTitle(Utils::APPLICATION_NAME);

    //register player
    FFEngine *player = PlayerManager::instance().registerPlayer(m_videoWidget);
    player->addGuiDelegate(this);

//    connect(m_subtitlesEditor, SIGNAL(fileNameTextChanged(const QString&)), this, SLOT(changeEditorFileName(const QString &)));
//    connect(m_subtitlesEditor, SIGNAL(infoLineTextChanged(const QString&)), this, SLOT(changeEditorInfoLine(const QString&)));
//    connect(m_videoArea, SIGNAL(fullscreen(bool)), this, SLOT(fullscreen(bool)));
//    connect(this, SIGNAL(loadingSubtitles(const QString&)), m_subtitlesEditor, SLOT(loadSubtitles(const QString&)));
//    connect(this, SIGNAL(closingSubtitles()), m_subtitlesEditor, SLOT(closeSubtitles()));
//    connect(this, SIGNAL(savingSubtitles()), m_subtitlesEditor, SLOT(saveSubtitles()));
//    connect(this, SIGNAL(savingSubtitlesAs(const QString&)), m_subtitlesEditor, SLOT(saveSubtitlesAs(const QString&)));
//    updatePlayerState();
    show();
}

MainWindow::~MainWindow()
{
    PlayerManager::instance().unregisterPlayer(m_videoWidget);
}

void MainWindow::updatePlaylistVisibility()
{
    if (Preferences::instance().getValue("Window/playlist", true).toBool())
        showPlaylist();
    else
        hidePlaylist();
}

void MainWindow::updateEditorVisibility()
{
    if (Preferences::instance().getValue("Window/editor", false).toBool())
        showEditor();
    else
        hideEditor();
}

QToolButton* MainWindow::addNavigationButton(const QString &iconFile, const char* member, QHBoxLayout *layout)
{
    QToolButton *button = new QToolButton(m_navigationPanel);
    button->setIcon(QIcon(Utils::imagePath(iconFile)));
    button->setFocusPolicy(Qt::NoFocus);
    connect(button, SIGNAL(clicked()), this, member);
    layout->addWidget(button);
    return button;
}

QAction* MainWindow::addMenuAction(QMenu *menu, const QString &iconFile, const QString &text,
                                   const QObject *receiver, const char *member, const QKeySequence &shortcut,
                                   const QVariant &data, QActionGroup *group, bool showIcon)
{
    QAction *action;
    if (!iconFile.isEmpty()) {
        QIcon icon(iconFile);
        action = menu->addAction(icon, text, receiver, member, shortcut);
        action->setIconVisibleInMenu(showIcon);
    } else {
        action = menu->addAction(text, receiver, member, shortcut);
    }
    action->setData(data);
    if (group) {
        action->setCheckable(true);
        action->setActionGroup(group);
    }
    return action;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    Preferences::instance().setValue("Window/geometry", saveGeometry());
    saveSplitterState(0, 0);
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    QMainWindow::moveEvent(e);
    Preferences::instance().setValue("Window/geometry", saveGeometry());
    saveSplitterState(0, 0);
}

void MainWindow::showEditor()
{
    m_subtitlesEditorArea->show();
    m_subtitlesEditor->setFocus();
    Preferences::instance().setValue("Window/editor", true);
}

void MainWindow::hideEditor()
{
    m_subtitlesEditor->clearFocus();
    m_subtitlesEditorArea->hide();
    Preferences::instance().setValue("Window/editor", false);
}

void MainWindow::toggleEditor()
{
    if (m_subtitlesEditorArea->isHidden())
        showEditor();
    else
        hideEditor();
}

void MainWindow::showPlaylist()
{
    m_playlistArea->show();
    Preferences::instance().setValue("Window/playlist", true);
}

void MainWindow::hidePlaylist()
{
    m_playlistArea->hide();
    Preferences::instance().setValue("Window/playlist", false);
}

void MainWindow::togglePlaylist()
{
    if (m_playlistArea->isHidden())
        showPlaylist();
    else
        hidePlaylist();
}

void MainWindow::loadVideo()
{
    QString filename = Utils::selectVideo(this);
    if (filename.isEmpty() || !QFile::exists(filename)) {
        qDebug() << "no file selected, exit";
        return;
    }
    Preferences::instance().setLastDir(filename);

    PlayerManager::instance().getPlayer(m_videoWidget)->openMedia(filename);

//    filename = filename.left(filename.lastIndexOf(".")) + "txt";
//    if (QFile::exists(filename)) {
//        emit loadingSubtitles(filename);
//    }
//    updatePlayerState();
}

void MainWindow::closeVideo()
{

}

void MainWindow::stop()
{

}

void MainWindow::togglePause()
{
    PlayerManager::instance().getPlayer(m_videoWidget)->togglePause();
}

void MainWindow::paused()
{
    m_playButton->setIcon(QIcon(Utils::imagePath("navigation/play.bmp")));
}

void MainWindow::resumed()
{
    m_playButton->setIcon(QIcon(Utils::imagePath("navigation/pause.bmp")));
}

void MainWindow::durationChanged(double duration)
{
    m_positionSlider->setMaximum(duration);
}

void MainWindow::positionChanged(double position)
{
    if (!m_positionSlider->isSliderDown())
        m_positionSlider->setSliderPosition(position);
}

void MainWindow::positionSliderChanged(int position)
{
    PlayerManager::instance().getPlayer(m_videoWidget)->seek(position);
}

void MainWindow::volumeSliderChanged(int volume)
{
    if (m_volumeSlider->isSliderDown())
        PlayerManager::instance().getPlayer(m_videoWidget)->setVolume(static_cast<double>(volume) / 100.0);
}

void MainWindow::muteButtonClicked()
{
    PlayerManager::instance().getPlayer(m_videoWidget)->toggleMute();
}

void MainWindow::muteChanged(int mute)
{
    qDebug() << "muteChanged outside" << mute;
}

void MainWindow::volumeChanged(double volume)
{
    if (!m_volumeSlider->isSliderDown())
        m_volumeSlider->setSliderPosition(volume * 100);
}

void MainWindow::saveSplitterState(int pos, int index)
{
    Q_UNUSED(pos) Q_UNUSED(index)
    Preferences::instance().setValue("Window/splitter_video_playlist_state",
                                     m_splitterVideoPlaylistArea->saveState());
    Preferences::instance().setValue("Window/splitter_video_subtitles_state",
                                     m_splitterVideoSubtitlesArea->saveState());
}
