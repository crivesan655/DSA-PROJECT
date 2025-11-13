#ifndef USERSTORAGE_H
#define USERSTORAGE_H

#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QStringList>
#include "User_Files/User.h" 

class UserStorage {
public:
    static UserStorage &instance(); // Accesses the single instance of UserStorage

    // load / save index file (users.json)
    bool load(const QString &indexPath = QString("users.json")); // Loads the list of all users
    bool save(const QString &indexPath = QString()); // Saves the list of all users

    // index operations
    QStringList users() const; // Returns a list of all usernames
    bool hasUser(const QString &username) const; // Checks if a user exists in the index
    bool addUser(const QString &username); // Adds a new user by name only
    bool addUser(const User &user); // Added: Adds a new user based on the full User struct
    bool removeUser(const QString &username); // Removes a user and their data

    // current user
    bool setCurrentUser(const QString &username); // Sets the current active user and loads their data
    QString currentUser() const; // Returns the name of the currently active user

    // per-user data
    bool loadUserData(const QString &username); // Loads detailed JSON data for a specific user
    bool saveUserData(const QString &username) const; // Saves detailed JSON data for a specific user
    bool saveCurrentUserData() const; // Saves data for the currently active user

    QJsonObject currentUserData() const; // Returns the detailed JSON data for the current user
    void updateCurrentUserData(const QJsonObject &data); // Updates the cached data for the current user

private:
    UserStorage() = default;
    QString m_indexPath;
    QMap<QString, QJsonObject> m_userIndex;     // metadata stored in index
    QMap<QString, QJsonObject> m_userDataCache; // actual per-user data
    QString m_currentUser;
};

#endif // USERSTORAGE_H