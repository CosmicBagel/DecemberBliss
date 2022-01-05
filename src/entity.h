// this (and the rest of the ECS arch) is heavily based on Dave Churchill's
// lecture https://www.youtube.com/watch?v=nLk8K07fLxM I highly recommend 
// watching it

#pragma once
//#include "entity_manager.h"
class Entity_Manager; //forward declare workaround for awkward header inclusion

//#include "entity_memory_pool.h"
class Entity_Memory_Pool;

class Entity
{
    friend class Entity_Manager;
    friend class Entity_Memory_Pool;
private:
    size_t id;
    bool is_active();

    // only Entity_Memory_Pool and Entity_Manager are allowed to construct entities
    Entity(size_t id) : id(id) {};
    Entity();
public:
    template <typename T>
    T& get_component()
    {
        return Entity_Memory_Pool::instance().get_component<T>(id);
    }

    template <typename T>
    bool has_component()
    {
        return Entity_Memory_Pool::instance().has_component<T>(id);
    }

    template <typename T>
    T& add_component()
    {
        return Entity_Memory_Pool::instance().add_component<T>(id);
    }

    template <typename T>
    void remove_component()
    {
       Entity_Memory_Pool::instance().remove_component<T>(id);
    }
};
