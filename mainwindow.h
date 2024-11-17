// Author: Zhixuan Chen
// Description: This header file declares the MainWindow class, which is the main interface for the application.
// Date: 2024.02.06
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> // Includes the QMainWindow class for creating the application's main window
#include <QLabel>  // Includes the QLabel class for use in showing text or images

// MainWindow class definition which is inheriting from QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT // Enables the use of Qt's meta-object system


public:
    //Sets up the main window and initializes the user interface components.
    MainWindow(QWidget *parent = 0);
    // Solves cleanup tasks when the MainWindow object is destroyed.
    ~MainWindow();
private slots:
    // Slot for changes in the game state.
    void gameState(int state);
};

#endif



