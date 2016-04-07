#ifndef STATE_GRAPHICS_OBJECT_H
#define STATE_GRAPHICS_OBJECT_H

#include "PointGraphicsObject.h"
#include <QRectF>

class StateGraphicsObject : public PointGraphicsObject
{
    Q_OBJECT
public:
    StateGraphicsObject(float center_x, float center_y, float radius, QColor fill_color = QColor::fromRgb(255, 0, 0),
                  QGraphicsItem* parent = nullptr);
    float getRadius();
    void setRadius(float radius);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

protected:
    float m_radius;
};

#endif // STATE_GRAPHICS_OBJECT_H
