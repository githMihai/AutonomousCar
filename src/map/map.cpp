#include "map.h"

Map::Map(const std::string jsonFilePath)
{
    Json::Reader reader;
    Json::Value root;
    std::string line;
    std::string jsonMap;
    std::ifstream jsonFile(jsonFilePath);
    if (jsonFile.is_open())
    {
        while (getline(jsonFile, line))
        {
            jsonMap += line;
        }
    }
    else 
    {
        std::cerr << "Map: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }

    if (!reader.parse(jsonMap, root))
    {
        std::cerr << reader.getFormattedErrorMessages() << std::endl;
        throw std::system_error(-1, std::generic_category(), reader.getFormatedErrorMessages());
    }

    this->mapSize = root["NOD"].size();
    for (int i = 0; i < this->mapSize; i++)
    {
        // Node *n = new Node(root["NOD"][i]);
        // std::shared_ptr<Node> n(new Node(root["NOD"][i]));
        NODE_PTR n = std::make_shared<Node>(root["NOD"][i]);
        this->nodesMap[n->name()] = n;
        this->coordMap[n->coord()].push_back(n->name());
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

        node.second->setLinked();
    }
}

void Map::setGoal(std::string nodeName)     { this->goal = this->nodesMap[nodeName]; }
void Map::setStart(std::string nodeName)    { this->start = this->nodesMap[nodeName]; }

Node Map::operator[] (const std::string nodeName)
{
    return *this->nodesMap[nodeName];
}

NodesVect Map::operator[] (const std::complex<double> coord)
{
    NodesVect nodes;
    for (auto &nodeName : this->coordMap[coord])
    {
        nodes.push_back(this->nodesMap[nodeName]);
    }
    return nodes;
}