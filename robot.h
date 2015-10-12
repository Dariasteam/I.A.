#ifndef ROBOT_H
#define ROBOT_H

#include <QLabel>
#include <QPixmap>

class robot : public QLabel
{
    Q_OBJECT
public:
    robot(int,int,QPixmap*,QWidget *parent = 0);
    ~robot();

    void resizeEvent(QResizeEvent* );
signals:

public slots:

private:
    QPixmap * pixRobot_;
    int i_;
    int j_;

};

#endif // ROBOT_H
