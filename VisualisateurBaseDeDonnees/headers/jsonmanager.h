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
    bool addUser(const QString &prenom, const QString &nom, const QString &motDepasse, const QString &profile);
    bool removeUser(const QString &prenom, const QString &nom);
    bool updateUsermotDepasse(const QString &prenom, const QString &nom, const QString &newmotDepasse);

private:
    QString m_filePath; // Chemin vers le fichier JSON
    QJsonArray m_usersArray; // Tableau JSON contenant les utilisateurs

    // Fonctions privées pour la manipulation du fichier JSON
    bool saveToFile();
    bool loadFromFile();
};

#endif // JSONMANAGER_H
