
#pragma once

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace cv;
using namespace std;

class Vehicles
{
        private:
                Rect r;
                Mat im;
                double v;
                double ts;                 //TimeStamp: Last update time

        public:
                Vehicles(Rect re, Mat i, double t) : r(re), im(i(re)), v(0), ts(t) {};
                Vehicles& operator=(const Vehicles& c);

                int area();
                Point2f pos();
                double velocity();

                void plot(Mat& f, int);

                bool onScene(Mat& f, double time);
                bool match(Vehicles& c);     //Assume function is being called from the 'newest' car.

};

