#ifndef MAP_H
#define MAP_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QLabel>
#include <QList>
#include <QTimer>

#include <vector>
#include <fstream>
#include <iostream>

#include "agent/agent.hpp"
#include "mapscene.h"

using namespace std;

class Agent;

struct QAgent {
    QGraphicsPixmapItem * pix_;
    Agent * agent_;
    int id_;
};

typedef QList<QAgent> Agents;

enum CellTile {
    Wall,
    Ground,
    Network,
    Metal,
    RedTile,
    Nuclear,
    Land,
};

enum Direcction {
    Up,
    Left,
    Down,
    Right
};

struct Cell {
    CellTile type_;
    QGraphicsPixmapItem * pix_;
};

struct CellWeight {
    int x_;
    int y_;
    double weight_;
};

class Map : public QGraphicsView
{
    Q_OBJECT
private:
    vector<Cell>    map_;
    vector<QPixmap> terrain_;
    CellTile        pencil_;

    int             rows_;
    int             cols_;
    double          lastZoom_;
    double          sizeTile_;
    int             loadFactor_;

    Agents          agents_;
    vector<QPixmap> agentDirections_;

public:
    explicit Map(int columns, int rows
                 , short wall=0, short ground=0, short network=0, short metal=0, QWidget *parent = 0);
    explicit Map(ifstream* fich, QWidget *parent=0);

    void initMap(void);

    QGraphicsPixmapItem* drawPixmap(int column, int row, QPixmap & pixmap);

    void exchangeCell(int column, int row, CellTile cell);

    void redraw(void);

    int pos(int col,int row){
        return row*cols_ + col;
    }

    bool save(ofstream * file);

    Cell operator()(int col, int row) {
        return map_[pos(col,row)];
    }

    void setPencil(CellTile);

    void tick(void);

    QList<CellWeight> getAroundCells(int col, int row);

    void addAgent(int col, int row);

    void moveAgent(int col, int row, int id);

signals:
    void newAgent(Map *, int id);

public slots:
    void drawOnMouse(QPointF mousePos);

    void drawAgentOnDrop(QPointF mousePos);

    void removeAgent(int id);

    void makeZoom(int factor);

    void speedMove(int speed);

    void startAI(void);

    void stopAI(void);

protected:
    void resizeEvent(QResizeEvent * e);
};
#endif // MAP_H
