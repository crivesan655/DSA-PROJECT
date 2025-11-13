#ifndef FUNCTION_H
#define FUNCTION_H

#include <QString>
#include <QVector>
#include <QPair>
#include "Word_Files/Word_Storage.h"

class Function {
private:
    QChar normalizeKey(const QString &word) const;

public:
    Function() = default;

    // Updated signatures with translation support
    int addWord(const QString &word, const QString &definition, const QString &translation = QString());

    QString searchWord(const QString &word, bool getTranslation = false) const;
    QVector<QPair<QString, QString>> getWordsByLetter(QChar letter, bool getTranslation = false) const;

    bool addWordEntry(const WordEntry &entry);
    bool addWordFromInputs(const QString &word,
                           const QString &definition,
                           const QString &translation,
                           const QString &synonymsCsv,
                           const QString &antonymsCsv,
                           const QString &background,
                           const QString &usage);
};

#endif // FUNCTION_H
