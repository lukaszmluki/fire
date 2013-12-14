#ifndef SRC_SUBTITLES_EDITOR_H
#define SRC_SUBTITLES_EDITOR_H

#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QString>

class ShortCut;
class QStringList;

//class QSubtitlesHighlighter : public QSyntaxHighlighter {
//public:
//    QSubtitlesHighlighter(QSubtitlesEditor* editor);
//protected:
//    virtual void	highlightBlock(const QString&);
//private:
//    QTextCharFormat 	format;
//    void		checkSpelling(const QString&, int offset);
//    QSubtitlesEditor	*editor;
//};

class SubtitlesEditor : public QTextEdit
{
    Q_OBJECT
public:
    SubtitlesEditor(QWidget *parent = 0);
    ~SubtitlesEditor();
//    SubtitlesFormat getSubtitlesFormat() const;
//    QString     getSubtitlesFileName() const;
//
//private:
//    int         getBlockStartTime(const QString &line) const;
//    int         getBlockEndTime(const QString &line) const;
//    double      getBlockStartTimeRealValue(const QString &line);
//    double      getBlockEndTimeRealValue(const QString &line);
//    double      getBlockDuration(const QString &line);
//    QString     getBlockText(const QString &line) const;
//    QStringList getBlockSublines(const QString &line) const;
//    QString     toBlockText(const SubtitlesEntry &entry);
//    double	getFps();
//    SubtitlesFormat autodetectSubtitlesFormat();
//    void 	executeShortcut();
//    void	insertStartTime();
//    void	insertEndTime();
//    void	shiftLineTimes(int shift);
//    void	seekToCurrentLine();
//    void	moveCursorToCurrentLine();
//
//    QString	     m_filename;
//    double	     m_fps;
//    int              m_lastBlockCount;
//    bool	     m_disabledSlots;
//    SubtitlesFormat  m_subtitlesFormat;
//    QList<ShortCut>  m_shortCuts;
//
//protected:
//    void        keyPressEvent(QKeyEvent *event);
//
//public slots:
//    void	setFps(double newfps);
//    void	loadShortCuts();
//    void	loadShortCuts(const QString &fileName);
//    void	applyGlobalSettings();
//    void	loadSubtitles(const QString &fileName);
//    void	saveSubtitles();
//    void	saveSubtitlesAs(const QString &fileName);
//    void	closeSubtitles();
//    void        updateContentFromSubtitlesManager();
//    void        updateContentInSubtitlesManager();
//
//private slots:
//    void	cursorPositionChanged();
//    void	modificationChanged(bool);
//    void        contentsChange(int position, int charsRemoved, int charsAdded);
//    void        selectionChanged();
//
//signals:
//    void        fileNameTextChanged(const QString &text);
//    void        infoLineTextChanged(const QString &text);

};

#endif /* SRC_SUBTITLES_EDITOR_H */
