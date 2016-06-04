#include <QDebug>
#include <QtMath>
#include "Scorp/GUI/Map/LinkGraphicsObject.h"
#include "Scorp/GUI/Map/StateGraphicsObject.h"
#include "Scorp/GUI/Map/TrackGraphicsObject.h"
#include "Scorp/GUI/Map/TransitionGraphicsObject.h"
#include "Scorp/GUI/Map/MapScene.h"
#include "Scorp/GUI/Map/MarkerObject.h"

LinkGraphicsObject::LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2,
                                       RailwayPetriNet* petri_net, const std::array<int, 4>& id_array)
    : m_id(id), m_selectionEnable(false), m_selected(false), m_minLength(100.f),
      m_middleHalfLength(0.25f), m_prologueLength(0.25f), m_extrude(40.f)
{
    createLinkItems(state1, state2, petri_net, id_array);
}

LinkGraphicsObject::LinkGraphicsObject(int id, StateGraphicsObject* state1, StateGraphicsObject* state2,
                                       RailwayPetriNet* petri_net)
    : m_id(id), m_selectionEnable(false), m_selected(false), m_minLength(100.f),
      m_middleHalfLength(0.25f), m_prologueLength(0.25f), m_extrude(40.f)
{
    if (state1 && state2 && petri_net)
    {
        // add to container and define id
        int exit_transition_id = petri_net->addTransition<PetriNetComponent::ExitFromStation>(PetriNetComponent::ExitFromStation());
        int enter_transition_id = petri_net->addTransition<PetriNetComponent::EnterToStation>(PetriNetComponent::EnterToStation());
        int inter_state_id = petri_net->addState<PetriNetComponent::InterState>(PetriNetComponent::InterState());
        int semaphore_id = petri_net->addState<PetriNetComponent::Semaphore>(PetriNetComponent::Semaphore());
        int access_token_id = petri_net->addMarker<PetriNetComponent::AccessToken>(semaphore_id, PetriNetComponent::AccessToken());
        petri_net->addStateToTransitionConnection<PetriNetComponent::Station, PetriNetComponent::ExitFromStation>(state1->getId(), exit_transition_id);
        petri_net->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Station>(enter_transition_id, state2->getId());
        petri_net->addTransitionToStateConnection<PetriNetComponent::ExitFromStation, PetriNetComponent::InterState>(exit_transition_id, inter_state_id);
        petri_net->addStateToTransitionConnection<PetriNetComponent::InterState, PetriNetComponent::EnterToStation>(inter_state_id, enter_transition_id);
        petri_net->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Semaphore>(enter_transition_id, semaphore_id);
        petri_net->addStateToTransitionConnection<PetriNetComponent::Semaphore, PetriNetComponent::ExitFromStation>(semaphore_id, exit_transition_id);
        std::array<int, 4> id_array{{
            exit_transition_id,
            enter_transition_id,
            inter_state_id,
            semaphore_id
        }};
        // create items and add them to scene
        createLinkItems(state1, state2, petri_net, id_array);
        // create markers
        StateGraphicsObject* semaphore_state = (StateGraphicsObject*)getDetailsItem(DetailsItem::Semaphore);
        MarkerObject* access_token = new MarkerObject(access_token_id, MarkerObject::MarkerType::AccessToken);
        access_token->setStyle(m_style.accessTokenStyle);
        semaphore_state->addMarker(access_token);
    }
}

