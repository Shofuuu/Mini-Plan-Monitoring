#ifndef WIDGETSTATUS_H
#define WIDGETSTATUS_H

#include <QVector>

class WidgetStatus{
public:
    WidgetStatus();
    void setFreeHandDrawStatus(bool stat);
    bool getFreeHandDrawStatus(void);
    QVector<double> getFreeHandDrawAsMapX(); // 0 for x; 1 for y
    QVector<double> getFreeHandDrawAsMapY(); // 0 for x; 1 for y
    int getFreeHandDrawASize(void);
    void setFreeHandDrawAsMapX(double map);
    void setFreeHandDrawAsMapY(double map);
    void clearFreeHandDraw();
    void setRenderMapStatus(bool status);
    bool getRenderMapStatus(void);
    void setClearStatusMap(bool status);
    bool getClearStatusMap();
    void setNetworkStatusOK(bool status);
    bool getNetworkStatusOK(void);
    void setDevicePositionX(double pos);
    void setDevicePositionY(double pos);
    void setDeviceAngle(int pos);
    double getDevicePositionX(void);
    double getDevicePositionY(void);
    double getDeviceAngle(void);
    void setDeviceTrackHistory(bool status);
    bool getDeviceTrackHistory(void);

private:
    bool freeHandDrawStatus = false;
    bool renderMapStatus = false;
    bool clearrendermap = false;
    bool networkstatusok = false;
    bool devicetrackh = false;
    double devicepositionx;
    double devicepositiony;
    double deviceangle;
    QVector<double> freeMapDataX;
    QVector<double> freeMapDataY;
};

inline WidgetStatus widget_status;

#endif // WIDGETSTATUS_H
