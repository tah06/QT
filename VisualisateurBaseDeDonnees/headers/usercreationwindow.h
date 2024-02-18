// usercreationwindow.h
#ifndef USERCREATIONWINDOW_H
#define USERCREATIONWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include "jsonmanager.h" // Assurez-vous d'inclure le fichier d'en-tête correspondant

class UserCreationWindow : public QDialog
{
    Q_OBJECT

public:
    UserCreationWindow(QWidget *parent = nullptr);
    ~UserCreationWindow();

private slots:
    void createUser();

private:
    QLineEdit *prenomLineEdit;
    QLineEdit *nomLineEdit;
    QLineEdit *motDepasseLineEdit;
    QLineEdit *confirmmotDepasseLineEdit;
    QPushButton *createUserButton;
    JSONManager manager; // Instance de JSONManager pour gérer les utilisateurs JSON
};

#endif // USERCREATIONWINDOW_H
