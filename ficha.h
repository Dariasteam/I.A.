#ifndef FICHA_H
#define FICHA_H

#include <QGroupBox>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QBoxLayout>
#include <QString>
#include <QColorDialog>
#include <QColor>

class ficha : public QGroupBox{
public:
    ficha(QString texto, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    void mousePressEvent(QMouseEvent* );
    QColor getColor();
private:
    QLabel          labelBot_;
    QLabel          labelText_;
    QLabel          labelColor_;
    QBoxLayout*     lay_;
    bool            activo_;
    QWidget*        parent_;
    QColor          color_;
signals:

public slots:
};

#endif // FICHA_H
