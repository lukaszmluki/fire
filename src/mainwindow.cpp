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
#include "utils.h"
#include "preferences.h"
#include "openglwidget.h"
#include "subtitleseditor.h"
#include "playermanager.h"
#include "ffengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QAction *action;
    QActionGroup *group;
    QMenu *menu;

//create main window
    m_normalViewArea = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_normalViewArea);
    layout->setSpacing(0);
    layout->setMargin(4);
    setCentralWidget(m_normalViewArea);

//create splitter and bottom text line
    QSplitter *splitter = new QSplitter(Qt::Vertical, m_normalViewArea);
    m_movieInfoLine = new QWidget();
    QHBoxLayout *hlayout = new QHBoxLayout(m_movieInfoLine);
    layout->addWidget(splitter);
    layout->addWidget(m_movieInfoLine);

//fill bottom line
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 1, 0, 0);
    m_videoLinePosition = new QLabel();
    hlayout->addWidget(m_videoLinePosition);
    hlayout->addStretch(1);
    m_videoLineInfo = new QLabel();
    hlayout->addWidget(m_videoLineInfo);

//create widget for the splitter (1)
    QWidget *vbox = new QWidget(splitter);
    layout = new QVBoxLayout(vbox);
    layout->setSpacing(2);
    layout->setMargin(0);
    splitter->addWidget(vbox);

    m_videoArea = new OpenGLWidget(vbox);
    m_videoArea->setMinimumSize(100,100);
    layout->addWidget(m_videoArea);
    splitter->setCollapsible(0, false);


        m_navigationPanel = new QWidget(vbox);
        hlayout = new QHBoxLayout(m_navigationPanel);
        hlayout->setSpacing(5);
        hlayout->setMargin(2);
        m_navigationPanel->setMaximumHeight(30);
            addNavigationButton("navigation/play.bmp", SLOT(play()), hlayout);
            addNavigationButton("navigation/pause.bmp", SLOT(pause()), hlayout);
            addNavigationButton("navigation/stop.bmp", SLOT(stop()), hlayout);
            addNavigationButton("navigation/backward.bmp", SLOT(backward()), hlayout);
            addNavigationButton("navigation/forward.bmp", SLOT(forward()), hlayout);
            addNavigationButton("navigation/previous.bmp", SLOT(previous()), hlayout);
            addNavigationButton("navigation/next.bmp", SLOT(next()), hlayout);
            hlayout->addSpacing(20);
            QSlider *volume = new QSlider(Qt::Horizontal, m_navigationPanel);
            volume->setFocusPolicy(Qt::NoFocus);
            volume->setRange(0, 100);
            volume->setSliderPosition(Preferences::instance().getValue("Player/volume", 100).toInt());
            volume->setMaximumWidth(100);
//	    connect(volume, SIGNAL(valueChanged(int)), this, SLOT(volumeSliderChanged(int)));
            hlayout->addWidget(volume);
            hlayout->addSpacing(20);
            m_positionSlider = new QSlider(Qt::Horizontal, m_navigationPanel);
            m_positionSlider->setTracking(false);
            m_positionSlider->setFocusPolicy(Qt::NoFocus);
            m_positionSlider->setRange(0, 1000);
            m_positionSlider->setSliderPosition(0);
//            connect(m_positionSlider, SIGNAL(valueChanged(int)), this, SLOT(positionSliderChanged(int)));
	    hlayout->addWidget(m_positionSlider);
	layout->addWidget(m_navigationPanel);

//create widget for the splitter (2)
    m_subtitlesEditorBox = new QWidget(splitter);
    layout = new QVBoxLayout(m_subtitlesEditorBox);
    layout->setSpacing(2);
    layout->setMargin(2);
    splitter->addWidget(m_subtitlesEditorBox);
    m_editorFileName = new QLabel("", m_subtitlesEditorBox);
    layout->addWidget(m_editorFileName);
    m_subtitlesEditor = new SubtitlesEditor(m_subtitlesEditorBox);
    m_subtitlesEditor->setLineWrapMode(QTextEdit::NoWrap);
//    highlighter = new QSubtitlesHighlighter(subtitlesEditor);
    layout->addWidget(m_subtitlesEditor);
    m_editorInfoLine = new QLabel("", m_subtitlesEditorBox);
    layout->addWidget(m_editorInfoLine);

//TOOL BARS
    m_fileToolBar = addToolBar(tr("File"));
//    m_stuffToolBar = addToolBar(tr("Stuff"));

//MENU
    menu = menuBar()->addMenu(tr("&File"));
    action = addMenuAction(menu, Utils::imagePath("menu/open_video.bmp"), tr("&Open video file"),
                           this, SLOT(loadVideo()), QKeySequence(Qt::CTRL + Qt::Key_O), QVariant(0));
    m_fileToolBar->addAction(action);
    action = addMenuAction(menu, Utils::imagePath("menu/close_video.bmp"), tr("&Close video file"),
                           this, SLOT(closeVideo()), QKeySequence(Qt::Key_F4 + Qt::CTRL), QVariant(0));
    m_fileToolBar->addAction(action);
