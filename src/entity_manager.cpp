#include "entity_manager.h"
#include "entity_memory_pool.h"
#include "entity.h"
#include "dev_ui.h"
#include "perf_timer.h"
#include <unordered_map>

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
    return entity_map[tag];
}

Entity Entity_Manager::add_entity(const std::string& tag)
{
    Entity e = Entity_Memory_Pool::instance().add_entity(tag);
    entities_to_add.push_back(e);

    //update the tag map
    entity_map[tag].push_back(e);
    return e;
}

void Entity_Manager::remove_entity(const Entity e)
{
    Entity_Memory_Pool::instance().remove_entity(e.id);
}

//OPTI: this smells, but need to profile first
void Entity_Manager::update_manager()
{
    Perf_Timer t(Dev_UI::instance().metrics.ecs_bookkeeping_time);

    // add new entities
    while (!entities_to_add.empty())
    {
        entities.push_back(entities_to_add.back());
        entities_to_add.pop_back();
    }

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
    //bug fix: removal happens after adding so that if an entity is created
    //and then removed in the same frame, it will be properly removed
    //before with the remove check before the add, entities would be attempted 
    //to be removed BEFORE they were added (during update)
    for (size_t i = entities.size(); i > 0; i--)
    {
        size_t e_ind = i - 1;
        if (!entities[e_ind].is_active())
        {
            auto e_id = entities[e_ind].id;
            //update the tag map the entity belongs to
            std::string tag = Entity_Memory_Pool::instance().get_tag(e_ind);
            std::vector<Entity>& e_tag_vec = entity_map[tag];

            size_t tag_ind = 0;
            bool e_found = false;
            for (size_t tag_i = e_tag_vec.size(); tag_i > 0; tag_i--)
            {
                if (e_tag_vec[tag_i-1].id == e_id) 
                {
                    tag_ind = tag_i-1;
                    e_found = true;
                    break;
                }
            }

            if (!e_found)
            {
                TraceLog(LOG_WARNING, "Unable to find entity in its tag vector to remove it");
            }
            else
            {
                e_tag_vec.erase(e_tag_vec.begin() + tag_ind);
                if (e_tag_vec.size() == 0) 
                {
                    entity_map.erase(tag);
                }
            }

            //update the main entities vector
            entities.erase(entities.begin() + e_ind);
        }
    }
}
