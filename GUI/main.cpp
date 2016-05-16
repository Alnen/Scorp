#include "mainwindow.h"
#include <QApplication>
//#include "DB/ScorpDBInterface.h"
#include "DB/ScorpDBSell.h"

#include <fstream>
#include <string>

#include <QDebug>

/*
enum class ObjectType
{
    MARKER,
    STATE,
    TRANSITION,
    STATE_DESCRIPTION_BEGIN,
    STATE_DESCRIPTION_END,
    TRANSITION_DESCRIPTION_END,
    TRANSITION_DESCRIPTION_BEGIN,
    IN_STATE_STORAGE_BEGIN,
    IN_STATE_STORAGE_END,
    OUT_STATE_STORAGE_BEGIN,
    OUT_STATE_STORAGE_END,
    IN_TRANSITION_STORAGE_BEGIN,
    IN_TRANSITION_STORAGE_END,
    OUT_TRANSITION_STORAGE_BEGIN,
    OUT_TRANSITION_STORAGE_END
};

void serialize(std::ofstream& output, ObjectType obj_type, int obj_id, int obj_type_id)
{
    if (obj_type == ObjectType::MARKER)
    {
        output << "Marker";
    }
    else if (obj_type == ObjectType::STATE)
    {
        output << "State";
    }
    else if (obj_type == ObjectType::TRANSITION)
    {
        output << "Transition";
    }
    else if (obj_type == ObjectType::STATE_DESCRIPTION_BEGIN)
    {
        output << "StateDescriptionBegin";
    }
    else if (obj_type == ObjectType::STATE_DESCRIPTION_END)
    {
        output << "StateDescriptionEnd";
    }
    else if (obj_type == ObjectType::TRANSITION_DESCRIPTION_BEGIN)
    {
        output << "TransitionDescriptionBegin";
    }
    else if (obj_type == ObjectType::TRANSITION_DESCRIPTION_END)
    {
        output << "TransitionDescriptionEnd";
    }
    else if (obj_type == ObjectType::IN_STATE_STORAGE_BEGIN)
    {
        output << "InStateStorageBegin";
    }
    else if (obj_type == ObjectType::IN_STATE_STORAGE_END)
    {
        output << "InStateStorageEnd";
    }
    else if (obj_type == ObjectType::OUT_STATE_STORAGE_BEGIN)
    {
        output << "OutStateStorageBegin";
    }
    else if (obj_type == ObjectType::OUT_STATE_STORAGE_END)
    {
        output << "OutStateStorageEnd";
    }
    else if (obj_type == ObjectType::IN_TRANSITION_STORAGE_BEGIN)
    {
        output << "InTransitionStorageBegin";
    }
    else if (obj_type == ObjectType::IN_TRANSITION_STORAGE_END)
    {
        output << "InTransitionStorageEnd";
    }
    else if (obj_type == ObjectType::OUT_TRANSITION_STORAGE_BEGIN)
    {
        output << "OutTransitionStorageBegin";
    }
    else if (obj_type == ObjectType::OUT_TRANSITION_STORAGE_END)
    {
        output << "OutTransitionStorageEnd";
    }
    else
    {
        output << "Unknown";
    }
    output << "," << std::to_string(obj_id);
    output << "," << std::to_string(obj_type_id);
}

void deserialize(std::ifstream& input, ObjectType* obj_type, int* obj_id, int* obj_type_id)
{
    std::string obj_type_line;
    std::string obj_id_line;
    std::string obj_type_id_line;
    std::getline(input, obj_type_line, ',');
    std::getline(input, obj_id_line, ',');
    std::getline(input, obj_type_id_line);
    if (obj_type)
    {
        if (obj_type_line.compare("Marker") == 0)
        {
            *obj_type = ObjectType::MARKER;
        }
        else if (obj_type_line.compare("State") == 0)
        {
            *obj_type = ObjectType::STATE;
        }
        else if (obj_type_line.compare("Transition") == 0)
        {
            *obj_type = ObjectType::TRANSITION;
        }
        else if (obj_type_line.compare("StateDescriptionBegin") == 0)
        {
            *obj_type = ObjectType::STATE_DESCRIPTION_BEGIN;
        }
        else if (obj_type_line.compare("StateDescriptionEnd") == 0)
        {
            *obj_type = ObjectType::STATE_DESCRIPTION_END;
        }
        else if (obj_type_line.compare("TransitionDescriptionBegin") == 0)
        {
            *obj_type = ObjectType::TRANSITION_DESCRIPTION_BEGIN;
        }
        else if (obj_type_line.compare("TransitionDescriptionEnd") == 0)
        {
            *obj_type = ObjectType::TRANSITION_DESCRIPTION_END;
        }
        else if (obj_type_line.compare("InStateStorageBegin") == 0)
        {
            *obj_type = ObjectType::IN_STATE_STORAGE_BEGIN;
        }
        else if (obj_type_line.compare("InStateStorageEnd") == 0)
        {
            *obj_type = ObjectType::IN_STATE_STORAGE_END;
        }
        else if (obj_type_line.compare("OutStateStorageBegin") == 0)
        {
            *obj_type = ObjectType::OUT_STATE_STORAGE_BEGIN;
        }
        else if (obj_type_line.compare("OutStateStorageEnd") == 0)
        {
            *obj_type = ObjectType::OUT_STATE_STORAGE_END;
        }
        else if (obj_type_line.compare("InTransitionStorageBegin") == 0)
        {
            *obj_type = ObjectType::IN_TRANSITION_STORAGE_BEGIN;
        }
        else if (obj_type_line.compare("InTransitionStorageEnd") == 0)
        {
            *obj_type = ObjectType::IN_TRANSITION_STORAGE_END;
        }
        else if (obj_type_line.compare("OutTransitionStorageBegin") == 0)
        {
            *obj_type = ObjectType::OUT_TRANSITION_STORAGE_BEGIN;
        }
        else if (obj_type_line.compare("OutTransitionStorageEnd") == 0)
        {
            *obj_type = ObjectType::OUT_TRANSITION_STORAGE_END;
        }
    }
    if (obj_id)
    {
        if (!(istringstream(obj_id_line) >> *obj_id)) *obj_id = 0;
    }
    if (obj_type_id)
    {
        if (!(istringstream(obj_type_id_line) >> *obj_type_id)) *obj_type_id = 0;
    }
}
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ScorpDBSell* db_interface = new ScorpDBSell("../Scorp/DB/ScorpDB.db", false);

    //std::ifstream ifs("../Scorp/test.txt", std::ifstream::in);
    //deserialize(ifs);

    MainWindow w;
    w.show();

    return a.exec();
}
