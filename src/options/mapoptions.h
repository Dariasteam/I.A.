#ifndef MAPOPTIONS_H
#define MAPOPTIONS_H

#include <QWidget>
#include <QSlider>
#include <vector>
#include <functional>

using namespace std;

struct TerrainSlider {
    QSlider * slider_;
    short oldValue_;
};

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
    void onSliderUpdate(int);
private:
    Ui::MapOptions *ui;
    vector<TerrainSlider> terrainSliders_;
};

#endif // MAPOPTIONS_H
