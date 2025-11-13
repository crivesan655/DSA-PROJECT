#include "User_Files/UserStorage.h"
#include "User_Files/User.h" 
#include "Qt_includes.h"

UserStorage &UserStorage::instance()
{
    static UserStorage s;
    return s;
}

// Loads the list of all users from the index file
bool UserStorage::load(const QString &indexPath)
{
    m_indexPath = indexPath;
    m_userIndex.clear();
    QFile f(m_indexPath);
    if (!f.exists()) {
        // ensure users dir exists and create empty index
        QDir().mkpath("users");
        return save(m_indexPath);
    }
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isObject()) return false;
    QJsonObject root = doc.object();
    QJsonArray list = root.value("users").toArray();
    for (const auto &v : list) {
        if (!v.isObject()) continue;
        QJsonObject u = v.toObject();
        QString name = u.value("username").toString();
        if (name.isEmpty()) continue;
        m_userIndex.insert(name, u);
    }
    return true;
}

// Saves the list of all users to the index file
bool UserStorage::save(const QString &indexPath)
{
    QString path = indexPath.isEmpty() ? m_indexPath : indexPath;
    if (path.isEmpty()) return false;
    QJsonArray arr;
    for (auto it = m_userIndex.constBegin(); it != m_userIndex.constEnd(); ++it) {
        QJsonObject obj = it.value();
        if (obj.isEmpty()) obj["username"] = it.key();
        arr.append(obj);
    }
    QJsonObject root;
    root["users"] = arr;
    QJsonDocument doc(root);
    QFile f(path);
    QDir().mkpath(QFileInfo(path).absolutePath());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();
    QDir().mkpath("users");
    return true;
}

// Returns a list of all usernames
QStringList UserStorage::users() const
{
    return m_userIndex.keys();
}

// Checks if a user exists in the index
bool UserStorage::hasUser(const QString &username) const
{
    return m_userIndex.contains(username);
}

// Adds a new user by name only (minimal metadata)
bool UserStorage::addUser(const QString &username)
{
    if (username.isEmpty() || hasUser(username)) return false;
    QJsonObject meta;
    meta["username"] = username;
    m_userIndex.insert(username, meta);
    // create empty user data file
    QJsonObject data;
    data["created"] = true;
    m_userDataCache.insert(username, data);
    QDir().mkpath("users");
    saveUserData(username);
    save();
    return true;
}

// Added: Adds a new user based on the full User struct, and saves detailed data
bool UserStorage::addUser(const User &u)
{
    if (u.name.isEmpty() || hasUser(u.name)) return false;

    // 1. Update the main index with minimal metadata
    QJsonObject meta;
    meta["username"] = u.name;
    m_userIndex.insert(u.name, meta);

    // 2. Cache the detailed user data (User::toJson() handles age, lists, etc.)
    m_userDataCache.insert(u.name, u.toJson());
    
    // Ensure "users" directory exists and save the individual user file
    QDir().mkpath("users"); 
    saveUserData(u.name);
    
    // 3. Save the index file to persist the new user's existence
    save();
    return true;
}

// Removes a user and their data
bool UserStorage::removeUser(const QString &username)
{
    if (!hasUser(username)) return false;
    m_userIndex.remove(username);
    m_userDataCache.remove(username);
    save();
    QFile::remove(QString("users/%1.json").arg(username));
    if (m_currentUser == username) m_currentUser.clear();
    return true;
}

// Sets the current active user and loads their data
bool UserStorage::setCurrentUser(const QString &username)
{
    if (!hasUser(username)) return false;
    m_currentUser = username;
    return loadUserData(username);
}

// Returns the name of the currently active user
QString UserStorage::currentUser() const
{
    return m_currentUser;
}

// Loads detailed JSON data for a specific user
bool UserStorage::loadUserData(const QString &username)
{
    if (!hasUser(username)) return false;
    QString file = QString("users/%1.json").arg(username);
    QFile f(file);
    if (!f.exists()) {
        // create empty
        m_userDataCache.insert(username, QJsonObject());
        saveUserData(username);
        return true;
    }
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isObject()) return false;
    m_userDataCache.insert(username, doc.object());
    return true;
}

// Saves detailed JSON data for a specific user
bool UserStorage::saveUserData(const QString &username) const
{
    if (!m_userIndex.contains(username)) return false;
    QString file = QString("users/%1.json").arg(username);
    QJsonObject data = m_userDataCache.value(username, QJsonObject());
    QJsonDocument doc(data);
    QFile f(file);
    QDir().mkpath(QFileInfo(file).absolutePath());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();
    return true;
}

// Saves data for the currently active user
bool UserStorage::saveCurrentUserData() const
{
    if (m_currentUser.isEmpty()) return false;
    return saveUserData(m_currentUser);
}

// Returns the detailed JSON data for the current user
QJsonObject UserStorage::currentUserData() const
{
    return m_userDataCache.value(m_currentUser, QJsonObject());
}

// Updates the cached data for the current user
void UserStorage::updateCurrentUserData(const QJsonObject &data)
{
    if (m_currentUser.isEmpty()) return;
    m_userDataCache.insert(m_currentUser, data);
}