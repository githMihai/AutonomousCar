#ifndef PATH_H
#include "path.h"
#endif

#include <algorithm>
#include <queue>

float manhattanHeuristic(std::complex<double> position, std::complex<double> goal)
{
    return abs(position.real() - goal.real()) + abs(position.imag() - goal.imag());
}


bool operator<(const NODE_PTR& n1, const NODE_PTR& n2)
{
    return n1->cost() > n2->cost();
}

Path::Path(NODE_PTR startingNode, NODE_PTR destinationNode)
{
    this->destination_ = destinationNode;
    aStar(startingNode);
}

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
    for (auto const& nodeName : nodes)
    {
        this->nodesSet.erase(nodeName);
    }
}

NODE_PTR Path::destination()
{
    return this->destination_;
}

bool Path::isDestination(NODE_PTR node)
{
    if (node->coord() == this->destination()->coord())
    {
        return true;
    }
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
        if (frontier.empty())
        {
            return;
        }

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
                if (s->name() == succ->name())
                {
                    ok = false;
                }
            }

            bool found = false;
            for (auto &state : explored)
            {
                if (state->name() == succ->name())
                {
                    found = true;
                }
            }

            if (found == false && ok)
            {
                to_explore->cost_ += manhattanHeuristic(to_explore->coord(), this->destination()->coord());
                frontier.push(to_explore);
                explorable.push_back(to_explore);
            }

            // bool ok
        }
    }
}

void Path::makeEdges()
{

}