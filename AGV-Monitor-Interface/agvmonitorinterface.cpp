#include "agvmonitorinterface.h"
#include "ui_agvmonitorinterface.h"
#include "widgetstatus.h"

AGVMonitorInterface::AGVMonitorInterface(QWidget *parent) : QMainWindow(parent), ui(new Ui::AGVMonitorInterface){
    ui->setupUi(this);
    this->setStyleSheet(
                "QMainWindow{"
                "background-color: rgb(52, 73, 94);"
                "}"
                "#agv_title{"
                "background-color: rgb(52, 73, 94);"
                "border: none;"
                "}"
                "#lcurrent, #lbattery, #lcheckpoint, #lxpos, #lypos{"
                "color: rgb(236, 240, 241);"
                "}"
                "#agv_stat_current, #agv_stat_batt, #agv_stat_cp, #agv_stat_xpos, #agv_stat_ypos{"
                "background-color: rgb(52, 73, 94);"
                "border-radius: 10px;"
                "color: rgb(236, 240, 241);"
                "}"
                "QTabWidget::pane {"
                "    border: 1px solid rgba(52, 73, 94, 70);"
                "    background: rgb(44, 62, 80);"
                "    color: rgb(236, 240, 241);"
                "}"
                "QTabWidget::tab-bar:top {"
                "    top: 1px;"
                "}"
                "QTabWidget::tab-bar:bottom {"
                "    bottom: 1px;"
                "}"
                "QTabWidget::tab-bar:left {"
                "    right: 1px;"
                "}"
                "QTabWidget::tab-bar:right {"
                "    left: 1px;"
                "}"
                "QTabBar::tab {"
                "    border: 1px solid rgba(52, 73, 94, 70);"
                "    color: rgb(236, 240, 241);"
                "}"
                "QTabBar::tab:selected {"
                "    background: rgb(44, 62, 80);"
                "}"
                "QTabBar::tab:!selected {"
                "    background: rgba(44, 62, 80, 150);"
                "}"
                "QTabBar::tab:!selected:hover {"
                "    background: rgba(44, 62, 80, 80);"
                "}"
                "QTabBar::tab:top:!selected {"
                "    margin-top: 3px;"
                "}"
                "QTabBar::tab:bottom:!selected {"
                "    margin-bottom: 3px;"
                "}"
                "QTabBar::tab:top, QTabBar::tab:bottom {"
                "    min-width: 8ex;"
                "    margin-right: -1px;"
                "    padding: 5px 10px 5px 10px;"
                "}"
                "QTabBar::tab:top:selected {"
                "    border-bottom-color: none;"
                "}"
                "QTabBar::tab:bottom:selected {"
                "    border-top-color: none;"
                "}"
                "QTabBar::tab:top:last, QTabBar::tab:bottom:last,"
                "QTabBar::tab:top:only-one, QTabBar::tab:bottom:only-one {"
                "    margin-right: 0;"
                "}"
                "QTabBar::tab:left:!selected {"
                "    margin-right: 3px;"
                "}"
                "QTabBar::tab:right:!selected {"
                "    margin-left: 3px;"
                "}"
                "QTabBar::tab:left, QTabBar::tab:right {"
                "    min-height: 8ex;"
                "    margin-bottom: -1px;"
                "    padding: 10px 5px 10px 5px;"
                "}"
                "QTabBar::tab:left:selected {"
                "    border-left-color: none;"
                "}"
                "QTabBar::tab:right:selected {"
                "    border-right-color: none;"
                "}"
                "QTabBar::tab:left:last, QTabBar::tab:right:last,"
                "QTabBar::tab:left:only-one, QTabBar::tab:right:only-one {"
                "    margin-bottom: 0;"
                "}"
                "QLineEdit{"
                "   background-color: rgb(44, 62, 80);"
                "   color: rgb(236, 240, 241);"
                "   border: 2px solid rgb(236, 240, 241);"
                "   border-left: none;"
                "   border-right: none;"
                "   border-top: none;"
                "   padding: 5px;"
                "}"
                "QLineEdit:focus{"
                "   background-color: rgb(52, 73, 94);"
                "}"
                "QPushButton{"
                "   background-color: rgb(52, 152, 219);"
                "   color: rgb(236, 240, 241);"
                "   border: 5px solid rgb(41, 128, 185);"
                "   border-left: none;"
                "   border-right: none;"
                "   border-top: none;"
                "   font-size: 11pt;"
                "}"
                "QPushButton:hover{"
                "   margin: 1px;"
                "   background-color: rgb(41, 128, 185);"
                "   font-size: 10pt;"
                "}"
                "QPushButton:pressed{"
                "   background-color: rgb(9, 132, 227);"
                "   border: none;"
                "}"
                "QCheckBox{"
                "   color: rgb(236, 240, 241);"
                "}"
                "#agv_conf_browse, #agv_conf_connect{"
                "   background-color: rgb(52, 152, 219);"
                "   color: rgb(236, 240, 241);"
                "   border: none;"
                "   font-size: 11pt;"
                "}"
                "#agv_conf_browse:hover, #agv_conf_connect:hover{"
                "   margin: 1px;"
                "   background-color: rgb(41, 128, 185);"
                "   font-size: 10pt;"
                "}"
                "#agv_conf_browse:pressed, #agv_conf_connect:pressed{"
                "   background-color: rgb(9, 132, 227);"
                "   padding-top: 3px;"
                "}"
                "#monitor_render{"
                "   background-color: rgb(245, 246, 250);"
                "}");

    this->showMaximized();
    AGVMonitorInterface::makeplot();
}

