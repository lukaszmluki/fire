#include "subtitleseditor.h"
//#include <QDebug>
//#include <QFocusEvent>
//#include <QKeyEvent>
//#include <QMessageBox>
//#include <QFileDialog>
//#include <QFile>
//#include <QLabel>
//#include <QTextStream>
//#include <QTextCodec>
//#include <QAction>
//#include <QRegExp>
//#include <QInputDialog>
//#include <QMainWindow>
//#include <QString>
//#include <QTextDocument>

SubtitlesEditor::SubtitlesEditor(QWidget *parent) :
    QTextEdit(parent)
//    m_fps(0),
//    m_disabledSlots(false),
//    m_subtitlesFormat(SUBTITLES_FORMAT_INVALID)
{
//    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
//    connect(this, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
//    connect(this->document(), SIGNAL(modificationChanged(bool)), this, SLOT(modificationChanged(bool)));
//    connect(this->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(contentsChange(int, int, int)));
    //debug only
    //loadShortCuts("./test/shoutcut.txt");
    //loadSubtitles("./test/mdvd.txt");
    //Q_ASSERT(false);
}

SubtitlesEditor::~SubtitlesEditor()
{
}

//SubtitlesFormat QSubtitlesEditor::getSubtitlesFormat() const
//{
//    return m_subtitlesFormat;
//}
//
//QString QSubtitlesEditor::getSubtitlesFileName() const
//{
//    return m_filename;
//}
//
//void QSubtitlesEditor::keyPressEvent(QKeyEvent *event)
//{
//    if (event->modifiers() == Qt::KeypadModifier) {
//        switch (event->key()) {
//            case Qt::Key_Insert:
//                insertStartTime();
//                return;
//            case Qt::Key_Delete:
//                insertEndTime();
//                return;
//            case Qt::Key_Plus:
//                shiftLineTimes(1);
//                return;
//            case Qt::Key_Minus:
//                shiftLineTimes(-1);
//                return;
//            default:
//                break;
//        }
//    }
//    else if (event->modifiers() == Qt::ControlModifier) {
//	switch (event->key()) {
//            case Qt::Key_Space:
//		executeShortcut();
//		return;
//	    case Qt::Key_P:
//		seekToCurrentLine();
//		return;
//	    case Qt::Key_J:
//		moveCursorToCurrentLine();
//		return;
//	    case Qt::Key_L:
//                if (!PlayerFactory::instance().isPlaying() || PlayerFactory::instance().isPaused()) {
//                    PlayerFactory::instance().play();
//                }
//                else {
//                    PlayerFactory::instance().pause();
//                }
//		return;
//	    case Qt::Key_BracketLeft:
//                PlayerFactory::instance().seekRelMs(-5000);
//		return;
//	    case Qt::Key_BracketRight:
//		PlayerFactory::instance().seekRelMs(5000);
//		return;
//	    default:
//		break;
//	}
//    }
//    QTextEdit::keyPressEvent(event);
//}
//
//int QSubtitlesEditor::getBlockStartTime(const QString &line) const
//{
//    int result = SUBTITLES_TIME_NO_VALUE;
//    QRegExp pattern;
//    pattern.setCaseSensitivity(Qt::CaseInsensitive);
//    pattern.setMinimal(true);
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//            pattern.setPattern("^\\{(-?\\d+)\\}");
//            if (0 != pattern.indexIn(line, 0) || pattern.capturedTexts().size() != 2) {
//                qDebug() << "getLineStartTime(): no match";
//                break;
//            }
//            result = pattern.capturedTexts().at(1).toInt();
//            break;
//        case SUBTITLES_FORMAT_MPL2:
//            pattern.setPattern("^\\[(-?\\d+)\\]");
//            if (0 != pattern.indexIn(line, 0) || pattern.capturedTexts().size() != 2) {
//                qDebug() << "getLineStartTime(): no match";
//                break;
//            }
//            result = pattern.capturedTexts().at(1).toInt();
//            break;
//        default:
//            break;
//    }
//    return result;
//}
//
//int QSubtitlesEditor::getBlockEndTime(const QString &line) const
//{
//    int result = SUBTITLES_TIME_NO_VALUE;
//    QRegExp pattern;
//    pattern.setCaseSensitivity(Qt::CaseInsensitive);
//    pattern.setMinimal(true);
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//            pattern.setPattern("^\\{-?\\d*\\}\\s*\\{(-?\\d+)\\}");
//            if (0 != pattern.indexIn(line, 0) || pattern.capturedTexts().size() != 2) {
//                qDebug() << "getLineEndTime(): no match";
//                break;
//            }
//            result = pattern.capturedTexts().at(1).toInt();
//            break;
//        case SUBTITLES_FORMAT_MPL2:
//            pattern.setPattern("^\\[-?\\d*\\]\\s*\\[(-?\\d+)\\]");
//            if (0 != pattern.indexIn(line, 0) || pattern.capturedTexts().size() != 2) {
//                qDebug() << "getLineEndTime(): no match";
//                break;
//            }
//            result = pattern.capturedTexts().at(1).toInt();
//            break;
//        default:
//            break;
//    }
//    return result;
//}
//
//double QSubtitlesEditor::getBlockStartTimeRealValue(const QString &line)
//{
//    double result = getBlockStartTime(line);
//    if (result == SUBTITLES_TIME_NO_VALUE) {
//        return SUBTITLES_TIME_NO_VALUE;
//    }
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//            result /= getFps();
//            break;
//        case SUBTITLES_FORMAT_MPL2:
//            result /= 10.0;
//            break;
//        default:
//            break;
//    }
//    return result;
//}
//
//double QSubtitlesEditor::getBlockEndTimeRealValue(const QString &line)
//{
//    double result = getBlockEndTime(line);
//    if (result == SUBTITLES_TIME_NO_VALUE) {
//        return SUBTITLES_TIME_NO_VALUE;
//    }
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//            result /= getFps();
//            break;
//        case SUBTITLES_FORMAT_MPL2:
//            result /= 10.0;
//            break;
//        default:
//            break;
//    }
//    return result;
//}
//
//double QSubtitlesEditor::getBlockDuration(const QString &line)
//{
//    double start = getBlockStartTimeRealValue(line);
//    double end = getBlockEndTimeRealValue(line);
//    if (start == SUBTITLES_TIME_NO_VALUE || end == SUBTITLES_TIME_NO_VALUE) {
//        return SUBTITLES_TIME_NO_VALUE;
//    }
//    return getBlockEndTimeRealValue(line) - getBlockStartTimeRealValue(line);
//}
//
//QString QSubtitlesEditor::getBlockText(const QString &line) const
//{
//    //qDebug() << "QSubtitlesEditor::getLineText" << line;
//    QRegExp pattern;
//    QString result = line;
//    pattern.setCaseSensitivity(Qt::CaseInsensitive);
//    pattern.setMinimal(false);
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//            pattern.setPattern("^(\\{-?\\d*\\}\\s*){0,2}");
//            result.replace(pattern, "");
//            break;
//        case SUBTITLES_FORMAT_MPL2:
//            pattern.setPattern("^(\\[-?\\d*\\]\\s*){0,2}");
//            result.replace(pattern, "");
//            break;
//        default:
//            break;
//    }
//    return result;
//}
//
//QStringList QSubtitlesEditor::getBlockSublines(const QString &line) const
//{
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//        case SUBTITLES_FORMAT_MPL2:
//            return getBlockText(line).split("|");
//        default:
//            break;
//    }
//    return QStringList(line);
//}
//
//
//double QSubtitlesEditor::getFps()
//{
//    if (m_fps <= 0) {
//	m_fps = QInputDialog::getDouble(this, tr("Default fps"), tr("Video file is not loaded.\nYou have to input fps value."), 23.976, 15.0, 30.0, 3);
//        Subtitles::instance().setFps(m_fps);
//    }
//    return m_fps;
//}
//
////TODO: maybe more advanced detection
//SubtitlesFormat QSubtitlesEditor::autodetectSubtitlesFormat()
//{
//    if (document()->blockCount() == 0) {
//        return SUBTITLES_FORMAT_INVALID;
//    }
//    QString first = document()->firstBlock().text();
//    if (first.length() < 1) {
//        return SUBTITLES_FORMAT_INVALID;
//    }
//    if (first.at(0) == '{') {
//	return SUBTITLES_FORMAT_MDVD;
//    }
//    else if (first.at(0) == '[') {
//	return SUBTITLES_FORMAT_MPL2;
//    }
//    return SUBTITLES_FORMAT_INVALID;
//}
//
//void QSubtitlesEditor::executeShortcut()
//{
//    qDebug() << "QSubtitlesEditor::executeShortcut()";
//    QTextCursor cursor = textCursor();
//    QString text = cursor.block().text();
//    int position = cursor.position() - cursor.block().position();
//    if (cursor.atBlockEnd() || (position < text.length() && text.at(position) == ' ')) {
//        cursor.select(QTextCursor::WordUnderCursor);
//        QString shortcut = cursor.selectedText();
//        if (shortcut.isEmpty()) {
//            return;
//        }
//        //TODO: shortcut list should not be long, but maybe use some better algorithm
//        QList<ShortCut>::const_iterator it;
//        for (it = m_shortCuts.begin(); it != m_shortCuts.end(); ++it) {
//            if (shortcut == it->getShort()) {
//                qDebug() << "replace" << it->getShort() << it->getFull();
//                cursor.insertText(it->getFull());
//                return;
//            }
//        }
//    }
//}
//
//void QSubtitlesEditor::insertStartTime() {
//    //qDebug() << "QSubtitlesEditor::insertStartTime()";
//    QTextCursor cursor = textCursor();
//    QString line = textCursor().block().text();
//    int startTime = getBlockStartTime(line);
//    int endTime = getBlockEndTime(line);
//    QString text = getBlockText(line);
//
//    //Save relative cursor position.
//    //Text is inserted at the beginning so save distance from the end.
//    int offsetFromEnd = cursor.block().length() - (cursor.position() - cursor.block().position());
//    cursor.movePosition(QTextCursor::StartOfLine);
//    //TODO: try to clear it in better way
//    m_disabledSlots = true;
//    while (!cursor.atBlockEnd()) {
//        cursor.deleteChar();
//    }
//    m_disabledSlots = false;
//    double reactionTime = Preferences::instance().getValue("editor/insert_reaction", 0.4).toDouble();
//    if (m_subtitlesFormat == SUBTITLES_FORMAT_MPL2) {
//	int newStartTime = PlayerFactory::instance().positionMs() / 100.0 + 0.5 - reactionTime * 10.0;
//	if (newStartTime < 0) {
//	    newStartTime = 0;
//        }
//	if (endTime > 0) {
//	    if (Preferences::instance().getValue("editor/keep_distance", true).toBool()) {
//		textCursor().insertText("[" + QString::number(newStartTime) + "][" + QString::number(newStartTime + (endTime - startTime)) + "]" + text);
//            }
//	    else {
//		textCursor().insertText("[" + QString::number(newStartTime) + "][" + QString::number(endTime) + "]" + text);
//            }
//	} else {
//	    textCursor().insertText("[" + QString::number(newStartTime) + "][]" + text);
//        }
//    }
//    else if (m_subtitlesFormat == SUBTITLES_FORMAT_MDVD || m_subtitlesFormat == SUBTITLES_FORMAT_INVALID) {
//	int newStartTime = (PlayerFactory::instance().positionMs() / 1000.0 - reactionTime) * getFps() + 0.5;
//	if (newStartTime < 0) {
//	    newStartTime = 0;
//        }
//	if (endTime > 0) {
//	    if (Preferences::instance().getValue("editor/keep_distance", true).toBool()) {
//		cursor.insertText("{" + QString::number(newStartTime) + "}{" + QString::number(newStartTime + (endTime - startTime)) + "}" + text);
//            }
//	    else {
//		cursor.insertText("{" + QString::number(newStartTime) + "}{" + QString::number(endTime) + "}" + text);
//            }
//	}
//        else {
//	    cursor.insertText("{" + QString::number(newStartTime) + "}{}" + text);
//        }
//    }
//    //Move cursor to its previous position.
//    --offsetFromEnd;
//    cursor.setPosition(cursor.position() - offsetFromEnd);
//    setTextCursor(cursor);
//    modificationChanged(document()->isModified());
//}
//
//void QSubtitlesEditor::insertEndTime()
//{
//    //qDebug() << "QSubtitlesEditor::insertStartTime()";
//    QTextCursor cursor = textCursor();
//    QString line = textCursor().block().text();
//    int startTime = getBlockStartTime(line);
//    QString text = getBlockText(line);
//
//    //Save relative cursor position.
//    //Text is inserted at the beginning so save distance from the end.
//    int offsetFromEnd = cursor.block().length() - (cursor.position() - cursor.block().position());
//    cursor.movePosition(QTextCursor::StartOfLine);
//    //TODO: try to clear it in better way
//    m_disabledSlots = true;
//    while (!cursor.atBlockEnd()) {
//        cursor.deleteChar();
//    }
//    m_disabledSlots = false;
//
//    if (m_subtitlesFormat == SUBTITLES_FORMAT_MPL2) {
//	int newEndTime = PlayerFactory::instance().positionMs() / 100.0 + 0.5;
//	cursor.insertText("[" + QString::number(startTime)  + "][" + QString::number(newEndTime) + "]" + text);
//    }
//    if (m_subtitlesFormat == SUBTITLES_FORMAT_MDVD || m_subtitlesFormat == SUBTITLES_FORMAT_INVALID) {
//	int newEndTime = PlayerFactory::instance().positionFrame();
//	cursor.insertText("{" + QString::number(startTime)  + "}{" + QString::number(newEndTime) + "}" + text);
//    }
//    //Move cursor to its previous position.
//    --offsetFromEnd;
//    cursor.setPosition(cursor.position() - offsetFromEnd);
//    setTextCursor(cursor);
//    modificationChanged(document()->isModified());
//}
//
//void QSubtitlesEditor::shiftLineTimes(int shift)
//{
//    //qDebug() << "QSubtitlesEditor::insertStartTime()";
//    QTextCursor cursor = textCursor();
//    QString line = textCursor().block().text();
//    int startTime = getBlockStartTime(line);
//    int endTime = getBlockEndTime(line);
//    QString text = getBlockText(line);
//
//    //Save relative cursor position.
//    //Text is inserted at the beginning so save distance from the end.
//    int offsetFromEnd = cursor.block().length() - (cursor.position() - cursor.block().position());
//    cursor.movePosition(QTextCursor::StartOfLine);
//    //TODO: try to clear it in better way
//    m_disabledSlots = true;
//    while (!cursor.atBlockEnd()) {
//        cursor.deleteChar();
//    }
//    m_disabledSlots = false;
//
//    if (m_subtitlesFormat == SUBTITLES_FORMAT_MPL2) {
//	cursor.insertText("[" + QString::number(startTime + shift)  + "][" + QString::number(endTime + shift) + "]" + text);
//    }
//    else if (m_subtitlesFormat == SUBTITLES_FORMAT_MDVD) {
//	cursor.insertText("{" + QString::number(startTime + shift)  + "}{" + QString::number(endTime + shift) + "}" + text);
//    }
//    //Move cursor to its previous position.
//    --offsetFromEnd;
//    cursor.setPosition(cursor.position() - offsetFromEnd);
//    setTextCursor(cursor);
//    modificationChanged(document()->isModified());
//}
//
//void QSubtitlesEditor::seekToCurrentLine()
//{
//    if (!PlayerFactory::instance().isVideoFileLoaded()) {
//        return;
//    }
//    QString line = textCursor().block().text();
//    saveSubtitles();
//    if (PlayerFactory::instance().isPaused() || !PlayerFactory::instance().isStopped()) {
//	PlayerFactory::instance().play();
//    }
//    double time = getBlockStartTime(line);
//    double shift = Preferences::instance().getValue("editor/seek_shift", -0.5).toDouble() * 1000.0;
//    if (m_subtitlesFormat == SUBTITLES_FORMAT_MPL2) {
//	time = time * 100.0 + shift;
//    }
//    else if (m_subtitlesFormat == SUBTITLES_FORMAT_MDVD) {
//	time = 1000.0 * time / getFps() + shift;
//    }
//    else {
//	time = 0;
//    }
//    if (time < 0) {
//	time = 0;
//    }
//    PlayerFactory::instance().seekMs(time);
//}
//
//void QSubtitlesEditor::moveCursorToCurrentLine()
//{
//    int start = 0;
//    int end = 0;
//    int find = 0;
//
//    if (m_subtitlesFormat == SUBTITLES_FORMAT_MPL2) {
//	find = (PlayerFactory::instance().positionMs() + 50) / 100;
//    }
//    else if (m_subtitlesFormat == SUBTITLES_FORMAT_MDVD) {
//	find = PlayerFactory::instance().positionFrame();
//    }
//
//    QTextBlock block = document()->begin();
//    while(block.isValid()) {
//	start = getBlockStartTime(block.text());
//        end = getBlockEndTime(block.text());
//	if (start > find || (start <= find && end >= find)) {
//	    break;
//        }
//	block = block.next();
//    }
//    if (start < 0) {
//	return;
//    }
//    QTextCursor cursor = textCursor();
//    cursor.setPosition(block.position());
//    setTextCursor(cursor);
//    ensureCursorVisible();
//}
//
////******************************
////*        public slots        *
////******************************
//
//void QSubtitlesEditor::setFps(double newfps)
//{
//    m_fps = newfps;
//}
//
//void QSubtitlesEditor::loadShortCuts()
//{
//    loadShortCuts(QFileDialog::getOpenFileName(NULL, tr("Open file with shortcuts"), Preferences::instance().getValue("last_dir", getenv("HOME")).toString(), "Files with shortcuts *.txt"));
//}
//
//void QSubtitlesEditor::loadShortCuts(const QString &fileName)
//{
//    if (!QFile::exists(fileName)) {
//	return;
//    }
//    m_shortCuts.clear();
//    QFile file(fileName);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream stream(&file);
//	if (Preferences::instance().getValue("editor/encoding", "default") == "default") {
//	    stream.setCodec(QTextCodec::codecForLocale());
//        }
//	else {
//	    stream.setCodec(Preferences::instance().getValue("editor/encoding", "utf-8").toByteArray().constData());
//        }
//	while(!stream.atEnd()) {
//	    m_shortCuts.append(ShortCut(stream.readLine()));
//        }
//    }
//    file.close();
//}
//
//void QSubtitlesEditor::applyGlobalSettings()
//{
//    QTextCursor cur = textCursor();
//    int pos = cur.position();
//    //setCurrentFont(Preferences::instance().getDefaultEditorFont());
//    setPlainText(toPlainText());
//    cur.setPosition(pos);
//    setTextCursor(cur);
//}
//
//void QSubtitlesEditor::loadSubtitles(const QString &fileName)  {
//    if (!QFile::exists(fileName)) {
//        qDebug() << "QSubtitlesEditor::loadSubtitles(): Subtitles file doesnt exist" << fileName;
//	return;
//    }
//    Preferences::instance().saveLastDir(fileName);
//    closeSubtitles();
//    m_filename = fileName;
//    QFile file(m_filename);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream stream(&file);
//	if (Preferences::instance().getValue("editor/encoding", "default") == "default") {
//	    stream.setCodec(QTextCodec::codecForLocale());
//        }
//	else {
//	    stream.setCodec(Preferences::instance().getValue("editor/encoding", "utf-8").toByteArray().constData());
//        }
//        setPlainText(stream.readAll());
//    }
//    file.close();
//    m_subtitlesFormat = autodetectSubtitlesFormat();
//    document()->setModified(false);
//    modificationChanged(false);
//}
//
//void QSubtitlesEditor::saveSubtitles()
//{
//    //save subtitles to file
//    if (m_filename.isEmpty()) {
//	m_filename = QFileDialog::getSaveFileName(NULL, tr("Save subtitles"), Preferences::instance().getValue("last_dir", getenv("HOME")).toString(), "Subtitles files *.txt");
//    }
//    if (m_filename.isEmpty()) {
//	return;
//    }
//    QFile file(m_filename);
//    if (file.open(QIODevice::WriteOnly)) {
//        QTextStream stream(&file);
//	if (Preferences::instance().getValue("editor/encoding", "default") == "default") {
//	    stream.setCodec(QTextCodec::codecForLocale());
//        }
//	else {
//	    stream.setCodec(Preferences::instance().getValue("editor/encoding", "utf-8").toString().toAscii().constData());
//        }
//        QTextBlock block = document()->firstBlock();
//        while (block.isValid()) {
//            stream << block.text();
//            block = block.next();
//        }
//    }
//    file.close();
//    document()->setModified(false);
//    modificationChanged(false);
//}
//
//void QSubtitlesEditor::saveSubtitlesAs(const QString &fileName)
//{
//    if (!fileName.isEmpty()) {
//	m_filename = fileName;
//	saveSubtitles();
//    }
//}
//
//void QSubtitlesEditor::closeSubtitles()
//{
//    m_lastBlockCount = -1;
//    if (document()->isModified() && (QMessageBox::question(NULL, tr("Alert"), tr("You havn't saved your subtitles.\nDo you want to do it now?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)) {
//	saveSubtitles();
//    }
//    Subtitles::instance().setFps(0.0);
//    clear();
//    m_filename.clear();
//    modificationChanged(false);
//}
//
////******************************
////*       private slots        *
////******************************
//
//void QSubtitlesEditor::cursorPositionChanged()
//{
//    //I really hate this method (23 march 2011, 1:43AM)
//    if (m_disabledSlots) {
//	return;
//    }
//    //qDebug() << "QSubtitlesEditor::cursorPositionChanged()";
//    QTextCursor cursor = textCursor();
//    Subtitles::instance().setCursorLine(cursor.block().blockNumber());
//    QString infoline;
//    QTextStream stream(&infoline);
//    QString line(cursor.block().text());
//    int totalPosition = -1;
//    int currentSubline = -1;
//    int sublineLength;
//    int sublinePosition;
//    double time = getBlockDuration(line);
//    QStringList sublines = getBlockSublines(line);
//    int cursorPosX = cursor.columnNumber();
//    int cursorPosY = cursor.blockNumber() + 1;
//    int linesCount = document()->blockCount();
//    int sublinesCount = sublines.size();
//    int totalLength = 0;
//    for(QStringList::const_iterator it = sublines.begin(); it != sublines.end(); ++it) {
//        totalLength += it->length();
//    }
//    int textLength = getBlockText(line).length();
//    int offset = line.length() - textLength;
//    int cursorBlockPosition = cursor.positionInBlock();
//    if (cursorBlockPosition >= offset) {
//        int sublineSeparatorLength = 0;
//        if (sublinesCount > 1) {
//            sublineSeparatorLength = (textLength - totalLength) / (sublinesCount - 1);
//        }
//	currentSubline = 1;
//	//sublinePosition = 0;
//        totalPosition = 0;
//        cursorBlockPosition -= offset;
//        for(QStringList::const_iterator it = sublines.begin(); it != sublines.end(); ++it) {
//            if (cursorBlockPosition > it->length() + sublineSeparatorLength) {
//                //not this one
//                ++currentSubline;
//                totalPosition += it->length();
//                cursorBlockPosition -= (it->length() + sublineSeparatorLength);
//                if (currentSubline == sublinesCount) {
//                    //sublineSeparatorLength = 0;
//                }
//            }
//            else if (cursorBlockPosition == it->length() + sublineSeparatorLength) {
//                if (currentSubline < sublinesCount) {
//                    ++currentSubline;
//                }
//                sublinePosition = 0;
//                totalPosition += it->length();
//                break;
//            }
//            else {
//                sublinePosition = cursorBlockPosition;
//                totalPosition += cursorBlockPosition;
//                break;
//            }
//        }
//    }
//    if (currentSubline != -1) {
//	sublineLength = sublines[currentSubline-1].length();
//    }
//
//    if (totalPosition < 0) {
//        stream << "<font color=\"red\">N/A</font>";
//    }
//    else {
//        stream << QString::number(totalPosition);
//    }
//    stream << ":" << totalLength << " (" << sublinesCount << ")&nbsp;&nbsp;&nbsp;&nbsp;";
//    if (currentSubline >= 0) {
//        if (sublinePosition < 0) {
//            stream << "<font color=\"red\">N/A</font>";
//        }
//        else {
//            stream << QString::number(sublinePosition);
//        }
//	stream << ":" << sublineLength << " (" << currentSubline << ")&nbsp;&nbsp;&nbsp;&nbsp;";
//    }
//    stream << "<font color=\"blue\">t</font>";
//    if (time == SUBTITLES_TIME_NO_VALUE) {
//        stream << "<font color=\"red\">N/A</font>";
//    }
//    else {
//        stream << QString::number(time, 'g', 3) << "s";
//    }
/*
    stream << "&nbsp;&nbsp;&nbsp;&nbsp;" << \
                "<font color=\"blue\">x</font>" << \
                cursorPosX << \
                "&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"blue\">y</font>" << \
                cursorPosY << \
                ":" << \
                linesCount;
 */
