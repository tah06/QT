// jsonmanager.cpp
#include "jsonmanager.h"

JSONManager::JSONManager(QObject *parent)
    : QObject(parent), m_filePath("../VisualisateurBaseDeDonnees/users.json") {}

bool JSONManager::loadUsers() {
    return loadFromFile();
}

bool JSONManager::addUser(const QString &prenom, const QString &nom, const QString &motDepasse, const QStringList &profiles) {
    if (!loadFromFile()) {
        qDebug() << "Erreur lors du chargement des utilisateurs.";
        return false;
    }

    QJsonObject newUser;
    newUser["prenom"] = prenom;
    newUser["nom"] = nom;
    newUser["motDepasse"] = motDepasse;
    newUser["profile"] = QJsonArray::fromStringList(profiles);

    m_usersArray.append(newUser);

    return saveToFile();
}

bool JSONManager::removeUser(const QString &prenom, const QString &nom) {
    if (!loadFromFile()) {
        qDebug() << "Erreur lors du chargement des utilisateurs.";
        return false;
    }

    // Parcourir le tableau des utilisateurs
    for (int i = 0; i < m_usersArray.size(); ++i) {
        QJsonObject user = m_usersArray.at(i).toObject();
        // Vérifier si les prénom et nom correspondent
        if (user["prenom"].toString() == prenom && user["nom"].toString() == nom) {
            // Supprimer l'utilisateur du tableau
            m_usersArray.removeAt(i);
            // Enregistrer les modifications dans le fichier
            if (saveToFile()) {
                qDebug() << "Utilisateur supprimé avec succès.";
                return true;
            } else {
                qDebug() << "Erreur lors de la sauvegarde des modifications.";
                return false;
            }
        }
    }

    qDebug() << "Utilisateur non trouvé.";
    return false;
}


bool JSONManager::updateUser(const QString &prenom, const QString &nom, const QString &newPrenom, const QString &newNom, const QString &newmotDepasse, const QStringList &newProfiles) {
    // Ouvrir le fichier JSON contenant les utilisateurs et leurs profils
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Erreur: Impossible d'ouvrir le fichier JSON";
        return false; // Retourner false en cas d'erreur d'ouverture du fichier
    }

    // Lire le contenu du fichier JSON
    QByteArray jsonData = file.readAll();

    // Analyser le contenu JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        qDebug() << "Erreur: Format de fichier JSON invalide";
        return false; // Retourner false en cas de format JSON invalide
    }

    // Convertir les noms et prénoms en minuscules pour ignorer les différences de casse
    QString cleanedPrenom = prenom.simplified().toLower().trimmed();
    QString cleanedNom = nom.simplified().toLower().trimmed();

    // Parcourir le tableau JSON pour trouver l'utilisateur
    QJsonArray jsonArray = jsonDoc.array();
    bool userFound = false; // Indicateur pour vérifier si l'utilisateur a été trouvé

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject user = jsonArray.at(i).toObject();
        QString storedPrenom = user["prenom"].toString().simplified().toLower().trimmed();
        QString storedNom = user["nom"].toString().simplified().toLower().trimmed();
        qDebug() << "compares les noms :nom dans la form:"+cleanedNom +" et "+ storedNom;
        qDebug() << "compares les prenom : " +cleanedPrenom +" et "+ storedPrenom;
        if (storedPrenom == cleanedPrenom && storedNom == cleanedNom) {
            // Mettre à jour les informations de l'utilisateur
            user["prenom"] = newPrenom;
            user["nom"] = newNom;
            user["motDepasse"] = newmotDepasse;
            user["profile"] = QJsonArray::fromStringList(newProfiles);
            jsonArray[i] = user;
            userFound = true;
            break; // Sortir de la boucle une fois que l'utilisateur est trouvé
        }
    }

    if (!userFound) {
        qDebug() << "Utilisateur non trouvé.";
        return false; // Retourner false si l'utilisateur n'est pas trouvé
    }

    // Réécrire le fichier JSON avec les modifications
    jsonDoc.setArray(jsonArray); // Mettre à jour le document JSON
    file.seek(0); // Déplacer le curseur au début du fichier
    file.write(jsonDoc.toJson()); // Réécrire le fichier avec les modifications
    file.resize(file.pos()); // Tronquer le fichier à la taille actuelle
    file.close();

    // Mettre à jour m_usersArray et enregistrer dans le fichier
    m_usersArray = jsonArray;
    return saveToFile(); // Appeler saveToFile pour enregistrer m_usersArray dans le fichier
}



QStringList JSONManager::getUserProfiles(const QString &username) const {
    QStringList profiles;

    // Ouvrir le fichier JSON contenant les utilisateurs et leurs profils
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur: Impossible d'ouvrir le fichier JSON";
        return profiles;
    }

    // Lire le contenu du fichier JSON
    QByteArray jsonData = file.readAll();
    file.close();

    // Analyser le contenu JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        qDebug() << "Erreur: Format de fichier JSON invalide";
        return profiles;
    }

    // Parcourir le tableau JSON pour trouver l'utilisateur
    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        QJsonObject userObject = value.toObject();
        QString storedUsername = userObject.value("nom").toString(); // Nom de l'utilisateur

        if (storedUsername == username) {
            // L'utilisateur est trouvé, récupérer ses profils
            QJsonArray profilesArray = userObject.value("profile").toArray();
            for (const QJsonValue &profileValue : profilesArray) {
                profiles.append(profileValue.toString());
            }
            return profiles; // Retourner les profils de l'utilisateur trouvé
        }
    }

    // Si l'utilisateur n'est pas trouvé dans le fichier JSON
    qDebug() << "L'utilisateur" << username << "n'existe pas";
    return profiles;
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

QList<QPair<QString, QString>> JSONManager::getAllUsers(const QString &profile) const {
    QList<QPair<QString, QString>> users;

    // Ouvrir le fichier JSON contenant les utilisateurs
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur: Impossible d'ouvrir le fichier JSON";
        return users;
    }

    // Lire le contenu du fichier JSON
    QByteArray jsonData = file.readAll();
    file.close();

    // Analyser le contenu JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        qDebug() << "Erreur: Format de fichier JSON invalide";
        return users;
    }

    // Parcourir le tableau JSON pour trouver les utilisateurs
    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        QJsonObject userObject = value.toObject();
        QJsonArray userProfiles = userObject.value("profile").toArray();

        // Afficher les profils de l'utilisateur pour débogage
        qDebug() << "Profils de l'utilisateur:" << userProfiles;

        // Vérifier si l'utilisateur a le profil requis
        if (profile == "SuperUser" || (profile == "Admin" && userProfiles.contains("Admin"))) {
            QString nom = userObject.value("nom").toString();
            QString prenom = userObject.value("prenom").toString();
            users.append(qMakePair(nom, prenom));
        }
    }

    // Afficher les utilisateurs récupérés pour débogage
    qDebug() << "Utilisateurs récupérés pour le profil" << profile << ":" << users;

    return users;
}

