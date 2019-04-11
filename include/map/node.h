#ifndef NODE_H
#define NODE_H

// #ifndef DICTIONARY_H
// #include "dictionary.h"
// #endif

#include <iostream>
#include <string>
#include <json/json.h>
#include <complex>

typedef enum arc
{
    IN_BACK = 0,
    IN_RIGHT,
    IN_AHEAD,
    IN_LEFT,
    OUT_BACK,
    OUT_RIGHT,
    OUT_AHEAD,
    OUT_LEFT
} Arcs;

// std::vector<std::string> nodeArcs = 
// {
//     "IN_BACK",
//     "IN_RIGHT",
//     "IN_AHEAD",
//     "IN_LEFT",
//     "OUT_BACK",
//     "OUT_RIGHT",
//     "OUT_AHEAD",
//     "OUT_LEFT"
// };

class Node
{
    // typedef std::map<std::string, Node*> Dictionary;
public:

    /*!
     * \name Node
     * \brief Construct for null node
     */
    Node();

    /*!
     * \name Node
     * \brief Constructor method. 
     * \param node:      node extracted from json file
     */
    Node(Json::Value node);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param obj   Node object to be copied
     * \return copy of obj 
     */
    Node& operator=(Node& obj);

    /*!
     * \name Nodee
     * \brief Copy construcor.
     */
    Node(Node&);
    
    /*!
     * \name name
     * \brief Getter for name_
     * \return name 
     */
    const std::string& name() const;
    
    /*!
     * \name coordinates
     * \brief Getter for coordinates_
     * \return Coordinates 
     */
    const std::complex<double>& coord() const;

    /*!
     * \name cost
     * \brief Getter for cost_
     * \return cost
     */
    int cost() const;

    /*!
     * \name arcs
     * \brief Getter for arcs_
     * \return arcs
     */
    const std::vector<std::string>& arcs() const;

    // /*!
    //  * \name link
    //  * \brief This mothod makes connections to neighboring nodes.
    //  * \param dict  Dictionary that contains more nodes
    //  */
    // void link(Dictionary *dict);

    friend std::ostream& operator<< (std::ostream& stream, const Node& node);

    Node* inBack;
    Node* inRight;
    Node* inAhead;
    Node* inLeft;
    Node* outBack;
    Node* outRight;
    Node* outAhead;
    Node* outLeft;

private:
    std::string name_;
    std::complex<double> coord_;
    int cost_;
    std::vector<std::string> arcs_;
};

#endif // NODE_H