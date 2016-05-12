#ifndef LINK_GRAPHICS_OBJECT_H
#define LINK_GRAPHICS_OBJECT_H

#include "GraphicsObjectsGroup.h"
#include <QObject>

class StateGraphicsObject;

enum class LinkViewMode { DETAILED, GENERIC, FLEXIBLE };

class LinkGraphicsObject
{
public:
    LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2);
    void select();
    void deselect();
    bool selectEnable();
    std::vector<PointGraphicsObject*> getLinkParts(bool all_parts = false);
    int getID();
    void setPosition(StateGraphicsObject* state1, StateGraphicsObject* state2);
    QPointF getP1(float r = 0.f) const;
    QPointF getP2(float r = 0.f) const;

private:
    enum DETAILS_INFO { FIRST_TRANSITION = 0, SECOND_TRANSITION, INTERIM_STATE, BLOCKING_STATE,
                        TRACK_FROM_ST1_TO_TR1, TRACK_FROM_ST2_TO_TR2, TRACK_FROM_TR1_TO_IST,
                        TRACK_FROM_TR2_TO_IST, TRACK_FROM_TR1_TO_BST, TRACK_FROM_TR2_TO_BST
                      };
    enum GENERIC_INFO { TRACK_FROM_ST1_TO_IST, TRACK_FROM_IST_TO_ST2 };

    GraphicsObjectsGroup m_detailsInfoGroup;
    GraphicsObjectsGroup m_genericInfoGroup;
    float m_middleHalfLength;
    float m_prologueLength;
    float m_extrude;
    bool m_selectionEnable;
    float m_minLength;
    LinkViewMode m_viewMode;
    bool m_drawDetailsEnable;
    bool m_selected;
    int m_id;
};

#endif // LINK_GRAPHICS_OBJECT_H
