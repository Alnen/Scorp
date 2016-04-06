#ifndef TRANSITION_GRAPHICS_OBJECT_H
#define TRANSITION_GRAPHICS_OBJECT_H

#include "PointGraphicsObject.h"
#include <QRectF>

#include <QObject>

class TransitionGraphicsObject : public PointGraphicsObject
{
public:
    TransitionGraphicsObject(float center_x, float center_y, float width, float height, QColor fill_color = QColor::fromRgb(0, 0, 0),
                  QGraphicsItem* parent = nullptr);
    float getWidth();
    void setWidth(float width);
    float getHeight();
    void setHeight(float height);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

protected:
    float m_width;
    float m_height;
};

#endif // TRANSITION_GRAPHICS_OBJECT_H
