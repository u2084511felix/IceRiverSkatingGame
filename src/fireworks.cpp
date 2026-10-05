#define OLC_PGE3_APPLICATION
#include "olcPixelGameEngine3.h"
#include <ranges>
#include <cmath>

class SeasonsChange : public olc::PixelGameEngine
{
public:
    SeasonsChange()
    {
        sAppName = "PGE3 WASM SeasonsChange";
    }

    bool reload = false;
    float deltaTime = 0.0f; 

    olc::Pixel col = olc::Colour::WHITE;

    struct Sparkle {
        olc::Pixel pixel;
        olc::vf2d position;
        olc::vf2d velocity;
        olc::vf2d acceleration;

        float mass = 0.0f;

        void applyForce(const olc::vf2d& force){
            acceleration += force/mass;
        }

        void update(float deltaTime){
            velocity += acceleration * deltaTime;
            position += velocity * deltaTime;
        }

        float speed() const {
            return velocity.mag();
        }

        olc::vf2d direction() const {
            if (velocity.mag() == 0.0f){
                return {0.0f, 0.0f};
            }
            return velocity.norm();
        }
    };

    std::vector<std::vector<Sparkle>> fireworks;

public:


    olc::Pixel ColourShift(olc::Pixel col) {
        col.r += 3;
        col.g += 1;
        col.b += 2;
        col.a = 255;
        return col;
    }

    float Makeneg(float inp) {
        float dirx = (float)rand() / (float)RAND_MAX;
        if (dirx < 0){
            inp = inp - (inp * 2);
        }
        return inp;
    }

    olc::Pixel SparkDecay(olc::Pixel& col){
        if (col.a > 0) {
            col.a -= 1;
        }
        if (col.a <= 0){
            reload = true;
        }
        return col;
    }

    olc::vf2d normVec(float x, float y){
        float norm = std::sqrt((x * x) + (y * y));        
        x /= norm;
        y /= norm;

        return olc::vf2d({x, y});
    }


    void AnimateText(olc::Pixel col, std::string text, float t){
        const float x = 120.0f + std::cos(t *2) * 60.0f;
        const float y = 120.0f + std::sin(t *2) * 60.0f;
        draw.String({x, y}, text, col);
    }

    void LoadMunition() {
        for (int y = 0; y < 20; y++){
            int posx = rand() % 320;
            int posy = rand() % 240;
            Sparkle spark;
            spark.position = {float(posx), float(posy)};

            std::vector<Sparkle> sparkles;
            for (int i = 0; i < 100; i++){
                spark.pixel = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);
                spark.mass = (float)rand() / (float)RAND_MAX;    

                float angle = ((float)rand() / RAND_MAX) * 2.0f * 3.14f;

                float speed =
                    30.0f +
                    ((float)rand() / RAND_MAX) * 180.0f;

                spark.velocity = {
                    std::cos(angle) * speed,
                    std::sin(angle) * speed
                };

                sparkles.push_back(spark);
            }

            fireworks.push_back(sparkles);
        }
    }

    void SparkFly(Sparkle& spark){

        olc::vf2d gravity = {0.0f, 10.0f};

        spark.applyForce(gravity * spark.mass);

        spark.update(deltaTime);
        draw.Pixel(spark.position, spark.pixel);
        spark.pixel = SparkDecay(spark.pixel);
        
        draw.Pixel(spark.position, spark.pixel);

    }


    bool OnUserCreate() override
    {
        LoadMunition();
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        (void)fElapsedTime;

        draw.Clear(olc::Colour::VERY_DARK_BLUE);

        deltaTime = fElapsedTime * 0.3;

        for (auto& firework : fireworks)
        {
            for (auto& spark : firework)
            {
                if (reload == true){
                    break;
                }
                SparkFly(spark);
            }
        }
        if (reload == true){
            fireworks.clear();
            LoadMunition();
            reload = false;
        }
        const float t = TotalTimeElapsed();

        std::string hello = "Hello world";
        col = ColourShift(col);
        AnimateText(col, hello, t);

        return true;
    }
};

int main()
{
    SeasonsChange SeasonsChange;

    if (SeasonsChange.Construct({320, 240}, {5, 5}))
    {
        SeasonsChange.Start();
    }

    return 0;
}