//    emit infoLineTextChanged(infoline);
//    //Subtitles::instance().print();
//}
//
//void QSubtitlesEditor::modificationChanged(bool changed)
//{
//    if (m_disabledSlots) {
//	return;
//    }
//    //qDebug() << "QSubtitlesEditor::modificationChanged()";
//    if (m_filename.isEmpty()) {
//        if (changed) {
//            emit fileNameTextChanged("<pre><font color=\"red\"><i>" + tr("No file specified") + "</i></font>    " + subtitilesFormatString(m_subtitlesFormat) + "</pre>");
//        }
//        else {
//            emit fileNameTextChanged("<pre><i>" + tr("No file specified") + "</i>    " + subtitilesFormatString(m_subtitlesFormat) + "</pre>");
//        }
//    }
//    else
//    {
//        if (changed) {
//            emit fileNameTextChanged("<pre><font color=\"red\">" + m_filename + "</font>    " + subtitilesFormatString(m_subtitlesFormat) + "</pre>");
//        }
//        else {
//            emit fileNameTextChanged("<pre>" +  m_filename + "    " + subtitilesFormatString(m_subtitlesFormat) + "</pre>");
//        }
//    }
//}
//
//void QSubtitlesEditor::contentsChange(int position, int charsRemoved, int charsAdded)
//{
//    Q_UNUSED(charsRemoved);
//    Q_UNUSED(charsAdded);
//    if (m_disabledSlots) {
//	return;
//    }
//    //qDebug() << "QSubtitlesEditor::contentsChange()" << position << charsRemoved << charsAdded;
//    if (position == 0) {
//        SubtitlesFormat newFormat = autodetectSubtitlesFormat();
//        if (m_subtitlesFormat != newFormat) {
//            m_subtitlesFormat = newFormat;
//            modificationChanged(document()->isModified());
//        }
//    }
//    if (m_lastBlockCount == document()->blockCount()) {
//        QTextBlock block = document()->findBlock(position);
//        double startTime = getBlockStartTimeRealValue(block.text());
//        double endTime = getBlockEndTimeRealValue(block.text());
//        QStringList text = getBlockText(block.text()).split("|");
//        Subtitles::instance().replace(block.blockNumber(), SubtitlesEntry(text, startTime, endTime));
//    }
//    else {
//        updateContentInSubtitlesManager();
//    }
//    //Subtitles::instance().print();
//}
//
//void QSubtitlesEditor::selectionChanged()
//{
//    if (m_disabledSlots) {
//        return;
//    }
//    qDebug() << "QSubtitlesEditor::selectionChanged()";
//    QTextCursor cursor = textCursor();
//    if (cursor.hasSelection()) {
//        Subtitles::instance().setSelectedLines(qMakePair(
//            document()->findBlock(cursor.selectionStart()).blockNumber(),
//            document()->findBlock(cursor.selectionEnd()).blockNumber()));
//    }
//    else {
//        Subtitles::instance().setSelectedLines(qMakePair(-1, -1));
//    }
//    //Subtitles::instance().print();
//}
//
//
///****************************************************
// *                   HIGHLIGHTER                    *
// ***************************************************/
//
//// Highlighter constructor
////QSubtitlesHighlighter::QSubtitlesHighlighter(QSubtitlesEditor* editor) : QSyntaxHighlighter(editor), editor(editor) { }
//
////void QSubtitlesHighlighter::highlightBlock(const QString& line) {
//////    static int licznik = 0;
//////    printf("highlightBlock: uzyto: %i razy\n", ++licznik);
////    format.setFont(pref.defaultFontEditor);
////    int start, end, time;
////    QString text;
////    bool timeOK = editor->getLineParams(line, &start, &end, &time, &text);
////    QStringList list = text.split('|');
////    QList<QString>::iterator i;
////
////    if ((!timeOK || start >= end) && pref.getValue("editor/highlight_time_format", true).toBool())
////	format.setForeground(QColor(255, 0, 0));
////    else if (list.count() > pref.getValue("editor/subline_count", 2).toInt() && pref.getValue("editor/highlight_sublines", true).toBool())
////	format.setForeground(QColor(255, 85, 255));
////    else {
////	int color = 0;
////	if (pref.getValue("editor/highlight_gray", true).toBool()) {
////	    int length = 0;
////	    for (i = list.begin(); i != list.end(); ++i)
////		length += (*i).length();
////	    if (length && time / length < 40)		//more than 25 chars per second of displaying
////		color = -1;
////	    else if (length && (double)time / (double)length < 100.0) {	// more then 10 chars per second
////		color = (int)((100.0 - (double)time / (double)length) * 3.5);
////	    }
////	}
////	if (color != -1)
////	    format.setForeground(QColor(color, color, color));
////	else
////	    format.setForeground(QColor(0, 230, 0));
////    }
////    setFormat(0, line.length(), format);
////    if (pref.getValue("editor/highlight_chars", true).toBool()) {
////	QBrush save = format.foreground();
////	int pos = line.length() - text.length();
////	for (i = list.begin(); i != list.end(); ++i) {
////	    if ((*i).length() > pref.getValue("editor/subline_chars", 50).toInt())
////		format.setForeground(QColor(255, 85, 255));
////	    else
////		format.setForeground(save);
////	    format.setUnderlineStyle(QTextCharFormat::NoUnderline);
////	    setFormat(pos, (*i).length(), format);
////#ifdef USE_ASPELL
////	    if (pref.getValue("editor/spellchecking", true).toBool())
////		checkSpelling((*i), pos);
////#endif
////	    pos += (*i).length() + 1;
////	}
////    }
////#ifdef USE_ASPELL
////    else {
////	if (pref.getValue("editor/spellchecking", true).toBool())
////	    checkSpelling(line, 0);
////    }
////#endif
////}
////
////#ifdef USE_ASPELL
////void QSubtitlesHighlighter::checkSpelling(const QString& line, int offset) {
////    int pos;
////    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
////    format.setUnderlineColor(QColor(255,0,0));
////    foreach(QString word, line.split(QRegExp("\\W+"), QString::SkipEmptyParts))
////	if (!checkWord(word.toUtf8().constData())) {
////	    pos = 0;
////	    while ((pos = line.indexOf(word, pos)) != -1)
////		setFormat(offset + pos++, word.length(), format);
////	}
////}
////#endif
//
//void QSubtitlesEditor::updateContentFromSubtitlesManager()
//{
//    //TODO: consider smart update according to change status
//    qDebug() << "QSubtitlesEditor::updateContentFromSubtitlesManager()";
//    m_disabledSlots = true;
//    clear();
//    QTextCursor cursor = textCursor();
//    Subtitles &instance = Subtitles::instance();
//    bool insert = false;
//    for (int i=0; i<instance.count(); ++i) {
//        SubtitlesEntry entry = instance.getSubtitlesByLineNumber(i);
//        if (entry.getStatus() != SubtitlesEntry::REMOVE) {
//            if (insert) {
//                cursor.insertBlock();
//            }
//            insert = true;
//            cursor.insertText(toBlockText(entry));
//        }
//    }
//    m_disabledSlots = false;
//    //call all slots to affect changes
//    cursorPositionChanged();
//    modificationChanged(true);
//    contentsChange(0, 0, 0);
//    selectionChanged();
//}
//
//void QSubtitlesEditor::updateContentInSubtitlesManager()
//{
//    m_lastBlockCount = document()->blockCount();
//    Subtitles::instance().clear();
//    for (int i = 0; i < m_lastBlockCount; ++i) {
//        double startTime = getBlockStartTimeRealValue(document()->findBlockByNumber(i).text());
//        double endTime = getBlockEndTimeRealValue(document()->findBlockByNumber(i).text());
//        QStringList text = getBlockText(document()->findBlockByNumber(i).text()).split("|");
//        Subtitles::instance().append(SubtitlesEntry(text, startTime, endTime));
//    }
//}
//
//QString QSubtitlesEditor::toBlockText(const SubtitlesEntry &entry)
//{
//    QString start;
//    QString end;
//    switch(m_subtitlesFormat) {
//        case SUBTITLES_FORMAT_MDVD:
//            if (SUBTITLES_TIME_NO_VALUE != entry.getShowTime()) {
//                start = QString::number(static_cast<int>(entry.getShowTime() * getFps() + 0.5), 10);
//            }
//            if (SUBTITLES_TIME_NO_VALUE != entry.getHideTime()) {
//                end = QString::number(static_cast<int>(entry.getHideTime() * getFps() + 0.5), 10);
//            }
//            return "{" + start + "}{" + end + "}" + entry.getSublines().join("|");
//        case SUBTITLES_FORMAT_MPL2:
//            if (SUBTITLES_TIME_NO_VALUE != entry.getShowTime()) {
//                start = QString::number(static_cast<int>(entry.getShowTime() * 10 + 0.5), 10);
//            }
//            if (SUBTITLES_TIME_NO_VALUE != entry.getHideTime()) {
//                end = QString::number(static_cast<int>(entry.getHideTime() * 10 + 0.5), 10);
//            }
//            return "[" + start + "][" + end + "]" + entry.getSublines().join("|");
//        default:
//            break;
//    }
//    return QString();
//}