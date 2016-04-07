#ifndef POINT_GRAPHICS_ITEM_H
#define POINT_GRAPHICS_ITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QColor>
#include <QPoint>
#include <QGraphicsItem>
#include <QObject>
#include <vector>

class TrackGraphicsObject;

namespace GraphicsObjectType
{
    enum { PointType = 10000, StateType, TransitionType, TrackType };
}

class PointGraphicsObject : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    PointGraphicsObject(float center_x, float center_y, QColor fill_color,
                 QColor border_color = QColor::fromRgb(0, 0, 0), float border_width = 1.f, QGraphicsItem* parent = nullptr);
    PointGraphicsObject(float center_x, float center_y, QGraphicsItem* parent = nullptr);
    float getCenterX();
    void setCenterX(float center_x);
    float getCenterY();
    void setCenterY(float center_y);
    QColor getFillColor();
    void setFillColor(QColor color);
    QColor getBorderColor();
    void setBorderColor(QColor color);
    float getBorderWidth();
    void setBorderWidth(float width);
    virtual void select();
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

    float m_centerX;
    float m_centerY;
    QColor m_borderColor;
    float m_borderWidth;
    QColor m_fillColor;
    QRectF m_boundingRect;
};

#endif // POINT_GRAPHICS_ITEM_H
