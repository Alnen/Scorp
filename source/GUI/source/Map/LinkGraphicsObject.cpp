#include "../../include/Map/LinkGraphicsObject.h"
#include <QDebug>
#include <QtMath>
#include "../../include/Map/StateGraphicsObject.h"
#include "../../include/Map/TrackGraphicsObject.h"
#include "../../include/Map/TransitionGraphicsObject.h"
#include "../../include/Map/MapScene.h"
#include "../../include/Map/MarkerObject.h"

#include <QDebug>

static int train_counter = 0;

LinkGraphicsObject::LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2,
                                       RailwayPetriNet* petri_net)
    : m_id(id), m_selectionEnable(false), m_viewMode(LinkViewMode::FLEXIBLE), m_selected(false)
{
    m_minLength = 100.f;
    m_middleHalfLength = 0.25f;
    m_prologueLength = 0.25f;
    m_extrude = 40.f;
    if (state1 && state2 && petri_net)
    {
        TrackGraphicsObject main_line(0, state1, state2);
        QPointF quarter_point = main_line.getPointFromScaledLine(m_prologueLength);
        QPointF half_point = main_line.getPointFromScaledLine(m_prologueLength+m_middleHalfLength);
        QPointF three_quarters_point = main_line.getPointFromScaledLine(m_prologueLength+2*m_middleHalfLength);
        float delta_x = half_point.x() - quarter_point.x();
        float delta_y = half_point.y() - quarter_point.y();
        float k_a = -delta_x/delta_y;
        float b_a = half_point.y() - k_a * half_point.x();
        QPointF interm_state_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
            half_point.y(), half_point.x() - 1, k_a, b_a, m_extrude);
        QPointF mutex_state_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
            half_point.y(), half_point.x() + 1, k_a, b_a, m_extrude);
        float part_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
        float cos_alpha = delta_x / part_length;
        float sin_alpha = delta_y / part_length;
        float angle = main_line.getAngle(sin_alpha, cos_alpha);

        // add to container and define id
        int exit_transition_id = petri_net->addTransition<PetriNetComponent::ExitFromStation>(PetriNetComponent::ExitFromStation());
        int enter_transition_id = petri_net->addTransition<PetriNetComponent::EnterToStation>(PetriNetComponent::EnterToStation());
        int inter_state_id = petri_net->addState<PetriNetComponent::InterState>(PetriNetComponent::InterState());
        int semaphore_id = petri_net->addState<PetriNetComponent::Semaphore>(PetriNetComponent::Semaphore());
        int track_id[6] = {
            petri_net->addStateToTransitionConnection<PetriNetComponent::Station, PetriNetComponent::ExitFromStation>(state1->getId(), exit_transition_id),
            petri_net->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Station>(enter_transition_id, state2->getId()),
            petri_net->addTransitionToStateConnection<PetriNetComponent::ExitFromStation, PetriNetComponent::InterState>(exit_transition_id, inter_state_id),
            petri_net->addStateToTransitionConnection<PetriNetComponent::InterState, PetriNetComponent::EnterToStation>(inter_state_id, enter_transition_id),
            petri_net->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Semaphore>(enter_transition_id, semaphore_id),
            petri_net->addStateToTransitionConnection<PetriNetComponent::Semaphore, PetriNetComponent::ExitFromStation>(semaphore_id, exit_transition_id)
        };
        ++train_counter;
        int train_marker_id = petri_net->addMarker<PetriNetComponent::Train>(state1->getId(), PetriNetComponent::Train(train_counter));
        int access_token_id = petri_net->addMarker<PetriNetComponent::AccessToken>(semaphore_id, PetriNetComponent::AccessToken());

        // add to details group
        TransitionGraphicsObject* first_transition = new TransitionGraphicsObject(exit_transition_id, quarter_point.x(), quarter_point.y(),
                                                         6, 18, QColor::fromRgb(0, 200, 200));
        first_transition->setRotation(angle);
        first_transition->setParentID(m_id);
        m_detailsInfoGroup.addItem(first_transition);
        TransitionGraphicsObject* second_transition = new TransitionGraphicsObject(enter_transition_id, three_quarters_point.x(), three_quarters_point.y(),
                                                          6, 18, QColor::fromRgb(200, 100, 0));
        second_transition->setRotation(angle);
        second_transition->setParentID(m_id);
        m_detailsInfoGroup.addItem(second_transition);
        StateGraphicsObject* interm_state = new StateGraphicsObject(inter_state_id, interm_state_point.x(), interm_state_point.y(), 10,
                                                                    QColor::fromRgb(200, 200, 0));
        interm_state->setParentID(m_id);
        m_detailsInfoGroup.addItem(interm_state);
        StateGraphicsObject* blocking_state = new StateGraphicsObject(semaphore_id, mutex_state_point.x(), mutex_state_point.y(), 10,
                                                                      QColor::fromRgb(200, 200, 200));
        blocking_state->setParentID(m_id);
        m_detailsInfoGroup.addItem(blocking_state);
        TrackGraphicsObject* track = new TrackGraphicsObject(track_id[0], state1, first_transition);
        track->setParentID(m_id);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(track_id[1], state2, second_transition, TrackDirection::FROM_SECOND_TO_FIRST);
        track->setParentID(m_id);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(track_id[2], interm_state, first_transition, TrackDirection::FROM_SECOND_TO_FIRST);
        track->setParentID(m_id);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(track_id[3], interm_state, second_transition);
        track->setParentID(m_id);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(track_id[4], blocking_state, first_transition);
        track->setParentID(m_id);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(track_id[5], blocking_state, second_transition, TrackDirection::FROM_SECOND_TO_FIRST);
        track->setParentID(m_id);
        m_detailsInfoGroup.addItem(track);

        // dispose generic
        interm_state->setRadius(5.f);
        interm_state->setCenter(half_point.x(), half_point.y());
        track = new TrackGraphicsObject(10, state1, interm_state);
        track->setParentID(m_id);
        m_genericInfoGroup.addItem(track);
        TrackGraphicsObject* track2 = new TrackGraphicsObject(11, interm_state, state2);
        track2->setParentID(m_id);
        m_genericInfoGroup.addItem(track2);
        m_genericInfoGroup.addItem(interm_state);

        m_drawDetailsEnable = ((m_viewMode == LinkViewMode::DETAILED) || ((m_viewMode == LinkViewMode::FLEXIBLE) && (main_line.getLength() >= m_minLength)));
        if (!m_drawDetailsEnable)
        {
            m_detailsInfoGroup.hideItems();
        }
        else
        {
            m_genericInfoGroup.hideItems();
            interm_state->setRadius(10.f);
            interm_state->setCenter(interm_state_point.x(), interm_state_point.y());
        }
        interm_state->show();

        // create markers
        MarkerObject* train_marker = new MarkerObject(train_marker_id, 0);
        state1->addMarker(train_marker);
        MarkerObject* access_token = new MarkerObject(access_token_id, 1);
        blocking_state->addMarker(access_token);
    }
}

