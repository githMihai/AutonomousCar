#ifndef MAPREPRESENTATION_H
#include "maprepresentation.h"
#endif // MAPREPRESENTATION_H

// #ifndef LINE_H
// #include "line.h"
// #endif // LINE_H

#ifndef POINT_H
#include "point.h"
#endif // POINT_H

MapRepresentation::MapRepresentation(const std::string imagePath, const int startX=0, const int startY=0, const int endX=0, const int endY=0, const int distance=1)
{
    this->imgCoord = ImageCoord(startX, startY, endX, endY, distance);
    this->imagePath = imagePath;
    this->image = cv::imread(this->imagePath, CV_LOAD_IMAGE_COLOR);
}

void MapRepresentation::addPrimitives(const std::vector<EDGE_PTR> edges)
{
    for (auto &edge : edges)
    {
        Line* line = new Line(*edge);
        line->setLine(this->imgCoord.toImageCoord(line->getP0()),
                     this->imgCoord.toImageCoord(line->getP1()));
        this->primitives.push_back(line);
    }
}
void MapRepresentation::addPrimitives(const std::vector<NODE_PTR> nodes) {}
void MapRepresentation::addPrimitive(const NODE_PTR node) {}

void MapRepresentation::drawAll()
{
    std::cout << "draw: " << this->primitives.size() << " primitives" << std::endl;
    for (auto &prim : this->primitives)
    {
        prim->draw(this->image);
        cv::imshow("image", this->image);
        cv::waitKey();
    }
}