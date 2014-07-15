/*
 * File:   ffavdictionary.h
 * Author: Lukasz Marek
 *
 * Created on May 4, 2013, 7:42 PM
 */

#ifndef SRC_FFMPEG_FFAVDICTIONARY_H
#define	SRC_FFMPEG_FFAVDICTIONARY_H

#include <QMap>
#include <QDebug>
extern "C" {
#include "libavutil/dict.h"
}

class FFAVDictionary {
    Q_DISABLE_COPY(FFAVDictionary)
public:

    typedef QMap<QString, QString> DictionaryMap;

    FFAVDictionary(struct AVDictionary **dict = NULL);

    ~FFAVDictionary()
    {
        av_dict_free(&m_dict);
    }

    AVDictionary** get()
    {
        return &m_dict;
    }

    AVDictionary* get() const
    {
        return m_dict;
    }

    bool insert(const QString &key, const QString &value);
    bool insert(const DictionaryMap &map);

private:
    struct AVDictionary *m_dict;
};

QDebug operator<<(QDebug out, const FFAVDictionary &d);

#endif /* SRC_FFMPEG_FFAVDICTIONARY_H */
