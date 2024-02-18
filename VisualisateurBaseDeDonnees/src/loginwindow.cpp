#include "loginwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>


LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Login");

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Nom d'utilisateur");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Mot de passe");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Se connecter", this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::loginUser);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

LoginWindow::~LoginWindow()
{

}

bool LoginWindow::loginUser() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // Ouvrir le fichier JSON contenant les utilisateurs et leurs mots de passe
    QFile file("../VisualisateurBaseDeDonnees/users.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur: Impossible d'ouvrir le fichier JSON";
        return false;
    }

    // Lire le contenu du fichier JSON
    QByteArray jsonData = file.readAll();
    file.close();

    // Analyser le contenu JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        qDebug() << "Erreur: Format de fichier JSON invalide";
        return false;
    }

    // Parcourir le tableau JSON pour trouver l'utilisateur
    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        QJsonObject userObject = value.toObject();
        QString storedUsername = userObject.value("nom").toString(); // Nom de l'utilisateur
        QString storedPassword = userObject.value("motDepasse").toString(); // Mot de passe

        if (storedUsername == username && storedPassword == password) {
            qDebug() << "Connexion réussie pour l'utilisateur:" << username;
            emit loginSuccessful();
            return true; // Utilisateur trouvé, connexion réussie
        }
    }

    // Si l'utilisateur n'est pas trouvé dans le fichier JSON
    qDebug() << "L'utilisateur" << username << "n'existe pas ou le mot de passe est incorrect";
    QMessageBox::critical(this, "Erreur", "Nom d'utilisateur ou mot de passe incorrect !");
    return false;
}