void LinkGraphicsObject::createLinkItems(StateGraphicsObject* state1, StateGraphicsObject* state2,
        RailwayPetriNet* petri_net, const std::array<int, 4>& id_array)
{
    if (state1 && state2 && petri_net)
    {
        // find items position
        TrackGraphicsObject main_line(0, state1, state2);
        QPointF quarter_point = main_line.getPointFromScaledLine(m_prologueLength);
        QPointF half_point = main_line.getPointFromScaledLine(m_prologueLength+m_middleHalfLength);
        QPointF three_quarters_point = main_line.getPointFromScaledLine(m_prologueLength+2*m_middleHalfLength);
        float delta_x = half_point.x() - quarter_point.x();
        float delta_y = half_point.y() - quarter_point.y();
        float k_a = -delta_x/delta_y;
        float b_a = half_point.y() - k_a * half_point.x();
        QPointF inter_state_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
            half_point.y(), half_point.x() - 1, k_a, b_a, m_extrude);
        QPointF semaphore_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
            half_point.y(), half_point.x() + 1, k_a, b_a, m_extrude);
        float part_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
        float cos_alpha = delta_x / part_length;
        float sin_alpha = delta_y / part_length;
        float angle = main_line.getAngle(sin_alpha, cos_alpha);

        // add items to details group
        TransitionGraphicsObject* exit_transition = new TransitionGraphicsObject(id_array[0], quarter_point.x(),
            quarter_point.y(), m_style.exitTransitionStyle.width, m_style.exitTransitionStyle.height, m_style.exitTransitionStyle.fillColor);
        exit_transition->setRotation(angle);
        exit_transition->setParentID(m_id);
        exit_transition->setStyle(m_style.exitTransitionStyle);
        m_detailsInfoGroup.addItem(exit_transition);
        TransitionGraphicsObject* enter_transition = new TransitionGraphicsObject(id_array[1], three_quarters_point.x(), three_quarters_point.y(),
            m_style.enterTransitionStyle.width, m_style.enterTransitionStyle.height, m_style.enterTransitionStyle.fillColor);
        enter_transition->setRotation(angle);
        enter_transition->setParentID(m_id);
        enter_transition->setStyle(m_style.enterTransitionStyle);
        m_detailsInfoGroup.addItem(enter_transition);
        StateGraphicsObject* inter_state = new StateGraphicsObject(id_array[2], inter_state_point.x(), inter_state_point.y(),
            m_style.interStateStyle.radius, m_style.interStateStyle.fillColor);
        inter_state->setParentID(m_id);
        inter_state->setStyle(m_style.interStateStyle);
        m_detailsInfoGroup.addItem(inter_state);
        StateGraphicsObject* semaphore_state = new StateGraphicsObject(id_array[3], semaphore_point.x(), semaphore_point.y(),
            m_style.semaphoreStyle.radius, m_style.semaphoreStyle.fillColor);
        semaphore_state->setParentID(m_id);
        semaphore_state->setStyle(m_style.semaphoreStyle);
        m_detailsInfoGroup.addItem(semaphore_state);
        TrackGraphicsObject* track = new TrackGraphicsObject(4, state1, exit_transition);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(5, state2, enter_transition, TrackGraphicsObject::Direction::FromSecondToFirst);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(6, inter_state, exit_transition, TrackGraphicsObject::Direction::FromSecondToFirst);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(7, inter_state, enter_transition);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(8, semaphore_state, exit_transition);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_detailsInfoGroup.addItem(track);
        track = new TrackGraphicsObject(9, semaphore_state, enter_transition, TrackGraphicsObject::Direction::FromSecondToFirst);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_detailsInfoGroup.addItem(track);

        // dispose generic
        inter_state->setRadius(7.f);
        inter_state->setCenter(half_point.x(), half_point.y());
        track = new TrackGraphicsObject(10, state1, inter_state);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_genericInfoGroup.addItem(track);
        track = new TrackGraphicsObject(11, inter_state, state2);
        track->setParentID(m_id);
        track->setStyle(m_style.trackStyle);
        m_genericInfoGroup.addItem(track);
        m_genericInfoGroup.addItem(inter_state);

        m_drawDetailsEnable = ((m_style.linkView == MapSceneStyle::LinkViewType::Detailed)
                               || ((m_style.linkView == MapSceneStyle::LinkViewType::Flexible)
                                   && (main_line.getLength() >= m_minLength)));
        if (!m_drawDetailsEnable)
        {
            m_detailsInfoGroup.hideItems();
        }
        else
        {
            m_genericInfoGroup.hideItems();
            inter_state->setRadius(10.f);
            inter_state->setCenter(inter_state_point.x(), inter_state_point.y());
        }
        inter_state->show();
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

