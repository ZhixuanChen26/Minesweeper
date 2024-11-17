// Author: Zhixuan Chen
// Description: This header file declares the QMineSweeper class, which manages the logic and minesweeper blocks, including their state, appearance, and interactions.
// Date: 2024.02.06
#ifndef QMINESWEEPER_H
#define QMINESWEEPER_H

#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QMouseEvent>

// Block state codes
#define BLOCK_STATE_AREA    0       // Not opened
#define BLOCK_STATE_OPENED  1       // Opened
#define BLOCK_STATE_FLAG    2       // Flagged
#define BLOCK_STATE_DOUBT   3       // Doubtful
#define BLOCK_STATE_BOOM    4       // Exploded
#define BLOCK_STATE_SWEEPED 5       // Safe
#define BLOCK_STATE_MISTAKE 6       // Incorrectly flagged

// Game state signals
#define GAME_STATE_SWEEP        0   // A block has been swept
#define GAME_STATE_SUCCESSFUL   1   // All blocks have been swept; game won
#define GAME_STATE_DEFEATED     2   // Game over
#define GAME_SET_FLAG           3   // A block has been flagged
#define GAME_RESET_FLAG         4   // A flag has been removed from a block



typedef QPushButton QBlock;


class QMineSweeper : public QBlock
{
    Q_OBJECT // Enables Qt's signal and slot

public:
    explicit QMineSweeper(int id,QWidget *parent = nullptr); // Constructor
    static void createMap(QWidget *w = mainWindow); // Initializes the game map
    static void setMainWindow(QWidget *w){mainWindow = w;} // Sets the main window
    static void initMap(); // Prepares the game map with mines
    static QMineSweeper *getEventMine(){return eventMine;} // Returns the instance responsible for event handling
    static void resetGame(); // Resets the game to its initial state

private:
    // Static members for game map dimensions and settings
    static QMap<int,QMineSweeper *> mineMap; // Map of all mine sweeper blocks
    static int mapRow; // Number of rows in the map
    static int mapCol; // Number of columns in the map
    static int mineNum; // Total number of mines
    static int mineSize; // Size of each mine block
    static int gameState; // Current state of the game
    static int mapX0; // X-coordinate offset for the map
    static int mapY0; // Y-coordinate offset for the map
    static int openCount; // Count of opened blocks
    static QWidget *mainWindow; // Pointer to the main window
    static QMineSweeper *eventMine; // Block used for event handling

signals:
    void mineSweeperGameOver(int state); // Signal emitted when the game is over

private slots:
    void transponder(int state); // Solves game over signals

protected:
    virtual void mousePressEvent(QMouseEvent *event); // Tackles mouse press events

private:
    void setSelfN(int n){selfN = n;} // Sets the danger level of the block
    int  getSelfN(){return selfN;} // Retrieves the danger level of the block
    void openBlock(); // Opens a block
    void updateStyle(); // Updates the block's appearance
    int selfid; // Unique identifier for the block
    int selfN; // Danger level of the block
    int selfState; // Current state of the block
    QList<int> neighbors; // List of neighboring block IDs
    QLabel *label; // Label used to display the block's state or image

};

#endif






