#include "mainwindow.h"
#include "jsonmanager.h"
#include "usercreationwindow.h"
#include "loginwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QMenuBar>
#include <QPushButton>
#include <QOverload>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), loginWindow(nullptr), mainWidget(nullptr), disconnectButton(nullptr), jsonManager(nullptr), userTable(nullptr)
{
    setWindowTitle("Main Window");

    jsonManager = new JSONManager(this);

    // Vérifier si c'est la première fois que l'utilisateur ouvre l'application
    QSettings settings("MonEntreprise", "MonApplication");
    bool isFirstRun = settings.value("firstRun", true).toBool();
    settings.setValue("firstRun", false);
    if (isFirstRun) {
        // Afficher la fenêtre de création d'utilisateur
        settings.setValue("firstRun", false);
        showUserCreationWindow();
    } else {
        // Afficher la fenêtre de connexion
        showLoginWindow();
    }
}

MainWindow::~MainWindow()
{
    delete loginWindow;
    delete mainWidget;
    delete jsonManager;
}

void MainWindow::showLoginWindow() {
    // Afficher la fenêtre de connexion
    loginWindow = new LoginWindow();
    connect(loginWindow, &LoginWindow::loginSuccessful, this, &MainWindow::showMainPage);

    setCentralWidget(loginWindow);
    loginWindow->show();
}

void MainWindow::showMainPage() {
    qDebug() << "Connexion réussie, affichage de la page principale...";

    // Récupérer l'utilisateur actuellement connecté
    username = loginWindow->getUsername(); // Assurez-vous que username est un membre de la classe MainWindow
    QStringList profiles = jsonManager->getUserProfiles(username);
    qDebug() << "Profils de l'utilisateur:" << profiles;

    // Créer le widget principal avec son contenu
    mainWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);

    // Vérifier si le premier profil de l'utilisateur est "User"
    if (!profiles.isEmpty() && profiles.first() == "User") {
        // Afficher uniquement le message de bienvenue pour les utilisateurs "User"
        QLabel *welcomeLabel = new QLabel("Bienvenue sur la page principale");
        layout->addWidget(welcomeLabel);
    } else {
        // Créer le QLabel avec le texte incluant le profil de l'utilisateur
        QLabel *titleLabel = new QLabel("Page principale - Profil : " + username);
        layout->addWidget(titleLabel);

        // Créer le tableau des utilisateurs
        userTable = new QTableWidget(this);
        userTable->setColumnCount(2); // Définir le nombre de colonnes
        userTable->setHorizontalHeaderLabels({"Nom", "Prénom"}); // Ajouter les en-têtes de colonne
        layout->addWidget(userTable);

        // Créer le QComboBox pour les profils
        QComboBox *profileComboBox = new QComboBox(this);
        profileComboBox->addItems(profiles);
        layout->addWidget(profileComboBox);

        // Connecter le signal de changement de profil au slot correspondant
        connect(profileComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                this, &MainWindow::profileChanged);

        // Sélectionner par défaut le premier profil de la liste
        if (!profiles.isEmpty()) {
            profileComboBox->setCurrentIndex(0);
            profileChanged(profiles.first());
        }
    }

    // Créer le bouton de déconnexion
    disconnectButton = new QPushButton("Déconnexion", this);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::disconnectUser);

    // Afficher le bouton de déconnexion dans la barre de menus
    menuBar()->addSeparator();
    menuBar()->setCornerWidget(disconnectButton, Qt::TopRightCorner);

    disconnectButton->show();
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

    // Vérifier le layout principal
    qDebug() << "Main layout: " << layout->count() << " items";

    // Appeler la méthode pour configurer le dropdown avec le bon nom d'utilisateur
    setupProfileDropdown();
}





void MainWindow::profileChanged(const QString &newProfile) {
    // Mettre à jour le profil de l'utilisateur ici
    qDebug() << "Profil changé : " << newProfile;

    // Charger la liste des utilisateurs en fonction du profil sélectionné
    QList<QPair<QString, QString>> users = jsonManager->getAllUsers(newProfile);

    // Effacer le contenu du tableau des utilisateurs
    userTable->clearContents();
    userTable->setRowCount(0);

    // Remplir le tableau avec les utilisateurs
    int row = 0;
    for (const auto &user : users) {
        userTable->insertRow(row);
        userTable->setItem(row, 0, new QTableWidgetItem(user.first)); // Nom
        userTable->setItem(row, 1, new QTableWidgetItem(user.second)); // Prénom
        ++row;
    }

    // Mettre à jour le titre de la fenêtre avec le nouveau profil
    setWindowTitle("Main Window - Profile: " + newProfile);
}



void MainWindow::disconnectUser() {
    // Afficher un message de déconnexion réussie
    disconnectButton->hide();
    QMessageBox::information(this, "Déconnexion", "Vous êtes déconnecté.");

    // Afficher la fenêtre de connexion
    showLoginWindow();
}

void MainWindow::showUserCreationWindow() {
    // Afficher la fenêtre de création d'utilisateur
    UserCreationWindow userCreationWindow;
    if (userCreationWindow.exec() == QDialog::Accepted) {
        qDebug() << "Création d'utilisateur réussie";
        // Mettre à jour les paramètres pour indiquer que l'application a déjà été ouverte une fois
        QSettings settings("MonEntreprise", "MonApplication");
        settings.setValue("firstRun", false);
        // Afficher la fenêtre de connexion
        showLoginWindow();
    } else {
        qDebug() << "Création d'utilisateur annulée";
        // Fermer l'application si la création d'utilisateur est annulée
        close();
    }
}

void MainWindow::setupProfileDropdown() {
    profileDropdown = new ProfileDropdown(this);
    QStringList profiles = jsonManager->getUserProfiles(username); // Assurez-vous d'avoir un moyen d'accéder au nom de l'utilisateur courant
    profileDropdown->setProfiles(profiles);
    connect(profileDropdown, &ProfileDropdown::profileChanged, this, &MainWindow::handleProfileDropdownChange);
}

void MainWindow::handleProfileDropdownChange(const QString &newProfile) {
    // Mettez ici le code pour gérer le changement de profil
    qDebug() << "Profil changé : " << newProfile;

    // Charger la liste des utilisateurs en fonction du profil sélectionné
    QList<QPair<QString, QString>> users = jsonManager->getAllUsers(newProfile);

    // Effacer le contenu du tableau des utilisateurs
    userTable->clearContents();
    userTable->setRowCount(0);

    // Remplir le tableau avec les utilisateurs
    int row = 0;
    for (const auto &user : users) {
        userTable->insertRow(row);
        userTable->setItem(row, 0, new QTableWidgetItem(user.first)); // Nom
        userTable->setItem(row, 1, new QTableWidgetItem(user.second)); // Prénom
        ++row;
    }

    // Mettre à jour le titre de la fenêtre avec le nouveau profil
    setWindowTitle("Main Window - Profile: " + newProfile);
}
