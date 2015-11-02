#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QToolBox>

#include "ui_mainwindow.h"
#include "options/mapoptions.h"
#include "options/agentoptions.h"
#include "map/map.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void connectMap();
    void disconnectMap();
signals:

public slots:
    void onOpen(void);
    void onSaveAs(void);
    void onSave(void);
    void onUpdateMap(void);
private:
    Ui::MainWindow *ui;
    Map * widMap_;
    MapOptions * mapOptions_;
    AgentOptions * agentOptions_;
    QString * rutaArchivo_;
    QFileDialog * dialogoAbrir_;
    QAction * active_;

};

#endif // MAINWINDOW_H
