#include "Function_Files/Function.h"
#include "Word_Files/Word_Storage.h"
#include "User_Files/UserStorage.h"
#include "Qt_includes.h"

QChar Function::normalizeKey(const QString &word) const {
    if (word.isEmpty()) return QChar('\0');
    QChar c = word.at(0);
    if (!c.isLetter()) return QChar('\0');
    return c.toLower();
}

int Function::addWord(const QString &word, const QString &definition, const QString &translation) {
    if (word.trimmed().isEmpty() || definition.trimmed().isEmpty()) return 1;
    QChar key = normalizeKey(word);
    if (key == QChar('\0')) return 1;

    auto list = WordStorage::instance().wordsForLetter(key);
    for (const auto &e : list) {
        if (e.word.compare(word, Qt::CaseInsensitive) == 0) return 3; // duplicate
    }

    const int MAX_PER_LETTER = 30;
    if (list.size() >= MAX_PER_LETTER) return 2;

    WordEntry entry;
    entry.word = word.trimmed();
    entry.definition = definition.trimmed();
    entry.translation = translation.trimmed();
    WordStorage::instance().addWord(entry);
    WordStorage::instance().save();
    return 0;
}

QString Function::searchWord(const QString &word, bool getTranslation) const {
    if (word.trimmed().isEmpty()) return QString();
    auto all = WordStorage::instance().allWords();
    for (const auto &e : all) {
        if (e.word.compare(word, Qt::CaseInsensitive) == 0) {
            return getTranslation ? e.translation : e.definition;
        }
    }
    return QString();
}

QVector<QPair<QString, QString>> Function::getWordsByLetter(QChar letter, bool getTranslation) const {
    QVector<QPair<QString, QString>> out;
    auto list = WordStorage::instance().wordsForLetter(letter);
    out.reserve(list.size());
    for (const auto &e : list) {
        out.append(qMakePair(e.word, getTranslation ? e.translation : e.definition));
    }
    return out;
}

bool Function::addWordEntry(const WordEntry &entry)
{
    WordStorage::instance().addWord(entry);

    QString currentUserName = UserStorage::instance().currentUser();
    if (!currentUserName.isEmpty()) {
        QJsonObject userData = UserStorage::instance().currentUserData();
        QStringList addedWordsList;
        QJsonValue addedWordsValue = userData.value("addedWords");

        if (addedWordsValue.isArray()) {
            QJsonArray addedArray = addedWordsValue.toArray();
            for (const QJsonValue &v : addedArray) if (v.isString()) addedWordsList.append(v.toString());
        }

        if (!addedWordsList.contains(entry.word, Qt::CaseInsensitive)) {
            addedWordsList.append(entry.word);

            QJsonArray newAddedWordsArray;
            for (const QString &w : addedWordsList) newAddedWordsArray.append(w);
            userData["addedWords"] = newAddedWordsArray;

            UserStorage::instance().updateCurrentUserData(userData);
            UserStorage::instance().saveCurrentUserData();
        }
    }

    return WordStorage::instance().save();
}

bool Function::addWordFromInputs(const QString &word,
                                 const QString &definition,
                                 const QString &translation,
                                 const QString &synonymsCsv,
                                 const QString &antonymsCsv,
                                 const QString &background,
                                 const QString &usage)
{
    if (word.trimmed().isEmpty()) return false;

    WordEntry e;
    e.word = word.trimmed();
    e.definition = definition.trimmed();
    e.translation = translation.trimmed(); // <-- add Tagalog support

    auto splitCsv = [](const QString &s) -> QStringList {
        QStringList parts;
        for (const auto &p : s.split(',', Qt::SkipEmptyParts)) parts.append(p.trimmed());
        return parts;
    };

    e.synonyms = splitCsv(synonymsCsv);
    e.antonyms = splitCsv(antonymsCsv);
    e.background = background.trimmed();
    e.usage = usage.trimmed();

    return addWordEntry(e);
}
