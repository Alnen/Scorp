#include <QDebug>
#include "Scorp/Core/MarkerCommandQueue.h"
/*
#include "Scorp/GUI/Map/MapScene.h"
#include "Scorp/GUI/Map/StateGraphicsObject.h"
#include "Scorp/GUI/Map/PointGraphicsObject.h"
#include "Scorp/GUI/Map/MarkerObject.h"
*/

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

void MarkerCommandQueue::addMarkerCommand(int id, int state_id, MarkerCommandStruct::MarkerType type)
{
    m_commands.push_back(MarkerCommandStruct(MarkerCommand::Add, id, state_id, type));
}

void MarkerCommandQueue::moveMarkerCommand(int id, int new_state_id)
{
    m_commands.push_back(MarkerCommandStruct(MarkerCommand::Move, id, new_state_id));
}

void MarkerCommandQueue::deleteMarkerCommand(int id)
{
    m_commands.push_back(MarkerCommandStruct(MarkerCommand::Delete, id));
}

bool MarkerCommandQueue::commandExist()
{
    return (!m_commands.empty());
}

MarkerCommandStruct MarkerCommandQueue::getCommand()
{
    MarkerCommandStruct temp = m_commands[0];
    m_commands.erase(m_commands.begin());
    return temp;
}