AGVMonitorInterface::~AGVMonitorInterface(){
    delete ui;
}

void AGVMonitorInterface::parsejsondevice(QNetworkReply *netreply){
    if(netreply->error() == QNetworkReply::NoError){
        QByteArray reply = netreply->readAll();
        QJsonDocument jresponse = QJsonDocument::fromJson(reply);
        QJsonObject jresobj = jresponse.object();

        ui->agv_stat_current->setText(statwrite(jresobj["current"].toInt()));
        ui->agv_stat_batt->setText(statwrite(jresobj["battery"].toInt()));
        ui->agv_stat_cp->setText(statwrite(jresobj["checkpoint"].toInt()));
        ui->agv_stat_xpos->setText(statwrite(jresobj["posx"].toInt(), 18));
        ui->agv_stat_ypos->setText(statwrite(jresobj["posy"].toInt(), 18));

        widget_status.setDevicePositionX(jresobj["posx"].toInt());
        widget_status.setDevicePositionY(jresobj["posy"].toInt());
        widget_status.setDeviceAngle(jresobj["head"].toInt());

        reply.clear();
    }else{
        qDebug() << "[-] Error occured while trying to connect device!";
        tnetwork->stop();
        QMessageBox::warning(0,"Device connection","Error occured while connecting to device!\n" + netreply->errorString());
    }

    netreply->deleteLater();
    netdevice->deleteLater();
    netreply->close();
}

void AGVMonitorInterface::timerinterrupt(){
    netdevice = new QNetworkAccessManager(this);
    //QNetworkAccessManager netdevice;
    connect(netdevice, &QNetworkAccessManager::finished, this, &AGVMonitorInterface::parsejsondevice);

    QString reqtext = "request";
    QUrlQuery query;
    query.addQueryItem("request", reqtext);

    QUrl deviceurl(ui->agv_conf_addr->text());
    deviceurl.setQuery(query);

    netdevice->get(QNetworkRequest(deviceurl));

    ui->monitor_render->update();
    ui->monitor_render->update();

    query.clear();
    deviceurl.clear();
    netdevice->clearConnectionCache();
    netdevice->clearAccessCache();
    //netdevice->deleteLater();
}

QString AGVMonitorInterface::statwrite(int data, int fsize){
    return ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
            "<html><head><meta name=\"qrichtext\" content=\"1\" />"
            "<style type=\"text/css\">"
            "p, li { white-space: pre-wrap; }"
            "</style></head>"
            "<body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
            "<span style=\" font-size:" +QString::number(fsize)+ "pt;\">" + QString::number(data) +"</span></p></body></html>");
}

double AGVMonitorInterface::convtopixel(int mm){
    return (mm*3.7795275591);
}

double AGVMonitorInterface::convtomm(int px){
    return (px*0.2645833333);
}

void AGVMonitorInterface::makeplot(){
    tnetwork = new QTimer(this);
    connect(tnetwork, SIGNAL(timeout()), this, SLOT(timerinterrupt()));
}

void AGVMonitorInterface::clearplot(int type){
    if(type == CLR_ALL){
        agv_map_vectx.clear();
        agv_map_vecty.clear();

        agv_track_vectx.clear();
        agv_track_vecty.clear();

        agv_vectx.clear();
        agv_vecty.clear();
        qDebug() << "[+] All vector cleared";
    }else if(type == CLR_MAP){
        agv_map_vectx.clear();
        agv_map_vecty.clear();
        qDebug() << "[+] Map vector cleared";
    }else if(type == CLR_TRACK){
        agv_track_vectx.clear();
        agv_track_vecty.clear();
        qDebug() << "[+] Track vector cleared";
    }

    makeplot();
}

