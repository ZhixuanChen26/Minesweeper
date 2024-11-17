#include "qminesweeper.h"
#include <QDebug>
#include <QDateTime>
#include <QPainter>
#include <QLabel>

// Static member initialization
QMap<int,QMineSweeper *> QMineSweeper::mineMap;
QWidget * QMineSweeper::mainWindow = nullptr;
QMineSweeper * QMineSweeper::eventMine = nullptr;

// Static member initialization (mentioned before)
int QMineSweeper::mapRow = 16;
int QMineSweeper::mapCol = 30;
int QMineSweeper::mineNum = 99;
int QMineSweeper::mineSize = 16;
int QMineSweeper::gameState = GAME_STATE_SWEEP;
int QMineSweeper::mapX0 = 0;
int QMineSweeper::mapY0 = 0;
int QMineSweeper::openCount = 0;

//Randomly generates a list of unique numbers within a specified range.
void myrand(int n,int min,int max,QList<int> &list);

//Initializes a QMineSweeper block with an ID and parent widget.
QMineSweeper::QMineSweeper(int id,QWidget *parent) : QBlock(parent)
{
    selfid = id;
    selfN = 0;
    selfState = BLOCK_STATE_AREA;
    mineMap.insert(id,this);
    updateStyle();
    label = new QLabel(this); // Initializes the label and sets its parent object to this
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(QPixmap(":images/original.png"));

    if(eventMine == nullptr)
        eventMine = this;

    connect(this,SIGNAL(mineSweeperGameOver(int)),eventMine,SLOT(transponder(int)));
}

//Initializes the game map on a specified QWidget.
void QMineSweeper::createMap(QWidget *w)
{
    mainWindow = w;
    eventMine = nullptr;
    initMap();
}

//Initializes the game map by creating blocks and setting mines randomly.
void QMineSweeper::initMap()
{
    qDeleteAll(mineMap);
    mineMap.clear();
    // Generate the id list of the mine block
    QList<int> id_list;
    myrand(mineNum,0,mapRow * mapCol,id_list);

    for(int i = 0 ; i < mapRow * mapCol ; i++)
    {
        int id = i;
        QMineSweeper *block = new QMineSweeper(id,mainWindow);

        int r = i / mapCol;
        int c = i % mapCol;
        // If the block id is in the Mine block id list, the value is Ray (9)
        if(id_list.contains(id))
            block->setSelfN(9);
        // Generate a list of ids of adjacent blocks
        for(int i_r = -1 ; i_r < 2 ; i_r ++)
            for(int i_c = -1 ; i_c < 2 ; i_c++)
            {
                int nid = id + i_r * mapCol + i_c;
                if(i_r+r>=0&&i_r+r<mapRow&&i_c+c>=0&&i_c+c<mapCol&&nid!=id)
                    block->neighbors.append(nid);
            }

        block->setGeometry(c*mineSize+mapX0,r*mineSize+mapY0,mineSize,mineSize);
        block->show();
    }
    // Map the minefield to the adjacent blocks of the minefield
    foreach(int mid, id_list)
    {
        foreach(int nid , mineMap[mid]->neighbors)
        {

            int n = mineMap[nid]->getSelfN();
            // If n is not 9, it indicates a minefield. The danger value is increased by one
            if(n != 9)
                mineMap[nid]->setSelfN(n+1);
        }
    }
}

//Forwards game over signals from individual blocks to the main event solution.
void QMineSweeper::transponder(int state)
{
    QMineSweeper *sender_p = qobject_cast<QMineSweeper *>(sender());
    // If the sender of the signal is not itself, forward the event signal
    if(sender_p != eventMine)
        emit mineSweeperGameOver(state);
}

