// this (and the rest of the ECS arch) is heavily based on Dave Churchill's
// lecture https://www.youtube.com/watch?v=nLk8K07fLxM I highly recommend
// watching it

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
class Entity;

class Entity_Manager {
   private:
    std::vector<Entity> entities;
    std::vector<Entity> entities_to_add;
    std::unordered_map<std::string, std::vector<Entity>> entity_map;
    Entity_Manager();

   public:
    static Entity_Manager& instance();
    std::vector<Entity>& get_entities();
    std::vector<Entity>& get_entities(const std::string& tag);
    Entity add_entity(const std::string& tag);
    static void remove_entity(Entity entity);
    void update_manager();
};