#include "olcPixelGameEngine3.h"



struct Collider {
    bool hasCollided = false;
    bool hasPoint = false;

};

struct Player {
    Collider collider;
    int points = 0;
    int health = 10;
    int fuel = 500;
    int bullets = 2000;
    olc::vf4d position;
};

struct Enemy {
    Collider collider;
    int health = 20;
    int damage = 2;
    float speed = 2.0f;
    olc::vf4d position;

    mesh3d body;
};