//    menu->addSeparator();
//    action = addMenuAction(menu, imagePath("menu/open_sub.bmp"), tr("Open &subtitles"), this, SLOT(loadSubtitles()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O), QVariant(0));
//    m_fileToolBar->addAction(action);
//    action = addMenuAction(menu, imagePath("menu/save_sub.bmp"), tr("Save subtitles"), this, SLOT(saveSubtitles()), QKeySequence(Qt::CTRL + Qt::Key_S), QVariant(0));
//    m_fileToolBar->addAction(action);
//    action = addMenuAction(menu, imagePath("menu/save_sub.bmp"), tr("Save subtitles as..."), this, SLOT(saveSubtitlesAs()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), QVariant(0));
//    action = addMenuAction(menu, imagePath("menu/close_sub.bmp"), tr("Close subtitles"), this, SLOT(closeSubtitles()), QKeySequence(Qt::Key_F4 + Qt::SHIFT + Qt::CTRL), QVariant(0));
//    m_fileToolBar->addAction(action);
    menu->addSeparator();
    action = addMenuAction(menu, Utils::imagePath("menu/exit.bmp"), tr("&Exit"), this, SLOT(quit()), QKeySequence(Qt::Key_F4 + Qt::ALT), QVariant(0));

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

//    m_subtitlesConvertersMenu = menuBar()->addMenu(tr("Subtitles converters"));
//
//    menu = menuBar()->addMenu(tr("&Editor"));
//    action = addMenuAction(menu, imagePath("menu/editor.bmp"), tr("Show/hide subtitles &editor"), this, SLOT(toggleEditorVisibility()), QKeySequence(Qt::CTRL + Qt::Key_E), QVariant(0));
//    m_stuffToolBar->addAction(action);
//    menu->addSeparator();
//    QMenu *encodingSubMenu = menu->addMenu(tr("En&coding"));
//	group = new QActionGroup(encodingSubMenu);
////	for(int i=0; Encodings[i].comment != NULL; i++) {
////	    action = add_menu_action(encodingSubMenu, NULL, tr(Encodings[i].comment), this, SLOT(changeEncoding()), 0, QVariant(Encodings[i].name), group);
////	    action->setChecked(Encodings[i].name == pref.getValue("editor/encoding", "default").toByteArray());
////	}
//    m_endLineSubMenu = menu->addMenu(tr("End line &mark"));
//	group = new QActionGroup(m_endLineSubMenu);
//	action = addMenuAction(m_endLineSubMenu, NULL, "Unix", this, SLOT(changeEndLine()), 0, QVariant(0), group);
//	action->setChecked(Preferences::instance().getValue("editor/end_line_mark", 0) == 0);
//	action = addMenuAction(m_endLineSubMenu, NULL, "Windows", this, SLOT(changeEndLine()), 0, QVariant(1), group);
//	action->setChecked(Preferences::instance().getValue("editor/end_line_mark", 0) == 1);
//	action = addMenuAction(m_endLineSubMenu, NULL, "Mac", this, SLOT(changeEndLine()), 0, QVariant(2), group);
//	action->setChecked(Preferences::instance().getValue("editor/end_line_mark", 0) == 2);
//    menu->addSeparator();
//
////	incorrectTimeHighlighter = add_menu_action(subMenu, NULL, tr("Mark as red lines with incorrect time format"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
////	incorrectTimeHighlighter->setCheckable(true);
////	incorrectTimeHighlighter->setChecked(pref.getValue("editor/highlight_time_format", true).toBool());
////	sublinesHighlighter = add_menu_action(subMenu, NULL, tr("Mark as ping lines with to many sublines"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
////	sublinesHighlighter->setCheckable(true);
////	sublinesHighlighter->setChecked(pref.getValue("editor/highlight_sublines", true).toBool());
////	charsHighlighter = add_menu_action(subMenu, NULL, tr("Mark as ping sublines with to many characters"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
////	charsHighlighter->setCheckable(true);
////	charsHighlighter->setChecked(pref.getValue("editor/highlight_chars", true).toBool());
////	grayHighlighter = add_menu_action(subMenu, NULL, tr("Mark as gray short-displayed lines"), this, SLOT(saveMenuOptions()), 0, QVariant(0));
////	grayHighlighter->setCheckable(true);
////	grayHighlighter->setChecked(pref.getValue("editor/highlight_gray", true).toBool());
//    addMenuAction(menu, imagePath("menu/shortcuts.bmp"), tr("Load &shortcuts"), m_subtitlesEditor, SLOT(loadShortCuts()), 0, QVariant(1));
////
//    menu = menuBar()->addMenu(tr("&Options"));
//	action = addMenuAction(menu, imagePath("menu/pref.bmp"), tr("&Preferences"), optionsWindow, SLOT(exec()), QKeySequence(Qt::Key_U + Qt::CTRL), QVariant(0));
//	m_stuffToolBar->addAction(action);
//
//display main window
    QDesktopWidget *desktop = QApplication::desktop();
    int x = Preferences::instance().getValue("Window/x", desktop->availableGeometry().x()).toInt();
    int y = Preferences::instance().getValue("Window/y", desktop->availableGeometry().y()).toInt();
    int w = Preferences::instance().getValue("Window/width", desktop->availableGeometry().width()).toInt();
    int h = Preferences::instance().getValue("Window/height", desktop->availableGeometry().height()).toInt();
    if (x > desktop->availableGeometry().width() + desktop->availableGeometry().x())
        x = desktop->availableGeometry().x();
    if (y > desktop->availableGeometry().height() + desktop->availableGeometry().y())
        y = desktop->availableGeometry().y();
