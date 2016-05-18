#include "LinkGraphicsObject.h"
#include <QDebug>
#include <QtMath>
#include "StateGraphicsObject.h"
#include "TrackGraphicsObject.h"
#include "TransitionGraphicsObject.h"
#include "MapScene.h"
#include "MarkerObject.h"

static int train_counter = 0;

LinkGraphicsObject::LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2,
                                       RailwayPetriNet* petri_net)
    : m_id(id), m_selectionEnable(false), m_viewMode(LinkViewMode::FLEXIBLE), m_selected(false)
{
    m_minLength = 100.f;
    m_middleHalfLength = 0.25f;
    m_prologueLength = 0.25f;
    m_extrude = 40.f;
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

    int state_id1 = state1->getId();
    int state_id2 = state2->getId();

    auto first_tr_id = (petri_net ? petri_net->addTransition<PetryNetComponent::ExitFromStation>(PetryNetComponent::ExitFromStation()) : 0);
    TransitionGraphicsObject* first_transition = new TransitionGraphicsObject((int)first_tr_id, quarter_point.x(), quarter_point.y(),
                                                     6, 18, QColor::fromRgb(0, 200, 200));
    first_transition->setRotation(angle);
    first_transition->setParentID(m_id);
    m_detailsInfoGroup.addItem(first_transition);

    auto second_tr_id = (petri_net ? petri_net->addTransition<PetryNetComponent::EnterToStation>(PetryNetComponent::EnterToStation()) : 1);
    TransitionGraphicsObject* second_transition = new TransitionGraphicsObject((int)second_tr_id, three_quarters_point.x(), three_quarters_point.y(),
                                                      6, 18, QColor::fromRgb(200, 100, 0));
    second_transition->setRotation(angle);
    second_transition->setParentID(m_id);
    m_detailsInfoGroup.addItem(second_transition);

    auto interm_state_id = (petri_net ? petri_net->addState<PetryNetComponent::InterState>(PetryNetComponent::InterState()) : 2);
    StateGraphicsObject* interm_state = new StateGraphicsObject((int)interm_state_id, interm_state_point.x(), interm_state_point.y(), 10,
                                                                QColor::fromRgb(200, 200, 0));
    interm_state->setParentID(m_id);
    m_detailsInfoGroup.addItem(interm_state);

    auto blocking_state_id = (petri_net ? petri_net->addState<PetryNetComponent::Semaphore>(PetryNetComponent::Semaphore()) : 3);
    StateGraphicsObject* blocking_state = new StateGraphicsObject((int)blocking_state_id, mutex_state_point.x(), mutex_state_point.y(), 10,
                                                                  QColor::fromRgb(200, 200, 200));
    blocking_state->setParentID(m_id);
    m_detailsInfoGroup.addItem(blocking_state);

    petri_net->addStateToTransitionConnection<PetryNetComponent::Station, PetryNetComponent::ExitFromStation>(state_id1, first_tr_id);
    TrackGraphicsObject* track = new TrackGraphicsObject(4, state1, first_transition);
    track->setParentID(m_id);
    m_detailsInfoGroup.addItem(track);
    petri_net->addTransitionToStateConnection<PetryNetComponent::EnterToStation, PetryNetComponent::Station>(second_tr_id, state_id2);
    track = new TrackGraphicsObject(5, state2, second_transition, TrackDirection::FROM_SECOND_TO_FIRST);
    track->setParentID(m_id);
    m_detailsInfoGroup.addItem(track);
    petri_net->addTransitionToStateConnection<PetryNetComponent::ExitFromStation, PetryNetComponent::InterState>(first_tr_id, interm_state_id);
    track = new TrackGraphicsObject(6, interm_state, first_transition, TrackDirection::FROM_SECOND_TO_FIRST);
    track->setParentID(m_id);
    m_detailsInfoGroup.addItem(track);
    petri_net->addStateToTransitionConnection<PetryNetComponent::InterState, PetryNetComponent::EnterToStation>(interm_state_id, second_tr_id);
    track = new TrackGraphicsObject(7, interm_state, second_transition);
    track->setParentID(m_id);
    m_detailsInfoGroup.addItem(track);
    petri_net->addTransitionToStateConnection<PetryNetComponent::ExitFromStation, PetryNetComponent::Semaphore>(first_tr_id, blocking_state_id);
    track = new TrackGraphicsObject(8, blocking_state, first_transition);
    track->setParentID(m_id);
    m_detailsInfoGroup.addItem(track);
    petri_net->addStateToTransitionConnection<PetryNetComponent::Semaphore, PetryNetComponent::EnterToStation>(blocking_state_id, first_tr_id);
    track = new TrackGraphicsObject(9, blocking_state, second_transition, TrackDirection::FROM_SECOND_TO_FIRST);
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
    auto train_marker_id = petri_net->addMarker<PetryNetComponent::Train>(state_id1, PetryNetComponent::Train(++train_counter));
    MarkerObject* train_marker = new MarkerObject(train_marker_id, 0);
    state1->addMarker(train_marker);
    auto access_token_id = petri_net->addMarker<PetryNetComponent::AccessToken>(blocking_state_id, PetryNetComponent::AccessToken());
    MarkerObject* access_token = new MarkerObject(access_token_id, 1);
    blocking_state->addMarker(access_token);
}

void LinkGraphicsObject::select()
{
    if (m_drawDetailsEnable)
    {
        m_detailsInfoGroup.selectItems();
    }
    else
    {
        m_genericInfoGroup.selectItems();
    }
    m_selected = true;
    //this->update();
}

void LinkGraphicsObject::deselect()
{
    if (m_drawDetailsEnable)
    {
        m_detailsInfoGroup.deselectItems();
    }
    else
    {
        m_genericInfoGroup.deselectItems();
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
