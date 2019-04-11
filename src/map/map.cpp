#include "map.h"

Map::Map(const std::string jsonMap)
{
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(jsonMap, root))
    {
        std::cerr << reader.getFormattedErrorMessages() << std::endl;
        throw std::system_error(-1, std::generic_category(), reader.getFormatedErrorMessages());
    }

    this->mapSize = root["NOD"].size();
    for (int i = 0; i < this->mapSize; i++)
    {
        Node *n = new Node(root["NOD"][i]);
        this->nodesMap[n->name()] = n;
    }
}

Map::Map(const Map& obj)
{
    this->mapSize = obj.mapSize;
    this->nodesMap = obj.nodesMap;
    this->goal = obj.goal;
    this->start = obj.start;
}

Map& Map::operator=(const Map& obj)
{
    this->mapSize = obj.mapSize;
    this->nodesMap = obj.nodesMap;
    this->goal = obj.goal;
    this->start = obj.start;
    return *this;
}

void Map::linkNodes()
{
    // Dictionary::iterator node;
    // for ( node = this->nodesMap.begin(); node != this->nodesMap.end(); node++ )
    for (auto & node : this->nodesMap)
    {
        node.second->inBack      = (node.second->arcs()[IN_BACK] != "none") ? this->nodesMap[node.second->arcs()[IN_BACK]] : NULL;
        node.second->inRight     = (node.second->arcs()[IN_RIGHT] != "none") ? this->nodesMap[node.second->arcs()[IN_RIGHT]] : NULL;
        node.second->inAhead     = (node.second->arcs()[IN_AHEAD] != "none") ? this->nodesMap[node.second->arcs()[IN_AHEAD]] : NULL;
        node.second->inLeft      = (node.second->arcs()[IN_LEFT] != "none") ? this->nodesMap[node.second->arcs()[IN_LEFT]] : NULL;
        node.second->outBack     = (node.second->arcs()[OUT_BACK] != "none") ? this->nodesMap[node.second->arcs()[OUT_BACK]] : NULL;
        node.second->outRight    = (node.second->arcs()[OUT_RIGHT] != "none") ? this->nodesMap[node.second->arcs()[OUT_RIGHT]] : NULL;
        node.second->outAhead    = (node.second->arcs()[OUT_AHEAD] != "none") ? this->nodesMap[node.second->arcs()[OUT_AHEAD]] : NULL;
        node.second->outLeft     = (node.second->arcs()[OUT_LEFT] != "none") ? this->nodesMap[node.second->arcs()[OUT_LEFT]] : NULL;
    }
}

void Map::setGoal(std::string nodeName)     { this->goal = this->nodesMap[nodeName]; }
void Map::setStart(std::string nodeName)    { this->start = this->nodesMap[nodeName]; }