#ifndef NODESET_H
#define NODESET_H

#ifndef NODE_H
#include "node.h"
#endif

template<typename T> struct less {};
    template <typename T>
    struct less<std::complex<T> >
    {
        bool operator() (std::complex<T> const& a, std::complex<T> const&b)
        {
            return std::array<T,2>{a.real(), a.imag()} < std::array<T,2>{b.real(), b.imag()};
        }
    };

typedef std::map<std::string, NODE_PTR> Dictionary;
typedef std::map<std::complex<double>, std::vector<std::string>, struct less<std::complex<double> >> CoordMapping;


class NodeSet
{
public:
    /*!
     * \name size
     * \brief Returns number of nodes in set.
     * \return number of nodes in set
     */
    virtual int size();

    /*!
     * \name operator[]
     * \brief Returns the node with specified name from the set.
     * \param nodeName  name of the desired node
     * \return Node 
     */
    virtual Node operator[] (const std::string nodeName);

    /*!
     * \name operator[]
     * \brief Returns the nodes placed at the specidied coordinates.
     * \param coord     coordinates of the desired nodes
     * \return std::vector<Node> 
     */
    virtual NodesVect operator[] (const std::complex<double>coord);
    
    Dictionary nodesSet;
    CoordMapping coordMap;
    int size_;

private:
};

#endif // NODESET_H