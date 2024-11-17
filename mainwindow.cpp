// Author: Zhixuan Chen
// Description: Initializes the game map, shows game state changes, and sends messages to the user based on the game's progress.
// Date: 2024.02.06
#include "mainwindow.h"
#include "qminesweeper.h"
#include <QMessageBox>
#include <QApplication>

// Initializes the main window, sets its size, creates the game map, and connects signals to slots.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(480,256); // Window size.
    // Create the Minesweeper game map within main window.
    QMineSweeper::createMap(this);
    // Connect the game over signal from the QMineSweeper to the gameState slot in this class.
    connect(QMineSweeper::getEventMine(),SIGNAL(mineSweeperGameOver(int)),
            this,SLOT(gameState(int)));
}
// Cleans up tasks if necessary.
MainWindow::~MainWindow()
{

}

void MainWindow::gameState(int state)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game over"); // Title of the message box.
    msgBox.setIconPixmap(QPixmap(":images/icon.png")); // Set an icon for the message box.
    // Only process if the game is won or lost to avoid repeated operation.
    if(state != GAME_STATE_SUCCESSFUL && state != GAME_STATE_DEFEATED) {
        return;
    }
    // Show information on whether the player won or lost.
    if(state == GAME_STATE_SUCCESSFUL) {
        msgBox.setText("You win! Congratulations! Play again?");
    } else if(state == GAME_STATE_DEFEATED) {
        msgBox.setText("Sorry, you lost. Play again?");
    }

    // Add buttons for replaying or exiting the game.
    QPushButton *continueButton = msgBox.addButton("Play again", QMessageBox::ActionRole);
    QPushButton *quitButton = msgBox.addButton("Exit", QMessageBox::RejectRole);

    msgBox.exec();

    // Determine the next action depend on user.
    if(msgBox.clickedButton() == continueButton) {
        // Reset the game for another round.
        QMineSweeper::resetGame();// // Reconnect the signal to ensure the gameState slot is triggered on game end.
        connect(QMineSweeper::getEventMine(), SIGNAL(mineSweeperGameOver(int)), this, SLOT(gameState(int)));
    } else if(msgBox.clickedButton() == quitButton) {
        // Exit the application.
        QApplication::quit();
    }
}
