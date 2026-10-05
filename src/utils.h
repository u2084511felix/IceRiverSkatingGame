#include "olcPixelGameEngine3.h"


olc::vf4d Vector_Mul(olc::vf4d &v1, float k)
{
    return { v1.x * k, v1.y * k, v1.z * k };
}

olc::vf4d Vector_Div(olc::vf4d &v1, float k)
{
	return { v1.x / k, v1.y / k, v1.z / k };
}

// Create look-at matrix
olc::mf4d CreateLookAtMatrix(const olc::vf4d& eye, const olc::vf4d& target, const olc::vf4d& up)
{
	olc::vf4d zaxis = (eye - target).norm();    // The "forward" vector.
	olc::vf4d xaxis = up.cross(zaxis).norm(); // The "right" vector.
	olc::vf4d yaxis = zaxis.cross(xaxis);     // The "up" vector.
	
	olc::mf4d viewMatrix;
	viewMatrix(0,0) = xaxis.x; viewMatrix(0,1) = yaxis.x; viewMatrix(0,2) = zaxis.x; viewMatrix(0,3) = 0;
	viewMatrix(1,0) = xaxis.y; viewMatrix(1,1) = yaxis.y; viewMatrix(1,2) = zaxis.y; viewMatrix(1,3) = 0;
	viewMatrix(2,0) = xaxis.z; viewMatrix(2,1) = yaxis.z; viewMatrix(2,2) = zaxis.z; viewMatrix(2,3) = 0;
	viewMatrix(3,0) = -xaxis.dot(eye); viewMatrix(3,1) = -yaxis.dot(eye); viewMatrix(3,2) = -zaxis.dot(eye); viewMatrix(3,3) = 1;
	return viewMatrix;
}


float toRadians(float degrees) {
	return degrees * (M_PI / 180);
}



class MyDraw : public olc::PixelGameEngine
{
public:
    MyDraw(){}
    void drawToImage(olc::Image &image, int size)
    {
        for (int i = 0; i < size; i++){
            float x = rand() % image.Size().x;
            float y = rand() % image.Size().y;
            draw.Pixel({x,y}, olc::Colour::RED);
        }
    }

private:


};


