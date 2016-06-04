#include <QDebug>
#include "Scorp/GUI/Map/MarkerCommandQueue.h"
#include "Scorp/GUI/Map/MapScene.h"
#include "Scorp/GUI/Map/StateGraphicsObject.h"
#include "Scorp/GUI/Map/PointGraphicsObject.h"
#include "Scorp/GUI/Map/MarkerObject.h"

MarkerCommandQueue::MarkerCommandQueue() : m_scene(nullptr)
{
}

MarkerCommandQueue* MarkerCommandQueue::p_instance = 0;
MarkerCommandQueueDestroyer MarkerCommandQueue::destroyer;

MarkerCommandQueueDestroyer::~MarkerCommandQueueDestroyer()
{
    delete p_instance;
}

void MarkerCommandQueueDestroyer::initialize(MarkerCommandQueue* p)
{
    p_instance = p;
}

MarkerCommandQueue& MarkerCommandQueue::getInstance()
{
    if (!p_instance)
    {
        p_instance = new MarkerCommandQueue();
        destroyer.initialize( p_instance);
    }
    return *p_instance;
}

void MarkerCommandQueue::addMarkerCommand(int id, int state_id, MarkerObject::MarkerType type)
{
    m_commands.push_back(MarkerCommandStruct(MarkerCommand::ADD, id, state_id, type));
}

void MarkerCommandQueue::moveMarkerCommand(int id, int new_state_id)
{
    m_commands.push_back(MarkerCommandStruct(MarkerCommand::MOVE, id, new_state_id));
}

void MarkerCommandQueue::deleteMarkerCommand(int id)
{
    m_commands.push_back(MarkerCommandStruct(MarkerCommand::DELETE, id));
}

void MarkerCommandQueue::makeCommand()
{
    if (!m_scene) return;
    qDebug() << "makeCommand: size=" << m_commands.size();
    if (m_commands.size() > 0)
    {
        StateGraphicsObject* curr_state = nullptr;
        MarkerObject* marker = nullptr;
        if (m_commands[0].command == MarkerCommand::ADD)
        {
            qDebug() << "makeCommand: ADD (" << m_commands[0].param1 << ")";
            for (auto item : m_scene->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    if (curr_state->getId() == m_commands[0].param2)
                    {
                        if (m_commands[0].markerType == MarkerObject::MarkerType::Train)
                        {
                            marker = new MarkerObject(m_commands[0].param1, MarkerObject::MarkerType::Train,
                                curr_state);
                            marker->setStyle(m_scene->getStyle().trainStyle);
                        }
                        else
                        {
                            marker = new MarkerObject(m_commands[0].param1, MarkerObject::MarkerType::AccessToken,
                                curr_state);
                            marker->setStyle(m_scene->getStyle().accessTokenStyle);
                        }
                        curr_state->addMarker(marker);
                        break;
                    }
                }
            }
            curr_state = nullptr;
        }
        else if (m_commands[0].command == MarkerCommand::DELETE)
        {
            qDebug() << "makeCommand: DELETE (" << m_commands[0].param1 << ")";
            MarkerObject* found_marker = nullptr;
            for (auto item : m_scene->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    found_marker = curr_state->getMarker(m_commands[0].param1);
                    if (found_marker)
                    {
                        curr_state->removeMarker(found_marker);
                        delete found_marker;
                        break;
                    }
                }
            }
        }
        else if (m_commands[0].command == MarkerCommand::MOVE)
        {
            qDebug() << "makeCommand: MOVE (" << m_commands[0].param1
                     << ", " << m_commands[0].param2 << ")";
            MarkerObject* found_marker = nullptr;
            StateGraphicsObject* old_state = nullptr;
            StateGraphicsObject* new_state = nullptr;
            for (auto item : m_scene->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    MarkerObject* current_found_marker = curr_state->getMarker(m_commands[0].param1);
                    if (current_found_marker)
                    {
                        found_marker = current_found_marker;
                        old_state = curr_state;
                    }
                    if (curr_state->getId() == m_commands[0].param2)
                    {
                        new_state = curr_state;
                    }
                    if (old_state && new_state)
                    {
                        break;
                    }
                }
            }
            if (old_state && new_state)
            {
                old_state->removeMarker(found_marker);
                new_state->addMarker(found_marker);
            }
        }
        m_commands.erase(m_commands.begin());
    }
}

void MarkerCommandQueue::makeAllCommands()
{
    while (m_commands.size() > 0)
    {
        makeCommand();
    }
}

void MarkerCommandQueue::setScene(MapScene* scene)
{
    if (p_instance)
    {
        m_scene = scene;
    }
}
