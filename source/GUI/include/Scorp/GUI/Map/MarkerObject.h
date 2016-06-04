#ifndef MARKER_OBJECT_H
#define MARKER_OBJECT_H

#include "MapSceneStyle.h"

class StateGraphicsObject;

class MarkerObject
{
public:
    enum class MarkerType { Train, AccessToken };

    MarkerObject(int id, MarkerType type, StateGraphicsObject* parent = nullptr);
    void connectToState(StateGraphicsObject* state);
    void disconnectFromState();
    int getId() const;
    MarkerType getType() const;
    static int MarkerTypeToInt(MarkerType type);
    void setStyle(const MapSceneStyle::MarkerStyle& style);
    MapSceneStyle::MarkerStyle getStyle() const;

private:
    int m_id;
    MarkerType m_type;
    StateGraphicsObject* m_parentState;
    MapSceneStyle::MarkerStyle m_style;
};

#endif // MARKER_OBJECT_H
