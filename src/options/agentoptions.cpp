#include "agentoptions.h"
#include "ui_agentoptions.h"

AgentOptions::AgentOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AgentOptions)
{
    ui->setupUi(this);
    ui->horizontalLayout->replaceWidget(ui->dropbot, new DragAgent(this));

    connect(ui->simulation,&QPushButton::clicked, this, &AgentOptions::simulate);
    connect(ui->step,      &QPushButton::clicked, this, &AgentOptions::step);
    connect(ui->speedMove,&QSlider::valueChanged, this, &AgentOptions::speed);
}

AgentOptions::~AgentOptions()
{
    delete ui;
}

void AgentOptions::createAgentInfo(Map *map, int id) {
    AgentToken * agentToken = new AgentToken(map,id);
    connect(map,SIGNAL(destroyed(QObject*)),agentToken, SLOT(close()));
    ui->agentScrollList->addWidget(agentToken);
}
