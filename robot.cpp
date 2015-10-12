#include "robot.h"
#include <QDebug>

robot::robot(int i, int j, QPixmap* robot,QWidget *parent) :
    QLabel(parent)
{
    i_ = i;
    j_ = j;
    setMinimumSize(0,0);
    pixRobot_ = robot;
    setPixmap(pixRobot_->scaled(this->width(),this->height()));
}

robot::~robot(){

}


void robot::resizeEvent(QResizeEvent* E){
   setPixmap(pixRobot_->scaled(this->width(),this->height()));
}
