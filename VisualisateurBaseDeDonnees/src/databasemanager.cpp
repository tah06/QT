#include "jsonmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

JSONManager::JSONManager(QObject *parent) : QObject(parent)
{
    // Définir le chemin vers le fichier JSON
    m_filePath = "users.json";
}

bool JSONManager::loadUsers(const QString &filePath)
{
    m_filePath = filePath;
    return loadFromFile();
}

bool JSONManager::addUser(const QString &firstName, const QString &lastName, const QString &password, const QString &profile)
{
    // Vérifier si l'utilisateur existe déjà
    for (const auto &user : m_usersArray) {
        QJsonObject userObj = user.toObject();
        if (userObj["nom"].toString() == firstName && userObj["prenom"].toString() == lastName) {
            qDebug() << "L'utilisateur existe déjà.";
            return false;
        }
    }

    // Créer un objet JSON pour l'utilisateur
    QJsonObject newUser;
    newUser["nom"] = firstName;
    newUser["prenom"] = lastName;
    newUser["password"] = password;
    newUser["profile"] = profile;

    // Ajouter l'utilisateur au tableau JSON
    m_usersArray.append(newUser);

    // Enregistrer les modifications dans le fichier
    return saveToFile();
}

bool JSONManager::removeUser(const QString &firstName, const QString &lastName)
{
    // Rechercher l'utilisateur dans le tableau JSON
    for (int i = 0; i < m_usersArray.size(); ++i) {
        QJsonObject userObj = m_usersArray[i].toObject();
        if (userObj["nom"].toString() == firstName && userObj["prenom"].toString() == lastName) {
            // Supprimer l'utilisateur du tableau
            m_usersArray.removeAt(i);

            // Enregistrer les modifications dans le fichier
            return saveToFile();
        }
    }

    qDebug() << "L'utilisateur n'existe pas.";
    return false;
}

bool JSONManager::updateUserPassword(const QString &firstName, const QString &lastName, const QString &newPassword)
{
    // Rechercher l'utilisateur dans le tableau JSON
    for (int i = 0; i < m_usersArray.size(); ++i) {
        QJsonObject userObj = m_usersArray[i].toObject();
        if (userObj["nom"].toString() == firstName && userObj["prenom"].toString() == lastName) {
            // Mettre à jour le mot de passe de l'utilisateur
            userObj["password"] = newPassword;
            m_usersArray[i] = userObj;

            // Enregistrer les modifications dans le fichier
            return saveToFile();
        }
    }

    qDebug() << "L'utilisateur n'existe pas.";
    return false;
}

bool JSONManager::saveToFile()
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Erreur lors de l'ouverture du fichier pour écriture:" << file.errorString();
        return false;
    }

    // Créer un document JSON à partir du tableau JSON
    QJsonDocument jsonDoc(m_usersArray);

    // Écrire le document JSON dans le fichier
    file.write(jsonDoc.toJson());

    // Fermer le fichier
    file.close();

    return true;
}

bool JSONManager::loadFromFile()
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur lors de l'ouverture du fichier pour lecture:" << file.errorString();
        return false;
    }

    // Lire tout le contenu du fichier
    QByteArray jsonData = file.readAll();

    // Créer un document JSON à partir des données lues
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // Vérifier si le document JSON est valide
    if (jsonDoc.isNull() || !jsonDoc.isArray()) {
        qDebug() << "Le fichier JSON est invalide.";
        return false;
    }

    // Récupérer le tableau JSON à partir du document
    m_usersArray = jsonDoc.array();

    // Fermer le fichier
    file.close();

    return true;
}
