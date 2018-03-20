

#include "TrafficFlow.hpp"

//ofstream outFile("out.txt");
//Helper function to genConfig
void CallBack(int event, int x, int y, int flags, void* param)
{
        static int pointID = -1;
        vector<Point2f>& v = *(vector<Point2f>*) param;

        //On Double click, select point to move
        if  (event == EVENT_LBUTTONDBLCLK)
        {

                Point2f t(x, y);

                //Find closest point to t
                int c=0;
                int d=norm(t-v[0]);
                for (int i=1; i< v.size(); i++)
                {
                        int dt = norm(t-v[i]);
                        if (dt < d)
                        {
                                c = i;
                                d = dt;
                        }
                }

                pointID = c;
        }

        //If a point was selected move the point with the movement of the mouse
        if  (event == EVENT_MOUSEMOVE && pointID != -1)
        {
                v[pointID].x = x;
                v[pointID].y = y;
        }

        //On click with left, point is fixed
        if (event == EVENT_LBUTTONDOWN)
                pointID= -1;
}

void TrafficFlow::generateConfig(string cFile)
{
        //system("Color F");
        cout << "\n\n\t\t\tTrafficFlow Configuration Process" << endl;
        //system("Color D");
        cout << "Type the stream url or the video file path.\n" << endl;
        string url;
        fflush(stdin);
        getline(cin, url);

        int w,h;
       // system("Color H");
        cout << "Type the size of the Region of Interest. width, height.\n" << endl;
        cin >> w >> h;
        ROISize.width = w;
        ROISize.height = h;
         //open stream

        system("Color 3");
        cout << "An image of the video will appear,\n please drag the points to adjust the Region of Interest (ROI). When done, press Q to quit. To begin, press ENTER.\n" << endl;
        cin.get();

        vc.open(url);
        //Open Stream and get 10th frame
        //cout<<"Sucess";
        Mat frame;
        for (int i=0; i<10; i++)
                vc >> frame;

        //Create points in ROI
        vector<Point2f> roi(4, Point2f(frame.size()));
        roi[0] *= 1./4;
        roi[1] *= 1./4;
        roi[2] *= 3./4;
        roi[3] *= 3./4;
        roi[1].x = roi[2].x;
        roi[2].x = roi[0].x;
        orderPointClockwise(roi);

        //Create window and set mouse event
        namedWindow("Video", 1);
        setMouseCallback("Video", CallBack, &roi);

        //Let user customize points
        Mat dr, preview;
        char c = 'c';
        while(c != 13)
        {
                frame.copyTo(dr);
                ROITransform = getTransfMatrix(roi, ROISize);

                //Show preview of ROI
                preview = getROI(dr);

                //Draw already selected points
                for (int i=0; i<roi.size(); i++)
                {
                        circle(dr, roi[i], 3, Scalar(255,0,0), -1);

                        if (i+1 < roi.size())
                                line(dr, roi[i], roi[i+1], Scalar(0,255,0));
                        else
                                line(dr, roi[i], roi[0], Scalar(0,255,0));
                }

                imshow("Video", dr);
                imshow("Preview ROI", preview);
                //cout<<c<<" ";
                c = waitKey(10);
                if(c=='q')
                    exit(0);
        }
       // cout<<"succes2\n";

        //Save config
        FileStorage fs(cFile, FileStorage::WRITE);
        fs << "url" << url;
        fs << "ROISize" << ROISize;
        fs << "ROITransform" << ROITransform;
        fs.release();
       cvDestroyWindow("Video");
       cvDestroyWindow("Preview ROI");

       // dr.CloseWindow();
        //preview.CloseWindow();

        //Finish
        system("Color 5");
        cout << "Configuration file created successfully." << endl;
        loadConfig(cFile);

        freopen("C:\\Users\\dellpc\\Desktop\\trafficline\\out.txt","w",stdout);
        playvideo();

        //exit(0);
}

void TrafficFlow::loadConfig(string cFile)
{
        //Check for configuration file
        ifstream confFile(cFile.c_str());
        if (!confFile.good())
        {
                cout << "Configuration File not found. Configure the system before using." << endl;
                return;
        }

        //Load configuration file and parameters
        FileStorage fs(cFile, FileStorage::READ);
        string url;
        fs["url"] >> url;
        fs["ROISize"] >> ROISize;
        fs["ROITransform"] >> ROITransform;
        fs.release();
        vc.open(url);
}

//Given an array of points, sort them clockwise
void TrafficFlow::orderPointClockwise(vector<Point2f>& pts)
{
        int n = pts.size();

        //Get centroid
        Point2f mean(0,0);
        for (int i=0; i<n; i++)
                mean += pts[i];
        mean *= double(1)/n;

        //Compare angles of points
        const auto comp = [&] (const Point2f a,const Point2f b)
        {
                Point2f ao = a - mean;
                Point2f bo = b - mean;
                double angA = atan2(ao.y, ao.x);
                double angB = atan2(bo.y, bo.x);

                return angA < angB;
        };

        sort(pts.begin(), pts.end(),comp);
}

