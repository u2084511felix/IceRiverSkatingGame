#include <olcPixelGameEngine3.h>


void plane3DWave(mesh3d& m, float time)
{
    float waveSpeed = 1.0f;
    float waveHeight = 2.0f;
    float waveLength = 100.0f;

    float k = 2.0f * M_PI / waveLength;

    for (size_t i = 0; i < m.pos.size(); i++)
    {
        m.pos[i].y = m.originalPos[i].y + std::sin(time * waveSpeed + m.originalPos[i].x * k) * waveHeight;
    }
}



void plane3DZWave(mesh3d& m, float time)
{
    float waveSpeed = 1.0f;
    float waveHeight = 2.0f;
    float waveLength = 100.0f;

    float k = 2.0f * M_PI / waveLength;
    
    for (size_t i = 0; i < m.pos.size(); i++){       
        float wave =
            std::sin(time * waveSpeed + m.originalPos[i].x * k) +
            std::sin(time * waveSpeed * 0.8f + m.originalPos[i].z * k * 0.7f);

        m.pos[i].y = m.originalPos[i].y + wave * waveHeight * 0.5f;
    }
}


void rippleWave(mesh3d &m, float time){
    int size = m.pos.size() /2;

    olc::vf4d origin = {m.pos[size].x, 0.0f, m.pos[size].y};

    for (int i = 0; i < m.pos.size(); i++){
        olc::vf4d vertex = m.pos[i];
        float distancefromCenter = (vertex - origin).mag();
        vertex.y = m.originalPos[i].y + std::sin(time+distancefromCenter);
        m.pos[i] = vertex;
    }
}

