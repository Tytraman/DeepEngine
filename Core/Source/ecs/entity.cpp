#include "DE/ecs/entity.hpp"
#include "DE/ecs/component.hpp"

namespace deep
{

    static entity_collection_id m_NextCollectionID = 0;     ///< Prochain ID de collection à attribuer.

    /*
    ========================
    entity_item::entity_item
    ========================
    */
    entity_manager::entity_item::entity_item(const entity &_ent)
        : ent(_ent),
          componentsType(component_manager::component_type::none)
    { }

    /*
    ==============
    entity::entity
    ==============
    */
    entity_manager::entity::entity(const char *name, entity_collection_id collectionID, entity_id entityID)
        : m_CollectionID(collectionID),
          m_EntityID(entityID),
          m_Name(name)
    { }

    /*
    ==============================
    entity_manager::entity_manager
    ==============================
    */
    entity_manager::entity_manager()
    { }

    /*
    =============================
    entity_manager::get_singleton
    =============================
    */
    entity_manager *entity_manager::get_singleton()
    {
        static entity_manager singleton;

        return &singleton;
    }

    /*
    =============================
    entity_manager::enum_entities
    =============================
    */
    void entity_manager::enum_entities(entity_collection_id collection, entity_enum_callback callback)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collection];
        if(hs == nullptr)
            return;

        hash_table_iterator begin = hs->value.begin();
        hash_table_iterator end = hs->value.end();

        for(; begin != end; ++begin)
            callback(begin->value.ent);

        delete_entities();
    }

    /*
    ======================================
    entity_manager::get_number_of_entities
    ======================================
    */
    size_t entity_manager::get_number_of_entities(entity_collection_id collection)
    {
        const auto hs = m_Collections[collection];

        if(hs == nullptr)
            return 0;

        return hs->value.getNumberOfElements();
    }

    /*
    ========================================
    entity_manager::create_entity_collection
    ========================================
    */
    entity_collection_id entity_manager::create_entity_collection()
    {
        entity_collection_id id = m_NextCollectionID;
        entity_id entID = 0;

        hash_table<entity_item> ta = hash_table<entity_item>();

        m_NextEntityID.insert(id, entID);
        m_Collections.insert(id, rvalue_cast(ta));

        m_NextCollectionID++;

        return id;
    }

    /*
    =============================
    entity_manager::create_entity
    =============================
    */
    entity_manager::entity entity_manager::create_entity(const char *name, entity_collection_id collection)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collection];
        if(hs == nullptr)
            return entity::bad();

        hash_entry<entity_id> *el = m_NextEntityID[collection];
        if(el == nullptr)
            return entity::bad();

        el->value++;

        entity_id id = m_Collections.getHashFunction()(name);
        hash_entry<entity_item> &hti = hs->value.insert(id, entity_item(entity(name, collection, id)));

        return hti.value.ent;
    }

    /*
    =============================
    entity_manager::create_entity
    =============================
    */
    entity_manager::entity entity_manager::create_entity(
        const char *name,
        entity_collection_id collection,
        const polygon &pol,
        const vec3<float> &position,
        const vec3<float> &size,
        imaterial *material)
    {
        entity ent = create_entity(name, collection);
        if(!ent.is_ok())
            return ent;

        component_manager *componentManager = component_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(pol.vbo(), pol.vao(), material);
        component_id transformationComponentID = componentManager->create_transformation_component(position, size, 0.0f);

        drawable_component *drawableComponent = componentManager->get_drawable_component(drawableComponentID);
        drawableComponent->renderCallback = drawable_component::classic_render_callback;

        entity_manager::attach_component(name, ent.m_CollectionID, drawableComponentID);
        entity_manager::attach_component(name, ent.m_CollectionID, transformationComponentID);

        return ent;
    }

    /*
    ==============================
    entity_manager::destroy_entity
    ==============================
    */
    void entity_manager::destroy_entity(uint64_t keyName, entity_collection_id collectionID)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collectionID];
        if(hs == nullptr)
            return;

        // Libère la mémoire utilisée par la liste.
        entity_item &item = (*hs).value[keyName]->value;

        item.components.free();

        hs->value.remove(keyName);
    }

    /*
    =================================
    entity_manager::destroy_all_entities
    =================================
    */
    void entity_manager::destroy_all_entities(entity_collection_id collection)
    {
        const auto hs = m_Collections[collection];
        if(hs == nullptr)
            return;

        for(auto &el : (*hs).value)
        {
            el.value.components.free();
        }

        m_Collections.remove(collection);
    }

    /*
    ===============================
    entity_manager::must_be_deleted
    ===============================
    */
    bool entity_manager::must_be_deleted(uint64_t keyName, entity_collection_id collectionID)
    {
        hash_entry<list<entity_id>> *hs = m_EntitiesToDelete[collectionID];
        hash_entry<list<entity_id>> *ls;

        if(hs == nullptr)
        {
            ls = &m_EntitiesToDelete.insert(collectionID, list<entity_id>());
        }
        else
        {
            ls = hs;
        }

        if(!ls->value.add(keyName))
            return false;

        return true;
    }

    /*
    ===============================
    entity_manager::delete_entities
    ===============================
    */
    void entity_manager::delete_entities()
    {
        hash_table_iterator begin = m_EntitiesToDelete.begin();
        hash_table_iterator end = m_EntitiesToDelete.end();

        

        for(; begin != end; ++begin)
        {
            list_iterator<entity_id> lBegin = begin->value.begin();
            list_iterator<entity_id> lEnd = begin->value.end();

            for(; lBegin != lEnd; ++lBegin)
                destroy_entity(*lBegin, begin->key);

            begin->value.free();
        }
    }

    /*
    ================================
    entity_manager::attach_component
    ================================
    */
    bool entity_manager::attach_component(uint64_t keyName, entity_collection_id collectionID, component_id component)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collectionID];
        if(hs == nullptr)
            return false;

        entity_item &item = hs->value[keyName]->value;

        component_manager *componentManager = component_manager::get_singleton();

        component_manager::component_type type = componentManager->get_type(component);

        if((to_utype(item.componentsType) & to_utype(type)) == to_utype(type))
            return false;

        item.components.add(component);
        item.componentsType = static_cast<component_manager::component_type>(to_utype(item.componentsType) | to_utype(type));

        return true;
    }

    /*
    ================================
    entity_manager::get_component_id
    ================================
    */
    component_id entity_manager::get_component_id(uint64_t keyName, entity_collection_id collectionID, component_manager::component_type type)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collectionID];
        if(hs == nullptr)
            return badID;

        hash_entry<entity_item> *ent =  hs->value[keyName];
        if(ent == nullptr)
            return badID;

        component_manager *componentManager = component_manager::get_singleton();

        entity_item &item = ent->value;
        
        component_manager::component_type t;

        list_iterator<entity_id> it = item.components.begin();
        list_iterator<entity_id> end = item.components.end();

        component_id component;

        // Itère à travers tous les composants de l'entité pour trouver celui qui a le bon type.
        for(; it != end; ++it)
        {
            component = *it;

            t = componentManager->get_type(component);
            if(t == type)
                return component;
        }

        return badID;
    }

    /*
    ===================================
    entity_manager::get_component_types
    ===================================
    */
    component_manager::component_type entity_manager::get_component_types(uint64_t keyName, entity_collection_id collectionID)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collectionID];
        if(hs == nullptr)
            return component_manager::component_type::none;

        hash_entry<entity_item> *h = hs->value[keyName];
        if(h == nullptr)
            return component_manager::component_type::none;

        return h->value.componentsType;
    }

    /*
    =====================
    entity_manager::query
    =====================
    */
    void entity_manager::query(entity_collection_id collection, underlying_type<component_manager::component_type> componentTypesToInclude, underlying_type<component_manager::component_type> componentTypesToExclude, list<entity_id> &dest)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collection];
        if(hs == nullptr)
            return;
            
        hash_table_iterator<entity_item> begin = hs->value.begin();
        hash_table_iterator<entity_item> end = hs->value.end();

        for(; begin != end; ++begin)
        {
            entity_item &item = begin->value;
            if((to_utype(item.componentsType) & componentTypesToInclude) == componentTypesToInclude && (to_utype(item.componentsType) & componentTypesToExclude) == 0)
                dest.add(item.ent.get_entity_id());
        }
    }

}