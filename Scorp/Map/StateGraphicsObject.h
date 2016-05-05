#ifndef STATE_GRAPHICS_OBJECT_H
#define STATE_GRAPHICS_OBJECT_H

#include "PointGraphicsObject.h"
#include <QRectF>

//static int paintStateCounter = 0;

class StateGraphicsObject : public PointGraphicsObject
{
    Q_OBJECT
public:
    StateGraphicsObject(int id, float center_x, float center_y, float radius, QColor fill_color = QColor::fromRgb(255, 0, 0),
                  QGraphicsItem* parent = nullptr);
    float getRadius() const;
    void setRadius(float radius);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    //QPainterPath shape() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select();
    void deselect();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

protected:
    float m_radius;
};

#endif // STATE_GRAPHICS_OBJECT_H
