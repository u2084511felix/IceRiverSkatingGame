#include <vector>
#include <olcPixelGameEngine3.h>

struct Vertex
{
	olc::vf4d pos;
	olc::vf4d norm;
	olc::vf2d uv;
};

struct TriangleV {
    std::array<Vertex, 3> triverts;
};

struct mesh3d
{
	std::vector<olc::vf4d> pos;
	std::vector<olc::vf4d> norm;
	std::vector<olc::vf2d> uv;
	std::vector<olc::Pixel> col;
	olc::Structure layout = olc::Structure::List;

    std::vector<olc::vf4d> originalPos;
    olc::vf4d centerOfmass;
    olc::Image texture;
    std::vector<TriangleV> triangles;
};




void pushVertex(Vertex& v, mesh3d& mesh, olc::Pixel col)
{
	mesh.pos.push_back(v.pos);
	mesh.norm.push_back(v.norm);
	mesh.uv.push_back(v.uv);
	mesh.col.push_back(col);
}

olc::vf4d getVertexMeshNormal(std::vector<olc::vf4d> verts) {

    olc::vf4d res;
    for (auto v : verts){
        res += v.norm();
    }
    res = res.norm();
    return res;
}

void pushTriangle(Vertex& a, Vertex& b, Vertex& c, mesh3d& m, olc::Pixel col = olc::Colour::WHITE)
{
    // olc::vf4d res = getVertexMeshNormal({a.pos, b.pos, c.pos});
    // a.norm = res;
    // b.norm = res;
    // c.norm = res;

	pushVertex(a, m, col);
	pushVertex(b, m, col);
	pushVertex(c, m, col);

    TriangleV t;
    t.triverts[0]= a; t.triverts[1] = b; t.triverts[2] = c;
    m.triangles.push_back(t);
}





void gridPlane (int planelength, mesh3d &p, int size)
{
	p.layout = olc::Structure::List;
	float plen = float(planelength);
    
    olc::vf2d detlatOffset = {plen, plen};
    olc::vf2d deltaOrigin = {0,0};

    for (int i = 0; i < size; i++){
        for (int y = 0; y < size; y ++){
            if (y % 2 == 0){
                continue;
            }
            olc::vf4d norm = {0,0,0,0};

            olc:: vf4d bl = {deltaOrigin.x,0,deltaOrigin.y};
            olc:: vf4d tl = {deltaOrigin.x,0,detlatOffset.y};
            olc:: vf4d tr = {detlatOffset.x,0,detlatOffset.y};
            olc:: vf4d br = {detlatOffset.x,0,deltaOrigin.y};
            
            Vertex v1 = {bl, norm, {1.0, 1.0}};
            Vertex v2 = {tl, norm, {1.0, 1.0}};
            Vertex v3 = {tr, norm, {2.0, 0.0}};
            pushTriangle(v1, v2, v3, p, olc::Colour::WHITE);

            Vertex v4 = {bl, norm, {1.0, 1.0}};
            Vertex v5 = {tr, norm, {2.0, 0.0}};
            Vertex v6 = {br, norm, {2.0, 1.0}};
            pushTriangle(v4, v5, v6, p, olc::Colour::WHITE);


            deltaOrigin.x += plen;
            detlatOffset.x += plen;

        }

        deltaOrigin.x = 0;
        detlatOffset.x = plen;

        detlatOffset.y += plen;
        deltaOrigin.y += plen;

    }

    p.originalPos = p.pos;
}








void longPlane (int planelength, mesh3d &p, int size)
{
	p.layout = olc::Structure::List;
	float plen = float(planelength);
    
    olc::vf2d detlatOffset = {plen, plen};
    olc::vf2d deltaOrigin = {0,0};

    for (int i = 0; i < size; i++){
        for (int y = 0; y < size; y ++){

            olc::vf4d norm = {0,0,0,0};

            olc:: vf4d bl = {deltaOrigin.x,0,deltaOrigin.y};
            olc:: vf4d tl = {deltaOrigin.x,0,detlatOffset.y};
            olc:: vf4d tr = {detlatOffset.x,0,detlatOffset.y};
            olc:: vf4d br = {detlatOffset.x,0,deltaOrigin.y};
            
            Vertex v1 = {bl, norm, {1.0, 1.0}};
            Vertex v2 = {tl, norm, {1.0, 1.0}};
            Vertex v3 = {tr, norm, {2.0, 0.0}};
            pushTriangle(v1, v2, v3, p, olc::Colour::WHITE);

            Vertex v4 = {bl, norm, {1.0, 1.0}};
            Vertex v5 = {tr, norm, {2.0, 0.0}};
            Vertex v6 = {br, norm, {2.0, 1.0}};
            pushTriangle(v4, v5, v6, p, olc::Colour::WHITE);


            deltaOrigin.x += plen;
            detlatOffset.x += plen;

        }
    
        if (i %2 == 0){
            continue;
        }
        deltaOrigin.x = 0;
    
        detlatOffset.x = plen;

        detlatOffset.y += plen;
        deltaOrigin.y += plen;

    }

    p.originalPos = p.pos;
}



