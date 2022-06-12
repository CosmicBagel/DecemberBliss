#include "entity.h"
#include "entity_memory_pool.h"

//---------------------------------------------------------
// private funcs

//default constructor *intentionally* not declared

bool Entity::is_active() 
{
    return Entity_Memory_Pool::instance().is_entity_active(id);
}
    
//---------------------------------------------------------
// public funcs


size_t Entity::get_id()
{
	return this->id;
}
