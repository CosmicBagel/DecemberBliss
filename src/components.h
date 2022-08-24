// this (and the rest of the ECS arch) is heavily based on Dave Churchill's
// lecture https://www.youtube.com/watch?v=nLk8K07fLxM I highly recommend
// watching it

#pragma once
#include "ray_includes.h"

class Entity_Memory_Pool;

struct I_Component {
    friend class Entity_Memory_Pool;
    // 1 byte
   private:
    bool active;
};

struct C_Enemy : public I_Component {};

struct C_Player : public I_Component {};

struct C_PlayerBullet : public I_Component {};

struct C_Position : public I_Component {
    // 9 bytes
   public:
    float x, y;
    C_Position() : x{0.0F}, y{0.0F}, I_Component{} {};

    // allowing this constructor, but only with explict, do not directly assign from Vec2 plz
    //careful with this one, can overwrite an active component's status with false
    explicit C_Position(Vector2 vec) : x {vec.x}, y{vec.y}, I_Component{} {};

    // copy constructor TO Vector2
    explicit operator Vector2() const { return Vector2{x, y}; }

    // copy assignment FROM Vector2 (active bool is preserved)
    C_Position& operator=(const Vector2 vec) {
        x = vec.x;
        y = vec.y;
        return *this;
    };
};

struct C_Rotation : public I_Component {
   public:
    float rotation;
};

struct C_Velocity : public I_Component {
    // 9 bytes
   public:
    float x, y;

    C_Velocity() : x{0.0F}, y{0.0F}, I_Component{} {};

    // allowing this constructor, but only with explict, do not directly assign from Vec2 plz
    //careful with this one, can overwrite an active component's status with false
    explicit C_Velocity(Vector2 vec) : x {vec.x}, y{vec.y}, I_Component{} {};

    // copy constructor TO Vector2
    explicit operator Vector2() const { return Vector2{x, y}; }

    // copy assignment FROM Vector2 (active bool is preserved)
    C_Velocity& operator=(const Vector2 vec) {
        x = vec.x;
        y = vec.y;
        return *this;
    };

    
};

struct C_Bounding_Circle : public I_Component {
    // 5 bytes
   public:
    float radius;
};

struct C_Texture : public I_Component {
    // 21 bytes
   public:
    // 20 bytes
    Texture2D texture;
};