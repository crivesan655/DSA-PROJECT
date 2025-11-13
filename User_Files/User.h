#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

// Simple POD for a user record (name + age) with JSON serialization helpers.
struct User {
    QString name;               // user's name
    int age = 0;                // user's age
    QStringList addedWords;     // words this user added
    QStringList recentSearches; // recent search terms, most-recent-first

    // Convert this user to a QJsonObject for saving.
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["name"] = name;
        obj["age"] = age;

        QJsonArray addedArr;
        for (const QString &w : addedWords) addedArr.append(w);
        obj["addedWords"] = addedArr;

        QJsonArray recentArr;
        for (const QString &s : recentSearches) recentArr.append(s);
        obj["recentSearches"] = recentArr;

        return obj;
    }

    // Construct a User from a QJsonObject loaded from disk.
    static User fromJson(const QJsonObject &obj) {
        User u;
        u.name = obj.value("name").toString();
        u.age = obj.value("age").toInt(0);

        u.addedWords.clear();
        QJsonValue addedVal = obj.value("addedWords");
        if (addedVal.isArray()) {
            QJsonArray addedArr = addedVal.toArray();
            for (const QJsonValue &v : addedArr) if (v.isString()) u.addedWords.append(v.toString());
        }

        u.recentSearches.clear();
        QJsonValue recentVal = obj.value("recentSearches");
        if (recentVal.isArray()) {
            QJsonArray recentArr = recentVal.toArray();
            for (const QJsonValue &v : recentArr) if (v.isString()) u.recentSearches.append(v.toString());
        }

        return u;
    }
};

#endif // USER_H