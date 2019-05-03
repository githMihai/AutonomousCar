#ifndef IMAGE_H
#include "image.h"
#endif // IMAGE_H

#include <exception>

Image::Image(int width, int height, double exposure, int mode) :
    width_(width), height_(height), exposure_(exposure), mode_(mode),
    cap(cv::VideoCapture(0)), frameNo_(0), updateTime(std::time(nullptr))
{
    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    this->cap.set(cv::CAP_PROP_EXPOSURE, exposure);
}

Image::Image(std::string path, int width, int height, int mode) :
    width_(width), height_(height), mode_(mode),
    cap(cv::VideoCapture(path)), frameNo_(0), updateTime(std::time(nullptr))
{
    std::cout << path << std::endl;
    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    // cv::startWindowThread();
}

Image::Image(const Image& img)
{
    this->cap = img.cap;
    this->updateTime = img.updateTime;
    this->width_ = img.width_;
    this->height_ = img.height_;
    this->exposure_ = img.exposure_;
    this->mode_ = img.mode_;
    this->frameNo_ = img.frameNo_;
}

Image& Image::operator=(const Image& img)
{
    this->cap = img.cap;
    this->updateTime = img.updateTime;
    this->width_ = img.width_;
    this->height_ = img.height_;
    this->exposure_ = img.exposure_;
    this->mode_ = img.mode_;
    this->frameNo_ = img.frameNo_;
    return *this;
}

void Image::setSize(int width, int height)
{
    this->width_ = width;
    this->height_ = height;
    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, this->width_);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, this->height_);
}

void Image::setExposure(double exposure)
{
    this->exposure_ = exposure;
    this->cap.set(cv::CAP_PROP_EXPOSURE, this->exposure_);
}

void Image::setColorMode(int mode)
{
    this->mode_ = mode;
}

void Image::read(cv::Mat& frame)
{
    if (this->cap.isOpened())   { this->cap.read(frame); }
    else                        { throw std::system_error(-1, std::generic_category(), "VideoCapture not opened."); }
}

void Image::nextFrame(void*)
{
    if (this->cap.isOpened())   
    { 
        this->cap.read(this->frame_); 
        // cv::imshow("image", this->frame_); 
        this->notifyObservers();
    }
    else                        { throw std::system_error(-1, std::generic_category(), "VideoCapture not opened."); }
}

void Image::close()
{
    this->cap.release();
}

int Image::width()          { return this->width_; }
int Image::height()         { return this->height_; }
double Image::exposure()    { return this->exposure_; }
int Image::mode()           { return this->mode_; }
int Image::frameNo()        { return this->frameNo_; }
cv::Mat& Image::frame()     { return this->frame_; }