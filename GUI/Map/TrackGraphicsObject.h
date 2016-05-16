#ifndef TRACK_GRAPHICS_OBJECT_H
#define TRACK_GRAPHICS_OBJECT_H

#include "PointGraphicsObject.h"
#include <QGraphicsLineItem>
#include <QColor>

class StateGraphicsObject;
class TransitionGraphicsObject;

enum class TrackDirection { NONE, FROM_FIRST_TO_SECOND, FROM_SECOND_TO_FIRST, BOTH };

class TrackGraphicsObject : public PointGraphicsObject
{
public:
    TrackGraphicsObject(int object_id, StateGraphicsObject* state, TransitionGraphicsObject* transition,
        TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor fill_color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackGraphicsObject(int object_id, StateGraphicsObject* state1, StateGraphicsObject* state2,
        TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor fill_color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackDirection getDirection() const;
    void setDirection(TrackDirection direction);
    float getX1() const;
    float getY1() const;
    float getX2() const;
    float getY2() const;
    float getLength() const;
    void setCenter(float x, float y);
    QPointF getCenter() const;
    void setFillColor(QColor color);
    float getWidth() const;
    void setWidth(float width);
    void setLine(StateGraphicsObject* state, TransitionGraphicsObject* transition);
    void setLine(StateGraphicsObject* state1, StateGraphicsObject* state2);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select();
    void deselect();
    QPointF getPointFromScaledLine(float scale_factor);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

private:
    QLineF expandLine(float width);
    QPolygonF getBoundingPolygon(float width);
    void setDrawDisable();
    QPointF getLineIntersection(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

protected:
    QLineF m_arrow_parts[4];
    TrackDirection m_direction;
    float m_angle;
    bool m_drawEnable;
    float m_x1;
    float m_x2;
    float m_y1;
    float m_y2;
    float m_length;
    float m_width;
    float m_arrowLenght;
    float m_arrowWidth;
    bool m_isLinkWithTransition;
};

#endif // TRACK_GRAPHICS_OBJECT_H