void LinkGraphicsObject::select(bool graphics_selection)
{
    if (m_drawDetailsEnable)
    {
        m_detailsInfoGroup.selectItems(graphics_selection);
    }
    else
    {
        m_genericInfoGroup.selectItems(graphics_selection);
    }
    m_selected = true;
    //this->update();
}

void LinkGraphicsObject::deselect(bool graphics_selection)
{
    if (m_drawDetailsEnable)
    {
        m_detailsInfoGroup.deselectItems(graphics_selection);
    }
    else
    {
        m_genericInfoGroup.deselectItems(graphics_selection);
    }
    m_selected = false;
    //this->update();
}

bool LinkGraphicsObject::selectEnable()
{
    return m_selected;
}

std::vector<PointGraphicsObject*> LinkGraphicsObject::getLinkParts(bool all_parts)
{
    if (all_parts)
    {
        auto detail_info_parts = m_detailsInfoGroup.getItems();
        auto generic_info_parts = m_genericInfoGroup.getItems();
        std::vector<PointGraphicsObject*> parts(detail_info_parts.size() + generic_info_parts.size());
        auto p = move(detail_info_parts.begin(), detail_info_parts.end(), parts.begin());
        move(generic_info_parts.begin(), generic_info_parts.end(), p);
        return parts;
    }
    else
    {
        if (m_drawDetailsEnable)
        {
            return m_detailsInfoGroup.getItems();
        }
        else
        {
            return m_genericInfoGroup.getItems();
        }
    }
}

int LinkGraphicsObject::getID()
{
    return m_id;
}

