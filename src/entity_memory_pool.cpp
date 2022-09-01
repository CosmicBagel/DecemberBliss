#include "entity_memory_pool.hpp"

#include "entity.hpp"
#include "ray_includes.hpp"

//---------------------------------------------------------
// private funcs

Entity_Memory_Pool::Entity_Memory_Pool(size_t max_entities) {
    num_entities = 0;
    last_added = max_entities - 1;  // so that we start at index 0 on first add
    pool = Entity_Component_Vector_Tuple(
        std::vector<C_Position>(max_entities),
        std::vector<C_Velocity>(max_entities),
        std::vector<C_Bounding_Circle>(max_entities),
        std::vector<C_Texture>(max_entities),
        std::vector<C_Enemy>(max_entities), std::vector<C_Player>(max_entities),
        std::vector<C_PlayerBullet>(max_entities),
        std::vector<C_Rotation>(max_entities));
    tags = std::vector<std::string>(max_entities);
    active = std::vector<bool>(max_entities);
};

size_t Entity_Memory_Pool::get_next_entity_index() {
    size_t ind = 0;
    bool slot_found = false;
    for (size_t i = 1; i < MAX_ENTITIES; i++) {
        ind = (i + last_added) % MAX_ENTITIES;
        if (!active[ind]) {
            slot_found = true;
            last_added = ind;
            break;
        }
    }

    if (!slot_found) {
        TraceLog(LOG_ERROR,
                 "NO MORE ENTITY SLOTS AVAILABLE! (Memory Pool Saturated)");
        // TODO: implement proper error handling
        exit(1);
    }

    return ind;
}

Entity_Memory_Pool& Entity_Memory_Pool::instance() {
    static Entity_Memory_Pool inst(MAX_ENTITIES);
    return inst;
}

//---------------------------------------------------------
// public funcs

const std::string& Entity_Memory_Pool::get_tag(size_t entity_id) const {
    return tags[entity_id];
}

Entity Entity_Memory_Pool::add_entity(const std::string& tag) {
    size_t index = get_next_entity_index();

    // TODO: find a way to zero out the components in a safe way
    // set all vectors[index] = 0

    tags[index] = tag;
    active[index] = true;
    return Entity(index);
}

void Entity_Memory_Pool::remove_entity(size_t entity_id) {
    active[entity_id] = false;
}

bool Entity_Memory_Pool::is_entity_active(size_t entity_id) {
    return active[entity_id];
}
