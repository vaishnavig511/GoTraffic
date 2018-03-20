#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <functional>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "Vehicles.hpp"

#define contour vector<Point>

using namespace std;
using namespace cv;

class TrafficFlow
{
        private:
                VideoCapture vc;
                Mat ROITransform;
                Size ROISize;

                void orderPointClockwise(vector<Point2f>& pts);
                Mat getTransfMatrix(vector<Point2f>& src1, Size s);
                Mat getROI(Mat f);

                list<Vehicles> vehicles;
                list<Vehicles> fgDetected;
                void getVehiclesFG(Mat fg, Mat ROI, double time); //Fills fgDetected with the cars detected for a new instant of time
                void updateVehicles(Mat ROI, double time);        //Update cars by matching the newly detected with previous ones

        public:
                TrafficFlow(){};
                void generateConfig(string cFile);
                void loadConfig(string cFile);
                void playvideo();
};