std::vector<PointGraphicsObject*> LinkGraphicsObject::getLinkParts(bool all_parts) const
{
    if (all_parts)
    {
        auto detail_info_parts = m_detailsInfoGroup.getItems();
        auto generic_info_parts = m_genericInfoGroup.getItems();
        std::vector<PointGraphicsObject*> parts(detail_info_parts.size() + generic_info_parts.size());
        auto p = std::copy(detail_info_parts.begin(), detail_info_parts.end(), parts.begin());
        std::copy(generic_info_parts.begin(), generic_info_parts.end(), p);
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

int LinkGraphicsObject::getID() const
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
    QPointF inter_state_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
        half_point.y(), half_point.x() - 1, k_a, b_a, m_extrude);
    QPointF semaphore_point = TrackGraphicsObject::getPointByEquation(half_point.x(),
        half_point.y(), half_point.x() + 1, k_a, b_a, m_extrude);
    float part_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
    float cos_alpha = delta_x / part_length;
    float sin_alpha = delta_y / part_length;
    float angle = main_line.getAngle(sin_alpha, cos_alpha);

    // dispose details
    TransitionGraphicsObject* exit_transition = (TransitionGraphicsObject*)getDetailsItem(DetailsItem::ExitTransition);
    TransitionGraphicsObject* enter_transition = (TransitionGraphicsObject*)getDetailsItem(DetailsItem::EnterTransition);
    StateGraphicsObject* inter_state = (StateGraphicsObject*)getDetailsItem(DetailsItem::InterState);
    StateGraphicsObject* semaphore_state = (StateGraphicsObject*)getDetailsItem(DetailsItem::Semaphore);
    exit_transition->setCenter(quarter_point.x(), quarter_point.y());
    exit_transition->setRotation(angle);
    enter_transition->setCenter(three_quarters_point.x(), three_quarters_point.y());
    enter_transition->setRotation(angle);
    inter_state->setCenter(inter_state_point.x(), inter_state_point.y());
    semaphore_state->setCenter(semaphore_point.x(), semaphore_point.y());

    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromSt1ToExitTr))->setLine(state1, exit_transition);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromEnterTrToSt2))->setLine(state2, enter_transition);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromExitTrToInterSt))->setLine(inter_state, exit_transition);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromInterStToEnterTr))->setLine(inter_state, enter_transition);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromExitTrToSemaphore))->setLine(semaphore_state, exit_transition);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromSemaphoreToEnterTr))->setLine(semaphore_state, enter_transition);

    // dispose generic
    inter_state->setRadius(7.f);
    inter_state->setCenter(half_point.x(), half_point.y());

    ((TrackGraphicsObject*)getGenericItem(GenericItem::TrackFromSt1ToInterSt))->setLine(state1, inter_state);
    ((TrackGraphicsObject*)getGenericItem(GenericItem::TrackFromInterStToSt2))->setLine(inter_state, state2);

    m_drawDetailsEnable = ((m_style.linkView == MapSceneStyle::LinkViewType::Detailed)
                           || ((m_style.linkView == MapSceneStyle::LinkViewType::Flexible)
                               && (main_line.getLength() >= m_minLength)));
    if (!m_drawDetailsEnable)
    {
        m_detailsInfoGroup.hideItems();
        m_genericInfoGroup.showItems();
    }
    else
    {
        m_genericInfoGroup.hideItems();
        inter_state->setRadius(10.f);
        inter_state->setCenter(inter_state_point.x(), inter_state_point.y());
        m_detailsInfoGroup.showItems();
    }
}

QPointF LinkGraphicsObject::getP1(float r) const
{
    TrackGraphicsObject* track = (TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromSt1ToExitTr);
    return QPointF(-qCos(qDegreesToRadians(track->rotation())) * r, -qSin(qDegreesToRadians(track->rotation())) * r) + track->pos();
}

QPointF LinkGraphicsObject::getP2(float r) const
{
    TrackGraphicsObject* track = (TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromEnterTrToSt2);
    return QPointF(-qCos(qDegreesToRadians(track->rotation())) * r, -qSin(qDegreesToRadians(track->rotation())) * r) + track->pos();
}

