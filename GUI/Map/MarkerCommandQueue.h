#ifndef MARKER_COMMAND_QUEUE_H
#define MARKER_COMMAND_QUEUE_H

#include <vector>

enum class MarkerCommand { ADD, DELETE, MOVE };

struct MarkerCommandStruct
{
    MarkerCommand command;
    int param1;
    int param2;
    MarkerCommandStruct(MarkerCommand cmd, int p1=0, int p2=0)
        : command(cmd), param1(p1), param2(p2)
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
    void addMarkerCommand(int id, int state_id);
    void moveMarkerCommand(int id, int new_state_id);
    void deleteMarkerCommand(int id);
    void makeCommand();
    void setScene(MapScene* scene);
};

#endif // MARKER_COMMAND_QUEUE_H
