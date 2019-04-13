#ifndef EDGE_H
#define EDGE_H

#ifndef NODE_H
#include "node.h"
#endif

#define EDGE_PTR std::shared_ptr<Edge>

class Edge
{
public:
    /*!
     * \name Edge
     * \brief Construct a new Edge object.
     * \param from      first node of the Edge
     * \param to        second node of the Edge
     * \param cost_     cost of the edge
     */
    Edge(NODE_PTR from, NODE_PTR to, int cost_);

    /*!
     * \name Edge
     * \brief Copy Contructor.
     * \param edge  edge to be copied.
     */
    Edge(Edge& edge);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param edge  edge to be copied
     * \return Edge& 
     */
    Edge& operator=(Edge& edge);

    /*!
     * \name cost
     * \brief Getter for cost_.
     * \return cost_
     */
    int cost();

    /*!
     * \name setCost
     * \brief Set the Cost object.
     * \param cost_     cost of the edge
     */
    void setCost(int cost_);

    /*!
     * name from
     * \brief Getter for form_ node.
     * \return NODE_PTR 
     */
    NODE_PTR from();
    
    /*!
     * \name to
     * \brief Getter for to_ node.
     * \return NODE_PTR 
     */
    NODE_PTR to();

private:
    NODE_PTR from_;
    NODE_PTR to_;
    int cost_;
};

#endif // EDGE_H