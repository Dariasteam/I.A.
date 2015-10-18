#include "mapoptions.h"
#include "ui_mapoptions.h"

MapOptions::MapOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapOptions)
{
    ui->setupUi(this);
    connect(ui->generar_Boton,SIGNAL(clicked(bool)),this,SIGNAL(onSpawn()));
    connect(ui->muro_slider,SIGNAL(valueChanged(int)),this,SLOT(onSliderUpdate()));
    connect(ui->tierra_slider,SIGNAL(valueChanged(int)),this,SLOT(onSliderUpdate()));
    connect(ui->metal_slider,SIGNAL(valueChanged(int)),this,SLOT(onSliderUpdate()));
    connect(ui->rejilla_slider,SIGNAL(valueChanged(int)),this,SLOT(onSliderUpdate()));
}

MapOptions::~MapOptions()
{
    delete ui;
}

void MapOptions::onSliderUpdate() {
    vector<QSlider *> sliders = {ui->muro_slider, ui->tierra_slider, ui->metal_slider, ui->rejilla_slider};

    //    int i=0;
    //    while(i<4 && editoresTerreno_[i].valorAnterior_==editoresTerreno_[i].slider_->value()){
    //        i++;
    //    }
    //    if(i<4){
    //        // if sum . map x.slider_->value editoresTerreno_  > 50
    //        if((editoresTerreno_[0].slider_->value() +
    //            editoresTerreno_[1].slider_->value() +
    //            editoresTerreno_[2].slider_->value() +
    //            editoresTerreno_[3].slider_->value())  > 50){

    //            // 50 - sum . map x.slider_->value editoresTerreno_
    //            float factorReduccion =
    //            editoresTerreno_[0].slider_->value() +
    //            editoresTerreno_[1].slider_->value() +
    //            editoresTerreno_[2].slider_->value() +
    //            editoresTerreno_[3].slider_->value() - 50;

    //            // factorReduccion / (sum . map (if x.slider_->value > 0 then 1 else 0)-1)
    //            factorReduccion = factorReduccion /
    //            (((editoresTerreno_[0].slider_->value()>0)) +
    //             ((editoresTerreno_[1].slider_->value()>0)) +
    //             ((editoresTerreno_[2].slider_->value()>0)) +
    //             ((editoresTerreno_[3].slider_->value()>0))-1);

    //            if(factorReduccion>0 && factorReduccion<INFINITY){
    //                // map ()
    //                for(int j=0;j<4;j++){
    //                    if(j!=i && editoresTerreno_[j].valorAnterior_ >
    //                       fabs(editoresTerreno_[j].valorAnterior_-(factorReduccion*editoresTerreno_[j].valorAnterior_>0))
    //                       && editoresTerreno_[j].valorAnterior_>0){

    //                            editoresTerreno_[j].valorAnterior_ = fabs(editoresTerreno_[j].valorAnterior_-
    //                                                                     (factorReduccion*editoresTerreno_[j].valorAnterior_>0));
    //                            editoresTerreno_[j].slider_->setValue(editoresTerreno_[j].valorAnterior_);
    //                            editoresTerreno_[0].label_->setNum(editoresTerreno_[0].valorAnterior_);
    //                    }
    //                }
    //            }
    //        }
    //        editoresTerreno_[i].valorAnterior_ = editoresTerreno_[i].slider_->value();
    //        editoresTerreno_[0].label_->setNum(editoresTerreno_[0].valorAnterior_);
    //        editoresTerreno_[1].label_->setNum(editoresTerreno_[1].valorAnterior_);
    //        editoresTerreno_[2].label_->setNum(editoresTerreno_[2].valorAnterior_);
    //        editoresTerreno_[3].label_->setNum(editoresTerreno_[3].valorAnterior_);
    //    }

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
