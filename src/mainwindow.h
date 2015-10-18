#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "map.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void onOpen(void);
    void onSaveAs(void);
    void onSave(void);
    void onUpdateMap(void);
    void onSliderUpdate(CellTile);
private:
    Ui::MainWindow *ui;
    Map * widMap_;
    QString * rutaArchivo_;
    QFileDialog * dialogoAbrir_;
};

#endif // MAINWINDOW_H
