/*
 * File:   ffavdictionary.cpp
 * Author: Lukasz Marek
 *
 * Created on May 4, 2013, 7:42 PM
 */

#include "ffavdictionary.h"
#include <QString>

FFAVDictionary::FFAVDictionary(struct AVDictionary **dict) :
    m_dict(NULL)
{
    //Takeover external pointer
    if (dict) {
        m_dict = *dict;
        *dict = NULL;
    }
}

bool FFAVDictionary::insert(const QString &key, const QString &value)
{
    return av_dict_set(&m_dict, key.toUtf8().constData(), value.toUtf8().constData(), 0) >= 0;
}

bool FFAVDictionary::insert(const DictionaryMap &map)
{
    DictionaryMap::const_iterator it;
    for (it = map.begin(); it != map.end(); ++it) {
        if (av_dict_set(&m_dict, it.key().toUtf8().constData(), it.value().toUtf8().constData(), 0) < 0)
            return false;
    }
    return true;
}

QDebug operator<<(QDebug out, const FFAVDictionary &d)
{
    AVDictionaryEntry *t = NULL;

    out.nospace();
    while ((t = av_dict_get(d.get(), "", t, AV_DICT_IGNORE_SUFFIX)))
        out << "\"" << t->key << "\" = \"" << t->value << "\", ";

    return out.maybeSpace();
}
