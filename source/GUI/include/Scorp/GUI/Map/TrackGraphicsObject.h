#ifndef TRACK_GRAPHICS_OBJECT_H
#define TRACK_GRAPHICS_OBJECT_H

#include <QGraphicsLineItem>
#include <QColor>
#include "Scorp/GUI/Map/PointGraphicsObject.h"
#include "Scorp/GUI/Map/MapSceneStyle.h"

class StateGraphicsObject;
class TransitionGraphicsObject;

class TrackGraphicsObject : public PointGraphicsObject
{
public:
    enum class Direction { None, FromFirstToSecond, FromSecondToFirst, Both };

    TrackGraphicsObject(int object_id, StateGraphicsObject* state, TransitionGraphicsObject* transition,
        Direction direction = Direction::FromFirstToSecond, QColor fill_color = QColor::fromRgb(0, 0, 0),
        float width = 1.f, QGraphicsItem* parent = nullptr);
    TrackGraphicsObject(int object_id, StateGraphicsObject* state1, StateGraphicsObject* state2,
        Direction direction = Direction::FromFirstToSecond, QColor fill_color = QColor::fromRgb(0, 0, 0),
        float width = 1.f, QGraphicsItem* parent = nullptr);
    Direction getDirection() const;
    void setDirection(Direction direction);
    float getX1() const;
    float getY1() const;
    float getX2() const;
    float getY2() const;
    float getLength() const;
    void setCenter(float x, float y) override;
    QPointF getCenter() const override;
    void setFillColor(QColor color);
    float getWidth() const;
    void setWidth(float width);
    void setLine(StateGraphicsObject* state, TransitionGraphicsObject* transition);
    void setLine(StateGraphicsObject* state1, StateGraphicsObject* state2);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select(bool graphics_selection) override;
    void deselect(bool graphics_selection) override;
    QPointF getPointFromScaledLine(float scale_factor) const;
    void setStyle(const MapSceneStyle::TrackStyle& style);
    MapSceneStyle::TrackStyle getStyle() const;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

private:
    QLineF expandLine(float width);
    QPolygonF getBoundingPolygon(float width);
    void setDrawDisable();
    QPointF getLineIntersection(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

protected:
    QLineF m_arrow_parts[4];
    Direction m_direction;
    float m_angle;
    bool m_drawEnable;
    float m_x1;
    float m_x2;
    float m_y1;
    float m_y2;
    float m_length;
    MapSceneStyle::TrackStyle m_style;
    bool m_isLinkWithTransition;
};

#endif // TRACK_GRAPHICS_OBJECT_H
