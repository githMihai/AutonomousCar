#if 1

#include "pathtracking.h"

#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <unistd.h>

#include <signal.h>

#include "carcontrol.h"

#include "findfile.h"

#include "RTIMULib.h"

#include "math.h"

#include <opencv2/core.hpp>

#include <opencv2/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <pthread.h>

#include <cstdlib>

#include <unistd.h>   

#include <errno.h>    

#include "pathDrriveTest.hpp"

#include <time.h>   /* Needed for struct timespec */

using namespace std;

using namespace cv;

#define NUM_THREADS 5
#define PI 3.14159265

pthread_mutex_t lock_encoder; 
pthread_mutex_t lock_yaw; 
pthread_mutex_t lock_XY; 
pthread_mutex_t lock_StableXY; 

CarControl *c;
RTIMU *imu;

int sampleCount = 0;
int sampleRate = 0;
float xAnt = Map::getInstance()["NOD1"].real();
float yAnt = 513.5 - Map::getInstance()["NOD1"].real();
float x = 0.0;
float y = 63.0;
float rotPerMetter = 148.00 ; // = 1 cm ? ...1 pixel = 1.28
int totalRots = 0;
uint64_t rateTimer;
uint64_t displayTimer;
uint64_t now;
vector<Point> points;
float curent_yaw_rad = 0.0;
float initial_angle_yaw = 0.0;
Mat mapTrack;
int readingCounter = 0;
int readingIMUCounter = 0;
float stableX = 0.0, stableY = 63.0;

bool runningIMU = false;


int nsleep(long miliseconds)
{
   struct timespec req, rem;

   if(miliseconds > 999)
   {   
        req.tv_sec = (int)(miliseconds / 1000);                            
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; 
   }   
   else
   {   
        req.tv_sec = 0;                         
        req.tv_nsec = miliseconds * 1000000;    
   }   
    //printf("time: %f",req.tv_nsec);
   return nanosleep(&req , NULL);
}

void interruptHandler(sig_t s){
    c->brake(0.0);
    c->close();
    exit(1); 
}

void initDisplay(){

   mapTrack = imread("/media/pi/PiDrive1/Holistic2/master/resources/Race_resize_mark.png");
    
}

void displayPosition(int x, int y){

    //circle(mapTrack, Point(x,y), 3, Scalar(255, 0, 0), 3);
//    imshow("map",mapTrack);
    //waitKey(1);

}

void storePoint(int x, int y){

    points.push_back(Point(x,y));

}

void drawPointsOnMap(){

    // Mat curve_points(points, true);
	// curve_points.convertTo(curve_points, CV_32S); //adapt type for polylines
	// polylines(mapTrack, curve_points, false, Scalar(255,0,0), 1, CV_AA);
	for(Point p : points){
        circle(mapTrack,p,1,Scalar(0,255,0),1);
    }

}

void setAngleToZero(){

        imu->IMURead();
        RTIMU_DATA imuData = imu->getIMUData();
        float theta = (float)imuData.fusionPose.z();
        initial_angle_yaw = theta;
        printf("initial angle:%f\n",initial_angle_yaw);
}

void readIMU(){
    int sum = 0.0;
    int reads = 0;
    while(runningIMU){
        usleep(imu->IMUGetPollInterval() * 1000);
        
        imu->IMURead();
        RTIMU_DATA imuData = imu->getIMUData();
        float theta = (float)imuData.fusionPose.z();
        int localRots = 0;
        
        pthread_mutex_lock(&lock_yaw);
            //localRots = totalRots;
            //totalRots = 0;
            curent_yaw_rad = theta - initial_angle_yaw;
        pthread_mutex_unlock(&lock_yaw); 
        // double flt = localRots;
        // double du = flt / rotPerMetter;
        
        // x = xAnt + (du * (float)cos(theta));
        // y = yAnt + (du * (float)sin(theta));
        // printf("@@@@@@@@@@@@@@@@@@@@ Sample rate %d: %s sumX: %f sumY: %f flt: %f du: %f theta: %f X: %f Y: %f Pozitive angle: %d @@@\r", sampleRate, RTMath::displayDegrees("", imuData.fusionPose),(du * cos(theta)),(du * sin(theta)),flt,du,theta,x,y, (int)(imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE + 180));
        // //printf("sumX: %f sumY: %f flt: %f du: %f theta: %f \n",(du * cos(theta)),(du * sin(theta)),flt,du,theta);
        // //printf("X: %f Y: %f \n",x,y);
        // //printf("Pozitive angle: %d\n", (int)(imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE + 180));
        // xAnt = x;
        // yAnt = y;
        // storePoint(x, y);
    }
}
// void driveClothoid(Point start, vector<PointAndAgle> &dstPoints, Mat &dst) {
// 	Point curent = start;
// 	float xAnt = start.x, yAnt = start.y;
// 	float du = 10;
// 	float curentAngle = PI;
// 	for (int i = 0; i < dstPoints.size() - 1; i++)
// 	{

// 		Point dstPoint = dstPoints.at(i).p;
// 		bool pointReached = pointInsideCircle(curent, dstPoints.at(i).p);
// 		float curentToNextPointAngle = PI - abs(atan2(curent.y - dstPoint.y, curent.x - dstPoint.x)); // curent angle from IMU;
// 			/*if (curentToNextPointAngle > 0.401426) {
// 				curentAngle += 0.401426;
// 			}
// 			else {
// 				curentAngle += curentToNextPointAngle;
// 			}*/
// 		curentAngle = curentToNextPointAngle;
// 		while (!pointReached) {
// 			float distanceToPoint, angleToPoint;

