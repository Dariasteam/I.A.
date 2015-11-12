#include "estadistica.h"

Estadistica::Estadistica(QWidget *parent) : QWidget(parent)
{
    setLayout(new QBoxLayout(QBoxLayout::TopToBottom,this));
    dataTree_ = new QTreeWidget(this);
    dataTree_->setHeaderLabels(QStringList()<< "Medida"<< "Valor");
    layout()->addWidget(dataTree_);
}


void Estadistica::addAgentInfo(agente *agent) {
    QTreeWidgetItem * infoAgent = new QTreeWidgetItem();
    infoAgent->setText(0, agent->nombre());
    QTreeWidgetItem * infoPasos = new QTreeWidgetItem();
    infoPasos->setText(0,"Pasos Totales");
    infoPasos->setText(1,QString::number(agent->pasos()) + " pasos");
    QTreeWidgetItem * infoPasosCamino = new QTreeWidgetItem();
    infoPasosCamino->setText(0,"Camino minimo en");
    infoPasosCamino->setText(1,agent->pasosCaminoOptimo() +" pasos");
    QTreeWidgetItem * porcentajeExplorado = new QTreeWidgetItem();
    porcentajeExplorado->setText(0,"Camino minimo en");
    //porcentajeExplorado->setText(1,agent->porcentajeExplorado() +" pasos");
    infoAgent->addChild(infoPasos);
    infoAgent->addChild(infoPasosCamino);
    infoAgent->addChild(porcentajeExplorado);
    dataTree_->addTopLevelItem(infoAgent);
    disconnect(agent,&agente::terminado, this,&Estadistica::addAgentInfo);
}

void Estadistica::reset() {
    dataTree_->clear();
}
