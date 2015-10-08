#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent* );
private:
    Ui::MainWindow *ui;
    QWidget* widPrincipal_;                     //widget de mapa
    QGridLayout* layPrincipal_;                 //layout de mapa
    int c_;                                     //columnas de la matriz
    int f_;                                     //filas de la matriz
    bool pintar_ =true;                         //true=añadir obstaculos, false=borrar obstaculos
private slots:

};

#endif // MAINWINDOW_H
