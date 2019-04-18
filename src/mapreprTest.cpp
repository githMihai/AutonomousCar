#if 1

#include "maprepresentation.h"
#include "path.h"

int main(int argc, char** argv)
{
    // Path p("NOD1", "NOD133");
    Path p("NOD1", "NOD46");
    MapRepresentation m("resources/Race_resize.png", 23, 23, 834, 518, 45);
    m.addPrimitives(p.edgePath);
    m.drawAll();

    return 0;
}

#endif