//Given a frame, a set of src points and a frame size, gives the perspective transformation Matrix
Mat TrafficFlow::getTransfMatrix(vector<Point2f>& src1, Size s)
{
        //Generate copy of src vector to avoid loosing references (Wcallback)
        vector<Point2f> src(src1);

        //Generate set of dst points
        vector<Point2f> dst(4, Point2f(s));
        dst[0].x=0; dst[0].y=0;
        dst[1].x=0;
        dst[2].y=0;

        //Order set of points
        orderPointClockwise(src);
        orderPointClockwise(dst);

        //Get Perspective Matrix
        Mat transform= getPerspectiveTransform(src, dst);
        return transform;
}

Mat TrafficFlow::getROI(Mat f)
{
        //Perform transformation
        Mat r;
        warpPerspective(f, r, ROITransform, Size(ROISize.width,ROISize.height));
        return r;
}

void TrafficFlow::getVehiclesFG(Mat fg, Mat ROI, double time)
{
        //Convert matrix to 8U with 1C
        Mat fg2;
        fg.convertTo(fg2, CV_8UC1);

        //Get contours
        vector<contour> contours;
        findContours(fg2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);


        //Remove contours with area less than 1% of ROI area (false positives)

        auto filter = [&](contour r)
        {
            return contourArea(r)<0.01*ROISize.area();
        };
        vector<contour>::iterator end = remove_if(contours.begin(), contours.end(),filter);
        contours.erase(end, contours.end());

        //Get corresponding rectangles and create Cars object for each one
        for (contour& c : contours)
        {
                Rect r = boundingRect(c);
                fgDetected.push_back(Vehicles(r, ROI, time));
                fgDetected.back().plot(fg2, 0);
        }

}

void TrafficFlow::updateVehicles(Mat ROI, double time)
{
        bool DEBUG = false;

        //For each car in the FG, try to match to a previous, existing car. If it matches, swap cars and delete the oldest one.
        list<Vehicles>::iterator itFG, itC;
        itFG = fgDetected.begin();
        while (itFG != fgDetected.end())
        {
                bool foundMatch = false;
                for (itC = vehicles.begin(); itC != vehicles.end(); itC++)
                {

                        //DEBUG
                        if (DEBUG)
                        {
                        Mat t;
                        ROI.copyTo(t);
                        itC->plot(t,0);
                        itFG->plot(t,1);
                        imshow("comparing", t);

                        ROI.copyTo(t);
                        for (Vehicles& c : vehicles)
                                c.plot(t,0);
                        imshow("Moving cars", t);

                        ROI.copyTo(t);
                        for (Vehicles& c : fgDetected)
                                c.plot(t,1);
                        imshow("FG detected cars", t);

                        waitKey();
                        }

                        //If there's a match, swap Cars and delete the oldest one
                        if (itFG->match(*itC))
                        {
                                //cout << "match found" << endl;
                                //cout << "velocity = " << itFG->velocity() << endl;
                                iter_swap(itC, itFG);
                                itFG = fgDetected.erase(itFG);
                                foundMatch = true;
                                break;
                        }

                }

                if (!foundMatch)
                        itFG++;
        }

        //Remove cars that are not in the scene anymore
        auto onScene = [&](Vehicles& c) {return !c.onScene(ROI, time);};
        itC = remove_if(vehicles.begin(), vehicles.end(),onScene);
        vehicles.erase(itC, vehicles.end());

        //New FG Cars that did not match old ones are included (should improve with some tests)
        vehicles.insert(vehicles.end(), fgDetected.begin(), fgDetected.end());
}

void TrafficFlow::playvideo()
{
        BackgroundSubtractorMOG2 bsm;

        bsm.set("backgroundRatio", 0.8);
        bsm.set("nShadowDetection", 0);

        Mat f, roi, fg;
        double t;

        char c = 'c';
        while(c != 'e')
        {
                //Get frame and time
                if (!vc.read(f))
                        break;
                t = vc.get(CV_CAP_PROP_POS_MSEC);

                //Get ROI and do BackgroundSubtraction to obtain Foreground MASK
                roi = getROI(f);
                bsm(roi, fg, 0.01);

                //Detect Foreground cars
                fgDetected.clear();
                getVehiclesFG(fg, roi, t);
                updateVehicles(roi,t);

                //Compute Statistics and show cars on ROI
                double VehicleArea, occ, flow;
                VehicleArea = 0; flow = 0;
                for (Vehicles& c : vehicles)
                {
                        VehicleArea += c.area();
                        flow += c.area()*c.velocity();
                        c.plot(roi,0);
                }
                occ = VehicleArea/ROISize.area();
                if (VehicleArea >0)
                        flow /= VehicleArea;
                else
                        flow = 0;

               // outFile<<t << "ms occ=" << occ << " flow=" << flow << endl;

                //Display results
                cout <<t << "ms occ=" << occ << " flow=" << flow << endl;

                imshow("Video", f);
                moveWindow("Video",650,350);
                imshow("ROI", roi);
                 moveWindow("ROI",650,20);
                imshow("Foreground", fg);
                 moveWindow("Foreground",950,20);
                //freopen("/dev/stdout", "a", stdout);
                c = waitKey(100);
        }

}


