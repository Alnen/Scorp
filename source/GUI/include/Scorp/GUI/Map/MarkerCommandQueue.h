#ifndef MARKER_COMMAND_QUEUE_H
#define MARKER_COMMAND_QUEUE_H

#include <vector>
#include "Scorp/GUI/Map/MarkerObject.h"

enum class MarkerCommand { ADD, DELETE, MOVE };

struct MarkerCommandStruct
{
    MarkerCommand command;
    int param1;
    int param2;
    MarkerObject::MarkerType markerType;
    MarkerCommandStruct(MarkerCommand cmd, int p1=0, int p2=0, MarkerObject::MarkerType type = MarkerObject::MarkerType::Train)
        : command(cmd), param1(p1), param2(p2), markerType(type)
    {
    }
};

class MapScene;
class MarkerCommandQueue;

class MarkerCommandQueueDestroyer
{
  private:
    MarkerCommandQueue* p_instance;
  public:
    ~MarkerCommandQueueDestroyer();
    void initialize(MarkerCommandQueue* p);
};

class MarkerCommandQueue
{
private:
    static MarkerCommandQueue* p_instance;
    static MarkerCommandQueueDestroyer destroyer;
    std::vector<MarkerCommandStruct> m_commands;
    MapScene* m_scene;

protected:
    MarkerCommandQueue();
    MarkerCommandQueue(const MarkerCommandQueue&);
    MarkerCommandQueue& operator=(MarkerCommandQueue&);
    ~MarkerCommandQueue() {}

    friend class MarkerCommandQueueDestroyer;

public:
    static MarkerCommandQueue& getInstance();
    void addMarkerCommand(int id, int state_id, MarkerObject::MarkerType type);
    void moveMarkerCommand(int id, int new_state_id);
    void deleteMarkerCommand(int id);
    void makeCommand();
    void makeAllCommands();
    void setScene(MapScene* scene);
};

#endif // MARKER_COMMAND_QUEUE_H
