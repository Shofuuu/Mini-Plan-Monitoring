#include "monitorrender.h"
#include "widgetstatus.h"

monitorrender::monitorrender(QWidget *parent) : QWidget(parent){
}

void monitorrender::mousePressEvent(QMouseEvent *event){
    if(widget_status.getFreeHandDrawStatus()){
        point.setX(event->pos().x());
        point.setY(event->pos().y());

        widget_status.setFreeHandDrawAsMapX(event->pos().x());
        widget_status.setFreeHandDrawAsMapY(event->pos().y());

        if(old_point.isNull()){
            old_point.setX(point.x());
            old_point.setY(point.y());
        }

        QLineF new_line(old_point.x(), old_point.y(), point.x(), point.y());
        drawing_line.append(new_line);

        old_point.setX(point.x());
        old_point.setY(point.y());

        msevent = true;
        update();
    }
}

void monitorrender::mouseReleaseEvent(QMouseEvent *event){
    msevent = false;

    if(widget_status.getFreeHandDrawStatus())
        update();
}

void monitorrender::paintEvent(QPaintEvent *event){
    QPainter painter_map(this);
    QPen pen_map;

    if(msevent){
        /*QPainter painter_map(this);
        QPen pen_map;*/

        QPainter dof(this);
        QPen pen_dof;

        pen_map.setWidth(3);
        pen_map.setColor(QColor(41, 128, 185));
        pen_dof.setColor(QColor(41, 128, 185));
        pen_dof.setWidth(3);

        painter_map.setRenderHints(QPainter::HighQualityAntialiasing);
        painter_map.setPen(pen_map);
        dof.setRenderHints(QPainter::HighQualityAntialiasing);
        dof.setPen(pen_dof);

        dof.drawEllipse(point.x()-3, point.y()-3, 6, 6);
        if(!widget_status.getNetworkStatusOK()){
            if(drawing_line.length() > 0)
                painter_map.drawLines(drawing_line);
        }
    }

    if(widget_status.getRenderMapStatus()){
        draw_map_x = widget_status.getFreeHandDrawAsMapX();
        draw_map_y = widget_status.getFreeHandDrawAsMapY();

        QPainter painter(this);
        QPen pen;

        for(int x=0; x<(widget_status.getFreeHandDrawASize()-1);x++){
            pen.setWidth(3);
            pen.setColor(QColor(44, 62, 80));

            painter.setPen(pen);
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            painter.drawLine(draw_map_x[x], draw_map_y[x], draw_map_x[x+1], draw_map_y[x+1]);
        }
    }

    if(widget_status.getNetworkStatusOK()){
        QPainter painter(this);
        QPen pen;
        QPixmap pixmap;
        QMatrix matrotate;

        pixmap.load(":/Logo/citra/agv.png");
        pixmap = pixmap.scaled(50,50);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        //painter.translate(QPointF(pixmap.width()/2.0, pixmap.height()/2.0));
        matrotate.rotate(widget_status.getDeviceAngle());
        pixmap = pixmap.transformed(matrotate);
        painter.drawPixmap(widget_status.getDevicePositionX() - (pixmap.width()/2.0),
                           widget_status.getDevicePositionY() - (pixmap.width()/2.0),
                           pixmap);

        QLineF line_history(widget_status.getDevicePositionX(),
                           widget_status.getDevicePositionY(),
                           widget_status.getDevicePositionX(),
                           widget_status.getDevicePositionY());
        drawing_history.append(line_history);

        /*qDebug() << "Devices :" << widget_status.getDeviceAngle()
                 << widget_status.getDevicePositionX()
                 << widget_status.getDevicePositionY();*/

        if(widget_status.getDeviceTrackHistory()){
            pen.setWidth(3);
            pen.setColor(QColor(41, 128, 185));

            painter.setPen(pen);

            if(drawing_history.length() > 0)
                painter.drawLines(drawing_history);
        }


        //if(!widget_status.getNetworkStatusOK()){
            pen_map.setWidth(3);
            pen_map.setColor(QColor(41, 128, 185));

            painter_map.setRenderHints(QPainter::HighQualityAntialiasing);
            painter_map.setPen(pen_map);

            if(drawing_line.length() > 0)
                painter_map.drawLines(drawing_line);

        //}
    }

    if(widget_status.getClearStatusMap()){
        drawing_line.clear();
        drawing_history.clear();
        widget_status.setClearStatusMap(false);
        update();
    }

    /*if(drawing_line.length() > 0)
        painter_map.drawLines(drawing_line);*/
}

void monitorrender::resetRenderView(){
    drawing_line.clear();
    draw_map_x.clear();
    draw_map_y.clear();
    widget_status.clearFreeHandDraw();
    widget_status.setClearStatusMap(true);
    widget_status.setRenderMapStatus(false);

    update();
}
