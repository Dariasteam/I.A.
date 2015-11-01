#include "agenttoken.h"
#include "ui_agenttoken.h"

AgentToken::AgentToken(Map * map, int id, QWidget *parent) :
    QWidget(parent),
    idAgent_(id),
    ui(new Ui::AgentToken)
{
    ui->setupUi(this);
    ui->name->setText("Agent "+QString::fromStdString(std::to_string(idAgent_)));
    connect(ui->del, &QPushButton::clicked, map, [=] (void) { map->removeAgent(idAgent_); close();});
    QPixmap P(":/recursos/testigo.png");
    color_ = QColor(rand()%255+1,rand()%255+1,rand()%255+1);
    P.fill(color_);
    ui->color->setPixmap(P);
}

AgentToken::~AgentToken()
{
    delete ui;
}

void AgentToken::mouseDoubleClickEvent(QMouseEvent* ev){
    if(ev->button() ==Qt::LeftButton){
            QColorDialog* d = new QColorDialog(this);
            QPixmap P(":/recursos/testigo.png");
            color_ = d->getColor();
            P.fill(color_);
            ui->color->setPixmap(P);
    }
}
