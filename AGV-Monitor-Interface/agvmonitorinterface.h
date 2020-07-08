#ifndef AGVMONITORINTERFACE_H
#define AGVMONITORINTERFACE_H

#define CLR_ALL 0
#define CLR_MAP 1
#define CLR_TRACK 2

#include <QMainWindow>

/*********** USER LIBRARY ***********/
#include <QGraphicsScene>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QTimerEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QPixmap>
#include <QKeyEvent>

#include "monitorrender.h"
/*********** USER LIBRARY ***********/

QT_BEGIN_NAMESPACE
namespace Ui { class AGVMonitorInterface; }
QT_END_NAMESPACE

class AGVMonitorInterface : public QMainWindow{
    Q_OBJECT

public:
    AGVMonitorInterface(QWidget *parent = nullptr);
    ~AGVMonitorInterface();
    QVector<double> agv_vectx, agv_vecty;
    QVector<double> agv_track_vectx, agv_track_vecty;
    QVector<double> agv_map_vectx, agv_map_vecty;

private slots:
    void makeplot();
    void on_agv_conf_browse_clicked();
    void on_agv_conf_done_clicked();
    void keyPressEvent(QKeyEvent *event);

    void on_agv_conf_fhand_stateChanged(int arg1);
    void timerinterrupt();
    void parsejsondevice(QNetworkReply *netreply);
    void on_agv_conf_connect_clicked();
    void on_agv_conf_reset_clicked();
    void on_agv_conf_show_stateChanged(int arg1);

private:
    Ui::AGVMonitorInterface *ui;

    QString statwrite(int data, int fsize=55);
    void translatemap(QString flname);
    void clearplot(int type);
    double convtopixel(int mm);

    QPoint last_point;
    QPoint last_track;
    bool enable_track = false;
    bool first_draw_map = true;

    QTimer *tnetwork;
    QNetworkAccessManager *netdevice;
    double convtomm(int px);
    monitorrender render;
};
#endif // AGVMONITORINTERFACE_H
