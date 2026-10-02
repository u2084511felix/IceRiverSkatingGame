/*
	olc::PixelGameEngine3 Example - olc::SanityCube!!!

	Draws the infamous olc::SanityCube using the hardware 3D rendering capabilities. 
	
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
#include <list> // Required header



		// Yaw
		//matViewRotate.y = (draw.GetTargetSize().x / 2) + mouse.GetPosition().x * fElapsedTime;
		// draw.SetViewport();
		// draw.ScreenToWorld();

		// draw.TaskWireMesh();

		
		// draw.WorldOffset();
		// draw.WorldOffset();
		// draw.WorldReset();
		// draw.WorldRotate();
		// draw.WorldScale();
		// draw.WorldToScreen();

		// draw.GetWorldTransform();
		// draw.SetWorldTransform();

		// draw.vViewportPos();
		// draw.vViewportSize();

		// Draw3D()

		// void olc::Draw::MatrixReset()
		// void olc::Draw::SetModelMatrix(const olc::mf4d& mat)
		// const olc::mf4d& olc::Draw::GetModelMatrix() const
		// void olc::Draw::SetViewMatrix(const olc::mf4d& mat)
		// const olc::mf4d& olc::Draw::GetViewMatrix() const
		// void olc::Draw::SetProjectionMatrix(const olc::mf4d& mat)
		// const olc::mf4d& olc::Draw::GetProjectionMatrix() const
		// void olc::Draw::SetMVPMatrix(const olc::mf4d& mat)
		// const olc::mf4d& olc::Draw::GetMVPMatrix() const


class Example_3DCube : public olc::PixelGameEngine
{
public:
	Example_3DCube(){}
	// Matrices for world transforms
	olc::mf4d matWorld;
	// Create a world matrix that rotates the cube over time. The cube is offset
	// so it rotates around its centre. Its verts are defined in the range 0..1
	olc::mf4d matTrans;
	olc::mf4d matRotX;
	olc::mf4d matRotY;

	olc::mf4d matView;







	int planelength = 800;
	olc::vi2d mapSize = {planelength, planelength};
	int planeOffset = -(planelength/4);
	olc::Image sprtex1;


	struct vec2d // vec2d 
	// UVW
	{
		float u = 0;
		float v = 0;
		float w = 1;
	};

	struct triangle
	{
		//olc::vf4d p[3];
		std::array<olc::vf4d, 3> p;
		std::array<olc::vf4d, 3> norm;
		std::array<vec2d, 3> t;
		std::vector<olc::vf2d> uv;
		olc::Pixel col;

		std::vector<olc::vf2d> setUV () {
			for (auto &_uv : t)
			{
				uv.push_back({_uv.u, _uv.v});
			}
			return uv;
		}
	};


	struct tmesh
	{
		std::list<triangle> triangles;
		std::vector<triangle> tris;

		std::vector<olc::vf4d> pos;
		std::vector<olc::vf4d> norm;
		std::vector<olc::vf2d> uv;
		std::vector<olc::Pixel> col;
		olc::Structure layout = olc::Structure::List;
	};

	tmesh trianglesMesh;
	
	// struct mat4x4
	// {
	// 	float m[4][4] = { 0 };
	// };

	olc::mf4d mat4x4;

	olc::mf4d matProj;	// Matrix that converts from view space to screen space
	olc::vf4d vCamera;	// Location of camera in world space
	olc::vf4d vLookDir;	// Direction vector along the direction camera points
	float fYaw;			// FPS Camera rotation in XZ plane
	float fPitch;			// FPS Camera rotation in XZ plane

	float fTheta;		// Spins World transform


	// PGE2 vector / matrix type aliases:
	// olc::vf4d = olc::vf4d
	// vec2d = olc::vf2d
	// mat4d = olc::mf4d

	// Matrix_MultiplyVector


	
	// Matrix_MakeIdentity 
	// olc::mf4d example.identity()
	
	// Matrix_MakeRotationX
	// olc::mf4d example.rotateX(fangle)
	// Matrix_MakeRotationY
	// olc::mf4d example.rotateY(fangle)
	// Matrix_MakeRotationZ
	// olc::mf4d example.rotateZ(fangle)

	// mat4x4 Matrix_MakeTranslation	
	// olc::mf4d example.translate(vf4d)

	// mat4x4 Matrix_MakeProjection
	// olc::mf4d example.projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)


	olc::vf4d Matrix_MultiplyVector(olc::mf4d m, olc::vf4d &i)
	{
		olc::vf4d v;
		v.x = i.x * m.idx(0,0) + i.y * m.idx(1,0) + i.z * m.idx(2,0) + i.w * m.idx(3,0);
		v.y = i.x * m.idx(0,1) + i.y * m.idx(1,1) + i.z * m.idx(2,1) + i.w * m.idx(3,1);
		v.z = i.x * m.idx(0,2) + i.y * m.idx(1,2) + i.z * m.idx(2,2) + i.w * m.idx(3,2);
		v.w = i.x * m.idx(0,3) + i.y * m.idx(1,3) + i.z * m.idx(2,3) + i.w * m.idx(3,3);
		return v;
	}



	float Vector_DotProduct(olc::vf4d &v1, olc::vf4d &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	olc::vf4d Vector_Mul(olc::vf4d &v1, float k)
	{
		return { v1.x * k, v1.y * k, v1.z * k };
	}

	olc::vf4d Vector_Div(olc::vf4d &v1, float k)
	{
		return { v1.x / k, v1.y / k, v1.z / k };
	}

	olc::mf4d Matrix_PointAt(olc::vf4d &pos, olc::vf4d &target, olc::vf4d &up)
	{
		// Calculate new forward direction
		olc::vf4d newForward;
		//newForward -=(pos, target);
		newForward = pos - target;
		newForward = newForward.norm();

		// Calculate new Up direction

		float dotprod = Vector_DotProduct(up, newForward);
		olc::vf4d a = Vector_Mul(newForward, dotprod);

		
		olc::vf4d newUp = up - a;

		newUp = newUp.norm();

		// New Right direction is easy, its just cross product
		olc::vf4d newRight = newUp.cross(newForward);

		// Construct Dimensioning and Translation Matrix	
		olc::mf4d matrix;

		matrix(0,0) = newRight.x;	matrix(0,1) = newRight.y;	matrix(0,2) = newRight.z;	matrix(0,3) = 0.0f;
		matrix(1,0) = newUp.x;		matrix(1,1) = newUp.y;		matrix(1,2) = newUp.z;		matrix(1,3) = 0.0f;
		matrix(2,0) = newForward.x;	matrix(2,1) = newForward.y;	matrix(2,2) = newForward.z;	matrix(2,3) = 0.0f;
		matrix(3,0) = pos.x;		matrix(3,1) = pos.y;		matrix(3,2) = pos.z;		matrix(3,3) = 1.0f;

		return matrix;
	}
	


	void DrawRectwithPos(olc::vf2d pos, float size){
		olc::vf2d rsize = {size, size};
		draw.FilledRect(pos, rsize, olc::Colour::BLUE, olc::Colour::YELLOW, olc::Colour::GREEN, olc::Colour::MAGENTA);
	}

	struct plane
	{
		std::vector<olc::vf4d> pos;
		std::vector<olc::vf4d> norm;
		std::vector<olc::vf2d> uv;
		std::vector<olc::Pixel> col;
		olc::Structure layout = olc::Structure::List;
	};

	olc::Image im2DPlane;

	plane mesh2Dplane;

	inline plane Flat2DPlane()
	{
		plane p;
		p.layout = olc::Structure::List;

		float plen = float(planelength /2);

		// Triangle 1;
		//x               x,y,z
		p.pos.push_back({ 0,0,0 }); p.norm.push_back({ 0, 1, 0, 0 }); p.uv.push_back({ 0.25, 0.25 }); p.col.push_back(olc::Colour::WHITE);
		//y
		p.pos.push_back({ 0,0,plen }); p.norm.push_back({ 0, 1, 0, 0 }); p.uv.push_back({ 0.25, 0.0 }); p.col.push_back(olc::Colour::WHITE);
		//z
		p.pos.push_back({ plen,0,plen }); p.norm.push_back({ 0, 1, 0, 0 }); p.uv.push_back({ 0.5, 0.0 }); p.col.push_back(olc::Colour::WHITE);

		// Triangle 2;
		//x
		p.pos.push_back({ 0,0,0 }); p.norm.push_back({ 0, 1, 0, 0 }); p.uv.push_back({ 0.25, 0.25 }); p.col.push_back(olc::Colour::WHITE);
		//y
		p.pos.push_back({ plen,0,plen }); p.norm.push_back({ 0, 1, 0, 0 }); p.uv.push_back({ 0.5, 0.0 }); p.col.push_back(olc::Colour::WHITE);
		//z
		p.pos.push_back({ plen,0,0 }); p.norm.push_back({ 0, 1, 0, 0 }); p.uv.push_back({ 0.5, 0.25 }); p.col.push_back(olc::Colour::WHITE);


		return p;
	}

	tmesh flatmesh;

	inline tmesh flatMesh()
	{

		tmesh p;

		triangle fm;
		
		float plen = float(planelength /2);

		fm.p[0] = { 0,0,0 }; fm.norm[0] = { 0, 1, 0, 0 }; fm.t[0] = { 0.25, 0.25 }; fm.col = olc::Colour::WHITE;
		fm.p[1] = { 0,0,plen }; fm.norm[1] = { 0, 1, 0, 0 }; fm.t[1] = { 0.25, 0.0 }; fm.col = olc::Colour::WHITE;
		fm.p[2] = { plen,0,plen }; fm.norm[2] = { 0, 1, 0, 0 }; fm.t[2] = { 0.5, 0.0 }; fm.col = olc::Colour::WHITE;
		
		triangle fm2;

		fm2.p[0] = { 0,0,0 }; fm2.norm[0] = { 0, 1, 0, 0 }; fm2.t[0] = { 0.25, 0.25 }; fm2.col = olc::Colour::WHITE;
		fm2.p[1] = { plen,0,plen }; fm2.norm[1] = { 0, 1, 0, 0 }; fm2.t[1] = { 0.5, 0.0 }; fm2.col = olc::Colour::WHITE;
		fm2.p[2] = { plen,0,0 }; fm2.norm[2] = { 0, 1, 0, 0 }; fm2.t[2] = { 0.5, 0.25 }; fm2.col = olc::Colour::WHITE;


		p.tris.push_back(fm);
		p.tris.push_back(fm2);

		return p;
	}

	float *pDepthBuffer = nullptr;




protected:

public:
	bool OnUserCreate() override
	{

		std::vector<std::vector<olc::vf4d>> postuff;

		postuff.insert(postuff.end(), {
		{{ 0.0f,0.0f,0.0f,1.0f}, {0.0f,1.0f,0.0f,1.0f},   { 1.0f,1.0f,0.0f,1.0f}},
		{{ 0.0f,0.0f,0.0f,1.0f}, {1.0f,1.0f,0.0f,1.0f},   { 1.0f,0.0f,0.0f,1.0f}},
		{{ 1.0f,0.0f,0.0f,1.0f}, {1.0f,1.0f,0.0f,1.0f},   { 1.0f,1.0f,1.0f,1.0f}},
		{{ 1.0f,0.0f,0.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f},   { 1.0f,0.0f,1.0f,1.0f}},
		{{ 1.0f,0.0f,1.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f},   { 0.0f,1.0f,1.0f,1.0f}},
		{{ 1.0f,0.0f,1.0f,1.0f}, {0.0f,1.0f,1.0f,1.0f},   { 0.0f,0.0f,1.0f,1.0f}},
		{{ 0.0f,0.0f,1.0f,1.0f}, {0.0f,1.0f,1.0f,1.0f},   { 0.0f,1.0f,0.0f,1.0f}},
		{{ 0.0f,0.0f,1.0f,1.0f}, {0.0f,1.0f,0.0f,1.0f},   { 0.0f,0.0f,0.0f,1.0f}},
		{{ 0.0f,1.0f,0.0f,1.0f}, {0.0f,1.0f,1.0f,1.0f},   { 1.0f,1.0f,1.0f,1.0f}},
		{{ 0.0f,1.0f,0.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f},   { 1.0f,1.0f,0.0f,1.0f}},
		{{ 1.0f,0.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,1.0f},   { 0.0f,0.0f,0.0f,1.0f}},
		{{ 1.0f,0.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f,1.0f},   { 1.0f,0.0f,0.0f,1.0f}}
		});

		std::vector<std::vector<vec2d>> uvstuff;
		uvstuff.insert(uvstuff.end(), {
        {{0.0f,1.0f,1.0f},		{0.0f,0.0f,1.0f},		{1.0f,0.0f,1.0f}}, 
        {{0.0f,1.0f,1.0f},		{1.0f,0.0f,1.0f},		{1.0f,1.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{0.0f,0.0f,1.0f},		{1.0f,0.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{1.0f,0.0f,1.0f},		{1.0f,1.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{0.0f,0.0f,1.0f},		{1.0f,0.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{1.0f,0.0f,1.0f},		{1.0f,1.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{0.0f,0.0f,1.0f},		{1.0f,0.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{1.0f,0.0f,1.0f},		{1.0f,1.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{0.0f,0.0f,1.0f},		{1.0f,0.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{1.0f,0.0f,1.0f},		{1.0f,1.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{0.0f,0.0f,1.0f},		{1.0f,0.0f,1.0f}},
        {{0.0f,1.0f,1.0f},		{1.0f,0.0f,1.0f},		{1.0f,1.0f,1.0f}}
		});

		for (int i = 0; i < postuff.size(); i ++) {
			triangle t;
			for (int x = 0; x < 3; x++){
				t.p[x] = postuff[i][x];
				t.t[x] = uvstuff[i][x];
			}

			trianglesMesh.tris.push_back(t);
		}



		pDepthBuffer = new float[ScreenSize().x * ScreenSize().y];

		matProj.perspective(60.0f * 3.14159f / 180.0f, float(ScreenSize().x) / float(ScreenSize().y), 0.1f, 1600.0f);

		draw.SetProjectionMatrix(matProj);
		
		CreateImage(im2DPlane, mapSize);
		mesh2Dplane = Flat2DPlane();


		CreateImageFromFile(sprtex1, "../space.png");


		draw.SetTarget(im2DPlane);

		draw.Clear(olc::Colour::WHITE);

		for (int i = 0; i < 500; i++){

			float size = 4.0f;

			float mx = float(planelength /2) - size;
			float my = float(planelength /4) - size;

			float posx = float(planelength /4);
			float posy = 0.0f;

	        int tsize = int(posx);
		    posx += float(rand() % tsize);
			posy = float(rand() % tsize);

			if (posx >= mx){
				posx = mx - size;
			}
		    if (posy >= my){
				posy = my - size;
			}
			DrawRectwithPos({posx, posy}, size);

		}



		draw.SetTarget(GetScreen());
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{


		draw.Clear(olc::Colour::VERY_DARK_BLUE);
		draw.FilledRect({ 0, 0 }, draw.GetTargetSize(), olc::Colour::WHITE, olc::Colour::YELLOW, olc::Colour::CYAN, olc::Colour::MAGENTA);

		// Yaw
		// if (keyboard.GetKey(olc::Key::LEFT).bHeld)
		// 	matViewRotate.y -= 2.0f * fElapsedTime;
		// if (keyboard.GetKey(olc::Key::RIGHT).bHeld)
		// 	matViewRotate.y += 2.0f * fElapsedTime;

		// // Pitch
		// if (keyboard.GetKey(olc::Key::UP).bHeld)
		// 	matViewRotate.x += 2.0f * fElapsedTime;
		// if (keyboard.GetKey(olc::Key::DOWN).bHeld)
		// 	matViewRotate.x -= 2.0f * fElapsedTime;

		// // Roll
		// if (keyboard.GetKey(olc::Key::NP7).bHeld)
		// 	matViewRotate.z -= 2.0f * fElapsedTime;
		// if (keyboard.GetKey(olc::Key::NP8).bHeld)
		// 	matViewRotate.z += 2.0f * fElapsedTime;

		auto vMouse = mouse.GetPosition();
		bool moved_mouse = olc::PGEWindow::olc_OnMouseMove(vMouse);

		olc::vf4d vForward = Vector_Mul(vLookDir, 8.0f * fElapsedTime);

		// olc_OnMouseMove
		// mouse.GetWheel()
		// mouse.GetButton(0).bHeld / bPressed / bClicked / bScrolled
		
		if (keyboard.GetKey(olc::Key::Q).bHeld)
			vCamera.y += 8.0f * fElapsedTime;	// Travel Upwards
		if (keyboard.GetKey(olc::Key::E).bHeld)
			vCamera.y -= 8.0f * fElapsedTime;	// Travel Downwards

		// Dont use these two in FPS mode, it is confusing :P
		if (keyboard.GetKey(olc::Key::LEFT).bHeld)
			vCamera.x -= 8.0f * fElapsedTime;	// Travel Along X-Axis
		if (keyboard.GetKey(olc::Key::RIGHT).bHeld)
			vCamera.x += 8.0f * fElapsedTime;	// Travel Along X-Axis




		// Standard FPS Control scheme, but turn instead of strafe
		if (keyboard.GetKey(olc::Key::W).bHeld)
			vCamera = vCamera + vForward;

		if (keyboard.GetKey(olc::Key::S).bHeld)
			vCamera = vCamera - vForward;

		if (keyboard.GetKey(olc::Key::A).bHeld)
			fYaw -= 2.0f * fElapsedTime;

		if (keyboard.GetKey(olc::Key::D).bHeld)
			fYaw += 2.0f * fElapsedTime;


		if (keyboard.GetKey(olc::Key::UP).bHeld)
			fPitch -= 2.0f * fElapsedTime;

		if (keyboard.GetKey(olc::Key::DOWN).bHeld)
			fPitch += 2.0f * fElapsedTime;

		
		// Create "Point At" Matrix for camera
		olc::vf4d vUp = { 0,-1,0 };
		olc::vf4d vTarget = { 0,0,1 };
		olc::mf4d matCameraRotY;
		olc::mf4d matCameraRotX;

		matCameraRotY.rotateY(fYaw);
		matCameraRotX.rotateX(fPitch);

		vLookDir = matCameraRotY * vTarget;

		vTarget = vCamera + vLookDir;
		olc::mf4d matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

		matView = matCamera.invert();

		draw.SetViewMatrix(matView);
	




		matTrans.translate(planeOffset, 0, planeOffset);
	
		matWorld = matTrans;
		
		draw.SetModelMatrix(matWorld);

		draw.SetCullMode(olc::CullMode::CounterClockWise);
		
		draw.Mesh(mesh2Dplane.layout, mesh2Dplane.pos, mesh2Dplane.col, mesh2Dplane.uv, im2DPlane);

		matWorld.translate(0,0,0);
		draw.SetModelMatrix(matWorld);


		draw.Line({ 0,100,0 }, { 100, 100, 0 }, olc::Colour::RED);
		draw.Line({ 0,100,0 }, { 0, 200, 0 }, olc::Colour::GREEN);
		draw.Line({ 0,100,0 }, { 0, 100, 100 }, olc::Colour::BLUE);
		draw.Line({ 0,100,0 }, { -100, 100, 0 }, olc::Colour::RED);
		draw.Line({ 0,100,0 }, { 0, 0, 0 }, olc::Colour::GREEN);
		draw.Line({ 0,100,0 }, { 0, 100, -100 }, olc::Colour::BLUE);

		// Decompose the quaternion to demonstrate the structured binding support
		const auto [x, y, z, w] = vTarget;
		draw.String({ 4, 4 }, std::format("X:{: 4.1f}  Y:{: 4.1f}  Z:{: 4.1f}  W:{: 4.1f}", x, y, z, w), olc::Colour::BLACK);


		olc::vf2d cpos = {float(ScreenSize().x / 2), float(ScreenSize().y /2)};
		draw.FilledCircle(cpos, 10.0f, olc::Colour::DARK_YELLOW);


		return true;
	}
};

int main()
{
	Example_3DCube demo;

	olc::PGEConfig config;
	config.vScreenSize = {1024, 960};
	config.vPixelSize = { 1, 1 };
	config.bAntiAliasMainScreen = true;
	config.sAppName = "3D realm";

	if (demo.Construct(config))
	{
		demo.Start();
	}

	return 0;
}