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
private:
    Ui::MainWindow *ui;
    QWidget* widPrincipal_;
    QGridLayout* layPrincipal_;
    int c_;
    int f_;
private slots:

};

#endif // MAINWINDOW_H
