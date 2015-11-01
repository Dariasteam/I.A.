#include "mapoptions.h"
#include "ui_mapoptions.h"
#include <iostream>

MapOptions::MapOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapOptions)
{
    ui->setupUi(this);
    connect(ui->generar_Boton,&QPushButton::clicked,this,&MapOptions::onSpawn);
    connect(ui->muro_slider,&QSlider::valueChanged,this,([=] (int) { onSliderUpdate(0);}));
    connect(ui->tierra_slider,&QSlider::valueChanged,this,([=] (int) { onSliderUpdate(1);}));
    connect(ui->metal_slider,&QSlider::valueChanged,this,([=] (int) { onSliderUpdate(2);}));
    connect(ui->rejilla_slider,&QSlider::valueChanged,this,([=] (int) { onSliderUpdate(3);}));
    terrainSliders_ = {{ui->muro_slider, 0}, {ui->tierra_slider, 0}, {ui->metal_slider, 0}, {ui->rejilla_slider, 0}};
}

MapOptions::~MapOptions()
{
    delete ui;
}

void MapOptions::onSliderUpdate(int i) {

    auto map = [=] (function<void (TerrainSlider*)> fn) {
        for (int i = 0; i < terrainSliders_.size(); i++) {
            fn(&terrainSliders_[i]);
        }
    };

    terrainSliders_[i].oldValue_ = terrainSliders_[i].slider_->value();
    int sum = 0;
    map([&sum](TerrainSlider* tSlider) {
        sum += tSlider->slider_->value();
    });

    if (sum > 50) {
        float factorReduction =sum-50;
        int total = 0;
        map([&total] (TerrainSlider* tSlider) {
            total += tSlider->slider_->value()>0;
        });
        factorReduction = factorReduction / (total - 1);

        if (factorReduction < INFINITY) {
            map([=] (TerrainSlider* tSlider) {
                if ((tSlider != &terrainSliders_[i])
                        && tSlider->oldValue_ > fabs(tSlider->oldValue_ - (factorReduction*tSlider->oldValue_>0))
                        && tSlider->oldValue_ > 0) {
                    tSlider->oldValue_ = fabs(tSlider->oldValue_ - (factorReduction*tSlider->oldValue_>0));
                    tSlider->slider_->setValue(tSlider->oldValue_);
                }
            });
        }
    }
}

int MapOptions::cols() {
    return ui->spin_cols->value();
}

int MapOptions::rows() {
    return ui->spin_rows->value();
}

int MapOptions::muro() {
    return ui->muro_slider->value();
}

int MapOptions::tierra() {
    return ui->tierra_slider->value();
}

int MapOptions::rejilla() {
    return ui->rejilla_slider->value();
}

int MapOptions::metal() {
    return ui->metal_slider->value();
}
