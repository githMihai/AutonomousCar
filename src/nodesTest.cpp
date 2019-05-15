#if 0

#include "pathtracking.h"

int main(int argc, char** argv)
{
    PathTracking p("NOD1", "NOD133");
    std::cout << "Current pos: " << p.globalPosition().getPosition() << std::endl;
    std::cout << "Points: " << p.nextNode() << std::endl;
    std::vector<std::complex<double>> vect;
    p.nextNodes(10, vect);
    for (int i = 0; i < 10; i++)
    {
        std::cout << vect[i] << std::endl;
    }
    return 0;
}

#endif