#ifndef MAP_H
#include "map.h"
#endif

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

    this->size_ = root["NOD"].size();
    for (int i = 0; i < this->size_; i++)
    {
        // Node *n = new Node(root["NOD"][i]);
        // std::shared_ptr<Node> n(new Node(root["NOD"][i]));
        NODE_PTR n = std::make_shared<Node>(root["NOD"][i]);
        this->nodesSet[n->name()] = n;
        this->coordMap[n->coord()].push_back(n->name());
    }
}

Map::Map(const Map& obj)
{
    this->size_ = obj.size_;
    this->nodesSet = obj.nodesSet;
    this->goal = obj.goal;
    this->start = obj.start;
}

Map& Map::operator=(const Map& obj)
{
    this->size_ = obj.size_;
    this->nodesSet = obj.nodesSet;
    this->goal = obj.goal;
    this->start = obj.start;
    return *this;
}

void Map::linkNodes()
{
    // Dictionary::iterator node;
    // for ( node = this->nodesSet.begin(); node != this->nodesSet.end(); node++ )
    for (auto & node : this->nodesSet)
    {
        node.second->inBack      = (node.second->arcs()[IN_BACK] != "none") ? this->nodesSet[node.second->arcs()[IN_BACK]] : NULL;
        node.second->inRight     = (node.second->arcs()[IN_RIGHT] != "none") ? this->nodesSet[node.second->arcs()[IN_RIGHT]] : NULL;
        node.second->inAhead     = (node.second->arcs()[IN_AHEAD] != "none") ? this->nodesSet[node.second->arcs()[IN_AHEAD]] : NULL;
        node.second->inLeft      = (node.second->arcs()[IN_LEFT] != "none") ? this->nodesSet[node.second->arcs()[IN_LEFT]] : NULL;
        node.second->outBack     = (node.second->arcs()[OUT_BACK] != "none") ? this->nodesSet[node.second->arcs()[OUT_BACK]] : NULL;
        node.second->outRight    = (node.second->arcs()[OUT_RIGHT] != "none") ? this->nodesSet[node.second->arcs()[OUT_RIGHT]] : NULL;
        node.second->outAhead    = (node.second->arcs()[OUT_AHEAD] != "none") ? this->nodesSet[node.second->arcs()[OUT_AHEAD]] : NULL;
        node.second->outLeft     = (node.second->arcs()[OUT_LEFT] != "none") ? this->nodesSet[node.second->arcs()[OUT_LEFT]] : NULL;

        this->addEdge(node.second, node.second->inBack, 1);
        this->addEdge(node.second, node.second->inRight, 1);
        this->addEdge(node.second, node.second->inAhead, 1);
        this->addEdge(node.second, node.second->inLeft, 1);
        this->addEdge(node.second, node.second->outBack, 1);
        this->addEdge(node.second, node.second->outRight, 1);
        this->addEdge(node.second, node.second->outAhead, 1);
        this->addEdge(node.second, node.second->outLeft, 1);

        node.second->setLinked();
    }
}

void Map::setGoal(std::string nodeName)     { this->goal = this->nodesSet[nodeName]; }
void Map::setStart(std::string nodeName)    { this->start = this->nodesSet[nodeName]; }

// TODO: remove commented
// Node Map::operator[] (const std::string nodeName)
// {
//     return *this->nodesSet[nodeName];
// }

// NodesVect Map::operator[] (const std::complex<double> coord)
// {
//     NodesVect nodes;
//     for (auto &nodeName : this->coordMap[coord])
//     {
//         nodes.push_back(this->nodesSet[nodeName]);
//     }
//     return nodes;
// }