//    if (x + w > desktop->width()) { w = desktop->availableGeometry().width(); x = desktop->availableGeometry().x(); }
//    if (y + h > desktop->height()) { h = desktop->availableGeometry().height(); desktop->availableGeometry().y(); }
    setGeometry(x, y, w, h);
    setWindowTitle(tr("Fire Plyaer"));
    hideEditor();

//register player
    PlayerManager::instance().registerPlayer(m_videoArea);

//    PlayerFactory::setMainWindow(this);
//    PlayerFactory::setSubtitlesEditor(m_subtitlesEditor);
//    PlayerFactory::setVideoWidget(m_videoArea);
//    connect(m_subtitlesEditor, SIGNAL(fileNameTextChanged(const QString&)), this, SLOT(changeEditorFileName(const QString &)));
//    connect(m_subtitlesEditor, SIGNAL(infoLineTextChanged(const QString&)), this, SLOT(changeEditorInfoLine(const QString&)));
//    connect(m_videoArea, SIGNAL(fullscreen(bool)), this, SLOT(fullscreen(bool)));
//    connect(this, SIGNAL(loadingSubtitles(const QString&)), m_subtitlesEditor, SLOT(loadSubtitles(const QString&)));
//    connect(this, SIGNAL(closingSubtitles()), m_subtitlesEditor, SLOT(closeSubtitles()));
//    connect(this, SIGNAL(savingSubtitles()), m_subtitlesEditor, SLOT(saveSubtitles()));
//    connect(this, SIGNAL(savingSubtitlesAs(const QString&)), m_subtitlesEditor, SLOT(saveSubtitlesAs(const QString&)));
//    updatePlayerState();
    if (Preferences::instance().getValue("Window/maximized", false).toBool())
        showMaximized();
    else
        show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::addNavigationButton(const QString &iconFile, const char* member, QHBoxLayout *layout)
{
    QToolButton *button = new QToolButton(m_navigationPanel);
    button->setIcon(QIcon(Utils::imagePath(iconFile)));
    button->setFocusPolicy(Qt::NoFocus);
    connect(button, SIGNAL(clicked()), this, member);
    layout->addWidget(button);
}

QAction* MainWindow::addMenuAction(QMenu *menu, const QString &iconFile, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut, const QVariant &data, QActionGroup *group, bool showIcon)
{
    QAction *action;
    if (!iconFile.isEmpty()) {
        QIcon icon(iconFile);
        action = menu->addAction(icon, text, receiver, member, shortcut);
        action->setIconVisibleInMenu(showIcon);
    }
    else {
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
    if (/*!m_videoArea->isFullScreen() &&*/ !isMaximized()) {
        Preferences::instance().setValue("Window/height", e->size().height(), false);
        Preferences::instance().setValue("Window/width", e->size().width(), false);
    }
    Preferences::instance().setValue("Window/maximized", isMaximized(), false);
    QMainWindow::resizeEvent(e);
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    if (/*!m_videoArea->isFullScreen() &&*/ !isMaximized()) {
        Preferences::instance().setValue("Window/x", e->pos().x(), false);
        Preferences::instance().setValue("Window/y", e->pos().y(), false);
    }
    QMainWindow::moveEvent(e);
}

void MainWindow::showEditor()
{
    m_subtitlesEditorBox->show();
    m_subtitlesEditor->setFocus();
}

void MainWindow::hideEditor()
{
    m_subtitlesEditor->clearFocus();
    m_subtitlesEditorBox->hide();
}

void MainWindow::loadVideo()
{
    QString filename = QFileDialog::getOpenFileName(NULL, tr("Select movie"), Preferences::instance().getValue("last_dir", QString(getenv("HOME"))).toString(), tr("Video files *.avi"));
    if (filename.isEmpty() || !QFile::exists(filename)) {
        qDebug() << "no file selected, exit";
        return;
    }
    Preferences::instance().setLastDir(filename);

    PlayerManager::instance().getPlayer(m_videoArea)->open(filename);


//    filename = filename.left(filename.lastIndexOf(".")) + "txt";
//    if (QFile::exists(filename)) {
//        emit loadingSubtitles(filename);
//    }
//    updatePlayerState();
}

void MainWindow::closeVideo()
{

}

void MainWindow::play()
{

}

void MainWindow::stop()
{

}

void MainWindow::pause()
{

}
