#ifndef MAP_H
#define MAP_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>

#include <vector>
#include <fstream>
#include <iostream>

#include "mapscene.h"

using namespace std;

enum CellTile {
    Wall,
    Ground,
    Network,
    Metal,
    RedTile,
    Nuclear,
    Land,
};

struct Cell {
    short type_;
    QGraphicsPixmapItem * pix_;
    float weight_;
    bool walkable_;
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
public:
    explicit Map(int columns, int rows
                 , short wall=0, short ground=0, short network=0, short metal=0, QWidget *parent = 0);
    explicit Map(ifstream* fich, QWidget *parent=0);

    void initMap(void);

    QGraphicsPixmapItem* drawPixmap(int column, int row, CellTile cell);

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

    void tick(void) {};


signals:

public slots:
    void drawOnMouse(QPointF mousePos);

    void makeZoom(int factor);

protected:
    void resizeEvent(QResizeEvent * e);
};
#endif // MAP_H
