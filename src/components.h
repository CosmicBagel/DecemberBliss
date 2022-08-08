// this (and the rest of the ECS arch) is heavily based on Dave Churchill's
// lecture https://www.youtube.com/watch?v=nLk8K07fLxM I highly recommend 
// watching it

#pragma once
#include "ray_includes.h"

class Entity_Memory_Pool;

struct I_Component
{
    friend class Entity_Memory_Pool;
    //1 byte
private:
    bool active;
};

struct C_Enemy : public I_Component
{};

struct C_Player : public I_Component
{};

struct C_PlayerBullet : public I_Component
{};

struct C_Position : public I_Component
{
    //9 bytes
public:
    float x, y;
};

struct C_Rotation : public I_Component
{
public:
    float rotation;
};

struct C_Velocity : public I_Component
{
    //9 bytes
public:
    float x, y;
};

struct C_Bounding_Circle : public I_Component
{
    //5 bytes
public:
    float radius;
};

struct C_Texture : public I_Component
{
    //21 bytes
public:
    //20 bytes
    Texture2D texture;
};