#include "estadistica.h"

Estadistica::Estadistica(QWidget *parent) : QWidget(parent)
{
    setLayout(new QBoxLayout(QBoxLayout::TopToBottom,this));
    dataTree_ = new QTreeWidget(this);
    layout()->addWidget(dataTree_);
}


void Estadistica::addAgentInfo(agente *agent) {
    QTreeWidgetItem * infoAgent = new QTreeWidgetItem();
    infoAgent->setText(0, agent->nombre()+ " " + QString::number(agent->pasos()) + " pasos");
    dataTree_->addTopLevelItem(infoAgent);
    disconnect(agent,&agente::terminado, this,&Estadistica::addAgentInfo);
}

void Estadistica::reset() {
}
