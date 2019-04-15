#ifndef PATH_H
#include "path.h"
#endif // PATH_H

#include <algorithm>
#include <queue>

#ifndef MAP_H
#include "map.h"
#endif // MAP_H

#ifndef HEURISTICS_H
#include "heuristics.h"
#endif // HEURISTICS_H

bool operator<(const NODE_PTR& n1, const NODE_PTR& n2)
{
    return n1->cost() > n2->cost();
}



Path::Path(NODE_PTR startingNode, NODE_PTR destinationNode)
{
    this->start_ = startingNode;
    this->destination_ = destinationNode;
    aStar(this->start_);
}

Path::Path(const std::string startingNode, const std::string destiantionNode)
{
    this->start_ = Map::getInstance().nodePointer(startingNode);
    this->destination_ = Map::getInstance().nodePointer(destiantionNode);
    aStar(this->start_);
}

// Path::Paht(const std::complex<double> startingPosition, std::complex<double> destinationPosition)
// {
    
// }


void Path::addNode(NODE_PTR node)
{
    this->nodesSet[node->name()] = node;
    this->coordMap[node->coord()].push_back(node->name());
}



void Path::removeNode(NODE_PTR node)
{
    this->nodesSet.erase(node->name());
    this->coordMap[node->coord()].erase(std::remove(
            this->coordMap[node->coord()].begin(),
            this->coordMap[node->coord()].end(),
            node->name()
        ),
        this->coordMap[node->coord()].end()
    );
    if (this->coordMap[node->coord()].size() == 0)
    {
        this->coordMap.erase(node->coord());
    }
}



void Path::removeNode(std::string nodeName)
{
    NODE_PTR node = this->nodesSet[nodeName];
    this->removeNode(node);
}



void Path::removeNodes(std::complex<double> coord)
{
    std::vector<std::string> nodes = this->coordMap[coord];
    this->coordMap.erase(coord);
    for (auto const& nodeName : nodes)      { this->nodesSet.erase(nodeName); }
}



NODE_PTR Path::destination()                { return this->destination_; }

bool Path::isDestination(NODE_PTR node)
{
    if (node->coord() == this->destination()->coord())  { return true; }
    return false;
}



void Path::aStar(NODE_PTR startNode)
{
    std::priority_queue<NODE_PTR> frontier;
    NodesVect explored;
    NodesVect explorable;

    NODE_PTR to_explore;
    NODE_PTR node = std::shared_ptr<Node>(new Node(*startNode));
    frontier.push(node);
    while (true)
    {
        if (frontier.empty())       { return; }

        node = frontier.top();
        frontier.pop();

        if (this->isDestination(node))
        {
            this->pathSet.push_back(node);
            this->size_++;

            while (node->parent)
            {
                node = node->parent;
                this->pathSet.push_back(node);
                this->size_++;
            } 
            std::reverse(this->pathSet.begin(), this->pathSet.end());
            this->makeEdges();
            
        }

        explored.push_back(node);

        NodesVect succ;
        if (node->successors(succ) <0)
        {
            std::cerr << "node not linked: " << node << std::endl;
            return;
        } 

        for (auto &succ : succ)
        {
            // if (!node)
            {
                to_explore = std::shared_ptr<Node>(new Node(*succ));
                // std::cout << "succ: " << succ << ", to_expl: " << to_explore << std::endl;
                to_explore->parent = node;
                to_explore->cost_ += node->cost();
            }


            bool ok = true;
            for (auto &s : explorable)
            {
                if (s->name() == succ->name())      { ok = false; }
            }

            bool found = false;
            for (auto &state : explored)
            {
                if (state->name() == succ->name())  { found = true; }
            }

            if (found == false && ok)
            {
                to_explore->cost_ += manhattanHeuristic(to_explore->coord(), this->destination()->coord());
                frontier.push(to_explore);
                explorable.push_back(to_explore);
            }

        }
    }
}

void Path::makeEdges()
{
    for (int i = 0; i < this->pathSet.size()-1; i++)
    {
        // this->edgePath.push_back(std::make_shared<Edge>(this->pathSet.at(i), this->pathSet.at(i+1)));
        EDGE_PTR e = std::make_shared<Edge>(
            this->pathSet.at(i), 
            this->pathSet.at(i+1),
            1
        );
        this->edgePath.push_back(e);
        this->addEdge(e);
    }
}

// Overload
NODE_PTR Path::closest(std::complex<double> coord)
{
    double min = MAXFLOAT;
    NODE_PTR n = NULL;
    for (auto &node : this->pathSet)
    {
        double distance = euclideanHeuristic(node->coord(), coord);
        if (distance < min)
        {
            min = distance;
            n = node;
        }
    }
    return n;
}

EDGE_PTR Path::currentEdge(std::complex<double> coord)
{
    double min1 = MAXFLOAT;
    double min2 = min1;
    NODE_PTR node1= NULL;
    NODE_PTR node2= NULL;
    for (auto &it : this->pathSet)
    {
        double distance = euclideanHeuristic(it->coord(), coord);
        if (distance < min1)
        {
            min2 = min1;
            min1 = distance;
            node2 = node1;
            node1 = it;
        }
        else if (distance < min2 && distance != min1)
        {
            min2 = distance;
            node2 = it;
        }
    }
    if (node1 == NULL || node2 == NULL)     { return NULL; }
    EDGE_PTR e = this->pointerEdge(node1->name(), node2->name());
    if (e == nullptr)  { e = this->pointerEdge(node2->name(), node1->name()); }
    return e;
}

double Path::displacement(std::complex<double> coord)
{
    EDGE_PTR e = this->currentEdge(coord);
    return e->direction(coord);
}