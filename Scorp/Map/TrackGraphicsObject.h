#ifndef TRACK_GRAPHICS_OBJECT_H
#define TRACK_GRAPHICS_OBJECT_H

#include "StateGraphicsObject.h"
#include "TransitionGraphicsObject.h"
#include <QGraphicsLineItem>
#include <QColor>

enum class TrackDirection { FROM_FIRST_TO_SECOND, FROM_SECOND_TO_FIRST, BOTH };

class TrackGraphicsObject : public QObject, public QGraphicsLineItem
{
public:
    TrackGraphicsObject::TrackGraphicsObject(TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackGraphicsObject(StateGraphicsObject* state, TransitionGraphicsObject* transition,
        TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    TrackGraphicsObject(StateGraphicsObject* state1, StateGraphicsObject* state2,
        TrackDirection direction = TrackDirection::FROM_FIRST_TO_SECOND,
        QColor color = QColor::fromRgb(0, 0, 0), float width = 1.f, QGraphicsItem *parent = 0);
    StateGraphicsObject* getState1();
    StateGraphicsObject* getState2();
    TransitionGraphicsObject* getTransition();
    TrackDirection getDirection();
    void setDirection(TrackDirection direction);
    float getX1();
    float getY1();
    float getX2();
    float getY2();
    float getLength();
    QColor getColor();
    void setColor(QColor color);
    float getWidth();
    void setWidth(float width);
    void setLine(StateGraphicsObject* state, TransitionGraphicsObject* transition);
    void setLine(StateGraphicsObject* state1, StateGraphicsObject* state2);
    int type() const Q_DECL_OVERRIDE;

private:
    StateGraphicsObject* m_state1;
    StateGraphicsObject* m_state2;
    TransitionGraphicsObject* m_transition;
    //PointGraphicsObject* m_first_vertex;
    //PointGraphicsObject* m_second_vertex;
    QPolygonF m_ftos_arrow; // from first to second
    QPolygonF m_stof_arrow; // from second to first
    TrackDirection m_direction;
    float m_x1;
    float m_x2;
    float m_y1;
    float m_y2;
    float m_length;
    QColor m_color;
    float m_width;
};

#endif // TRACK_GRAPHICS_OBJECT_H
