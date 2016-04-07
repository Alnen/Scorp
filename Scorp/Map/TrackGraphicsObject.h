#ifndef TRACK_GRAPHICS_OBJECT_H
#define TRACK_GRAPHICS_OBJECT_H

#include "StateGraphicsObject.h"
#include "TransitionGraphicsObject.h"
#include <QGraphicsLineItem>
#include <QColor>

enum class TrackDirection { FROM_FIRST_TO_SECOND, FROM_SECOND_TO_FIRST, BOTH };

class TrackGraphicsObject : public PointGraphicsObject
{
public:
    TrackGraphicsObject::TrackGraphicsObject(TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor fill_color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackGraphicsObject(StateGraphicsObject* state, TransitionGraphicsObject* transition,
        TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor fill_color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackGraphicsObject(StateGraphicsObject* state1, StateGraphicsObject* state2,
        TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor fill_color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackDirection getDirection();
    void setDirection(TrackDirection direction);
    float getX1();
    float getY1();
    float getX2();
    float getY2();
    float getLength();
    void setFillColor(QColor color);
    float getWidth();
    void setWidth(float width);
    void setLine(StateGraphicsObject* state, TransitionGraphicsObject* transition);
    void setLine(StateGraphicsObject* state1, StateGraphicsObject* state2);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

private:
    QLineF expandLine(float width);
    QPointF getPointByEquation(float x_p, float y_p, float x_n, float k,
                               float b, float length);
    QPolygonF getBoundingPolygon(float width);

protected:
    QPolygonF m_ftos_arrow; // from first to second
    QPolygonF m_stof_arrow; // from second to first
    TrackDirection m_direction;
    float m_x1;
    float m_x2;
    float m_y1;
    float m_y2;
    float m_length;
    float m_width;
    //QPolygonF m_selectedPolygon;
    //QRectF m_boundingRect;
};

#endif // TRACK_GRAPHICS_OBJECT_H