bool LinkGraphicsObject::isSemaphore(StateGraphicsObject* state) const
{
    StateGraphicsObject* semaphore_state = (StateGraphicsObject*)getDetailsItem(DetailsItem::Semaphore);
    if (state == semaphore_state)
    {
        return true;
    }
    return false;
}

PointGraphicsObject* LinkGraphicsObject::getDetailsItem(DetailsItem item) const
{
    if (item == DetailsItem::ExitTransition)
    {
        return m_detailsInfoGroup.getItem(0);
    }
    else if (item == DetailsItem::EnterTransition)
    {
        return m_detailsInfoGroup.getItem(1);
    }
    else if (item == DetailsItem::InterState)
    {
        return m_detailsInfoGroup.getItem(2);
    }
    else if (item == DetailsItem::Semaphore)
    {
        return m_detailsInfoGroup.getItem(3);
    }
    else if (item == DetailsItem::TrackFromSt1ToExitTr)
    {
        return m_detailsInfoGroup.getItem(4);
    }
    else if (item == DetailsItem::TrackFromEnterTrToSt2)
    {
        return m_detailsInfoGroup.getItem(5);
    }
    else if (item == DetailsItem::TrackFromExitTrToInterSt)
    {
        return m_detailsInfoGroup.getItem(6);
    }
    else if (item == DetailsItem::TrackFromInterStToEnterTr)
    {
        return m_detailsInfoGroup.getItem(7);
    }
    else if (item == DetailsItem::TrackFromExitTrToSemaphore)
    {
        return m_detailsInfoGroup.getItem(8);
    }
    else if (item == DetailsItem::TrackFromSemaphoreToEnterTr)
    {
        return m_detailsInfoGroup.getItem(9);
    }
    return nullptr;
}

PointGraphicsObject* LinkGraphicsObject::getGenericItem(GenericItem item) const
{
    if (item == GenericItem::TrackFromSt1ToInterSt)
    {
        return m_genericInfoGroup.getItem(0);
    }
    else if (item == GenericItem::TrackFromInterStToSt2)
    {
        return m_genericInfoGroup.getItem(1);
    }
    else if (item == GenericItem::InterState)
    {
        return m_genericInfoGroup.getItem(2);
    }
    return nullptr;
}

void LinkGraphicsObject::setStyle(const MapSceneStyle::LinkStyle& style)
{
    m_style = style;
    ((TransitionGraphicsObject*)getDetailsItem(DetailsItem::ExitTransition))->setStyle(m_style.exitTransitionStyle);
    ((TransitionGraphicsObject*)getDetailsItem(DetailsItem::EnterTransition))->setStyle(m_style.enterTransitionStyle);
    StateGraphicsObject* inter_state = (StateGraphicsObject*)getDetailsItem(DetailsItem::InterState);
    inter_state->setStyle(m_style.interStateStyle);
    StateGraphicsObject* semaphore_state = (StateGraphicsObject*)getDetailsItem(DetailsItem::Semaphore);
    semaphore_state->setStyle(m_style.semaphoreStyle);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromSt1ToExitTr))->setStyle(m_style.trackStyle);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromEnterTrToSt2))->setStyle(m_style.trackStyle);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromExitTrToInterSt))->setStyle(m_style.trackStyle);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromInterStToEnterTr))->setStyle(m_style.trackStyle);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromExitTrToSemaphore))->setStyle(m_style.trackStyle);
    ((TrackGraphicsObject*)getDetailsItem(DetailsItem::TrackFromSemaphoreToEnterTr))->setStyle(m_style.trackStyle);
    updateMarkerStyle(inter_state->getMarkers());
    updateMarkerStyle(semaphore_state->getMarkers());
}

MapSceneStyle::LinkStyle LinkGraphicsObject::getStyle() const
{
    return m_style;
}

void LinkGraphicsObject::updateMarkerStyle(const std::vector<MarkerObject*>& marker_storage)
{
    for (MarkerObject* marker : marker_storage)
    {
        if (marker->getType() == MarkerObject::MarkerType::Train)
        {
            marker->setStyle(m_style.trainStyle);
        }
        else
        {
            marker->setStyle(m_style.accessTokenStyle);
        }
    }
}
