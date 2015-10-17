#include "map.h"


Map::Map(int columns, int rows
         , short wall, short ground, short network, short metal, QWidget *parent):
    QGraphicsView(parent),
    map_(columns*rows),
    terrain_(7),
    pencil_(RedTile),
    rows_(rows),
    cols_(columns),
    lastZoom_(1)
{
    buildMap();
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j <cols_; j++) {
            exchangeCell(j, i, Metal);
        }
    }
}


Map::Map(ifstream* fich, QWidget *parent):
    QGraphicsView(parent),
    map_(),
    terrain_(7),
    pencil_(RedTile),
    rows_(0),
    cols_(0),
    lastZoom_(1)
{
    *fich>>rows_;
    *fich>>cols_;
    buildMap();
    for(int i=0;i<rows_;i++){
        for(int j=0;j<cols_;j++){
            short pixId;
            *fich>>pixId;
            exchangeCell(j,i,(CellTile)pixId);
        }
    }
}

void Map::buildMap(void) {
    this->setScene(new MapScene(this));
    connect(this->scene(),SIGNAL(mousePos(QPointF)),this,SLOT(drawOnMouse(QPointF)));
    terrain_[Wall]    = QPixmap(":/recursos/muro.png");
    terrain_[RedTile] = QPixmap(":/recursos/rojo.png");
    terrain_[Ground]  = QPixmap(":/recursos/suelo.png");
    terrain_[Metal]   = QPixmap(":/recursos/metal.png");
    terrain_[Land]    = QPixmap(":/recursos/tierra.png");
    terrain_[Network] = QPixmap(":/recursos/rejilla.png");
    terrain_[Nuclear] = QPixmap(":/recursos/nuclear.png");
    double sizeTile_x = this->width()/cols_;
    double sizeTile_y = this->height()/rows_;
    cout << "w h" <<this->size().width() << " " << this->size().height() << endl;
    sizeTile_ = sizeTile_x < sizeTile_y ? sizeTile_x : sizeTile_y;
}

void Map::makeZoom(int factor) {
    this->scale(factor*1/lastZoom_,factor*1/lastZoom_);
    lastZoom_ = factor;
}

QGraphicsPixmapItem* Map::drawPixmap(int column, int row, CellTile cell) {
    QPixmap * pix = &terrain_[cell];
    QGraphicsPixmapItem * auxPix = this->scene()->addPixmap(*pix);
    auxPix->setScale(sizeTile_/pix->size().height());
    auxPix->setPos(sizeTile_*column, sizeTile_*row);
    return auxPix;
}


void Map::exchangeCell(int column, int row, CellTile cell) {
    QPixmap* pix = &terrain_[cell];
    QGraphicsPixmapItem* auxPixRemove = map_[pos(column,row)].pix_;
    QGraphicsPixmapItem* auxPix = this->scene()->addPixmap(*pix);
    auxPix->setScale(sizeTile_/pix->size().height());
    auxPix->setPos(sizeTile_*column, sizeTile_*row);
    if(auxPixRemove==NULL){
        delete auxPixRemove;
        auxPixRemove == NULL;
    }
    map_[pos(column,row)].pix_=auxPix;
    map_[pos(column,row)].type_=cell;
}

void Map::redraw(void) {
    this->scene()->clear();
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j <cols_; j++) {
            exchangeCell(j, i, (CellTile)map_[pos(j,i)].type_);
        }
    }
}

void Map::drawOnMouse(QPointF mousePos) {
    if((mousePos.x() > 0) && (mousePos.x() < (sizeTile_*cols_))
       && (mousePos.y() > 0)  && (mousePos.y() < (sizeTile_*rows_)))
        exchangeCell(mousePos.x() / sizeTile_, mousePos.y() / sizeTile_, pencil_);
}

void Map::resizeEvent(QResizeEvent * e){
    double sizeTile_x = e->size().width()/cols_;
    double sizeTile_y = e->size().height()/rows_;
    sizeTile_ = sizeTile_x < sizeTile_y ? sizeTile_x : sizeTile_y;
    redraw();
}
