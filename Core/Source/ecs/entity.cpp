#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

namespace deep
{

    static entity_collection_id m_NextCollectionID = 0;     ///< Prochain ID de collection à attribuer.

    /*
    ========================
    entity_item::entity_item
    ========================
    */
    entity_item::entity_item(const entity &_ent)
        : ent(_ent),
          componentsType(0)
    { }

    /*
    ==============
    entity::entity
    ==============
    */
    entity::entity(entity_collection_id collectionID, entity_id entityID)
        : m_CollectionID(collectionID),
          m_EntityID(entityID)
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
    ============================
    entity_manager::enumEntities
    ============================
    */
    void entity_manager::enumEntities(entity_collection_id collection, entity_enum_callback callback)
    {
        const auto hs = m_Collections[collection];
        if(hs == nullptr)
            return;

        for(auto &el : (*hs).value)
            callback(collection, el.value.ent.m_EntityID);

        deleteEntities();
    }

    /*
    ===================================
    entity_manager::getNumberOfEntities
    ===================================
    */
    size_t entity_manager::getNumberOfEntities(entity_collection_id collection)
    {
        const auto hs = m_Collections[collection];

        if(hs == nullptr)
            return 0;

        return hs->value.getNumberOfElements();
    }

    /*
    =====================================
    entity_manager::createEntityCollection
    =====================================
    */
    entity_collection_id entity_manager::createEntityCollection()
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
    ==============================
    entity_manager::__createEntity
    ==============================
    */
    hash_entry<entity_item> *entity_manager::__createEntity(entity_collection_id collection)
    {
        hash_entry<hash_table<entity_item>> *hs = m_Collections[collection];
        if(hs == nullptr)
            return nullptr;

        hash_entry<entity_id> *el = m_NextEntityID[collection];
        if(el == nullptr)
            return nullptr;

        entity_id di = el->value;
        el->value++;

        hash_entry<entity_item> &hti = hs->value.insert(di, entity_item(entity(collection, di)));

        return &hti;
    }

    /*
    ============================
    entity_manager::createEntity
    ============================
    */
    entity entity_manager::createEntity(entity_collection_id collection)
    {
        hash_entry<entity_item> *hti = __createEntity(collection);
        if(hti == nullptr)
            return entity::bad();

        return hti->value.ent;
    }

    /*
    ============================
    entity_manager::createEntity
    ============================
    */
    entity entity_manager::createEntity(
        entity_collection_id collection,
        const polygon &pol,
        GL3::gl_id program,
        const fvec3 &position,
        const fvec3 &size,
        GL3::gl_id texture,
        uint8_t textureUnit)
    {
        component_manager *componentManager = component_manager::get_singleton();

        hash_entry<entity_item> *hti = __createEntity(collection);
        if(hti == nullptr)
            return entity::bad();

        component_id drawableComponentID       = componentManager->createDrawableComponent(program, pol.vbo(), pol.vao());
        component_id transformationComponentID = componentManager->createTransformationComponent(position, size, 0.0f);

        drawable_component *drawableComponent = componentManager->getDrawableComponent(drawableComponentID);
        drawableComponent->texture = texture;
        drawableComponent->textureUnit = textureUnit;
        drawableComponent->renderCallback = drawable_component::classicRenderCallback;

        entity_manager::attachComponent(hti->value.ent, drawableComponentID);
        entity_manager::attachComponent(hti->value.ent, transformationComponentID);

        return hti->value.ent;
    }

    /*
    =============================
    entity_manager::destroyEntity
    =============================
    */
    void entity_manager::destroyEntity(const entity &entity)
    {
        const auto hs = m_Collections[entity.m_CollectionID];
        if(hs == nullptr)
            return;

        // Libère la mémoire utilisée par la liste.
        entity_item &item = (*hs).value[entity.m_EntityID]->value;

        item.components.free();

        hs->value.remove(entity.m_EntityID);
    }

    /*
    ============================
    entity_manager::destroyEntity
    ============================
    */
    void entity_manager::destroyEntity(entity_collection_id collection, entity_id entity)
    {
        const auto hs = m_Collections[collection];
        if(hs == nullptr)
            return;

        entity_item &item = (*hs).value[entity]->value;

        item.components.free();
        hs->value.remove(entity);
    }

    /*
    =================================
    entity_manager::destroyAllEntities
    =================================
    */
    void entity_manager::destroyAllEntities(entity_collection_id collection)
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
    ============================
    entity_manager::mustBeDeleted
    ============================
    */
    bool entity_manager::mustBeDeleted(entity_collection_id collection, entity_id entity)
    {
        hash_entry<list<entity_id>> *hs = m_EntitiesToDelete[collection];
        hash_entry<list<entity_id>> *ls;

        if(hs == nullptr)
        {
            ls = &m_EntitiesToDelete.insert(collection, list<entity_id>());
        }
        else
        {
            ls = hs;
        }

        if(!ls->value.add(entity))
            return false;

        return true;
    }

    /*
    =============================
    entity_manager::deleteEntities
    =============================
    */
    void entity_manager::deleteEntities()
    {
        for(auto &el : m_EntitiesToDelete)
        {
            for(auto &le : el.value)
                destroyEntity(static_cast<entity_collection_id>(el.key), le);

            el.value.free();
        }
    }

    /*
    ==============================
    entity_manager::attachComponent
    ==============================
    */
    bool entity_manager::attachComponent(const entity &entity, component_id component)
    {
        const auto hs = m_Collections[entity.m_CollectionID];
        if(hs == nullptr)
            return false;

        entity_item &item = hs->value[entity.m_EntityID]->value;

        component_manager *componentManager = component_manager::get_singleton();

        component_type type = componentManager->getType(component);

        // Vérifie que l'entité ne possède pas déjà un composant du même type.
        if((item.componentsType & type) == type)
            return false;

        item.components.add(component);
        item.componentsType |= type;

        return true;
    }

    /*
    =============================
    entity_manager::getComponentID
    =============================
    */
    component_id entity_manager::getComponentID(entity_collection_id collection, entity_id entity, component_type componentType)
    {
        const auto hs = m_Collections[collection];
        if(hs == nullptr)
            return badID;

        component_manager *componentManager = component_manager::get_singleton();

        entity_item &item = hs->value[entity]->value;
        
        component_type type;

        auto it = item.components.begin();
        auto end = item.components.end();

        component_id component;

        // Itère à travers tous les composants de l'entité pour trouver celui qui a le bon type.
        for(; it != end; ++it)
        {
            component = *it;

            type = componentManager->getType(component);
            if(type == componentType)
                return component;
        }

        return badID;
    }

    /*
    ====================
    entity_manager::query
    ====================
    */
    void entity_manager::query(entity_collection_id collection, component_type componentTypesToInclude, component_type componentTypesToExclude, list<entity_id> &dest)
    {
        const auto hs = m_Collections[collection];
        if(hs == nullptr)
            return;
            
        for(auto &el : hs->value)
        {
            entity_item &item = el.value;
            if((item.componentsType & componentTypesToInclude) == componentTypesToInclude && (item.componentsType & componentTypesToExclude) == 0)
                dest.add(item.ent.getEntityID());
        }
    }

}