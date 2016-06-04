#ifndef TRANSITION_GRAPHICS_OBJECT_H
#define TRANSITION_GRAPHICS_OBJECT_H

#include <QRectF>
#include <QObject>
#include "Scorp/GUI/Map/PointGraphicsObject.h"
#include "Scorp/GUI/Map/MapSceneStyle.h"

class TransitionGraphicsObject : public PointGraphicsObject
{
public:
    TransitionGraphicsObject(int object_id, float center_x, float center_y, float width, float height, QColor fill_color = QColor::fromRgb(0, 0, 0),
        QGraphicsItem* parent = nullptr);
    float getWidth() const;
    void setWidth(float width);
    float getHeight() const;
    void setHeight(float height);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select(bool graphics_selection) override;
    void deselect(bool graphics_selection) override;
    QPolygonF getVerteciesCoord() const;
    QPolygonF getVerteciesOffset() const;
    float getTilt() const;
    void setRotation(qreal angle);
    void setStyle(const MapSceneStyle::TransitionStyle& style);
    MapSceneStyle::TransitionStyle getStyle() const;

protected:
    void updateTilt();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

protected:
    float m_width;
    float m_height;
    float m_tilt;
    MapSceneStyle::TransitionStyle m_style;
};

#endif // TRANSITION_GRAPHICS_OBJECT_H
