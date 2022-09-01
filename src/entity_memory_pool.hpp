// this (and the rest of the ECS arch) is heavily based on Dave Churchill's
// lecture https://www.youtube.com/watch?v=nLk8K07fLxM I highly recommend
// watching it

#pragma once
#include <string>
#include <tuple>
#include <vector>

#include "components.hpp"

class Entity;

typedef std::tuple<std::vector<C_Position>, std::vector<C_Velocity>,
                   std::vector<C_Bounding_Circle>, std::vector<C_Texture>,
                   std::vector<C_Enemy>, std::vector<C_Player>,
                   std::vector<C_PlayerBullet>, std::vector<C_Rotation>>
    Entity_Component_Vector_Tuple;

// const int MAX_ENTITIES = 10'000;
const int MAX_ENTITIES = 500;

class Entity_Memory_Pool {
   private:
    size_t num_entities;
    size_t last_added;
    Entity_Component_Vector_Tuple pool;
    std::vector<std::string> tags;
    std::vector<bool> active;

    size_t get_next_entity_index();
    Entity_Memory_Pool(size_t max_entities);

   public:
    static Entity_Memory_Pool& instance();

    // each entity gets 1 tag
    const std::string& get_tag(size_t entity_id) const;

    Entity add_entity(const std::string& tag);

    void remove_entity(size_t entity_id);
    bool is_entity_active(size_t entity_id);

    template <typename T>
    T& get_component(size_t entity_id) {
        // compile time sanity check
        static_assert(std::is_base_of<I_Component, T>::value,
                      "type parameter must be a component (I_Component)");

        return std::get<std::vector<T>>(pool)[entity_id];
    }

    template <typename T>
    bool has_component(size_t entity_id) {
        // compile time sanity check
        static_assert(std::is_base_of<I_Component, T>::value,
                      "type parameter must be a component (I_Component)");

        return std::get<std::vector<T>>(pool)[entity_id].active;
    }

    template <typename T>
    T& add_component(size_t entity_id) {
        // compile time sanity check
        static_assert(std::is_base_of<I_Component, T>::value,
                      "type parameter must be a component (I_Component)");

        std::get<std::vector<T>>(pool)[entity_id].active = true;

        return std::get<std::vector<T>>(pool)[entity_id];
    }

    template <typename T>
    void remove_component(size_t entity_id) {
        // compile time sanity check
        static_assert(std::is_base_of<I_Component, T>::value,
                      "type parameter must be a component (I_Component)");

        std::get<std::vector<T>>(pool)[entity_id].active = false;
    }
};
