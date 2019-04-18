#ifndef MAPREPRESENTATION_H
#define MAPREPRESENTATION_H

#ifndef IMAGEPRIMITIVE_H
#include "imageprimitive.h"
#endif // IMAGEPRIMITIVE_H

#ifndef IMAGECOORD_H
#include "imagecoord.h"
#endif // IMAGECOORD_H

#ifndef EDGE_H
#include "edge.h"
#endif // EDGE_H

#ifndef LINE_H
#include "line.h"
#endif // LINE_H

class MapRepresentation
{
public:
    MapRepresentation(const std::string imagePath, const int startX=0, const int startY=0, const int endX=0, const int endY=0, const int distance=1);
    void addPrimitives(const std::vector<EDGE_PTR> edges);
    void addPrimitives(const std::vector<NODE_PTR> nodes);
    void addPrimitive(const NODE_PTR node);
    void addPrimitive(const EDGE_PTR edge);
    void drawAll();
private:
    std::string imagePath;
    cv::Mat image;
    // std::vector<IImagePrimitive> primitives;
    std::vector<Line*> primitives;
    ImageCoord imgCoord;
};

#endif // MAPREPRESENTATION_H