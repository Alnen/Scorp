#ifndef MARKER_OBJECT_H
#define MARKER_OBJECT_H

class StateGraphicsObject;

class MarkerObject
{
public:
    MarkerObject(int id, int color, StateGraphicsObject* parent = nullptr);
public:
    void connectToState(StateGraphicsObject* state);
    void disconnectFromState();
    int getId();
    int getColor();

private:
    int m_id;
    int m_color;
    StateGraphicsObject* m_parentState;
};

#endif // MARKER_OBJECT_H
