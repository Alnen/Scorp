#ifndef POINT_GRAPHICS_ITEM_H
#define POINT_GRAPHICS_ITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QColor>
#include <QPoint>
#include <QGraphicsItem>
#include <QObject>
#include <vector>

namespace GraphicsObjectType
{
    enum { PointType = QAbstractGraphicsShapeItem::UserType + 1, StateType, TransitionType, TrackType };
}

class PointGraphicsObject : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    PointGraphicsObject(int object_id, float center_x, float center_y, QColor fill_color,
                 QColor border_color = QColor::fromRgb(0, 0, 0), float border_width = 1.f, QGraphicsItem* parent = nullptr);
    PointGraphicsObject(int object_id, float center_x, float center_y, QGraphicsItem* parent = nullptr);
    virtual void setCenter(float x, float y);
    virtual QPointF getCenter() const;
    QColor getFillColor() const;
    void setFillColor(QColor color);
    QColor getBorderColor() const;
    void setBorderColor(QColor color);
    float getBorderWidth() const;
    void setBorderWidth(float width);
    virtual void select();
    virtual void deselect();
    bool selectEnable() const;
    virtual void setParentID(int parent);
    virtual int parentID() const;
    virtual int id() const;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;

    static QPointF getPointByEquation(float x_p, float y_p, float x_n, float k,
                               float b, float length);
    static QPointF getEquationByPoints(QPointF p1, QPointF p2);
    static QPointF getEquationIntersectionPoint(float k1, float b1, float k2, float b2);
    static float normalizeAngle(float angle);
    static float getAngle(float sin_a, float cos_a);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

    QColor m_borderColor;
    float m_borderWidth;
    QColor m_fillColor;
    QRectF m_boundingRect;
    float m_selectionExtrude;
    bool m_selected;
    int m_id;
    int m_parentID;
};

#endif // POINT_GRAPHICS_ITEM_H