// 			//distanceAnglePointToVector(curent, dstPoints.at(i).p, dstPoints.at(i + 1).p, distanceToPoint, angleToPoint);
// 			float ap2 = dstPoints.at(i).angle;
// 			Clothoid::valueType k = 0.0, dk = 0.0, L = 0.0;
// 			Clothoid::buildClothoid(curent.x, curent.y, curentAngle, dstPoint.x, dstPoint.y, ap2, k, dk, L);
// 			Clothoid::valueType C[1], S[1];
// 			for (int j = 0; j < 100; j++)
// 			{
// 				double tvec = (L / 100)*j;
// 				Clothoid::GeneralizedFresnelCS(1, dk*tvec*tvec, k*tvec, curentAngle, C, S);
// 				Point p11 = Point(curent.x + tvec * C[0], curent.y + tvec * S[0]);
// 				curent = p11;
// 				float curentToNextPointAngle = PI - abs(atan2(curent.y - p11.y, curent.x - p11.x)); // curent angle from IMU;
// 				if (curentToNextPointAngle > 0.401426) {
// 					curentAngle += 0.401426;
// 				}
// 				else {
// 					curentAngle += curentToNextPointAngle;
// 				}
// 				//curentAngle = curentToNextPointAngle;
// 				pointReached = pointInsideCircle(curent, dstPoints.at(i).p);
// 				circle(dst, curent, 1, Scalar(255, 0, 255));
// 			}
			
// 		}

// 	}

// }

void* print(void* str)
{
    //std::cout << (char*)str << std::endl;
    //while (imu->IMURead()) {
    //imu->IMURead();
    //RTIMU_DATA imuData = imu->getIMUData();
    //sampleCount++;

    //now = RTMath::currentUSecsSinceEpoch();

    //  display 10 times per second 198906242
    float local_yaw = 0.0;
    pthread_mutex_lock(&lock_yaw);
        //todo swap comments
        //totalRots += atoi( (char*)str);
        local_yaw = curent_yaw_rad;
        //totalRots += 30.0;
    pthread_mutex_unlock(&lock_yaw); 
        double flt = atoi( (char*)str);
        double du = (flt / rotPerMetter);
        float locx = xAnt + (du * (float)cos(local_yaw));
        float locy = yAnt + (du * (float)sin(local_yaw)); // + sin.. for y growind downwords

    pthread_mutex_lock(&lock_XY);
        x = locx;
        y = locy;
        xAnt = x;
        yAnt = y;
    pthread_mutex_unlock(&lock_XY); 
        //printf("@@@@@@@@@@@@@@@@@@@@ Sample rate %d: %s sumX: %f sumY: %f flt: %f du: %f theta: %f X: %f Y: %f Pozitive angle: %d @@@\r", sampleRate, RTMath::displayDegrees("", imuData.fusionPose),(du * cos(theta)),(du * sin(theta)),flt,du,theta,x,y, (int)(imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE + 180));
        //printf("sumX: %f sumY: %f flt: %f du: %f theta: %f \n",(du * cos(theta)),(du * sin(theta)),flt,du,theta);
        
        //printf("Pozitive angle: %d\n", (int)(imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE + 180) * RTMATH_DEGREE_TO_RAD);
        // xAnt = x;
        // yAnt = y;
        //printf("X: %f Y: %f \n",x,y);
        readingCounter++;
        if(readingCounter == 20){
        pthread_mutex_lock(&lock_StableXY);
            stableX = x, stableY = y;
        pthread_mutex_unlock(&lock_StableXY); 
            
            
            storePoint(x, y);
            readingCounter = 0;
        }


    // if ((now - displayTimer) > 1) {
    //     printf("Enc: %s Sample rate %d: %s\n",(char*)str, sampleRate, RTMath::displayDegrees("", imuData.fusionPose));
    //     fflush(stdout);
    //     displayTimer = now;
    //     //double flt = atof( (char*)str);
    //     double flt = 30.0;
    //     double du = flt / rotPerMetter;
    //     float theta = (float)imuData.fusionPose.z();
    //     x = xAnt + (du * (float)cos(theta));
    //     y = yAnt + (du * (float)sin(theta));
    //     printf("sumX: %f sumY: %f flt: %f du: %f theta: %f \n",(du * cos(theta)),(du * sin(theta)),flt,du,theta);
    //     printf("X: %f Y: %f \n",x,y);
    //     printf("Pozitive angle: %d\n", (int)(imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE + 180));
    //     xAnt = x;
    //     yAnt = y;
    //     storePoint(x, y);
    // }

    // //  update rate every second
    // if ((now - rateTimer) > 100) {
    //     sampleRate = sampleCount;
    //     sampleCount = 0;
    //     rateTimer = now;
    //     //displayPosition(x,y);
    // }
    // if ((now - rateTimer) > 100) {
    //     printf("drawing\n");
    //     displayPosition(x,y);
    // }
//}
}

