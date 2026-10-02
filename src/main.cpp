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

	// Matrices for the 3D Projection and view transforms.
	olc::mf4d matView;
	olc::mf4d matViewRotateX;
	olc::mf4d matViewRotateY;
	olc::mf4d matViewRotateZ;

	olc::mf4d mViewTranslate;
	// Position in 3D space of "the camera"
	olc::vf4d matViewTranslate = { -150.0f, -150.0f, -200.0f };
	olc::vf4d matViewRotate = { 2.5f, 0.0f, 0.0f };


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
		std::vector<triangle> vecTrianglesToRaster;

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

	// template<typename Q>
	// olc::vf4d Matrix_MultiplyVector(olc::mf4d me, olc::v_4d<Q>& v)
	// {
	// 	olc::v_4d<Q> vOut;
	// 	vOut.x = Q(me(0, 0) * v.x + me(1, 0) * v.y + me(2, 0) * v.z + me(3, 0) * v.w);
	// 	vOut.y = Q(me(0, 1) * v.x + me(1, 1) * v.y + me(2, 1) * v.z + me(3, 1) * v.w);
	// 	vOut.z = Q(me(0, 2) * v.x + me(1, 2) * v.y + me(2, 2) * v.z + me(3, 2) * v.w);
	// 	vOut.w = Q(me(0, 3) * v.x + me(1, 3) * v.y + me(2, 3) * v.z + me(3, 3) * v.w);
	// 	return vOut;
	// }

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
	

	olc::vf4d Vector_IntersectPlane(olc::vf4d &plane_p, olc::vf4d &plane_n, olc::vf4d &lineStart, olc::vf4d &lineEnd, float &t)
	{
		plane_n = plane_n.norm();
		float plane_d = -Vector_DotProduct(plane_n, plane_p);
		float ad = Vector_DotProduct(lineStart, plane_n);
		float bd = Vector_DotProduct(lineEnd, plane_n);
		t = (-plane_d - ad) / (bd - ad);
		olc::vf4d lineStartToEnd = lineEnd - lineStart;
		olc::vf4d lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return lineStart + lineToIntersect;
	}


	int Triangle_ClipAgainstPlane(olc::vf4d plane_p, olc::vf4d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
	{
		// Make sure plane normal is indeed normal
		plane_n = plane_n.norm();

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](olc::vf4d &p)
		{
			olc::vf4d n = p.norm();
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		olc::vf4d* inside_points[3];  int nInsidePointCount = 0;
		olc::vf4d* outside_points[3]; int nOutsidePointCount = 0;
		vec2d* inside_tex[3]; int nInsideTexCount = 0;
		vec2d* outside_tex[3]; int nOutsideTexCount = 0;


		// Get signed distance of each point in triangle to plane
		float d0 = dist(in_tri.p[0]);
		float d1 = dist(in_tri.p[1]);
		float d2 = dist(in_tri.p[2]);

		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; inside_tex[nInsideTexCount++] = &in_tri.t[0]; }
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[0]; outside_tex[nOutsideTexCount++] = &in_tri.t[0];
		}
		if (d1 >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.p[1]; inside_tex[nInsideTexCount++] = &in_tri.t[1];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[1];  outside_tex[nOutsideTexCount++] = &in_tri.t[1];
		}
		if (d2 >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.p[2]; inside_tex[nInsideTexCount++] = &in_tri.t[2];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[2];  outside_tex[nOutsideTexCount++] = &in_tri.t[2];
		}

		// Now classify triangle points, and break the input triangle into 
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			out_tri1 = in_tri;

			return 1; // Just the one returned original triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1.col =  in_tri.col;
			

			// The inside point is valid, so keep that...
			out_tri1.p[0] = *inside_points[0];
			out_tri1.t[0] = *inside_tex[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			float t;
			out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.t[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
			out_tri1.t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.t[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1.col =  in_tri.col;
			

			out_tri2.col =  in_tri.col;
			

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.p[0] = *inside_points[0];
			out_tri1.p[1] = *inside_points[1];
			out_tri1.t[0] = *inside_tex[0];
			out_tri1.t[1] = *inside_tex[1];

			float t;
			out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.t[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2.p[0] = *inside_points[1];
			out_tri2.t[0] = *inside_tex[1];
			out_tri2.p[1] = out_tri1.p[2];
			out_tri2.t[1] = out_tri1.t[2];
			out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
			out_tri2.t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
			out_tri2.t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
			out_tri2.t[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;

			return 2; // Return two newly formed triangles which form a quad
		}
	}



	void TexturedTriangle(	int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3,
		olc::Image &tex)
	{
		if (y2 < y1)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
			std::swap(u1, u2);
			std::swap(v1, v2);
			std::swap(w1, w2);
		}

		if (y3 < y1)
		{
			std::swap(y1, y3);
			std::swap(x1, x3);
			std::swap(u1, u3);
			std::swap(v1, v3);
			std::swap(w1, w3);
		}

		if (y3 < y2)
		{
			std::swap(y2, y3);
			std::swap(x2, x3);
			std::swap(u2, u3);
			std::swap(v2, v3);
			std::swap(w2, w3);
		}

		int dy1 = y2 - y1;
		int dx1 = x2 - x1;
		float dv1 = v2 - v1;
		float du1 = u2 - u1;
		float dw1 = w2 - w1;

		int dy2 = y3 - y1;
		int dx2 = x3 - x1;
		float dv2 = v3 - v1;
		float du2 = u3 - u1;
		float dw2 = w3 - w1;

		float tex_u, tex_v, tex_w;

		float dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step=0, dw2_step=0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1)
		{
			for (int i = y1; i <= y2; i++)
			{
				int ax = x1 + (float)(i - y1) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u1 + (float)(i - y1) * du1_step;
				float tex_sv = v1 + (float)(i - y1) * dv1_step;
				float tex_sw = w1 + (float)(i - y1) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					if (tex_w > pDepthBuffer[i*ScreenSize().x + j])
					{
						draw.Pixel({float(j),float(i)}, tex.Sample({tex_u / tex_w, tex_v / tex_w}));

						//draw.Image(tex.region({tex_u / tex_w, tex_v / tex_w}, {1,1}), {float(j),float(i)});

						//Draw(j, i, tex->Sample(tex_u / tex_w, tex_v / tex_w));
						pDepthBuffer[i*ScreenSize().x + j] = tex_w;
					}
					t += tstep;
				}

			}
		}

		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;
		dw1 = w3 - w2;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0, dv1_step = 0;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy1)
		{
			for (int i = y2; i <= y3; i++)
			{
				int ax = x2 + (float)(i - y2) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u2 + (float)(i - y2) * du1_step;
				float tex_sv = v2 + (float)(i - y2) * dv1_step;
				float tex_sw = w2 + (float)(i - y2) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					if (tex_w > pDepthBuffer[i*ScreenSize().x + j])
					{

						//draw.Image(tex,{float(j),float(i)});
						draw.Pixel({float(j),float(i)}, tex.Sample({tex_u / tex_w, tex_v / tex_w}));
						//draw.Image(tex.region({tex_u / tex_w, tex_v / tex_w}, {1,1}), {float(j),float(i)});

						pDepthBuffer[i*ScreenSize().x + j] = tex_w;
					}
					t += tstep;
				}
			}	
		}		
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

		matProj.perspective(90.0f * 3.14159f / 180.0f, float(ScreenSize().x) / float(ScreenSize().y), 0.1f, 1600.0f);

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

		// // Yaw
		// if (keyboard.GetKey(olc::Key::LEFT).bHeld)
		// 	vCamera.y -= 2.0f * fElapsedTime;
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
		
		if (keyboard.GetKey(olc::Key::UP).bHeld)
			vCamera.y += 8.0f * fElapsedTime;	// Travel Upwards
		if (keyboard.GetKey(olc::Key::DOWN).bHeld)
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



		// Set up "World Tranmsform" though not updating theta 
		// makes this a bit redundant
		olc::mf4d matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime; // Uncomment to spin me right round baby right round

		matRotZ.rotateZ(fTheta * 0.5f);
		matRotX.rotateX(fTheta);
		
		olc::mf4d matTrans;
		matTrans.translate(0.0f, 0.0f, 5.0f);


		olc::mf4d matWorld;
		matWorld.identity();

		matWorld = matRotX * matRotY;
		matWorld = matWorld * matTrans;

		// Create "Point At" Matrix for camera
		olc::vf4d vUp = { 0,1,0 };
		olc::vf4d vTarget = { 0,0,1 };
		olc::mf4d matCameraRot;
		matCameraRot.rotateY(fYaw);
		vLookDir = matCameraRot * vTarget;
		vTarget = vCamera + vLookDir;
		olc::mf4d matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

		// Make view matrix from camera
		olc::mf4d matView;
		matView = matCamera.invert();

		// Store triagles for rastering later


		// Draw Triangles
		for (auto tri : trianglesMesh.tris)
		{
			triangle triProjected, triTransformed, triViewed;

			// //World Matrix Transform
			
			// triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
			// triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
			// triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
			triTransformed.p[0] = matWorld * tri.p[0];
			triTransformed.p[1] = matWorld * tri.p[1];
			triTransformed.p[2] = matWorld * tri.p[2];
			triTransformed.t[0] = tri.t[0];
			triTransformed.t[1] = tri.t[1];
			triTransformed.t[2] = tri.t[2];
			
			// // catch(std::exception const & ex)  {

			// // 	printf("%s", ex);

			// // };

			// Calculate triangle Normal
			olc::vf4d normal, line1, line2;

			// Get lines either side of triangle
			line1 = triTransformed.p[1] - triTransformed.p[0];
			line2 = triTransformed.p[2] - triTransformed.p[0];

			// Take cross product of lines to get normal to triangle surface
			normal = line1.cross(line2);

			// You normally need to normalise a normal!
			normal = normal.norm();
			
			// Get Ray from triangle to camera
			olc::vf4d vCameraRay = triTransformed.p[0] - vCamera;

			//If ray is aligned with normal, then triangle is visible

			if (Vector_DotProduct(normal, vCameraRay) < 0.0f)
			{
				// Illumination
				olc::vf4d light_direction = { 0.0f, 1.0f, -1.0f };
				light_direction = light_direction.norm();

				// How "aligned" are light direction and triangle surface normal?
				float dp = std::max(0.1f, Vector_DotProduct(light_direction, normal));

				// Choose console colours as required (much easier with RGB)				
				triTransformed.col = olc::Pixel(dp * 255, dp * 255, dp * 255);


				// Convert World Space --> View Space
				triViewed.p[0] = matWorld * triTransformed.p[0];
				triViewed.p[1] = matWorld * triTransformed.p[1];
				triViewed.p[2] = matWorld * triTransformed.p[2];
				triViewed.col = triTransformed.col;
				triViewed.t[0] = triTransformed.t[0];
				triViewed.t[1] = triTransformed.t[1];
				triViewed.t[2] = triTransformed.t[2];

				// Clip Viewed Triangle against near plane, this could form two additional
				// additional triangles. 
				int nClippedTriangles = 0;
				triangle clipped[2];
				nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				// We may end up with multiple triangles form the clip, so project as
				// required
				for (int n = 0; n < nClippedTriangles; n++)
				{
					// Project triangles from 3D --> 2D
					triProjected.p[0] = matProj * clipped[n].p[0];
					triProjected.p[1] = matProj * clipped[n].p[1];
					triProjected.p[2] = matProj * clipped[n].p[2];
					triProjected.col = clipped[n].col;
					triProjected.t[0] = clipped[n].t[0];
					triProjected.t[1] = clipped[n].t[1];
					triProjected.t[2] = clipped[n].t[2];


					triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
					triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
					triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

					triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
					triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
					triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

					triProjected.t[0].w = 1.0f / triProjected.p[0].w;
					triProjected.t[1].w = 1.0f / triProjected.p[1].w;
					triProjected.t[2].w = 1.0f / triProjected.p[2].w;


					// Scale into view, we moved the normalising into cartesian space
					// out of the matrix.vector function from the previous videos, so
					// do this manually
					triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
					triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
					triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

					// X/Y are inverted so put them back
					triProjected.p[0].x *= -1.0f;
					triProjected.p[1].x *= -1.0f;
					triProjected.p[2].x *= -1.0f;
					triProjected.p[0].y *= -1.0f;
					triProjected.p[1].y *= -1.0f;
					triProjected.p[2].y *= -1.0f;

					// Offset verts into visible normalised space
					olc::vf4d vOffsetView = { 1,1,0 };
					triProjected.p[0] = triProjected.p[0] + vOffsetView;
					triProjected.p[1] = triProjected.p[1] + vOffsetView;
					triProjected.p[2] = triProjected.p[2] + vOffsetView;
					triProjected.p[0].x *= 0.5f * (float)ScreenSize().x;
					triProjected.p[0].y *= 0.5f * (float)ScreenSize().y;
					triProjected.p[1].x *= 0.5f * (float)ScreenSize().x;
					triProjected.p[1].y *= 0.5f * (float)ScreenSize().y;
					triProjected.p[2].x *= 0.5f * (float)ScreenSize().x;
					triProjected.p[2].y *= 0.5f * (float)ScreenSize().y;

					// Store triangle for sorting
					trianglesMesh.vecTrianglesToRaster.push_back(triProjected);
				}			
			}
		}

		// Sort triangles from back to front
		// sort(trianglesMesh.vecTrianglesToRaster.begin(), trianglesMesh.vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2)
		// {
		// 	float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
		// 	float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
		// 	return z1 > z2;
		// });

		draw.Clear(olc::Colour::VERY_DARK_BLUE);

		// Clear Depth Buffer
		for (int i = 0; i < ScreenSize().x * ScreenSize().y; i++)
			pDepthBuffer[i] = 0.0f;

		draw.FilledRect({ 0, 0 }, draw.GetTargetSize(), olc::Colour::WHITE, olc::Colour::YELLOW, olc::Colour::CYAN, olc::Colour::MAGENTA);

		
		draw.SetViewMatrix(matView);
		//draw.SetModelMatrix(matView);
		//draw.SetCullMode(olc::CullMode::CounterClockWise);


		// Loop through all transformed, viewed, projected, and sorted triangles
		for (auto &triToRaster : trianglesMesh.vecTrianglesToRaster)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			triangle clipped[2];

			// Add initial triangle
			trianglesMesh.triangles.push_back(triToRaster);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					triangle test = trianglesMesh.triangles.front();
					trianglesMesh.triangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)ScreenSize().y - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)ScreenSize().x - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						trianglesMesh.triangles.push_back(clipped[w]);
				}
				nNewTriangles = trianglesMesh.triangles.size();
			}
		


			


			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			
			for (auto &t : trianglesMesh.triangles)
			{
				TexturedTriangle(
					t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
					t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
					t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w, 
					sprtex1
				);

				olc::vf2d tp1;
				olc::vf2d tp2;
				olc::vf2d tp3;
				tp1 = {t.p[0].x, t.p[0].y};
				tp2 = {t.p[1].x, t.p[1].y};
				tp3 = {t.p[2].x, t.p[2].y};

				//draw.Mesh(trianglesMesh.layout, t.p, trianglesMesh.col);
				draw.FilledTriangle(tp1, tp2, tp3, t.col);
			}
		}



		

		//draw.Mesh(mesh2Dplane.layout, mesh2Dplane.pos, mesh2Dplane.col, mesh2Dplane.uv, im2DPlane);



		olc::vf2d cpos = {float(1024 / 2), float(960/2)};
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