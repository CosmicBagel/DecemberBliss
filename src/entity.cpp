#include "entity.hpp"

#include "entity_memory_pool.hpp"

//---------------------------------------------------------
// private funcs

// default constructor *intentionally* not declared

bool Entity::is_active() const {
    return Entity_Memory_Pool::instance().is_entity_active(id);
}

//---------------------------------------------------------
// public funcs

size_t Entity::get_id() const { return this->id; }

const std::string& Entity::get_tag() const {
    return Entity_Memory_Pool::instance().get_tag(id);
}
