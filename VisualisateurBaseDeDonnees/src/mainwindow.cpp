#include "mainwindow.h"
#include "usercreationwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), loginWindow(nullptr)
{
    setWindowTitle("Main Window");




    // Vérifier si c'est la première fois que l'utilisateur ouvre l'application
    QSettings settings("MonEntreprise", "MonApplication");
    bool isFirstRun = settings.value("firstRun", true).toBool();
    //settings.setValue("firstRun", true);

    if (isFirstRun) {
        // Afficher la fenêtre de création d'utilisateur
        UserCreationWindow userCreationWindow;
        if (userCreationWindow.exec() == QDialog::Accepted) {
            qDebug() << "Création d'utilisateur réussie";
            // Mettre à jour les paramètres pour indiquer que l'application a déjà été ouverte une fois
            settings.setValue("firstRun", false);
        } else {
            qDebug() << "Création d'utilisateur annulée";
        }
    } else {
        // Afficher la fenêtre de connexion

        showLoginWindow();

    }
}

MainWindow::~MainWindow()
{
    if (loginWindow) {
        delete loginWindow;
    }
}


void MainWindow::showLoginWindow() {
    // Afficher la fenêtre de connexion
    loginWindow = new LoginWindow();
    connect(loginWindow, &LoginWindow::loginSuccessful, this, &MainWindow::showMainPage);
   // connect(loginWindow, &QObject::finished, this, &MainWindow::hideDisconnectButton);     // Connectez le signal finished de la fenêtre de connexion
    setCentralWidget(loginWindow);
    loginWindow->show();
    // disconnectButton->hide();
    //hideDisconnectButton();
}

void MainWindow::hideDisconnectButton() {
    // Cacher le bouton de déconnexion
    //if (disconnectButton)
        disconnectButton->hide();
}



void MainWindow::showMainPage() {

    // delete loginWindow;
    // loginWindow = nullptr;

    qDebug() << "Connexion réussie, affichage de la page principale...";


    // Créer et afficher la fenêtre de la page principale
    QWidget *mainPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(mainPage);
    QLabel *titleLabel = new QLabel("Bienvenue sur la page principale !");
    layout->addWidget(titleLabel);

    // Ajouter d'autres éléments à la page principale si nécessaire
    // Créer le bouton de déconnexion
    disconnectButton = new QPushButton("Déconnexion", this);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::disconnectUser);
    disconnectButton->show();
    // Afficher le bouton de déconnexion dans la barre de menus
    menuBar()->addSeparator();
    menuBar()->setCornerWidget(disconnectButton, Qt::TopRightCorner);

    mainPage->setLayout(layout);
    setCentralWidget(mainPage);

}

// Définissez la fonction de déconnexion
void MainWindow::disconnectUser() {
    // Afficher un message de déconnexion réussie
    QMessageBox::information(this, "Déconnexion", "Vous êtes déconnecté.");
    disconnectButton->hide();

    // Afficher la fenêtre de connexion
    showLoginWindow();
}
