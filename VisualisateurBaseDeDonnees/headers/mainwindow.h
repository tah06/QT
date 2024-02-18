#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loginwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showMainPage();
    void disconnectUser();
    void showLoginWindow();
    void hideDisconnectButton();

private:
    LoginWindow *loginWindow;
    QPushButton *disconnectButton;
};

#endif // MAINWINDOW_H
