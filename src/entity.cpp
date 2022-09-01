#include "entity.hpp"

#include "entity_memory_pool.hpp"

//---------------------------------------------------------
// private funcs

// default constructor *intentionally* not declared

const bool Entity::is_active() {
    return Entity_Memory_Pool::instance().is_entity_active(id);
}

//---------------------------------------------------------
// public funcs

const size_t Entity::get_id() { return this->id; }

const std::string& Entity::get_tag() {
    return Entity_Memory_Pool::instance().get_tag(id);
}
