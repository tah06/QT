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

};

#endif // MAINWINDOW_H
