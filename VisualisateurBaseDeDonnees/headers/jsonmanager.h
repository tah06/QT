// jsonmanager.h
#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>

class JSONManager : public QObject
{
    Q_OBJECT
public:
    explicit JSONManager(QObject *parent = nullptr);

    // Fonctions pour charger, ajouter, supprimer et modifier les utilisateurs
    bool loadUsers();
    bool addUser(const QString &prenom, const QString &nom, const QString &motDepasse, const QStringList &profiles);
    bool removeUser(const QString &prenom, const QString &nom);
    bool updateUser(const QString &prenom, const QString &nom, const QString &newmotDepasse, const QStringList &newProfiles);

    QStringList getUserProfiles(const QString &username) const;
    QList<QPair<QString, QString>> getAllUsers(const QString &profile) const;

private:
    QString m_filePath;
    QJsonArray m_usersArray;

    bool saveToFile();
    bool loadFromFile();
};

#endif // JSONMANAGER_H