void initImu(){
    
    RTIMUSettings *settings = new RTIMUSettings("/media/pi/PiDrive1/Holistic2/master/src/imu/","RTIMULib");

    imu = RTIMU::createIMU(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    //  This is an opportunity to manually override any settings before the call IMUInit

    //  set up IMU

    imu->IMUInit();
    runningIMU = true;

    //  this is a convenient place to change fusion parameters
    //  The slerp power valule controls the influence of the measured state to correct the predicted state
    //  0 = measured state ignored (just gyros), 1 = measured state overrides predicted state.
    //  In between 0 and 1 mixes the two conditions default 0.02
    imu->setSlerpPower(0);
    imu->setGyroEnable(true);
    imu->setAccelEnable(false);
    imu->setCompassEnable(true);

    //  set up for rate timer

    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
    
    usleep(imu->IMUGetPollInterval() * 1000);
    setAngleToZero();
}

void testPoints(vector<Point> &points) {
    //small turns
    // points.push_back(Point(70, 66));
    // points.push_back(Point(112, 63));
    // points.push_back(Point(161, 61));
    // points.push_back(Point(207, 53));
    // points.push_back(Point(244, 51));
    // points.push_back(Point(301, 59));
    // points.push_back(Point(344, 64));
    // points.push_back(Point(388, 67));
    // points.push_back(Point(440, 71));
    // points.push_back(Point(492, 78));
	
	//points.push_back(Point(28, 66));
	points.push_back(Point(68, 66));
	points.push_back(Point(110, 67));
	points.push_back(Point(112, 114));
	points.push_back(Point(114, 158));
	// points.push_back(Point(114, 201));
	// points.push_back(Point(114, 246));
	// points.push_back(Point(116, 290));
	// points.push_back(Point(155, 291));
	// points.push_back(Point(204, 288));
	// points.push_back(Point(251, 291));
	// points.push_back(Point(296, 291));
	// points.push_back(Point(342, 292));
	// points.push_back(Point(380, 294));
	// points.push_back(Point(430, 294));
	// points.push_back(Point(427, 249));
	// points.push_back(Point(430, 202));
	// points.push_back(Point(424, 158));
	// points.push_back(Point(424, 110));
	// points.push_back(Point(422, 73));
	// points.push_back(Point(388, 60));
	// points.push_back(Point(380, 25));
	// points.push_back(Point(335, 24));
	// points.push_back(Point(288, 24));
	// points.push_back(Point(245, 27));
	// points.push_back(Point(204, 24));


}


void complexPointsToPoinst(vector<Point> &points){

    PathTracking p("NOD1", "NOD121");
    std::vector<std::complex<double>> pointsVector;
    p.nextNodes(15, pointsVector);
    for (int i = 0; i < pointsVector.size(); i++)
    {
        std::complex<double> point = pointsVector[i];
        
        points.push_back(Point(point.real()*100, (double)516.5 - (double)(point.imag()) *100));
    }
    
}
void drive(Point start,vector<Point> &dstPoints,Mat &dst) {
	Point curent = start;
	//float xAnt = start.x, yAnt = start.y;
	float du = 10;
	float curentAngle = curent_yaw_rad;
	while (dstPoints.size() > 1)
	{       
        pthread_mutex_lock(&lock_XY);
        curent = Point(x,y);
        pthread_mutex_unlock(&lock_XY); 

		printf("remaining points : %d\n",dstPoints.size());
        printf("curent:Point(%d,%d), dstPoint(0):Point(%d,%d), dstPoints.at(0 + 1):Point(%d,%d)\n",curent.x,curent.y,dstPoints.at(0).x,dstPoints.at(0).y, dstPoints.at(0 + 1).x,dstPoints.at(0 + 1).y);
		//bool pointReached = pointInsideCircle(curent, dstPoints.at(0));
		//while (!pointReached) {
			float distanceToPoint, angleToPoint,innerAngle;
			distanceAnglePointToVector(curent, dstPoints.at(0), dstPoints.at(1), distanceToPoint, angleToPoint, innerAngle);
            printf("distanceToPoint:%f , angleToPoint:%f , innerAngle:%f ;\n",distanceToPoint, angleToPoint,innerAngle);
			if (innerAngle > 23) {
				//curentAngle += copysignf(23,angleToPoint);
				//bezierPoints(curent, dstPoints.at( 1), dstPoints.at(0 ), dst);
				//i = i + 1;
				//pointReached = true;
                printf("bezier\n");
				if(angleToPoint < 0 ){
                    c->moveBezier(complex<double>(curent.x/100,curent.y/100),complex<double>(dstPoints.at(0).x/100,dstPoints.at(0).y/100),complex<double>(dstPoints.at(0).x/100,-dstPoints.at(0).y/100),complex<double>(dstPoints.at(1).x/100,-dstPoints.at(1).y/100),4.0,true);
                    sleep(2);
                c->brake(0.0);
                }else{
                c->moveBezier(complex<double>(curent.x/100,curent.y/100),complex<double>(dstPoints.at(0).x/100,dstPoints.at(0).y/100),complex<double>(dstPoints.at(0).x/100,dstPoints.at(0).y/100),complex<double>(dstPoints.at(1).x/100,dstPoints.at(1).y/100),4.0,true);
                sleep(2);
                c->brake(0.0);
                }
                //dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
				// curent = Point(x,y);
                // pointReached = true;
				// curentAngle = curent_yaw_rad;
			}
			else {
				printf("move:0.5  angle: %f  sleep: %f\n",angleToPoint,distanceToPoint*(1/0.5)/100);
                //float newAngle = curentAngle * PI/180 + 
                c->move(0.5,angleToPoint);
                sleep(distanceToPoint*(1/0.5)/100);
                c->brake(0.0);
                //dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
                // curentAngle = curent_yaw_rad;
                // curent = Point(x,y);
                // pointReached = true;
				// curent.x = curent.x + (du * (float)cos(curentAngle*PI / 180.0));
				// curent.y = curent.y + (du * (float)sin(curentAngle*PI / 180.0));
				// xAnt = curent.x;
				// yAnt = curent.y;
				// curentAngle += angleToPoint;
			}
            // pthread_mutex_lock(&lock_encoder);
            //             curentAngle = curent_yaw_rad ;
            // pthread_mutex_unlock(&lock_encoder); 
            //curent = Point(x,y);
            //printf("x: %f , y:  %f",x,y);
            //pointReached = true;
			//pointReached = pointInsideCircle(curent, dstPoints.at(0));
			//circle(dst, curent, 1, Scalar(255, 0, 255));

		//}
		//if (pointReached) {
		dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
		//}
	}
}

void driveNoBezier(Point start,vector<Point> &dstPoints,Mat &dst) {
	// Point curent = Point(stableX , stableY);
    // float curentX = 0.0, curentY = 63.0;
	// //float xAnt = start.x, yAnt = start.y;
	
	// float curentAngle = curent_yaw_rad;
	// while (dstPoints.size() > 1)
	// {       
	// 	printf("remaining points : %d\n",dstPoints.size());
    //     printf("curent:Point(%d,%d), dstPoint(0):Point(%d,%d), dstPoints.at(0 + 1):Point(%d,%d)\n",curent.x,curent.y,dstPoints.at(0).x,dstPoints.at(0).y, dstPoints.at(0 + 1).x,dstPoints.at(0 + 1).y);
	// 	bool pointReached = pointInsideCircle(curent, dstPoints.at(0));
    //     //bool pointReached = false;
	// 	while (!pointReached) {
    //         pthread_mutex_lock(&lock_XY);
    //             curentX = x, curentY = y;
    //         pthread_mutex_unlock(&lock_XY); 
    //         printf("Local x:%f y:%f\n",stableX , stableY);
    //         curent = Point(stableX , stableY);
	// 		float distanceToPoint, angleToPoint, innerAngle;
	// 		distanceAnglePointToVector(curent, dstPoints.at(0), dstPoints.at(1), distanceToPoint, angleToPoint, innerAngle);
    //         printf("Curent: x:%d y:%d distanceToPoint:%f , angleToPoint:%f , innerAngle:%f ;\n",curent.x,curent.y,distanceToPoint, angleToPoint, innerAngle);
	// 		if (innerAngle > 23.0) {
    //             //c->brake(0.0);
    //             //sleep(5);
    //             //float newAngle = curentAngle * PI/180 + 
    //             vector<Point>beziers;
	// 			bezierPoints(curent, points.at(1), points.at(0), beziers);
                
    //             for(int k = 0; k < beziers.size() ;k++){
    //                 int k1 = k + 1;
    //                 if(k1 >= beziers.size() ){
    //                     k1 = k;
    //                 }
    //                 distanceAnglePointToVector(curent, beziers.at(k), beziers.at(k1), distanceToPoint, angleToPoint, innerAngle);
    //                 printf("bezier:0.5  angle: %f  sleep: %f\n",angleToPoint,(distanceToPoint*(1/0.5)*10.0));
    //                 if (innerAngle > 23.0) {
    //                     if(angleToPoint < 0){
    //                         c->move(0.5,23.0);
    //                     }else{
    //                         c->move(0.5, -23.0);
    //                     }
    //                     //sleep((distanceToPoint*(1/0.5)));
                        
    //                 }else {
    //                     printf("K:%d bezier:0.5  angle: %f  sleep: %f\n",k,angleToPoint,(distanceToPoint*(1/0.5)*10.0));
                        
    //                     c->move(0.5,angleToPoint);
                            
	// 		        }
    //             }
    //             nsleep(distanceToPoint*(1/0.5)*10.0);
    //             c->brake(angleToPoint);   
    //             if(pointInsideCircle(curent, dstPoints.at(1))){
    //                 dstPoints.erase(dstPoints.begin() + 1, dstPoints.begin() + 1);
    //             }

	// 		}
	// 		else {
	// 			printf("move:0.5  angle: %f  sleep: %f\n",angleToPoint,distanceToPoint*(1/0.5)*10.0);
    //             //float newAngle = curentAngle * PI/180 + 
    //             c->move(0.5,angleToPoint);
    //             //float dist = distanceWithAngle(curent, dstPoints.at(0),angleToPoint,angleToPoint);
    //             nsleep(distanceToPoint*(1/0.5)*10.0);
    //             //sleep(distanceToPoint*(1/0.5)/10.0);
    //             c->brake(angleToPoint);
    //             //sleep(3);
    //             //dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
    //             // curentAngle = curent_yaw_rad;
    //             // curent = Point(x,y);
    //             // pointReached = true;
	// 			// curent.x = curent.x + (du * (float)cos(curentAngle*PI / 180.0));
	// 			// curent.y = curent.y + (du * (float)sin(curentAngle*PI / 180.0));
	// 			// xAnt = curent.x;
	// 			// yAnt = curent.y;
	// 			// curentAngle += angleToPoint;
	// 		}
    //         // pthread_mutex_lock(&lock_encoder);
    //         //             curentAngle = curent_yaw_rad ;
    //         // pthread_mutex_unlock(&lock_encoder); 
    //         //curent = Point(x,y);
    //         //printf("x: %f , y:  %f",x,y);
    //         //pointReached = true;
	// 		//pointReached = pointInsideCircle(curent, dstPoints.at(0));
    //          if(distanceToPoint < 10){
    //              pointReached = true;
    //          }
	// 		//circle(dst, curent, 1, Scalar(255, 0, 255));

	// 	}
	// 	//if (pointReached) {
	// 	dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
	// 	//}
	// }
}


void drive2(vector<Point> &dstPoints){

	//Point curent = Point(stableX , stableY);
    float curentX = 0.0, curentY = 63.0;
	//float xAnt = start.x, yAnt = start.y;
	
	float curentAngle = curent_yaw_rad;
	while (dstPoints.size() > 1)
	{       
		printf("remaining points : %d\n",dstPoints.size());
        printf("curent: x:%f, y:%f), dstPoint(0):Point( %d, %d), dstPoints.at(1):Point( %d, %d)\n",curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, dstPoints.at(1).x,dstPoints.at( 1).y);
		//bool pointReached = pointInsideCircle(curent, dstPoints.at(0));
        bool pointReached = false;
		while (!pointReached) {
            pthread_mutex_lock(&lock_XY);
                curentX = x, curentY = y;
            pthread_mutex_unlock(&lock_XY); 
			float distanceToPoint, angleToPoint, innerAngle; //todo distance
			//distanceAnglePointToVector(curent, dstPoints.at(0), dstPoints.at(1), distanceToPoint, angleToPoint, innerAngle);
            angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, curent_yaw_rad, angleToPoint,distanceToPoint);
            printf("Local: x:%f y:%f distanceToPoint:%f , angleToPoint:%f , innerAngle:%f ;\n",curentX , curentY,distanceToPoint, angleToPoint, innerAngle);

			while(distanceToPoint > 10.0){
                while (abs(angleToPoint) > 23.0 && distanceToPoint > 10.0) {
                    if(abs(angleToPoint) < 0.2){
                      angleToPoint = 0.0;  
                    }

                    float angleTurn = fmod(angleToPoint,23.0);
                    printf("moveMaxTurn:0.5  angle: %f distance:%f  sleep:\n",angleTurn,distanceToPoint);
                    c->move(0.5,angleTurn);
                    int ret = nsleep(200);
                    printf("sleep result %d\n",ret);
                    pthread_mutex_lock(&lock_XY);
                        curentX = x, curentY = y;
                    pthread_mutex_unlock(&lock_XY);  
                    angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, curent_yaw_rad, angleToPoint,distanceToPoint);

                    
                }
                if(abs(angleToPoint) < 0.2){
                      angleToPoint = 0.0;  
                }
                printf("moveLowTurn:0.5  angle: %f distance:%f  sleep: %f\n",angleToPoint,distanceToPoint,distanceToPoint*(1/0.5)*10.0);
                c->move(0.5,angleToPoint);
                int ret = nsleep(distanceToPoint*(1/0.5)*10.0 );
                printf("sleep result %d\n",ret);
                pthread_mutex_lock(&lock_StableXY);
                    curentX = stableX, curentY = stableY;
                pthread_mutex_unlock(&lock_StableXY); 
                angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, curent_yaw_rad, angleToPoint,distanceToPoint);
            }
             if(distanceToPoint <= 10.0){
                 pointReached = true;
                 dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
             }
             
             
        }
		
	}

}
void drive3(vector<Point> &dstPoints){

	//Point curent = Point(stableX , stableY);
    float curentX = 0.0, curentY = 63.0;
	//float xAnt = start.x, yAnt = start.y;
	
	float curentAngle = curent_yaw_rad;
	while (dstPoints.size() > 4)
	{       
		printf("remaining points : %d\n",dstPoints.size());
        printf("curent: x:%f, y:%f), dstPoint(0):Point( %d, %d), dstPoints.at(1):Point( %d, %d)\n",curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, dstPoints.at(1).x,dstPoints.at( 1).y);
		//bool pointReached = pointInsideCircle(curent, dstPoints.at(0));
        bool pointReached = false;
		while (!pointReached) {
            pthread_mutex_lock(&lock_XY);
                curentX = x, curentY = y;
            pthread_mutex_unlock(&lock_XY); 
			float distanceToPoint, angleToPoint; //todo distance
            angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, curent_yaw_rad, angleToPoint,distanceToPoint);
            float distanceToPointP0, angleToPointP1, innerAngleP1;
            distanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, angleToPointP1, innerAngleP1);
            printf("Local: x:%f y:%f distanceToPointP0:%f , angleToPoint:%f,angleToPointP1: %f innerAngleP1:%f ;\n",curentX ,curentY, distanceToPoint, angleToPoint, angleToPointP1, innerAngleP1);
			while(distanceToPoint > 10.0){
                if (abs(angleToPointP1) > 23.0) {
                    
                    if(angleToPoint < 0 ){
                        printf("Bezier stanga\n");
                        c->moveBezier(complex<double>(curentX/100,curentY/100),complex<double>(dstPoints.at(0).x/100,dstPoints.at(0).y/100),complex<double>(dstPoints.at(0).x/100,dstPoints.at(0).y/100),complex<double>(dstPoints.at(1).x/100,dstPoints.at(1).y/100),4.0,true);                        
                        int ret = nsleep(3000);
                        printf("sleep result %d\n",ret);
                        c->brake(0.0);
                    }else{
                        printf("Bezier dreapta\n");
                        //c->moveBezier(complex<double>(0.67,0.65),complex<double>(1.31,0.50),complex<double>(1.31,-0.50),complex<double>(1.12,-1.14),4.0,true);
                        c->moveBezier(complex<double>(curentX/100,curentY/100),complex<double>(dstPoints.at(0).x/100,dstPoints.at(0).y/100),complex<double>(dstPoints.at(0).x/100,-dstPoints.at(0).y/100),complex<double>(dstPoints.at(1).x/100,-dstPoints.at(1).y/100),4.0,true);                        
                        int ret = nsleep(3000);
                        printf("sleep result %d\n",ret);
                        c->brake(0.0);
                    }
                    
                    pthread_mutex_lock(&lock_XY);
                        curentX = x, curentY = y;
                    pthread_mutex_unlock(&lock_XY);  
                    float distP0, distP1;
                    angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, curent_yaw_rad, angleToPoint,distP0);
                    angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(1).x,dstPoints.at(1).y, curent_yaw_rad, angleToPoint,distP1);
                    if(distP1 < 20.0){
                        dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
                    }
                    if(distP1 <= 10.0){
                        pointReached = true;
                        dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
                    }
                    
                }else{
                    if(abs(angleToPoint) < 0.2){
                        angleToPoint = 0.0;  
                    }
                    printf("moveLowTurn:0.5  angle: %f distance:%f  sleep: %f\n",angleToPoint,distanceToPoint,distanceToPoint*(1/0.5)*10.0);
                    c->move(0.5,angleToPoint);
                    int ret = nsleep(distanceToPoint*(1/0.5)*10.0);
                    printf("sleep result %d\n",ret);
                    pthread_mutex_lock(&lock_StableXY);
                        curentX = stableX, curentY = stableY;
                    pthread_mutex_unlock(&lock_StableXY); 
                    angleAndDirectionOfPoints2(curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, curent_yaw_rad, angleToPoint,distanceToPoint);
                    if(distanceToPoint <= 10.0){
                        pointReached = true;
                        dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
                    }
                }
            }

            
        }
		
	}

}
void driveBezier(vector<Point> &dstPoints){

    //Point curent = Point(stableX , stableY);
    float curentX = 0.0, curentY = 539.0 - 63.0;
	//float xAnt = start.x, yAnt = start.y;
	
	float curentAngle = curent_yaw_rad;
	while (dstPoints.size() > 1)
	{       
		//printf("remaining points : %d\n",dstPoints.size());
		bool pointReached = pointInsideCircle(Point( curentX , curentY), dstPoints.at(0));
        //bool pointReached = false;
		while (!pointReached) {
            pthread_mutex_lock(&lock_XY);
                curentX = x, curentY = y;
            pthread_mutex_unlock(&lock_XY); 
            //curentY = 539.0 - curentY;
            printf("curent: x:%f, y:%f), dstPoint(0):Point( %d, %d), dstPoints.at(1):Point( %d, %d)\n",curentX,curentY,dstPoints.at(0).x,dstPoints.at(0).y, dstPoints.at(1).x,dstPoints.at( 1).y);
			//while(distanceToPoint > 10.0){
                //c->moveBezier(complex<double>(0.68,4.73),complex<double>(0.95,4.72),complex<double>(1.18,4.57),complex<double>(1.12,4.25),3.0,true);
            float distanceToPointP0, angleToPointP1, innerAngleP1;
            distanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, angleToPointP1, innerAngleP1);
            Point pcurent = Point(curentX, curentY);
            //printf("Local: x:%d y:%d distanceToPointP0:%f , angleToPointP1: %f innerAngleP1:%f ;\n",pcurent.x ,pcurent.y, distanceToPointP0, angleToPointP1, angleToPointP1, innerAngleP1);
            
            
            if (innerAngleP1 > 23) {
                printf("largeAngle\n");
                //Point control, control2;
                float control1X,control1Y,control2X,control2Y;
                vector<Point2f>beziers;
                bezierPoints(pcurent, dstPoints.at(1), dstPoints.at(0), beziers,mapTrack,false);			
                bezierControlPointsQuadToCubic(pcurent,dstPoints.at(0), dstPoints.at(1), control1X,control1Y,control2X,control2Y);
                printf("c->moveBezier(complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),3.0,true);\n", curentX / 100.0,(539.0 - curentY)/100.0,control1X /100.0,(539.0 - control1Y)/100.0, control2X / 100.0, (539.0 - control2Y) / 100.0,dstPoints.at(1).x/100.0,(539.0 - dstPoints.at(1).y)/100.0);
                printf("c->moveBezier(complex<double>(0.68,4.73),complex<double>(0.96,4.73),complex<double>(1.10,4.57),complex<double>(1.12,4.25),3.0,true); \n");
                //std::cout << "bez comand" << curentX/100.0 << (539.0 - curentY)/100.0 << control.x/100.0 << (539.0 - control.y)/100.0 << control2.x/100.0 << (539.0-control2.y)/100.0 << dstPoints.at(1).x/100.0 << (539.0 - dstPoints.at(1).y)/100.0 << std::endl;
                c->moveBezier(complex<double>(curentX /100.0,(539.0 - curentY)/100.0),complex<double>( control1X /100.0 ,( 539.0 - control1Y )/100.0),complex<double>(control2X / 100.0,(539.0 - control2Y)/100.0 ),complex<double>(dstPoints.at(1).x/100.0,(539.0 - dstPoints.at(1).y)/100.0),3.0,true);                        
                int ret = nsleep(3000);
                //printf("sleep result %d\n",ret);
                //brake at angle ? might help
                dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 2);
                }
                else{
                    printf("smallAngle\n");
                    float control1X,control1Y,control2X,control2Y;
                    vector<Point2f>beziers;
                    bezierPoints(pcurent, dstPoints.at(0), pcurent, beziers,mapTrack,false);
                    bezierControlPointsQuadToCubic(pcurent,pcurent, dstPoints.at(0), control1X,control1Y,control2X,control2Y);
                    //calculateBezierControlPoints(pcurent, dstPoints.at(0), beziers, control, control2,mapTrack);
                    //printf("c->moveBezier(complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),3.0,true);\n", points.at(i).x / 100.0, (539.0 - points.at(i).y) / 100.0, control.x / 100.0, (539.0 - control.y) / 100.0, control2.x / 100.0, (539.0 - control2.y) / 100.0, points.at(i + 2).x / 100.0, (539.0 - points.at(i + 2).y) / 100.0);
                    printf("c->moveBezier(complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),complex<double>(%.2f,%.2f),3.0,true);\n", curentX/100.0,(539.0 - curentY)/100.0, control1X /100.0 ,( 539.0 - control1Y )/100.0, control2X / 100.0,(539.0 - control2Y)/100.0 ,dstPoints.at(1).x/100.0,(539.0 - dstPoints.at(1).y)/100.0);
                    c->moveBezier(complex<double>(curentX /100.0,(539.0 - curentY)/100.0),complex<double>( control1X /100.0 ,( 539.0 - control1Y )/100.0),complex<double>(control2X / 100.0,(539.0 - control2Y)/100.0 ),complex<double>(dstPoints.at(1).x/100.0,(539.0 - dstPoints.at(1).y)/100.0),3.0,true);                                      
                    int ret = nsleep(3000);
                    printf("sleep result %d\n",ret);
                    //brake at angle ? might help
                    dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
                    }
                    
            distanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, angleToPointP1, innerAngleP1);
            pointReached = pointInsideCircle(Point( curentX , curentY), dstPoints.at(0));
            // if(distanceToPointP0 < 10){
            //     dstPoints.erase(dstPoints.begin(), dstPoints.begin() + 1);
            }
            
        }
		
	//}
}

