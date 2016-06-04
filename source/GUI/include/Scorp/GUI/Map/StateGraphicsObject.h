#ifndef STATE_GRAPHICS_OBJECT_H
#define STATE_GRAPHICS_OBJECT_H

#include <QRectF>
#include "Scorp/GUI/Map/PointGraphicsObject.h"
#include "Scorp/GUI/Map/MapSceneStyle.h"

class MarkerObject;

class StateGraphicsObject : public PointGraphicsObject
{
    Q_OBJECT
public:
    StateGraphicsObject(int id, float center_x, float center_y, float radius, QColor fill_color = QColor::fromRgb(255, 0, 0),
                  QGraphicsItem* parent = nullptr);
    float getRadius() const;
    void setRadius(float radius);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    int type() const Q_DECL_OVERRIDE;
    void select(bool graphics_selection) override;
    void deselect(bool graphics_selection) override;
    void addMarker(MarkerObject* marker);
    void removeMarker(MarkerObject* marker);
    MarkerObject* getMarker(int marker_id) const;
    MarkerObject* getLastMarker() const;
    std::vector<MarkerObject*> getMarkers() const;
    void setStyle(const MapSceneStyle::StateStyle& style);
    MapSceneStyle::StateStyle getStyle() const;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) Q_DECL_OVERRIDE;

protected:
    float m_radius;
    std::vector<MarkerObject*> m_markerList;
    MapSceneStyle::StateStyle m_style;
};

#endif // STATE_GRAPHICS_OBJECT_H