//Tackles the logic for opening a block, including revealing mines and calculating adjacent mine numbers.
void QMineSweeper::openBlock()
{
    if(selfState == BLOCK_STATE_OPENED)
        return;
    if(selfN == 9)
    {
        gameState = GAME_STATE_DEFEATED;
        selfState = BLOCK_STATE_BOOM;
        updateStyle();
        emit mineSweeperGameOver(gameState);
        return;
    }

    selfState = BLOCK_STATE_OPENED;
    openCount++;
    updateStyle();

    emit mineSweeperGameOver(GAME_STATE_SWEEP);

    if(openCount >= mapRow*mapCol - mineNum)
    {
        gameState = GAME_STATE_SUCCESSFUL;
        emit mineSweeperGameOver(gameState);
        return;
    }

    if(selfN == 0)
    {
        foreach(int id , neighbors)
            mineMap[id]->openBlock();
    }

}

//Updates the visual representation of a block based on its current state and value.
void QMineSweeper::updateStyle() {
    // Make sure the label is initialized and added to the layout
    if (!label) return;

    if(selfN == 9 && selfState == BLOCK_STATE_BOOM) {
        // If it is a mine and triggered
        label->setPixmap(QPixmap(":images/bomb.png"));
    } else if(selfState == BLOCK_STATE_FLAG) {
        // If the block is marked as a mine
        label->setPixmap(QPixmap(":images/flag.png"));
    } else if(selfState == BLOCK_STATE_DOUBT) {
        // If the box is marked as a question
        label->setPixmap(QPixmap(":images/q.png"));
    } else if(selfState == BLOCK_STATE_OPENED && selfN > 0 && selfN <= 8) {
        // If the box is opened and has numbers from 1 to 8
        QString imagePath = QString(":images/%1.png").arg(selfN);
        label->setPixmap(QPixmap(imagePath));
    } else if(selfState == BLOCK_STATE_OPENED && selfN == 0) {
        // If the block is opened but there is no thunder around it
        label->setPixmap(QPixmap(":images/safe.png"));
    } else {
        // Other cases, such as unopened blocks
        label->setPixmap(QPixmap(":images/original.png"));
    }

    label->setFixedSize(mineSize, mineSize); // Make sure the image size is the same as the square size
    label->show();
}

//Handles mouse press events on blocks, including right-clicks for flagging and left-clicks for opening.
void QMineSweeper::mousePressEvent(QMouseEvent *event)
{
    if(gameState != GAME_STATE_SWEEP)
        return;

    if(event->button() == Qt::RightButton)
    {
        if(selfState == BLOCK_STATE_OPENED)
            return;
        else if(selfState == BLOCK_STATE_AREA)
        {
            selfState = BLOCK_STATE_FLAG;
            emit mineSweeperGameOver(GAME_SET_FLAG);
        }
        else if(selfState == BLOCK_STATE_FLAG)
        {
            selfState = BLOCK_STATE_DOUBT;
            emit mineSweeperGameOver(GAME_RESET_FLAG);
        }
        else if(selfState == BLOCK_STATE_DOUBT)
        {
            selfState = BLOCK_STATE_AREA;
        }
        updateStyle();

    }
    else if(event->button() == Qt::LeftButton)
    {
        openBlock();

    }
}

//(mentioned before)
void myrand(int n,int min,int max,QList<int> &list)
{
    QMap<int,bool> maps;

    srand((uint)QDateTime::currentMSecsSinceEpoch());

    for(int i = 0 ; i < n && i < max - min; i++)
    {
        int r_count = rand()%(max-min)+min;
        while(maps[r_count] == true)
            r_count = rand()%(max-min)+min;

        maps[r_count] = true;
        list.append(r_count);

    }
}

// Resets the game to its initial state, clearing the map and resetting counters.
void QMineSweeper::resetGame() {
    qDeleteAll(mineMap.values());
    mineMap.clear(); // Clear the map to remove all entries
    openCount = 0;
    gameState = GAME_STATE_SWEEP;
    createMap(mainWindow);
    eventMine = mineMap.value(0); // Reassign the eventMine to the first mine in the map to handle game over signals
}

