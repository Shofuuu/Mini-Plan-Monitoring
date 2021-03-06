#ifndef MONITORRENDER_H
#define MONITORRENDER_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QLabel>

class monitorrender : public QWidget{
public:
    monitorrender(QWidget *parent=0);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resetRenderView();
    void findPathJacobian(QLineF data, QVector<double> coordMapX, QVector<double> coordMapY);

private:
    QPoint point, old_point;
    QVector<QLineF> drawing_line;
    QVector<QLineF> drawing_history;
    QVector<double> draw_map_x, draw_map_y;
    QVector<double> coord_pos_X, coord_pos_Y;
    bool msevent = false;
    int old_pos_history_x, old_pos_history_y;
};

#endif // MONITORRENDER_H