void Cube(float radius, mesh3d &p, olc::vf4d centerPos = {0,0,0}, bool squash = false){
	p.layout = olc::Structure::List;

    float sqa = 0.0f;
    if (squash) {
        sqa = radius /2;
    }
    // top
    
    olc::vf4d top = {0, centerPos.y + radius - sqa, 0};
    // bottom
    olc::vf4d bottom = {0, centerPos.y - (radius - sqa), 0};

    // east
    olc::vf4d east = {centerPos.y + radius, 0, 0};
    // west
    olc::vf4d west = {centerPos.y - radius, 0, 0};

    // north
    olc::vf4d north = {0, 0, centerPos.y + radius};
    // south
    olc::vf4d south = {0, 0, centerPos.y - radius};


    //front

    olc: vf2d an1, an2, an3, bn1, bn2, bn3;
    an1 = { 1.0, 1.0 };
    an2 = { 1.0, 1.0 };
    an3 = { 2.0, 0.0 };
    bn1 = { 1.0, 1.0 };
    bn2 = { 2.0, 0.0 };
    bn3 = { 2.0, 1.0 };

    olc::vf4d norm = {0,0,0,0};
    Vertex v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24;
    v1 = {south, norm, an1};
    v3 = {top, norm, an2};
    v2 = {west, norm, an3};
    pushTriangle(v1, v2, v3, p);

    v4 = {south, norm, an1};
    v5 = {top, norm, an2};
    v6 = {east, norm, an3};
    pushTriangle(v4, v5, v6, p);

    v7 = {south, norm, an1};
    v9 = {bottom, norm, an2};
    v8 = {east, norm, an3};
    pushTriangle(v7, v8, v9, p);

    v10 = {south, norm, an1};
    v11 = {bottom, norm, an2};
    v12 = {west, norm, an3};
    pushTriangle(v10, v11, v12, p);

    v13 = {north, norm, an1};
    v14 = {top, norm, an2};
    v15 = {west, norm, an3};
    pushTriangle(v13, v14, v15, p);

    v16 = {north, norm, an1};
    v18 = {top, norm, an2};
    v17 = {east, norm, an3};
    pushTriangle(v16, v17, v18, p);

    v19 = {north, norm, an1};
    v20 = {bottom, norm, an2};
    v21 = {east, norm, an3};
    pushTriangle(v19, v20, v21, p);
    
    v22 = {north, norm, an1};
    v24 = {bottom, norm, an2};
    v23 = {west, norm, an3};
    pushTriangle(v22, v23, v24, p);
    
    int complexity = 4;

}




olc::vf4d getSphereTranslation(float u, float v)
{
	float theta = u * 2.0f * M_PI;
	float phi   = v * M_PI;
	float sinPhi = std::sin(phi);
	float x = sinPhi * std::cos(theta);
	float y = std::cos(phi);
	float z = sinPhi * std::sin(theta);
    return {x, y, z, 1.0f};
}


Vertex makeSphereVertex(float u, float v, float radius)
{
    olc::vf4d pos = getSphereTranslation(u, v);
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;

	return Vertex
	{
		{ x * radius, y * radius, z * radius, 1.0f },
		{ x, y, z, 0.0f },
		{ u, v }
	};
}

void CreateSphere(mesh3d& mesh, float radius = 10.0f, int slices = 32, int stacks = 16, olc::Pixel colour = olc::Colour::WHITE)
{
	mesh.layout = olc::Structure::List;

	slices = std::max(slices, 3);
	stacks = std::max(stacks, 2);
	radius = std::max(radius, 0.001f);

	for (int stack = 0; stack < stacks; stack++)
	{
		const float v0 = float(stack) / float(stacks);
		const float v1 = float(stack + 1) / float(stacks);

		for (int slice = 0; slice < slices; slice++)
		{
			const float u0 = float(slice) / float(slices);
			const float u1 = float(slice + 1) / float(slices);

			Vertex p00 = makeSphereVertex(u0, v0, radius);
			Vertex p01 = makeSphereVertex(u1, v0, radius);
			Vertex p10 = makeSphereVertex(u0, v1, radius);
			Vertex p11 = makeSphereVertex(u1, v1, radius);

            if (stack != 0)
                pushTriangle(p00, p01, p11, mesh, colour);

            if (stack != stacks - 1)
                pushTriangle(p00, p11, p10, mesh, colour);
        }
    }

	mesh.originalPos = mesh.pos;
}