void drive4(){
        Point dstPoint = Point(112, 114);
        //points.push_back(Point(112, 114));
    	//bool pointReached = pointInsideCircle(Point( curentX , curentY),dstPoint);
        float curentX = 0.0, curentY = 539.0 - 63.0;
        bool pointReached = false;
		while (!pointReached) {
            pthread_mutex_lock(&lock_XY);
                curentX = x, curentY = y;
            pthread_mutex_unlock(&lock_XY); 
            Point pcurent = Point(curentX, curentY);

            float distanceToPointP0, angleToPointP1, innerAngleP1;
            //distanceAnglePointToVector(curentX,curentY, dstPoint, dstPoint, distanceToPointP0, angleToPointP1, innerAngleP1);
            angleAndDirectionOfPoints2(curentX,curentY,dstPoint.x,dstPoint.y, (curent_yaw_rad*180/PI), angleToPointP1,distanceToPointP0);
            printf("Local: x:%d y:%d distanceToPointP0:%f , angleToPointP1: %f innerAngleP1:%f ;\n",pcurent.x ,pcurent.y, distanceToPointP0, angleToPointP1, angleToPointP1, innerAngleP1);
            if (abs(angleToPointP1) > 23) {
                c->move(0.2,23.0);
                }
                else{
                    c->move(0.2,angleToPointP1);
                    }
            nsleep(50);
            pointReached = pointInsideCircle(pcurent,dstPoint);
        }

}
void drive5(vector<Point> &dstPoints){

  //Point curent = Point(stableX , stableY);
    float curentX = 0.0, curentY = 539.0 - 63.0;
	//float xAnt = start.x, yAnt = start.y;
	
    PathTracking p("NOD1", "NOD121");

	float curentAngle = curent_yaw_rad;
	while (dstPoints.size() > 4)
	{       
        // float distanceToPointP0, angleToPointP1, innerAngleP1;
        // distanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, angleToPointP1, innerAngleP1);
        // if(innerAngleP1 > 23.0){
        //     printf("erasing large angle Point px:%d py:%d \n",  dstPoints.at(1).x,  dstPoints.at(1).y);
        //     dstPoints.erase(dstPoints.begin() + 1);
        // }
        Point dstPoint = dstPoints.at(0);
        //printf("Local: x:%d y:%d \n",x ,y);
		printf("dstPoint x:%d y:%d \n",dstPoint.x,dstPoint.y);
		bool pointReached = pointInsideCircle(Point( curentX , curentY), dstPoints.at(0));
        float previousEstimatedDistance = 0.0;
        float distanceDriven = 0.0;
        bool noPointEliminated = true;
        //bool pointReached = false;
        while (!pointReached) {
            pthread_mutex_lock(&lock_XY);
                curentX = x, curentY = y;
            pthread_mutex_unlock(&lock_XY); 
            Point pcurent = Point(curentX, curentY);
            p.update((double)curentX, (double)curentY);
            float distanceToPointP0,distanceToPointP1, angleToPointP1, innerAngleP1;
            distanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, distanceToPointP1 , angleToPointP1, innerAngleP1);
            distanceDriven += (previousEstimatedDistance - distanceToPointP0);
            if(innerAngleP1 > 30.0 && ( (distanceDriven >= distanceToPointP1) || noPointEliminated)){
                printf("estim dist: %f driven dist %f \n",previousEstimatedDistance,distanceDriven );
                distanceDriven = 0.0;
                previousEstimatedDistance = distanceToPointP1;
                dstPoints.erase(dstPoints.begin());
                dstPoint = dstPoints.at(0);
                noPointEliminated = false;
                
                printf("Point eliminated next dstPoint x:%d y:%d \n",dstPoint.x,dstPoint.y);
            }
            
            float distanceToPoint, angleToPoint;
            //distanceAnglePointToVector(curentX,curentY, dstPoint, dstPoint, distanceToPointP0, angleToPointP1, innerAngleP1);
            angleAndDirectionOfPoints2(curentX,curentY,dstPoint.x,dstPoint.y,  (curent_yaw_rad*180/PI), angleToPoint,distanceToPoint);
            //istanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, angleToPointP1, innerAngleP1);
            printf("Local: x:%d y:%d distanceToPointDst:%f , angleToPointDst: %f;\n",pcurent.x ,pcurent.y, distanceToPoint, angleToPoint);
            if (angleToPoint > 16) {
                c->move(0.2,23.0);
                printf(" c->move(0.2,%.2f);\n",23.0);
                }
            else if (angleToPoint < -16  ) {
                c->move(0.2,-23.0);
                printf(" c->move(0.2,%.2f);\n",-23.0);
                }
            else{
                c->move(0.2,angleToPoint);
                printf(" c->move(0.2,%.2f);\n",angleToPoint);
                }
            nsleep(100);
            pointReached = pointInsideCircle(pcurent,dstPoint);
        }
        dstPoints.erase(dstPoints.begin());    
    }

}
int main(int argc, char **argv)
{
    //cout << "before signal" << endl;
    vector<Point> dstPoints;
    // testPoints(dstPoints);
    complexPointsToPoinst(dstPoints);

    initDisplay();


    sleep(1);

    signal (SIGINT, interruptHandler);

    //cout << "after signal" << endl;

    c = new CarControl();

    initImu();




    if (pthread_mutex_init(&lock_encoder, NULL) != 0) 
        { 
            printf("\n mutex init has failed\n"); 
            return 1; 
        } 
    if (pthread_mutex_init(&lock_yaw, NULL) != 0) 
    { 
    printf("\n mutex init has failed\n"); 
    return 1; 
    } 
    if (pthread_mutex_init(&lock_XY, NULL) != 0) 
    { 
    printf("\n mutex init has failed\n"); 
    return 1; 
    } 
    

    //cout << "after ne car control" << endl;

    // c->move(20.0, 0.0);

    sleep(1);

       pthread_t threads[NUM_THREADS];
   //struct thread_data td[NUM_THREADS];
   int rc;
   int i = 1;
   int thread_stat;
   
//    td[i].thread_id = i;
//    td[i].message = "This is message";
   rc = pthread_create(&threads[i], NULL, readIMU, (void *)i);

    c->brake(0.0);

    //sleep(1);

    c->enablePID();

    c->enableEncoder(print);

    sleep(1);

    drive5(dstPoints);

    c->brake(0.0);
    c->brake(0.0);

    sleep(1);

    c->brake(0.0);

    c->disableEncoder();

    c->disablePID();

    c->close();
    

    cout << totalRots << endl;

//	imshow("mapFin",mapTrack);
    //pthread_exit(NULL);

//	waitKey(0);
    
    drawPointsOnMap();

    sleep(1);

	imwrite("/media/pi/PiDrive1/Holistic2/master/resources/mapat2.jpg",mapTrack);


    sleep(1);
    printf("X: %f Y: %f \n",x,y);
    printf("end angle:%f\n", curent_yaw_rad);
    // int status = pthread_kill( rc, SIGUSR1);                                     
    // if ( status <  0)                                                              
    //     perror("pthread_kill failed\n");
    runningIMU = false;                                                                                            
    int status = pthread_join( rc, (void *)&thread_stat);                        
    if ( status <  0)                                                              
        perror("pthread_join failed\n");  
    printf("Thread join\n");  

    pthread_mutex_destroy(&lock_encoder); 
    printf("Lock destroyed\n");

    return 0;
}


#endif