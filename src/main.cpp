/*
	olc::PixelGameEngine3	
	Licenced under the OLC-3 License
*/

#define OLC_PGE3_APPLICATION
#include "olcPixelGameEngine3.h"
#include <format>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <math.h>
#include "materials.h"
#include "utils.h"
#include "animations.h"
#include "gameobjects.h"


class Example_3DCube : public olc::PixelGameEngine
{
public:
	Example_3DCube(){}
	
	olc::mf4d matWorld;
	olc::mf4d matTrans;
	olc::mf4d matView;
	float totalTime = 0.0f;

	olc::vf2d centerPos = {float(ScreenSize().x /2), float(ScreenSize().y/2)};

	olc::vf4d vUp = { 0,-1,0 };
	olc::vf4d vDefaultForward = { 0, 0, 1, 0 };
	olc::mf4d matCameraRotY;
	olc::mf4d matCameraRotX;


	int planelength = 16;
	olc::vi2d mapSize = {planelength, planelength};

	olc::Image sprtex1;

	olc::mf4d matProj;	// Matrix that converts from view space to screen space
	olc::vf4d vCamera;	// Location of camera in world space
	olc::vf4d vLookDir = {0.0f,0.0f,0.0f,0.0f};	// Direction vector along the direction camera points
	
	float fYaw = 0.0f;
	float fPitch = 0.0f;

	float fTargetYaw = 0.0f;
	float fTargetPitch = 0.0f;
	float speed = 0.0f;
	float m_sensitivity = 1.0f;

	olc::vf2d vPreviousMouse = { 0.0f, 0.0f };
	bool bFirstMouse = true;

	olc::Image im2DPlane;
	olc::Image spheres;

	mesh3d mesh2Dplane;
	mesh3d farlands;

	mesh3d cube1;
	mesh3d cube2;
	mesh3d cube3;
	mesh3d cube4;
	mesh3d sphere1;

	int enemies = 0;
	
