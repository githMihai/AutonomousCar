#if 1

#include "caffenetwork.h"
#include "timer.h"
#include "image.h"

int main(int argc, char **argv)
{
    CaffeNetwork caffe("resources/model.prototxt", "/home/mihai/Workspace/BOSCH_2019/Dataset/video_process/snap_a/CAR_iter_4000.caffemodel");
    Image img("/home/mihai/Workspace/BOSCH_2019/Dataset/16_apr/output_1555404694.9536414.avi");
    Timer t("t", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();
    img.attachObserver(&caffe);
    sleep(60);
    t.stop();
    img.close();

    // cv::Mat m = cv::imread("/home/mihai/Pictures/FoxSpeed.png");
    // cv::imshow("image", m);
    // cv::waitKey();

    return 0;
}

#endif