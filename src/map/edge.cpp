#ifndef EDGE_H
#include "edge.h"
#endif

Edge::Edge(NODE_PTR from, NODE_PTR to, int cost_)
{
    this->from_ = from;
    this->to_ = to;
    this->cost_ = cost_;
}

Edge::Edge(Edge& edge)
{
    this->from_ = edge.from();
    this->to_ = edge.to();
    this->cost_ = edge.cost();
}

Edge& Edge::operator=(Edge& edge)
{
    this->from_ = edge.from();
    this->to_ = edge.to();
    this->cost_ = edge.cost();
    return *this;
}

int Edge::cost()
{
    return this->cost_;
}

void Edge::setCost(int cost_)
{
    this->cost_ = cost_;
}

NODE_PTR Edge::from()   { return this->from_; }
NODE_PTR Edge::to()     { return this->to_; }