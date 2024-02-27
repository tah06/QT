// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "loginwindow.h"
#include "usercreationwindow.h"
#include "profiledropdown.h"
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMainPage();
    void disconnectUser();
    void profileChanged(const QString &newProfile);
    void handleProfileDropdownChange(const QString &newProfile);
    void handleDeleteButtonClicked(const QString &prenom, const QString &nom);
    void handleUpdateButtonClicked(const QString &prenom, const QString &nom);

    void showUpdateUserDialog(const QString &nom, const QString &prenom);
    void addUserButtonClicked();







private:
    void showLoginWindow();
    void showUserCreationWindow();
    QTableWidget *userTable;
    LoginWindow *loginWindow;
    QPushButton *disconnectButton;
    ProfileDropdown *profileDropdown;
    QWidget *mainWidget;
    JSONManager *jsonManager;
    QLabel *titleLabel;
     QComboBox *profileComboBox;
     void setupProfileDropdown();
     QString username;
     void clearLayout(QLayout *layout);
     QPushButton *deleteButton;
     QPushButton *editButton;

     void updateUserSuccess(QDialog *dialog, const QString &nom, const QString &prenom, const QString &newNom, const QString &newPrenom);
     void refreshUserTable(const QString &profile);

    QDialog dialog;

    void setUsername(const QString &newUsername);
    QPushButton *addUserButton;




};

#endif // MAINWINDOW_H
