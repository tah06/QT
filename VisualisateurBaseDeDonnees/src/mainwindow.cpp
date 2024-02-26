#include "mainwindow.h"
#include "jsonmanager.h"
#include "usercreationwindow.h"
#include "loginwindow.h"

#include <QFormLayout>


#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QMenuBar>
#include <QPushButton>
#include <QOverload>
#include <QInputDialog>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), loginWindow(nullptr), mainWidget(nullptr), disconnectButton(nullptr), jsonManager(nullptr), userTable(nullptr)
{
    setWindowTitle("Main Window");

    jsonManager = new JSONManager(this);
    // Connecter le signal de LoginWindow à la méthode de MainWindow pour mettre à jour username
    connect(loginWindow, &LoginWindow::loginSuccessful, this, &MainWindow::setUsername);


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
void MainWindow::setUsername(const QString &newUsername) {
    username = newUsername;
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
        userTable->setColumnCount(4); // Définir le nombre de colonnes
        userTable->setHorizontalHeaderLabels({"Nom", "Prénom", "Modifier", "Supprimer"}); // Ajouter les en-têtes de colonne
        layout->addWidget(userTable);

       refreshUserTable();


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


    // Créer le bouton "Ajouter" dans la barre de menus
    QPushButton *addUserButton = new QPushButton("Ajouter", this);
    menuBar()->setCornerWidget(addUserButton, Qt::TopRightCorner);
    connect(addUserButton, &QPushButton::clicked, this, &MainWindow::addUserButtonClicked);

    // Vérifier le layout principal
    qDebug() << "Nombre d'éléments dans le layout principal : " << layout->count();

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


void MainWindow::clearLayout(QLayout *layout) {
    if (!layout)
        return;
    while (auto item = layout->takeAt(0)) {
        if (auto widget = item->widget()) {
            widget->deleteLater();
        } else {
            clearLayout(item->layout());
        }
        delete item;
    }
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


void MainWindow::handleUpdateButtonClicked(const QString &nom, const QString &prenom) {

    showUpdateUserDialog(nom, prenom);
}



void MainWindow::showUpdateUserDialog(const QString &nom, const QString &prenom) {
    // Créez un QDialog personnalisé pour le formulaire de mise à jour de l'utilisateur

    QDialog dialog(this);
    dialog.setWindowTitle("Modification de l'utilisateur");

    // Ajoutez les widgets pour les champs de nom, prénom, mot de passe et profils
    QLabel *nomLabel = new QLabel("Nom:", &dialog);
    QLineEdit *nomLineEdit = new QLineEdit(nom, &dialog);
    QLabel *prenomLabel = new QLabel("Prénom:", &dialog);
    QLineEdit *prenomLineEdit = new QLineEdit(prenom, &dialog);
    QLabel *motDePasseLabel = new QLabel("Mot de passe:", &dialog);
    QLineEdit *motDePasseLineEdit = new QLineEdit(&dialog);
    QLabel *profilesLabel = new QLabel("Profils (séparés par des virgules):", &dialog);
    QLineEdit *profilesLineEdit = new QLineEdit(&dialog);

    // Ajoutez un bouton de confirmation et un bouton d'annulation
    QPushButton *confirmButton = new QPushButton("Confirmer", &dialog);
    QPushButton *cancelButton = new QPushButton("Annuler", &dialog);

    // Ajoutez des layouts pour organiser les widgets
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // Ajoutez les widgets au layout
    formLayout->addRow(nomLabel, nomLineEdit);
    formLayout->addRow(prenomLabel, prenomLineEdit);
    formLayout->addRow(motDePasseLabel, motDePasseLineEdit);
    formLayout->addRow(profilesLabel, profilesLineEdit);

    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(formLayout);
    layout->addLayout(buttonLayout);

    // Connectez le bouton de confirmation au slot pour gérer la mise à jour de l'utilisateur
    connect(confirmButton, &QPushButton::clicked, this, [=, &dialog]() {
        // Récupérez les valeurs des champs
        QString newNom = nomLineEdit->text();
        QString newPrenom = prenomLineEdit->text();
        QString newMotDePasse = motDePasseLineEdit->text();
        QStringList newProfiles = profilesLineEdit->text().split(',');

        // Mettez à jour l'utilisateur avec les nouvelles informations
        bool success = jsonManager->updateUser(prenom, nom, newNom, newPrenom, newMotDePasse, newProfiles);
        if (success) {
            qDebug() << "Utilisateur mis à jour avec succès";
            updateUserSuccess(&dialog, nom, prenom, newNom, newPrenom); // Passer le pointeur de QDialog à updateUserSuccess
        } else {
            qDebug() << "Erreur lors de la mise à jour de l'utilisateur";
            QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour de l'utilisateur");
        }
    });



    // Connectez le bouton d'annulation pour fermer le QDialog
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Affichez le QDialog modale
    if (dialog.exec() == QDialog::Accepted) {
        // Le QDialog a été accepté
    } else {
        // Le QDialog a été rejeté ou fermé
    }
}
void MainWindow::updateUserSuccess(QDialog *dialog, const QString &nom, const QString &prenom, const QString &newNom, const QString &newPrenom) {
    qDebug() << "Utilisateur mis à jour avec succès";

    // Actualisez les cellules correspondantes dans le tableau avec les nouvelles valeurs
    int rowCount = userTable->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *nomItem = userTable->item(row, 0);
        QTableWidgetItem *prenomItem = userTable->item(row, 1);

        // Vérifiez si le nom et le prénom correspondent à ceux de l'utilisateur modifié
        if (nomItem && prenomItem && nomItem->text() == nom && prenomItem->text() == prenom) {
            nomItem->setText(newNom);
            prenomItem->setText(newPrenom);
            // Vous pouvez également mettre à jour les autres colonnes si nécessaire
            break; // Sortir de la boucle une fois la mise à jour effectuée
        }
    }
    // Fermez le QDialog
    if (dialog)
        dialog->accept();
}


void MainWindow::handleDeleteButtonClicked(const QString &prenom, const QString &nom) {
    // Demander confirmation avant de supprimer l'utilisateur
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                  "Êtes-vous sûr de vouloir supprimer cet utilisateur?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Supprimer l'utilisateur
        bool success = jsonManager->removeUser(prenom, nom);
        if (success) {

            refreshUserTable();
        }else {
            qDebug() << "Erreur lors de la suppression de l'utilisateur";
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'utilisateur");
        }
    }
}

void MainWindow::refreshUserTable() {
    qDebug() << "Im hereeeeeeee";

    qDebug() << "Username retrieved:" << username;

    QStringList profiles = jsonManager->getUserProfiles(username);
    qDebug() << "User profiles:" << profiles;

    QList<QPair<QString, QString>> users = jsonManager->getAllUsers(profiles.first());

    // Effacer le contenu du tableau des utilisateurs
    userTable->clearContents();
    userTable->setRowCount(0);

    // Remplir le tableau avec les utilisateurs
    int row = 0;
    for (const auto &user : users) {
        userTable->insertRow(row);
        userTable->setItem(row, 0, new QTableWidgetItem(user.first)); // Nom
        userTable->setItem(row, 1, new QTableWidgetItem(user.second)); // Prénom

        // Créer le bouton "Modifier" pour cette ligne
        QPushButton *editButton = new QPushButton("Modifier", this);
        // Connecter le bouton "Modifier" à son slot correspondant
        connect(editButton, &QPushButton::clicked, this, [this, user](){ handleUpdateButtonClicked(user.first, user.second); });

        // Créer le bouton "Supprimer" pour cette ligne
        QPushButton *deleteButton = new QPushButton("Supprimer", this);
        // Connecter le bouton "Supprimer" à son slot correspondant
        connect(deleteButton, &QPushButton::clicked, this, [this, user](){ handleDeleteButtonClicked(user.second, user.first); });

        // Définir la taille des boutons
        editButton->setFixedSize(100, 30);
        deleteButton->setFixedSize(100, 30);

        // Insérer le bouton "Modifier" dans la cellule de la colonne 2
        userTable->setCellWidget(row, 2, editButton);
        // Insérer le bouton "Supprimer" dans la cellule de la colonne 3
        userTable->setCellWidget(row, 3, deleteButton);

        // Passer à la prochaine ligne
        ++row;
    }
}

void MainWindow::addUserButtonClicked() {
    // Créez une boîte de dialogue pour saisir les informations de l'utilisateur
    QDialog dialog(this);
    dialog.setWindowTitle("Ajouter un utilisateur");

    // Ajoutez les widgets pour les champs de saisie
    QLabel *nomLabel = new QLabel("Nom:", &dialog);
    QLineEdit *nomLineEdit = new QLineEdit(&dialog);
    QLabel *prenomLabel = new QLabel("Prénom:", &dialog);
    QLineEdit *prenomLineEdit = new QLineEdit(&dialog);
    QLabel *motDePasseLabel = new QLabel("Mot de passe:", &dialog);
    QLineEdit *motDePasseLineEdit = new QLineEdit(&dialog);
    QLabel *profilesLabel = new QLabel("Profils (séparés par des virgules):", &dialog);
    QLineEdit *profilesLineEdit = new QLineEdit(&dialog);

    // Ajoutez un bouton de confirmation et un bouton d'annulation
    QPushButton *confirmButton = new QPushButton("Confirmer", &dialog);
    QPushButton *cancelButton = new QPushButton("Annuler", &dialog);

    // Ajoutez des layouts pour organiser les widgets
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // Ajoutez les widgets au layout
    formLayout->addRow(nomLabel, nomLineEdit);
    formLayout->addRow(prenomLabel, prenomLineEdit);
    formLayout->addRow(motDePasseLabel, motDePasseLineEdit);
    formLayout->addRow(profilesLabel, profilesLineEdit);

    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(formLayout);
    layout->addLayout(buttonLayout);

    // Connectez le bouton de confirmation au slot pour ajouter un nouvel utilisateur
    connect(confirmButton, &QPushButton::clicked, this, [=,&dialog]() {
        QString nom = nomLineEdit->text();
        QString prenom = prenomLineEdit->text();
        QString motDePasse = motDePasseLineEdit->text();
        QStringList profiles = profilesLineEdit->text().split(',');

        // Ajoutez le nouvel utilisateur en appelant la méthode addUser de JSONManager
        bool success = jsonManager->addUser(prenom, nom, motDePasse, profiles);
        if (success) {
            qDebug() << "Utilisateur ajouté avec succès";
            refreshUserTable(); // Rafraîchissez le tableau des utilisateurs après l'ajout
        } else {
            qDebug() << "Erreur lors de l'ajout de l'utilisateur";
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'utilisateur");
        }

        // Fermez la boîte de dialogue
        dialog.close();
    });

    // Connectez le bouton d'annulation pour fermer le QDialog
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Affichez le QDialog modale
    dialog.exec();
}


