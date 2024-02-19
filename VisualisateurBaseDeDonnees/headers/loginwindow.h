#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QString getUsername() const;

signals:
    void loginSuccessful();

private slots:
    bool loginUser();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
};

#endif // LOGINWINDOW_H
