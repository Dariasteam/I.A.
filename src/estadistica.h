#ifndef ESTADISTICA_H
#define ESTADISTICA_H

#include <QWidget>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "agente/agente.h"

class Estadistica : public QWidget
{
    Q_OBJECT
private:
    QTreeWidget * dataTree_;
public:
    explicit Estadistica(QWidget *parent = 0);

signals:
public slots:
    void addAgentInfo(agente * agent);
    void reset();
};

#endif // ESTADISTICA_H
