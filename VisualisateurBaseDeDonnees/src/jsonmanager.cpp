#include "jsonmanager.h"

JSONManager::JSONManager(QObject *parent)
    : QObject(parent), m_filePath("../VisualisateurBaseDeDonnees/users.json") {}

bool JSONManager::loadUsers() {
    if (loadFromFile()) {
        qDebug() << "Utilisateurs chargés avec succès.";
        return true;
    } else {
        qDebug() << "Erreur lors du chargement des utilisateurs.";
        return false;
    }
}

bool JSONManager::addUser(const QString &prenom, const QString &nom, const QString &motDepasse, const QString &profile) {
    // Charger d'abord les utilisateurs existants
    if (!loadFromFile()) {
        qDebug() << "Erreur lors du chargement des utilisateurs.";
        return false;
    }

    // Créer un objet JSON pour le nouvel utilisateur
    QJsonObject userObject;
    userObject["nom"] = nom;
    userObject["prenom"] = prenom;
    userObject["motDepasse"] = motDepasse;
    userObject["profile"] = profile;

    // Ajouter le nouvel utilisateur à la fin de la liste
    m_usersArray.append(userObject);

    // Sauvegarder le fichier avec le nouvel utilisateur ajouté
    if (saveToFile()) {
        qDebug() << "Utilisateur ajouté avec succès.";

        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de l'utilisateur.";
        return false;
    }
}



bool JSONManager::removeUser(const QString &prenom, const QString &nom) {
    for (int i = 0; i < m_usersArray.size(); ++i) {
        QJsonObject userObject = m_usersArray.at(i).toObject();
        if (userObject["prenom"].toString() == prenom && userObject["nom"].toString() == nom) {
            m_usersArray.removeAt(i);
            return saveToFile();
        }
    }
    return false; // L'utilisateur n'a pas été trouvé
}

bool JSONManager::updateUsermotDepasse(const QString &prenom, const QString &nom, const QString &newmotDepasse) {
    for (int i = 0; i < m_usersArray.size(); ++i) {
        QJsonObject userObject = m_usersArray.at(i).toObject();
        if (userObject["prenom"].toString() == prenom && userObject["nom"].toString() == nom) {
            userObject["motDepasse"] = newmotDepasse;
            m_usersArray[i] = userObject;
            return saveToFile();
        }
    }
    return false; // L'utilisateur n'a pas été trouvé
}

bool JSONManager::saveToFile() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Erreur lors de l'ouverture du fichier pour l'écriture.";
        return false;
    }
    QJsonDocument jsonDoc(m_usersArray);
    file.write(jsonDoc.toJson());

    file.close();
    return true;
}

bool JSONManager::loadFromFile() {
    QFile file(m_filePath);
    if (!file.exists()) {
        qDebug() << "Le fichier" << m_filePath << "n'existe pas.";
        return false;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur lors de l'ouverture du fichier pour la lecture.";
        return false;
    }
    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    m_usersArray = jsonDoc.array();
    file.close();
    return true;
}
