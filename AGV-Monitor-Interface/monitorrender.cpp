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
    if(msevent){
        if(widget_status.getClearStatusMap()){
            drawing_line.clear();
            widget_status.setClearStatusMap(false);
        }

        QPainter painter(this);
        QPen pen;
        QPainter dof(this);
        QPen pen_dof;

        pen.setWidth(3);
        pen_dof.setColor(QColor(41, 128, 185));
        pen_dof.setWidth(3);
        pen.setColor(QColor(41, 128, 185));

        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        dof.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setPen(pen);
        dof.setPen(pen_dof);

        dof.drawEllipse(point.x()-2.5, point.y()-2.5, 5, 5);
        if(drawing_line.length() > 0)
            painter.drawLines(drawing_line);
    }

    if(widget_status.getRenderMapStatus()){
        drawing_line.clear();
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
            if(widget_status.getClearStatusMap()){
                drawing_history.clear();
                widget_status.setClearStatusMap(false);
            }

            pen.setWidth(3);
            pen.setStyle(Qt::DotLine);
            pen.setColor(QColor(41, 128, 185));

            painter.setPen(pen);

            if(drawing_history.length() > 0)
                painter.drawLines(drawing_history);
        }
    }
}

void monitorrender::resetRenderView(){
    drawing_line.clear();
    draw_map_x.clear();
    draw_map_y.clear();
    widget_status.clearFreeHandDraw();

    update();
}
