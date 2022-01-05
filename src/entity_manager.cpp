#include "entity_manager.h"
#include "entity_memory_pool.h"
#include "entity.h"
#include "dev_ui.h"
#include "perf_timer.h"

Entity_Manager::Entity_Manager()
{
    entities.reserve(MAX_ENTITIES);
    entities_to_add.reserve(MAX_ENTITIES);
}

Entity_Manager& Entity_Manager::instance()
{
    static Entity_Manager inst{};
    return inst;
}

std::vector<Entity>& Entity_Manager::get_entities()
{
    return entities;
}

std::vector<Entity>& Entity_Manager::get_entities(const std::string& tag)
{
    // TODO: return the actual tag array
    return entities;
}

Entity Entity_Manager::add_entity(const std::string& tag)
{
    Entity e = Entity_Memory_Pool::instance().add_entity(tag);
    entities_to_add.push_back(e);
    //TODO update the tag map
    return e;
}

void Entity_Manager::remove_entity(const Entity e)
{
    Entity_Memory_Pool::instance().remove_entity(e.id);
    //TODO update the tag map
}

//OPTI: this smells, but need to profile first
void Entity_Manager::update_manager()
{
    Perf_Timer t(Dev_UI::instance().metrics.ecs_bookkeeping_time);
    //TODO: figure out how to make removing work nicely 
    //TODO: This remove loop is ass, each erase is (I'm guessing) O(N)
    //      can probably get this down significantly

    //idea, have two entities vectors (A and B) that we flip flop between
    //then we just copy over elements that are active, and clear the old list
    //both keep a full memory reserved, so no mem alloc should be needed
    //adding should also be relatively painless
    //only do this when a dirty flag is raised (when remove_entity is called)

    //alternatively, we keep one list, clear if dirty, and loop over the full
    //memory pool of entities

    //ONLY IMPL IF CURRENT LOOP ACTUALLY PROFILES SLOWLY

    for (size_t i = entities.size(); i > 0; i--)
    {
        if (!entities[i - 1].is_active())
        {
            entities.erase(entities.begin() + i - 1);
        }
    }

    // add new entities
    while (!entities_to_add.empty())
    {
        entities.push_back(entities_to_add.back());
        entities_to_add.pop_back();
    }
}