	Player player;




protected:

public:
	bool OnUserCreate() override
	{
		matProj.perspective(60.0f * 3.14159f / 180.0f, float(ScreenSize().x) / float(ScreenSize().y), 0.1f, 1600.0f);
		draw.SetProjectionMatrix(matProj);

		Player player;
		
		
		//Flat2DPlane(planelength, mesh2Dplane);
		gridPlane(planelength, mesh2Dplane, 100);

		int min = 40;


		int small = min + rand() % 40;
		int med = min + 10 + rand() % 80;
		int large  = min + 20 + rand() % 160;	
		int offs = rand() % 500;
		olc::vf4d cPos = {float(offs),0,float(offs)};

		Cube(float(small), cube2, cPos);

		small = min + rand() % 40;
		med = min + 10 + rand() % 80;
		large  = min + 20 + rand() % 160;	
		offs = rand() % 500;
		cPos = {float(offs),0,float(offs)};

		Cube(float(med), cube3, cPos);

		small = min + rand() % 40;
		med = min + 10 + rand() % 80;
		large  = min + 20 + rand() % 160;	
		offs = rand() % 500;
		cPos = {float(offs),0,float(offs)};

		Cube(float(large), cube4, cPos);

		CreateImage(spheres, {32, 32});

		// draw.SetTarget(spheres);
		// draw.Clear(olc::Colour::WHITE);

		// // bool _draw = true;
		// for (int i = 0; i < 32; i++){
		// 	for(int y = 0; y < 32; y ++){
		// 		if (_draw) {
		// 			//olc::Pixel p = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);
		// 			olc::Pixel p = olc::Colour::MAGENTA;
		// 			p.a = 160;
		// 			draw.Pixel({float(i), float(y)}, p);
		// 			_draw = false;
		// 		}
		// 		else {
		// 			olc::Pixel p = olc::Colour::VERY_DARK_MAGENTA;
		// 			p.a = 160;
		// 			draw.Pixel({float(i), float(y)}, p);
		// 			_draw = true;
		// 		}
		// 	}
		// 	if (_draw == true){
		// 		_draw = false;
		// 	}
		// 	else {
		// 		_draw = true;
		// 	}
		// }


		CreateImage(im2DPlane, mapSize);		
		draw.SetTarget(im2DPlane);
		olc::Pixel bg = olc::Colour::WHITE;
		draw.Clear(bg);

		// bool _draw = true;
		// for (int i = 0; i < planelength; i++){
		// 	for(int y = 0; y < planelength; y ++){
		// 		if (_draw) {
		// 			//olc::Pixel p = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);
		// 			olc::Pixel p = olc::Colour::BLUE;
		// 			draw.Pixel({float(i), float(y)}, p);
		// 			_draw = false;
		// 		}
		// 		else {
		// 			olc::Pixel p = olc::Colour::VERY_DARK_BLUE;
		// 			draw.Pixel({float(i), float(y)}, p);
		// 			_draw = true;
		// 		}
		// 	}
		// 	if (_draw == true){
		// 		_draw = false;
		// 	}
		// 	else {
		// 		_draw = true;
		// 	}
		// }
		
		draw.FilledRect({ 0, 0 }, draw.GetTargetSize(), olc::Colour::VERY_DARK_CYAN, olc::Colour::CYAN, olc::Colour::CYAN, olc::Colour::WHITE);

		draw.SetTarget(GetScreen());
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		totalTime += fElapsedTime;
		draw.Clear(olc::Colour::WHITE);
		draw.FilledRect({ 0, 0 }, draw.GetTargetSize(), olc::Colour::WHITE, olc::Colour::YELLOW, olc::Colour::CYAN, olc::Colour::MAGENTA);
		
		speed = 80.0f * fElapsedTime;
		
		auto vMouse = mouse.GetPosition();

		if (bFirstMouse)
		{
			vPreviousMouse = vMouse;
			bFirstMouse = false;
		}

		olc::vf2d mouseDelta = vMouse - vPreviousMouse;
		vPreviousMouse = vMouse;

		fTargetYaw += toRadians(mouseDelta.x * m_sensitivity);
		fTargetPitch += toRadians(mouseDelta.y * m_sensitivity);
		float maxPitch = toRadians(89.0f);

		fTargetPitch = std::clamp(fTargetPitch, -maxPitch, maxPitch);
		float rotationSmoothness = 22.0f;

		float rotationAlpha = 1.0f - std::exp(-rotationSmoothness * fElapsedTime);
		fYaw += (fTargetYaw - fYaw) * rotationAlpha;
		fPitch += (fTargetPitch - fPitch) * rotationAlpha;


		matCameraRotY.rotateY(fYaw);
		matCameraRotX.rotateX(fPitch);
		vLookDir = matCameraRotY * matCameraRotX * vDefaultForward;
		vLookDir = vLookDir.norm();


		olc::vf4d cameraBack = {-vLookDir.x, -vLookDir.y, -vLookDir.z, 0.0f};
		olc::vf4d vRight = vUp.cross(cameraBack).norm();
		olc::vf4d moveRight = Vector_Mul(vRight, speed);
		olc::vf4d moveForward = Vector_Mul(vLookDir, speed);


		if (keyboard.GetKey(olc::Key::A).bHeld)
			vCamera -= moveRight;

		if (keyboard.GetKey(olc::Key::D).bHeld)
			vCamera += moveRight;

		if (keyboard.GetKey(olc::Key::W).bHeld)
			vCamera += moveForward;

		if (keyboard.GetKey(olc::Key::S).bHeld)
			vCamera -= moveForward;

		if (keyboard.GetKey(olc::Key::Q).bHeld)
			vCamera.y += speed;

		if (keyboard.GetKey(olc::Key::E).bHeld)
			vCamera.y -= speed;


		// Set view matrix
		olc::vf4d vTarget = vCamera + vLookDir;
		olc::mf4d matCamera = CreateLookAtMatrix(vCamera, vTarget, vUp);
		matView = matCamera;
		draw.SetViewMatrix(matView);

		// Set model matrix
		matWorld = matTrans;
		
		draw.SetModelMatrix(matWorld);
		draw.SetCullMode(olc::CullMode::CounterClockWise);
		
		//planeWave(mesh2Dplane, fElapsedTime);
		//rippleWave(mesh2Dplane, totalTime);
		plane3DZWave(mesh2Dplane, totalTime);
		//plane3DWave(mesh2Dplane, totalTime);



		draw.Mesh(mesh2Dplane.layout, mesh2Dplane.pos, mesh2Dplane.col, mesh2Dplane.uv, im2DPlane);

		draw.Mesh(cube2.layout, cube2.pos, cube2.col, cube2.uv, im2DPlane);
		draw.Mesh(cube3.layout, cube3.pos, cube3.col, cube3.uv, im2DPlane);
		draw.Mesh(cube4.layout, cube4.pos, cube4.col, cube4.uv, im2DPlane);


		matWorld.translate(0,0,0);
		draw.SetModelMatrix(matWorld);

		const auto [x, y, z, w] = vTarget;
		draw.String({ 4, 4  }, std::format("Coordinates   = X:{: 4.1f}  Y:{: 4.1f}  Z:{: 4.1f}  W:{: 4.1f}", x, y, z, w), olc::Colour::BLACK);
		const auto [cx, cy, cz, cw] = vCamera;
		draw.String({ 4, 28 }, std::format("Calibration   = :{: }", im2DPlane.Size().x), olc::Colour::BLACK);


		draw.String({ 4, 4  },  std::format("Health   = :{:} ", player.health), olc::Colour::BLACK);
		draw.String({ 4, 52 },  std::format("Bullets  = :{:}", player.bullets), olc::Colour::BLACK);
		draw.String({ 4, 78  }, std::format("Points   = X:{:}", player.points), olc::Colour::BLACK);


		olc::vf2d cpos = {float(ScreenSize().x / 2), float(ScreenSize().y /2)};
		int a = 125;
		olc::Pixel cpcol = olc::Colour::DARK_MAGENTA;
		cpcol.a = a;

		int a1 = 145;
		olc::Pixel ccpcol = olc::Colour::DARK_RED;
		ccpcol.a = a1;


		draw.FilledCircle({cpos.x, cpos.x-cpos.x/2}, ScreenSize().x - 200, cpcol);
		draw.FilledCircle(cpos, 40, ccpcol);

		draw.RoundedRect({cpos.x-360, cpos.y}, {720, 3}, 0.4f, ccpcol);
		draw.RoundedRect({cpos.x, cpos.y-360}, {3, 720}, 0.4f, ccpcol);
		//draw.Image(im2DPlane, cpos, {16.0f, 16.0f});

		return true;
	}
};

int main()
{
	Example_3DCube demo;

	olc::PGEConfig config;
	config.vScreenSize = {1024, 960};
	config.vPixelSize = { 1, 1 };
	config.bAntiAliasMainScreen = false;
	config.sAppName = "Ice River Skating";

	if (demo.Construct(config))
	{
		demo.Start();
	}

	return 0;
}