#ifndef LINK_GRAPHICS_OBJECT_H
#define LINK_GRAPHICS_OBJECT_H

#include <array>
#include <QObject>
#include "Scorp/Core/PetriNetUsing.h"
#include "Scorp/GUI/Map/GraphicsObjectsGroup.h"
#include "Scorp/GUI/Map/MapSceneStyle.h"

class StateGraphicsObject;
class MarkerObject;

class LinkGraphicsObject
{
public:
    enum class DetailsItem { ExitTransition, EnterTransition, InterState, Semaphore,
        TrackFromSt1ToExitTr, TrackFromEnterTrToSt2, TrackFromExitTrToInterSt,
        TrackFromInterStToEnterTr, TrackFromExitTrToSemaphore, TrackFromSemaphoreToEnterTr
    };
    enum class GenericItem { TrackFromSt1ToInterSt, TrackFromInterStToSt2, InterState };

    LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2,
        RailwayPetriNet* petri_net, const std::array<int, 4>& id_array);
    LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2,
        RailwayPetriNet* petri_net);
    void select(bool graphics_selection);
    void deselect(bool graphics_selection);
    bool selectEnable();
    std::vector<PointGraphicsObject*> getLinkParts(bool all_parts = false) const;
    int getID() const;
    void setPosition(StateGraphicsObject* state1, StateGraphicsObject* state2);
    QPointF getP1(float r = 0.f) const;
    QPointF getP2(float r = 0.f) const;
    bool isSemaphore(StateGraphicsObject* state) const;
    PointGraphicsObject* getDetailsItem(DetailsItem item) const;
    PointGraphicsObject* getGenericItem(GenericItem item) const;
    void setStyle(const MapSceneStyle::LinkStyle& style);
    MapSceneStyle::LinkStyle getStyle() const;

private:
    void createLinkItems(StateGraphicsObject* state1, StateGraphicsObject* state2,
        RailwayPetriNet* petri_net, const std::array<int, 4>& id_array);
    void updateMarkerStyle(const std::vector<MarkerObject*>& marker_storage);
    
private:
    MapSceneStyle::LinkStyle m_style;
    GraphicsObjectsGroup m_detailsInfoGroup;
    GraphicsObjectsGroup m_genericInfoGroup;
    float m_middleHalfLength;
    float m_prologueLength;
    float m_extrude;
    bool m_selectionEnable;
    float m_minLength;
    bool m_drawDetailsEnable;
    bool m_selected;
    int m_id;
};

#endif // LINK_GRAPHICS_OBJECT_H
