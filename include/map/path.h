#ifndef PATH_H
#define PATH_H

#ifndef NODESET_H
#include "nodeset.h"
#endif

#ifndef EDGESET_H
#include "edgeset.h"
#endif

class Path : public NodeSet, public EdgeSet
{
public:
    /*!
     * \name Path
     * \brief Construct a new Path object.
     * \param startingNode
     */
    Path(NODE_PTR startingNode, NODE_PTR destinationNode);

    /*!
     * \name addNode
     * \brief Add a node to the path.
     * \param node      node to be added to path
     */
    void addNode(NODE_PTR node);

    /*!
     * \name removeNode
     * \brief Remove a node from the path.
     * \param node  Pointer to node that will be removed
     */
    void removeNode(NODE_PTR node);


    /*!
     * \name removeNode
     * \brief Remove a node from the path.
     * \param nodeName  name of the node to be removed
     */
    void removeNode(std::string nodeName);

    /*!
     * \name removeNodes
     * \brief Remove all nodes from a coordinates. 
     * \param coord     nodes coordinates
     */
    void removeNodes(std::complex<double> coord);

    /*!
     * \name aStar
     * \brief Searching algorithm to find the shorttest path from start to destination
     * \param startNode     starting node
     */
    void aStar(NODE_PTR startNode);
    
    /*!
     * \name startNode
     * \brief Getter for start_ node
     * \return NODE_PTR 
     */
    NODE_PTR startNode();

    /*!
     * \name destination
     * \brief Getter for destination_ node
     * \return NODE_PTR 
     */
    NODE_PTR destination();


    /*!
     * \name isDestination
     * \brief Verify if the current node is a destiantion node.
     * \param node  current node
     * \return 1 if the node is destination and 0 otherwise
     */
    bool isDestination(NODE_PTR node);

    /*!
     * \name makeEdges
     * \brief Create edges for all nodes of the path.
     */
    void makeEdges();

    NodesVect pathSet;

private:
    NODE_PTR start_;
    NODE_PTR destination_;
    // NodesVect pathSet;
};

#endif // PATH_H