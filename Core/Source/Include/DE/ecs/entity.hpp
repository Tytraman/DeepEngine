#ifndef __DEEP_ENGINE_ENTITY_HPP__
#define __DEEP_ENGINE_ENTITY_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/ecs/ecs.hpp"
#include "DE/memory/list.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/drivers/opengl/texture.hpp"

namespace deep
{

    class entity_manager;

    class DE_API entity
    {

        private:
            entity_collection_id  m_CollectionID;
            entity_id m_EntityID;

            friend entity_manager;

        public:
            static entity bad();


            bool isOK() const;

            entity_collection_id getCollectionID() const;
            entity_id getEntityID() const;

        protected:
            entity(entity_collection_id collectionID, entity_id entityID);

        public:
            entity() = delete;

    };

    using entity_enum_callback = void (*)(entity_collection_id collection, entity_id entity);

    /*
    ===========
    entity::bad
    ===========
    */
    inline entity entity::bad()
    {
        return entity(badID, badID);
    }

    /*
    ============
    entity::isOK
    ============
    */
    inline bool entity::isOK() const
    {
        return (m_CollectionID != badID && m_EntityID != badID);
    }

    /*
    =======================
    entity::getCollectionID
    =======================
    */
    inline scene_id entity::getCollectionID() const
    {
        return m_CollectionID;
    }

    /*
    ===================
    entity::getEntityID
    ===================
    */
    inline entity_id entity::getEntityID() const
    {
        return m_EntityID;
    }

    /// @struct entity
    /// @brief	Une entité peut contenir plusieurs composants et est représentée par un ID.
    struct entity_item
    {
        entity ent;
        list<component_id> components;      ///< Contient tous les composants que l'entité possède.
        component_type componentsType;      ///< Masque des types de composants attribués à l'entité.

        entity_item(const entity &ent);
    };

    /// @brief	Classe qui permet la gestion des entités, de leur création à leur destruction, en passant par leur modification etc.
    class entity_manager
    {

        public:
            DE_API static entity_manager *get_singleton();

            DE_API void enum_entities(entity_collection_id collection, entity_enum_callback callback);
            DE_API size_t get_number_of_entities(entity_collection_id collection);

            DE_API entity_collection_id create_entity_collection();

            /// @brief	Crée une entité.
            /// @return L'entité nouvellement créée.
            DE_API entity create_entity(entity_collection_id collection);

            DE_API entity create_entity(
                entity_collection_id collection,
                const polygon &pol,
                GL3::gl_id program,
                const fvec3 &position,
                const fvec3 &size,
                GL3::gl_id texture = GL3::texture_manager::get_singleton()->get_white_texture(),
                uint8_t textureUnit = 0);

            /// @brief		Supprime une entité de la liste.
            DE_API void destroyEntity(const entity &entity);
            DE_API void destroyEntity(entity_collection_id collection, entity_id entity);
            DE_API void destroyAllEntities(entity_collection_id collection);

            DE_API bool mustBeDeleted(entity_collection_id collection, entity_id entity);
            DE_API void deleteEntities();

            /// @brief				Attache un composant à une entité.
            /// @param entity		L'entité à laquelle attacher le composant.
            /// @return				\c true si le composant à bien été attaché.
            DE_API bool attachComponent(const entity &entity, component_id component);

            /// @brief							Cherche toutes les entités qui respectent les conditions d'inclusions et les ajoute dans la liste destination.
            /// @param collection				ID de la collection d'entités dans laquelle chercher.
            /// @param componentTypesToInclude	Masque des types de composants que les entités doivent posséder afin d'être incluses.
            /// @param componentTypesToExclude	Masque des types de composants que les entités ne doivent pas avoir pour être incluses.
            /// @param dest						Pointeur vers la liste qui stockera les entités répondant aux conditions.
            DE_API void query(entity_collection_id collection, component_type componentTypesToInclude, component_type componentTypesToExclude, list<entity_id> &dest);

            DE_API component_id getComponentID(entity_collection_id collection, entity_id entity, component_type componentType);
            DE_API component_id getComponentID(const entity &entity, component_type componentType);

        private:
            entity_manager();

            hash_table<hash_table<entity_item>> m_Collections;   ///< Les entités sont stockées des collections.
            hash_table<entity_id> m_NextEntityID;                ///< À chaque nouvelle entité créée, son ID sera celle de cette variable.
            hash_table<list<entity_id>> m_EntitiesToDelete;      ///< Contient la liste des entités à détruire pour chaque collection.

        private:
            hash_entry<entity_item> *__createEntity(entity_collection_id collection);

        public:
            entity_manager(const entity_manager &) = delete;
            entity_manager(const entity_manager &&) = delete;

    };

    /*
    =============================
    entity_manager::getComponentID
    =============================
    */
    inline component_id entity_manager::getComponentID(const entity &entity, component_type componentType)
    {
        return getComponentID(entity.m_CollectionID, entity.m_EntityID, componentType);
    }

}

#endif