void AGVMonitorInterface::on_agv_conf_browse_clicked(){
    QString map_flname = QFileDialog::getOpenFileName(this);
    ui->agv_conf_map->setText(map_flname);
    translatemap(map_flname);
}

void AGVMonitorInterface::translatemap(QString flname){
    QFile file(flname);
    QString txtline;
    QString rcoord; // raw coordinate we still didn't know x or y
    double coordx, coordy;
    bool flag = false; // sign for coordinate x or y; false = x; true = y

    if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "[*] Opening map file! :" << flname;
        clearplot(CLR_ALL);
        widget_status.clearFreeHandDraw();
        QTextStream readtxt(&file);

        while(!readtxt.atEnd()){
            txtline = readtxt.readLine();

            for(uint8_t x=0; x<txtline.length(); x++){
                if(txtline[x] != ';')
                    rcoord.append(txtline[x]);
                else{
                    if(!flag){ // if x coord
                        coordx = rcoord.toDouble();
                        rcoord = "";
                        widget_status.setFreeHandDrawAsMapX(coordx);
                        flag = true;
                    }else{ // if y coord
                        coordy = rcoord.toDouble();
                        rcoord = "";
                        widget_status.setFreeHandDrawAsMapY(coordy);
                        flag = false;
                    }
                    //qDebug() << txtline << " | X : " << convtopixel(coordx) << "Y : " << convtopixel(coordy);
                    continue;
                }
            }
        }
        qDebug() << "[+] Done reading the map!";
    }else{
        qDebug() << "[-] Error while opening map file!";
        QMessageBox::warning(0, "Error", "Error while opening map\n\n" + file.errorString());
        return;
    }

    file.close();

    widget_status.setRenderMapStatus(true);
    ui->monitor_render->update();
    ui->monitor_render->update();
}

void AGVMonitorInterface::on_agv_conf_done_clicked(){
    if(ui->agv_conf_fhand->isChecked()){
        ui->agv_conf_fhand->setCheckState(Qt::Unchecked);

        agv_map_vectx = widget_status.getFreeHandDrawAsMapX();
        agv_map_vecty = widget_status.getFreeHandDrawAsMapY();

        QString flname = QFileDialog::getSaveFileName(this);
        QFile flw(flname);

        if(flw.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "[*] Writing map file.. :" << flname;
            QTextStream flwrite(&flw);
            for(int x=0; x<agv_map_vectx.length(); x++){
                flwrite << agv_map_vectx[x] << ";" << agv_map_vecty[x] << ";\n";
            }
            flwrite << agv_map_vectx[0] << ";" << agv_map_vecty[0] << ";\n";

            qDebug() << "[+] Done writing map file!";
        }

        agv_map_vectx.clear();
        agv_map_vecty.clear();
    }

    makeplot();
}

void AGVMonitorInterface::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() == Qt::ControlModifier){
        switch(event->key()){
            case Qt::Key_R:
                //ui->monitor_render->update();
                break;
        }
    }
}

void AGVMonitorInterface::on_agv_conf_fhand_stateChanged(int arg1){
    if(arg1 != 0){
        //clearplot(CLR_MAP);
        /*if(tnetwork->isActive()){
            ui->agv_conf_fhand->setCheckState(Qt::Unchecked);
            QMessageBox::warning(0,"Network", "Please disconnect the device first!");
            return;
        }*/

        widget_status.setFreeHandDrawStatus(true);
    }else{
        widget_status.setFreeHandDrawStatus(false);
    }
}

void AGVMonitorInterface::on_agv_conf_connect_clicked(){
    if((ui->agv_conf_addr->text() != "")){
        if(!tnetwork->isActive()){
            tnetwork->start(500);
            widget_status.setNetworkStatusOK(true);
        }else{
            tnetwork->stop();
            widget_status.setNetworkStatusOK(false);
        }
    }
}

void AGVMonitorInterface::on_agv_conf_reset_clicked(){
    tnetwork->stop();
    clearplot(CLR_ALL);
    ui->agv_conf_fhand->setCheckState(Qt::Unchecked);
    ui->agv_conf_show->setCheckState(Qt::Unchecked);
    first_draw_map = true;

    widget_status.setRenderMapStatus(false);
    widget_status.setClearStatusMap(true);
    ui->monitor_render->update();
    ui->monitor_render->update();
    render.resetRenderView();

    makeplot();
}

void AGVMonitorInterface::on_agv_conf_show_stateChanged(int arg1){
    if(arg1 != 0){
        if(!tnetwork->isActive()){
            ui->agv_conf_show->setCheckState(Qt::Unchecked);
            QMessageBox::warning(0,"Network","Connect to devices first!");
            return;
        }
        widget_status.setDeviceTrackHistory(true);
    }else
        widget_status.setDeviceTrackHistory(false);
}
