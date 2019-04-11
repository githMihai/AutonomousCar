#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <list>
#include <json/json.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <complex>

#include "node.h"

typedef std::map<std::string, Node*> Dictionary;

class Map
{
public:
    /*!
     * \name Map
     * \brief Construct a new Map object.
     * \param jsonMap   path to json file representing map
     */
    Map(const std::string jsonMap);

    /*!
     * \name Map
     * \brief Copy Constructor.
     * \param obj   object to pe copied
     */
    Map(const Map& obj);
    
    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param obj   object to be copied
     * \return Map& 
     */
    Map& operator=(const Map& obj);

    /*!
     * \name size
     * \brief Returns number of nodes in map.
     * \return number of nodes in map
     */
    int size();

    /*!
     * \name setGoal
     * \brief Set the Goal node.
     * \param nodeName  destination node
     */
    void setGoal(const std::string nodeName);

    /*!
     * \name setGoal
     * \brief Set the Goal node.
     * \param coordinates   represents coordinates of the destination node
     */
    void setGoal(const std::complex<double> coordinates);

    /*!
     * \name setStrat
     * \brief Set the Start node.
     * \param nodeName  start node
     */
    void setStart(const std::string nodeName);

    /*!
     * \name setStart
     * \brief Set the Start node.
     * \param coordinates   coordonates of the start node
     */
    void setStart(const std::complex<double> coordinates);

    /*!
     * \name linkNodes
     * \brief Construct a graph from all nodes.
     */
    void linkNodes();


    Node* goal;             // TODO: private
    Node* start;            // TODO: private
    Dictionary nodesMap;    // TODO: private
    int mapSize;            // TODO: private

private:
    
};

#endif // MAP_H