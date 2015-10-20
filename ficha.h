#ifndef FICHA_H
#define FICHA_H

#include <QGroupBox>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QBoxLayout>
#include <QString>
#include <QColorDialog>

class ficha : public QGroupBox{
public:
    ficha(QString texto, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    void keyPressEvent(QKeyEvent* );
private:
    QLabel          labelBot_;
    QLabel          labelText_;
    QLabel          labelColor_;
    QBoxLayout*     lay_;
    bool            activo_;
signals:

public slots:
};

#endif // FICHA_H
