// usercreationwindow.cpp
#include "usercreationwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

UserCreationWindow::UserCreationWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Créer un utilisateur");

    prenomLineEdit = new QLineEdit(this);
    prenomLineEdit->setPlaceholderText("Prénom");

    nomLineEdit = new QLineEdit(this);
    nomLineEdit->setPlaceholderText("Nom");

    motDepasseLineEdit = new QLineEdit(this);
    motDepasseLineEdit->setPlaceholderText("Mot de passe");
    motDepasseLineEdit->setEchoMode(QLineEdit::Password); // Correction ici

    confirmmotDepasseLineEdit = new QLineEdit(this);
    confirmmotDepasseLineEdit->setPlaceholderText("Confirmer le mot de passe");
    confirmmotDepasseLineEdit->setEchoMode(QLineEdit::Password); // Correction ici

    createUserButton = new QPushButton("Créer", this);
    connect(createUserButton, &QPushButton::clicked, this, &UserCreationWindow::createUser);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(prenomLineEdit);
    layout->addWidget(nomLineEdit);
    layout->addWidget(motDepasseLineEdit);
    layout->addWidget(confirmmotDepasseLineEdit);
    layout->addWidget(createUserButton);

    setLayout(layout);
}


UserCreationWindow::~UserCreationWindow()
{
}

void UserCreationWindow::createUser() {
    QString prenom = prenomLineEdit->text();
    QString nom = nomLineEdit->text();
    QString motDepasse = motDepasseLineEdit->text();
    QString confirmmotDepasse = confirmmotDepasseLineEdit->text();

    // Vérifier si les mots de passe correspondent
    if (motDepasse != confirmmotDepasse) {
        QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas.");
        return;
    }

    // Appeler la méthode addUser avec les informations fournies par l'utilisateur et le profil par défaut
    QStringList profiles;
    profiles << "User";
    if (manager.addUser(prenom, nom, motDepasse, profiles)) {
        qDebug() << "Utilisateur créé avec succès.";
        accept(); // Fermer la fenêtre de création d'utilisateur si la création est réussie
    } else {
        qDebug() << "Erreur lors de la création de l'utilisateur.";
        // Vous pouvez ajouter du code pour afficher un message d'erreur à l'utilisateur
    }
}
