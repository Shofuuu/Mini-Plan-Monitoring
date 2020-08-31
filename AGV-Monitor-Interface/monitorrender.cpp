#include "monitorrender.h"
#include "widgetstatus.h"
#include <QtMath>

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

        coord_pos_X.append(point.x());
        coord_pos_Y.append(point.y());

        QLineF new_line(old_point.x(), old_point.y(), point.x(), point.y());
        findPathJacobian(new_line, coord_pos_X, coord_pos_Y); // calculate robot track
        drawing_line.append(new_line);

        old_point.setX(point.x());
        old_point.setY(point.y());

        if(coord_pos_X.length() > 2)
            coord_pos_X.remove(0);
        if(coord_pos_Y.length() > 2)
            coord_pos_Y.remove(0);

        msevent = true;
        update();
    }
}

void monitorrender::mouseReleaseEvent(QMouseEvent *event){
    msevent = false;

    if(widget_status.getFreeHandDrawStatus()){
        update();
    }
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
        point.setX(0);
        point.setY(0);
        old_point.setX(0);
        old_point.setY(0);

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

void monitorrender::findPathJacobian(QLineF data, QVector<double> coordMapX, QVector<double> coordMapY){
    QVector<double> coord_X, coord_Y;
    QPointF* point = new QPointF();
    QPixmap* pixelmap = new QPixmap;
    QImage* pixelimg = new QImage;
    QColor* pixelcolor = new QColor;
    *pixelmap = QPixmap::grabWidget(this);
    QPen pen;
    QRgb pixelrgb;

    int x1, y1;
    int x2, y2;

    widget_status.setRenderMapStatus(true);
    pen.setColor(Qt::transparent);

    if(coordMapX.length() > 1){
        coord_X = coordMapX;
        coord_Y = coordMapY;

        x1 = coord_X[0];
        x2 = coord_X[1];
        y1 = coord_Y[0];
        y2 = coord_Y[1];

        /*qDebug() << "\n***** NEW DATA LOADED *****\n"
                 << "X1 : " << x1 << " | Y1 : " << y1
                 << "\nX2 : " << x2 << " | Y2 : " << y2
                 << "\nFlag : (x1 > x2)" << (x1 > x2)
                 << "\nFlag : (y1 > y2)" << (y1 > y2) << "\n";*/

        if((x1 > x2) & (y1 < y2)){
            //qDebug() << "Condition 1/2/8";
            for(int y=y1; y<=y2; y++){
                for(int x=x2; x<=x1; x++){
                    *pixelimg = pixelmap->toImage();
                    pixelrgb = pixelimg->pixel(x, y);
                    pixelcolor->setRgb(pixelrgb);

                    if(pixelcolor->name() == "#2c3e50")
                        qDebug() << x << ";" << y << ";";
                }
            }
        }
        if((x1 > x2) & (y1 > y2)){
            //qDebug() << "Condition 3/5";
            for(int y=y2; y<=y1; y++){
                for(int x=x2; x<=x1; x++){
                    *pixelimg = pixelmap->toImage();
                    pixelrgb = pixelimg->pixel(x, y);
                    pixelcolor->setRgb(pixelrgb);

                    if(pixelcolor->name() == "#2c3e50")
                        qDebug() << x << ";" << y << ";";
                }
            }
        }
        if((x1 < x2) & (y1 > y2)){
            //qDebug() << "Condition 4/6";
            for(int y=y2; y<=y1; y++){
                for(int x=x1; x<=x2; x++){
                    *pixelimg = pixelmap->toImage();
                    pixelrgb = pixelimg->pixel(x, y);
                    pixelcolor->setRgb(pixelrgb);

                    if(pixelcolor->name() == "#2c3e50")
                        qDebug() << x << ";" << y << ";";
                }
            }
        }
        if((x1 < x2) & (y1 < y2)){
            //qDebug() << "Condition 7";
            for(int y=y1; y<=y2; y++){
                for(int x=x1; x<=x2; x++){
                    *pixelimg = pixelmap->toImage();
                    pixelrgb = pixelimg->pixel(x, y);
                    pixelcolor->setRgb(pixelrgb);

                    if(pixelcolor->name() == "#2c3e50")
                        qDebug() << x << ";" << y << ";";
                }
            }
        }
    }
}
