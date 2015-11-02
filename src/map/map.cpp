#include "map.h"


Map::Map(int columns, int rows
         , short wall, short land, short network, short metal, QWidget *parent):
    QGraphicsView(parent),
    map_(columns*rows),
    terrain_(7),
    pencil_(RedTile),
    rows_(rows),
    cols_(columns),
    lastZoom_(1),
    sizeTile_(0),
    loadFactor_(0),
    agents_(),
    agentDirections_(4),
    timer_(new QTimer(this)),
    speed_(250)
{
    initMap();
    for (int j = 0; j < rows_; j++) {
        for (int i = 0; i <cols_; i++) {
            int aleatoriedad = rand()%100+ 1;
            if(i==0 || j==0 || j==rows_-1 || i==cols_-1){
                exchangeCell(i,j,Wall);
            }else if(aleatoriedad<=wall){
                exchangeCell(i,j,RedTile);
            }else if(aleatoriedad<=wall+metal){
                exchangeCell(i,j,Metal);
            }else if(aleatoriedad<=wall+metal+network){
                exchangeCell(i,j,Network);
            }else if(aleatoriedad<=wall+metal+network+land){
                exchangeCell(i,j,Land);
            }else{
                exchangeCell(i,j,Ground);
            }
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
    lastZoom_(1),
    sizeTile_(0),
    loadFactor_(0),
    agents_(),
    agentDirections_(4),
    timer_(new QTimer(this)),
    speed_(250)
{
    *fich>>rows_;
    *fich>>cols_;
    map_ = vector<Cell>(rows_*cols_);
    initMap();
    for(int i=0;i<rows_;i++){
        for(int j=0;j<cols_;j++){
            short pixId;
            *fich>>pixId;
            exchangeCell(j,i,(CellTile)pixId);
        }
    }
}

void Map::initMap(void) {
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
    sizeTile_ = sizeTile_x < sizeTile_y ? sizeTile_x : sizeTile_y;

    connect(this->scene(),SIGNAL(dropAgent(QPointF)),this,SLOT(drawAgentOnDrop(QPointF)));
    agentDirections_[Up]    = QPixmap(":/recursos/robotArriba.png");
    agentDirections_[Down]  = QPixmap(":/recursos/robotAbajo.png");
    agentDirections_[Left]  = QPixmap(":/recursos/robotIzquierda.png");
    agentDirections_[Right] = QPixmap(":/recursos/robotDerecha.png");
}

void Map::makeZoom(int factor) {
    this->scale(factor*1/lastZoom_,factor*1/lastZoom_);
    lastZoom_ = factor;
}

PixmapItem* Map::drawPixmap(int column, int row, QPixmap & pixmap) {
    PixmapItem * pix = new PixmapItem(pixmap);
    scene()->addItem(pix);
    pix->setScale(sizeTile_/pixmap.size().height());
    pix->setPos(sizeTile_*column, sizeTile_*row);
    pix->setZValue(2);
    return pix;
}


void Map::exchangeCell(int column, int row, CellTile cell) {
    QPixmap* pix = &terrain_[cell];
    QGraphicsPixmapItem* auxPixRemove = map_[pos(column,row)].pix_;
    QGraphicsPixmapItem* auxPix = this->scene()->addPixmap(*pix);

    auxPix->setScale(sizeTile_/pix->size().height());
    auxPix->setPos(sizeTile_*column, sizeTile_*row);
    auxPix->setZValue(-1);
    map_[pos(column,row)] = { cell, auxPix};

    if(auxPixRemove!=NULL){
        scene()->removeItem(auxPixRemove);
        delete auxPixRemove;
        auxPixRemove = NULL;
    }
}

void Map::redraw(void) {
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j <cols_; j++) {
            QGraphicsPixmapItem * auxPix = map_[pos(j,i)].pix_;
            auxPix->setScale(sizeTile_/32);
            auxPix->setPos(sizeTile_*j, sizeTile_*i);
        }
    }
    for (auto &i : agents_) {
        i.pix_->setScale(sizeTile_/32);
        i.pix_->setPos(sizeTile_*i.agent_->x_, sizeTile_*i.agent_->y_);
    }
}

void Map::drawOnMouse(QPointF mousePos) {
    if((mousePos.x() > 0) && (mousePos.x() < (sizeTile_*cols_))
       && (mousePos.y() > 0)  && (mousePos.y() < (sizeTile_*rows_)))
        exchangeCell(mousePos.x() / sizeTile_, mousePos.y() / sizeTile_, pencil_);
}

void Map::drawAgentOnDrop(QPointF mousePos) {
    if((mousePos.x() > 0) && (mousePos.x() < (sizeTile_*cols_))
       && (mousePos.y() > 0)  && (mousePos.y() < (sizeTile_*rows_)))
        addAgent(mousePos.x() / sizeTile_, mousePos.y() / sizeTile_);
}


void Map::resizeEvent(QResizeEvent * e){
    double sizeTile_x = e->size().width()/cols_;
    double sizeTile_y = e->size().height()/rows_;
    sizeTile_ = sizeTile_x < sizeTile_y ? sizeTile_x : sizeTile_y;
    redraw();
}

bool Map::save(ofstream* fich){
    *fich<<rows_<<" "<<cols_<<endl;
    for(int i=0;i<rows_;i++){
        for(int j=0;j<cols_;j++){
            *fich<<(short)map_[pos(j,i)].type_<<" ";
        }
        *fich<<endl;
    }
    return true;
}

void Map::setPencil(CellTile pencil) {
    pencil_=pencil;
}

QList<CellWeight> Map::getAroundCells(int col, int row) {
    QList<CellWeight> listCells;
    auto helper = [=, &listCells] (int i, int j) {
        if (col+i < cols_ && row+j < rows_ && map_[pos(col+i,row+j)].type_ != RedTile) {
            Cell cell = map_[pos(col+i,row+j)];
            listCells.push_back({col+i,row+j,cell.type_});
        }
    };
    helper(1,0);
    helper(0,1);
    helper(-1,0);
    helper(0,-1);
    return listCells;
}

void Map::addAgent(int col, int row) {
    static int count;  // esto no mola mucho pero bueno
    count++;
    Agent * agent = new Agent(col,row);
    agents_.push_back({drawPixmap(col,row,agentDirections_[Down]),agent, count});
    emit newAgent(this,count);
    QPropertyAnimation * anim= new QPropertyAnimation(agents_.back().pix_, "pos");
    anim->setDuration(speed_);
    //anim->setStartValue(QPointF(0,0));
    anim->setEndValue(QPointF(100,100));
    anim->start();
}

void Map::moveAgent(Pos before, Pos current, QAgent & agent) {
    agent.pix_->setPos(sizeTile_*current.x_, sizeTile_*current.y_);
    // agent->pix_->setScale(sizeTile_/32);
}

void Map::removeAgent(int id) {
    for (auto it = agents_.begin(); it < agents_.end(); it++) {
        if (it->id_ == id) {
            scene()->removeItem(it->pix_);
            agents_.erase(it);
        }
    }
}

void Map::speedMove(int speed) {
    timer_->setInterval(speed);
    speed_ = speed;
}


void Map::startAI(void) {
    timer_->start();
    connect(timer_,&QTimer::timeout,this, &Map::tick);

}

void Map::tick() {
    for (auto &i : agents_) {
        Pos posBefore = {i.agent_->x_, i.agent_->y_};
        Pos current = (*i.agent_)(getAroundCells(i.agent_->x_,i.agent_->y_));
        moveAgent(posBefore,current,i);
    }
}

void Map::stopAI(void) {
    timer_->stop();
    disconnect(timer_,&QTimer::timeout,this, &Map::tick);
}
