#ifndef IMAGEPRIMITIVE_H
#define IMAGEPRIMITIVE_H

#include <opencv2/opencv.hpp>

class IImagePrimitive
{
public:
    virtual void draw(cv::Mat& image) = 0;
    virtual void setColor(const cv::Vec3b& color) = 0;
    virtual void setColor(const int red, const int green, const int blue) = 0;
    virtual cv::Vec3b getColor() = 0;
    virtual void setThickness(const int thickness) = 0;
    virtual int getThickness() = 0;
protected:
    cv::Vec3b color;
    int thickness;
};

#endif // IMAGEPRIMITIVE_H