void LinkGraphicsObject::setPosition(StateGraphicsObject* state1, StateGraphicsObject* state2)
{
    TrackGraphicsObject main_line(0, state1, state2);
    QPointF quarter_point = main_line.getPointFromScaledLine(m_prologueLength);
    QPointF half_point = main_line.getPointFromScaledLine(m_prologueLength+m_middleHalfLength);
    QPointF three_quarters_point = main_line.getPointFromScaledLine(m_prologueLength+2*m_middleHalfLength);
    float delta_x = half_point.x() - quarter_point.x();
    float delta_y = half_point.y() - quarter_point.y();
    float k_a = -delta_x/delta_y;
    float b_a = half_point.y() - k_a * half_point.x();
    QPointF interm_state_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
        half_point.y(), half_point.x() - 1, k_a, b_a, m_extrude);
    QPointF blocking_state_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
        half_point.y(), half_point.x() + 1, k_a, b_a, m_extrude);
    float part_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
    float cos_alpha = delta_x / part_length;
    float sin_alpha = delta_y / part_length;
    float angle = main_line.getAngle(sin_alpha, cos_alpha);

    // dispose details
    TransitionGraphicsObject* first_transition = (TransitionGraphicsObject*)m_detailsInfoGroup.m_items[FIRST_TRANSITION];
    TransitionGraphicsObject* second_transition = (TransitionGraphicsObject*)m_detailsInfoGroup.m_items[SECOND_TRANSITION];
    StateGraphicsObject* interm_state = (StateGraphicsObject*)m_detailsInfoGroup.m_items[INTERIM_STATE];
    StateGraphicsObject* blocking_state = (StateGraphicsObject*)m_detailsInfoGroup.m_items[BLOCKING_STATE];
    first_transition->setCenter(quarter_point.x(), quarter_point.y());
    first_transition->setRotation(angle);
    second_transition->setCenter(three_quarters_point.x(), three_quarters_point.y());
    second_transition->setRotation(angle);
    interm_state->setCenter(interm_state_point.x(), interm_state_point.y());
    blocking_state->setCenter(blocking_state_point.x(), blocking_state_point.y());

    ((TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_ST1_TO_TR1])->setLine(state1, first_transition);
    ((TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_ST2_TO_TR2])->setLine(state2, second_transition);
    ((TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_TR1_TO_IST])->setLine(interm_state, first_transition);
    ((TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_TR2_TO_IST])->setLine(interm_state, second_transition);
    ((TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_TR1_TO_BST])->setLine(blocking_state, first_transition);
    ((TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_TR2_TO_BST])->setLine(blocking_state, second_transition);

    // dispose generic
    interm_state->setRadius(5.f);
    interm_state->setCenter(half_point.x(), half_point.y());
    ((TrackGraphicsObject*)m_genericInfoGroup.m_items[TRACK_FROM_ST1_TO_IST])->setLine(state1, interm_state);
    ((TrackGraphicsObject*)m_genericInfoGroup.m_items[TRACK_FROM_IST_TO_ST2])->setLine(interm_state, state2);

    m_drawDetailsEnable = ((m_viewMode == LinkViewMode::DETAILED) || ((m_viewMode == LinkViewMode::FLEXIBLE) && (main_line.getLength() >= m_minLength)));
    if (!m_drawDetailsEnable)
    {
        m_detailsInfoGroup.hideItems();
        m_genericInfoGroup.showItems();
    }
    else
    {
        m_genericInfoGroup.hideItems();
        interm_state->setRadius(10.f);
        interm_state->setCenter(interm_state_point.x(), interm_state_point.y());
        m_detailsInfoGroup.showItems();
    }
}

QPointF LinkGraphicsObject::getP1(float r) const
{
    TrackGraphicsObject* track = (TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_ST1_TO_TR1];
    return QPointF(-qCos(qDegreesToRadians(track->rotation())) * r, -qSin(qDegreesToRadians(track->rotation())) * r) + track->pos();
}

QPointF LinkGraphicsObject::getP2(float r) const
{
    TrackGraphicsObject* track = (TrackGraphicsObject*)m_detailsInfoGroup.m_items[TRACK_FROM_ST2_TO_TR2];
    return QPointF(-qCos(qDegreesToRadians(track->rotation())) * r, -qSin(qDegreesToRadians(track->rotation())) * r) + track->pos();
}
