#ifndef WORD_STORAGE_H
#define WORD_STORAGE_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

// Structure to hold data for a single word entry.
struct WordEntry {
    QString word;
    QString definition;
    QStringList synonyms;
    QStringList antonyms;
    QString background;
    QString usage;
    QString translation; 

    QJsonObject toJson() const {
        QJsonObject o;
        o["word"] = word;
        o["definition"] = definition;
        QJsonArray syn; for (const auto &s : synonyms) syn.append(s);
        QJsonArray ant; for (const auto &a : antonyms) ant.append(a);
        o["synonyms"] = syn;
        o["antonyms"] = ant;
        o["background"] = background;
        o["usage"] = usage;
        o["translation"] = translation; // save translation
        return o;
    }

    static QStringList jsonArrayToStringList(const QJsonArray &arr) {
        QStringList out;
        out.reserve(arr.size());
        for (const QJsonValue &v : arr) out.append(v.toString());
        return out;
    }

    static WordEntry fromJson(const QJsonObject &o) {
        WordEntry e;
        e.word = o.value("word").toString();
        e.definition = o.value("definition").toString();
        e.synonyms = jsonArrayToStringList(o.value("synonyms").toArray());
        e.antonyms = jsonArrayToStringList(o.value("antonyms").toArray());
        e.background = o.value("background").toString();
        e.usage = o.value("usage").toString();
        e.translation = o.value("translation").toString();
        return e;
    }
};

// Singleton class for managing the dictionary's word storage.
class WordStorage {
public:
    static WordStorage &instance();

    bool load(const QString &path = QString("words.json"));
    bool save(const QString &path = QString());

    void addWord(const WordEntry &entry);
    QVector<WordEntry> allWords() const;
    QVector<WordEntry> wordsForLetter(QChar letter) const;
    bool empty() const { return m_words.isEmpty(); }
    void insertInitialWords();

private:
    WordStorage() = default;
    QVector<WordEntry> m_words;
    QString m_path;
};

#endif // WORD_STORAGE_H
