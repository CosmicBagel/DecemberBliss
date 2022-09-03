#include "entity_manager.hpp"

#include <unordered_map>

#include "dev_ui.hpp"
#include "entity.hpp"
#include "entity_memory_pool.hpp"
#include "perf_timer.hpp"

Entity_Manager::Entity_Manager() {
    entities.reserve(MAX_ENTITIES);
    entities_to_add.reserve(MAX_ENTITIES);
}

Entity_Manager& Entity_Manager::instance() {
    static Entity_Manager inst{};
    return inst;
}

std::vector<Entity>& Entity_Manager::get_entities() { return entities; }

std::vector<Entity>& Entity_Manager::get_entities(const std::string& tag) {
    return entity_map[tag];
}

Entity Entity_Manager::add_entity(const std::string& tag) {
    Entity entity = Entity_Memory_Pool::instance().add_entity(tag);
    entities_to_add.push_back(entity);

    return entity;
}

void Entity_Manager::remove_entity(Entity entity) {
    // we need to disable all possible components
    entity.remove_component<C_Position>();
    entity.remove_component<C_Rotation>();
    entity.remove_component<C_Velocity>();
    entity.remove_component<C_Bounding_Circle>();
    entity.remove_component<C_Texture>();
    entity.remove_component<C_PlayerBullet>();
    entity.remove_component<C_Enemy>();
    entity.remove_component<C_Player>();

    Entity_Memory_Pool::instance().remove_entity(entity.id);
}

// OPTI (CosmicBagel): this smells, but need to profile first
void Entity_Manager::update_manager() {
    Perf_Timer function_perf_timer(Metrics::instance().ecs_bookkeeping_time);

    // add new entities
    while (!entities_to_add.empty()) {
        auto& entity = entities_to_add.back();
        entities.push_back(entity);

        const auto& tag = entity.get_tag();
        entity_map[tag].push_back(entity);

        entities_to_add.pop_back();
    }

    // TODO (CosmicBagel): figure out how to make removing work nicely
    // TODO (CosmicBagel): This remove loop is ass, each erase is (I'm guessing)
    // O(N)
    //       can probably get this down significantly

    // idea, have two entities vectors (A and B) that we flip flop between
    // then we just copy over elements that are active, and clear the old list
    // both keep a full memory reserved, so no mem alloc should be needed
    // adding should also be relatively painless
    // only do this when a dirty flag is raised (when remove_entity is called)

    // alternatively, we keep one list, clear if dirty, and loop over the full
    // memory pool of entities

    // ONLY IMPL IF CURRENT LOOP ACTUALLY PROFILES SLOWLY
    // bug fix: removal happens after adding so that if an entity is created
    // and then removed in the same frame, it will be properly removed
    // before with the remove check before the add, entities would be attempted
    // to be removed BEFORE they were added (during update)
    for (size_t i = entities.size(); i > 0; i--) {
        size_t e_ind = i - 1;
        if (!entities[e_ind].is_active()) {
            auto e_id = entities[e_ind].id;
            // update the tag map the entity belongs to
            std::string tag = Entity_Memory_Pool::instance().get_tag(e_id);
            std::vector<Entity>& e_tag_vec = entity_map[tag];

            size_t tag_ind = 0;
            bool e_found = false;
            for (size_t tag_i = e_tag_vec.size(); tag_i > 0; tag_i--) {
                if (e_tag_vec[tag_i - 1].id == e_id) {
                    tag_ind = tag_i - 1;
                    e_found = true;
                    break;
                }
            }

            if (!e_found) {
                TraceLog(
                    LOG_WARNING,
                    "Unable to find entity in its tag vector to remove it");
            } else {
                e_tag_vec.erase(e_tag_vec.begin() +
                                static_cast<int64_t>(tag_ind));
                if (e_tag_vec.empty()) {
                    entity_map.erase(tag);
                }
            }

            // update the main entities vector
            entities.erase(entities.begin() + static_cast<int64_t>(e_ind));
        }
    }
}
