#include "widgetstatus.h"

WidgetStatus::WidgetStatus()
{

}

void WidgetStatus::setFreeHandDrawStatus(bool stat) {freeHandDrawStatus = stat;}

bool WidgetStatus::getFreeHandDrawStatus(){return freeHandDrawStatus;}

QVector<double> WidgetStatus::getFreeHandDrawAsMapX(){
    return freeMapDataX;
}

QVector<double> WidgetStatus::getFreeHandDrawAsMapY(){
    return freeMapDataY;
}

void WidgetStatus::setFreeHandDrawAsMapX(double map){
        freeMapDataX.append(map);
}

void WidgetStatus::setFreeHandDrawAsMapY(double map){
    freeMapDataY.append(map);
}

void WidgetStatus::clearFreeHandDraw(){
    freeMapDataX.clear();
    freeMapDataY.clear();
}

void WidgetStatus::setRenderMapStatus(bool status){
    renderMapStatus = status;
}

bool WidgetStatus::getRenderMapStatus(){
    return renderMapStatus;
}

void WidgetStatus::setClearStatusMap(bool status){
    clearrendermap = status;
}

bool WidgetStatus::getClearStatusMap(){
    return clearrendermap;
}

void WidgetStatus::setNetworkStatusOK(bool status){
    networkstatusok = status;
}

bool WidgetStatus::getNetworkStatusOK(){
    return networkstatusok;
}

void WidgetStatus::setDevicePositionX(double pos){
    devicepositionx = pos;
}

void WidgetStatus::setDevicePositionY(double pos){
    devicepositiony = pos;
}

void WidgetStatus::setDeviceAngle(int pos){
    deviceangle = pos;
}

double WidgetStatus::getDevicePositionX(){
    return devicepositionx;
}

double WidgetStatus::getDevicePositionY(){
    return devicepositiony;
}

double WidgetStatus::getDeviceAngle(){
    return deviceangle;
}

void WidgetStatus::setDeviceTrackHistory(bool status){
    devicetrackh = status;
}

bool WidgetStatus::getDeviceTrackHistory(){
    return devicetrackh;
}

int WidgetStatus::getFreeHandDrawASize(){
    return freeMapDataX.length();
}
