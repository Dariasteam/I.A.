#ifndef MAPOPTIONS_H
#define MAPOPTIONS_H

#include <QWidget>
#include <QSlider>
#include <vector>

using namespace std;

namespace Ui {
class MapOptions;
}

class MapOptions : public QWidget
{
    Q_OBJECT

public:
    explicit MapOptions(QWidget *parent = 0);
    ~MapOptions();
    int cols();
    int rows();
    int muro();
    int tierra();
    int rejilla();
    int metal();
signals:
    void onSpawn(void);
public slots:
    void onSliderUpdate();
private:
    Ui::MapOptions *ui;
};

#endif // MAPOPTIONS_H
