// Author: Zhixuan Chen
// Description: Entry point for the Minesweeper application.
// Date: 2024.02.06
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Create the main window
    MainWindow w;
    // Display the main window
    w.show();
    // Enter the application's event loop
    return a.exec();
}


