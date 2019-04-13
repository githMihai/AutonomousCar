#ifndef NODESET_H
#include "nodeset.h"
#endif

int NodeSet::size()
{
    return this->size_;
}

Node NodeSet::operator[] (const std::string nodeName)
{
    return *this->nodesSet[nodeName];
}

NodesVect NodeSet::operator[] (const std::complex<double> coord)
{
    NodesVect nodes;
    for (auto &nodeName : this->coordMap[coord])
    {
        nodes.push_back(this->nodesSet[nodeName]);
    }
    return nodes;
}