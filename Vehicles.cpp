#include "Vehicles.hpp"
#include <cstring>
inline int Vehicles::area()
{
        return r.area();
}

inline double Vehicles::velocity()
{
        return v;
}

inline Point2f Vehicles::pos()
{
        return 0.5*(r.tl()+r.br());
}

Vehicles& Vehicles::operator=(const Vehicles& c)
{
        r = c.r;
        c.im.copyTo(im);
        v = c.v;
        ts = c.ts;
}

void Vehicles::plot(Mat& f, int c)
{
        Scalar color;

        if (c==0)
                color=Scalar(255,0,0);
        else
                color=Scalar(0,255,0);

        rectangle(f, r, color);
        stringstream ss;
            ss << velocity();
        string s2=ss.str();
        string s = string("v=")+s2;
        putText(f, s, pos(), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
}

bool Vehicles::onScene(Mat& f, double time)
{
        //If timestamp is not current, force velocity to be zero (cannot be moving), otherwise it wouldnt match
        if (ts<time)
                v = 0;

        //Crop image to car area
        Mat im2 = f(r);

        //Calculate absolute difference between new frame and the one with the car
        Mat abd;
        absdiff(im, im2, abd);
        double cmp = norm(abd, NORM_L1)/area();

        return cmp < 30;
}

bool Vehicles::match(Vehicles& c)
{
        //Distance between points
        double d = norm(pos()-c.pos());

        //Area comparisson
        double a = abs(double(area())/ c.area());
        a = abs(a-1);

        //Im comparisson
        Mat c2, abd;
        resize(c.im, c2, im.size());
        absdiff(im, c2, abd);
        double cmp = norm(abd, NORM_L1)/area();

        //DEBUG
       //cout << "d="<< d << "   a=" << a << "     cmp="<< cmp << endl;

        //Check all conditions to confirm a match
        if (d < 20 && a < 0.5 && cmp < 200)
        {
                //Update velocity
                v = d/(ts-c.ts);

                return true;
        }
        else
                return false;
}

