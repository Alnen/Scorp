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
    /*
    void linkTrack(TrackGraphicsObject* track);
    void unlinkTrack(TrackGraphicsObject* track);
    std::vector<TrackGraphicsObject*> getLinkedTracks();
    TrackGraphicsObject* firstLink();
    TrackGraphicsObject* lastLink();
    int linkedTracksCount();
    */
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;

signals:
    void selected();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

protected:
    //std::vector<TrackGraphicsObject*> m_linkedTracks;
    float m_radius;
    bool m_isSelected;
};

#endif // STATE_GRAPHICS_OBJECT_H
