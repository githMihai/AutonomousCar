#include "TrafficSignRecognition.h"

VideoWriter videoResult("result1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 20, Size(643, img_height), true);




TrafficSignRecognition::TrafficSignRecognition()
{

	hog = HOGDescriptor (
		Size(32, 32), //winSize
		Size(8, 8), //blocksize
		Size(4, 4), //blockStride, 50% block size
		Size(8, 8), //cellSize,
		9, //nbins,
		1, //derivAper,
		-1, //winSigma,
		0, //histogramNormType,
		0.2, //L2HysThresh,
		0,//gammal correction,
		64,//nlevels=64
		1);

	if (write_video) {
		videoResult.release();
		videoResult = VideoWriter("result1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 20, Size(img_width, img_height), true);
	}

	loadAutoTrainedSVM();

	getSlidingWindowsROIs(imageROIs, imageROIsInBigROI, rect);

}

//TrafficSignRecognition::TrafficSignRecognition(int overlaping_rects_threshold, float overlaping_rects_eps, int step_size_ratio_sliding_windows, int background_images_ratio, int number_of_sliding_windows_widths, float red_hue_color_threshold, float blue_hue_color_threshold, bool write_video, bool show_image_result, int xROIOffset, int yROIOffset, int ROIWidth, int ROIHeight)
//{
//}

//returns just stop sign
void TrafficSignRecognition::trafficSignInImage(Mat src, bool &parkingSign,bool &stopSign)
{		
		CapturedImage capImg;
		capImg.src = src(bigRoi.rect);
		//capImg.filePath = imagePath;
		//capImg.roi = bigRoi.rect;
		addHueMatToCapturedImgROI(capImg);

		//drawRoisTest(capImg, imageROIs);
		
		addPosibleThresholdROIs(capImg, imageROIsInBigROI);

		Mat testMat = Mat::zeros(capImg.possibleSigns.size(), descriptor_size, CV_32FC1);
		createTrainHogMatrix(testMat,capImg.possibleSigns);

		Mat preditionResults = Mat::zeros(capImg.possibleSigns.size(), 1, CV_32FC1);
		svmPredict(testMat, preditionResults);

		vector<Rect> overlappingRectanglesStopSign;
		vector<Rect> overlappingRectanglesParkingSign;
		//just stop sign
	groupOverlappingRectangles(capImg, preditionResults, overlappingRectanglesStopSign, overlappingRectanglesParkingSign,parkingSign,stopSign);
	
}


TrafficSignRecognition::~TrafficSignRecognition()
{
}
