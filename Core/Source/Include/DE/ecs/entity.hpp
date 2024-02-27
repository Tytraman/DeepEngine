#ifndef __DEEP_ENGINE_ENTITY_HPP__
#define __DEEP_ENGINE_ENTITY_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/ecs/ecs.hpp"
#include "DE/ecs/component.hpp"
#include "DE/memory/list.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/drivers/opengl/texture.hpp"

namespace deep
{

    /// @brief	Classe qui permet la gestion des entités, de leur création à leur destruction, en passant par leur modification etc.
    class entity_manager
    {

        public:
            class entity
            {

                public:
                    DE_API static entity bad();

                    DE_API bool is_ok() const;

                    DE_API entity_collection_id get_collection_id() const;
                    DE_API entity_id get_entity_id() const;
                    DE_API string &get_name();

                private:
                    entity_collection_id  m_CollectionID;
                    entity_id m_EntityID;
                    string m_Name;

                    friend entity_manager;

                protected:
                    entity(const char *name, entity_collection_id collectionID, entity_id entityID);

                public:
                    entity() = delete;

            };

            using entity_enum_callback = void (*)(entity &ent);

            /// @struct entity
            /// @brief	Une entité peut contenir plusieurs composants et est représentée par un ID.
            struct entity_item
            {
                entity ent;
                list<component_id> components;      ///< Contient tous les composants que l'entité possède.
                component_manager::component_type componentsType;      ///< Masque des types de composants attribués à l'entité.

                DE_API entity_item(const entity &ent);
            };

        public:
            DE_API static entity_manager *get_singleton();

            DE_API void enum_entities(entity_collection_id collection, entity_enum_callback callback);
            DE_API size_t get_number_of_entities(entity_collection_id collection);

            DE_API entity_collection_id create_entity_collection();


            DE_API entity create_entity(const char *name, entity_collection_id collection);
            DE_API entity create_entity(
                const char *name,
                entity_collection_id collection,
                const polygon &pol,
                const vec3<float> &position,
                const vec3<float> &size,
                material *material = nullptr);

            DE_API void destroy_entity(uint64_t keyName, entity_collection_id collectionID);
            DE_API void destroy_entity(const char *entityName, entity_collection_id collectionID);
            DE_API void destroy_entity(const entity &entity);
            DE_API void destroy_all_entities(entity_collection_id collection);

            DE_API bool must_be_deleted(uint64_t keyName, entity_collection_id collectionID);
            DE_API bool must_be_deleted(const char *entityName, entity_collection_id collectionID);

            DE_API void delete_entities();

            DE_API bool attach_component(uint64_t keyName, entity_collection_id collectionID, component_id component);
            DE_API bool attach_component(const char *entityName, entity_collection_id collectionID, component_id component);

            /// @brief							Cherche toutes les entités qui respectent les conditions d'inclusions et les ajoute dans la liste destination.
            /// @param collection				ID de la collection d'entités dans laquelle chercher.
            /// @param componentTypesToInclude	Masque des types de composants que les entités doivent posséder afin d'être incluses.
            /// @param componentTypesToExclude	Masque des types de composants que les entités ne doivent pas avoir pour être incluses.
            /// @param dest						Pointeur vers la liste qui stockera les entités répondant aux conditions.
            DE_API void query(entity_collection_id collection, underlying_type<component_manager::component_type> componentTypesToInclude, underlying_type<component_manager::component_type> componentTypesToExclude, list<entity_id> &dest);

            DE_API component_id get_component_id(uint64_t keyName, entity_collection_id collectionID, component_manager::component_type type);
            DE_API component_id get_component_id(const char *entityName, entity_collection_id collection, component_manager::component_type componentType);

            DE_API component_manager::component_type get_component_types(uint64_t keyName, entity_collection_id collectionID);

        private:
            entity_manager();

            hash_table<hash_table<entity_item>> m_Collections;   ///< Les entités sont stockées des collections.
            hash_table<entity_id> m_NextEntityID;                ///< À chaque nouvelle entité créée, son ID sera celle de cette variable.
            hash_table<list<entity_id>> m_EntitiesToDelete;      ///< Contient la liste des entités à détruire pour chaque collection.

        public:
            entity_manager(const entity_manager &) = delete;
            entity_manager(const entity_manager &&) = delete;

    };

    

    /*
    ==============================
    entity_manager::destroy_entity
    ==============================
    */
    inline void entity_manager::destroy_entity(const char *entityName, entity_collection_id collectionID)
    {
        destroy_entity(m_Collections.getHashFunction()(entityName), collectionID);
    }

    /*
    ==============================
    entity_manager::destroy_entity
    ==============================
    */
    inline void entity_manager::destroy_entity(const entity &entity)
    {
        destroy_entity(entity.m_EntityID, entity.m_CollectionID);
    }

    /*
    ===============================
    entity_manager::must_be_deleted
    ===============================
    */
    inline bool entity_manager::must_be_deleted(const char *entityName, entity_collection_id collectionID)
    {
        return must_be_deleted(m_EntitiesToDelete.getHashFunction()(entityName), collectionID);
    }

    /*
    ================================
    entity_manager::get_component_id
    ================================
    */
    inline component_id entity_manager::get_component_id(const char *entityName, entity_collection_id collection, component_manager::component_type componentType)
    {
        return get_component_id(m_Collections.getHashFunction()(entityName), collection, componentType);
    }

    /*
    ================================
    entity_manager::attach_component
    ================================
    */
    inline bool entity_manager::attach_component(const char *entityName, entity_collection_id collectionID, component_id component)
    {
        return attach_component(m_Collections.getHashFunction()(entityName), collectionID, component);
    }

    /*
    ===========================
    entity_manager::entity::bad
    ===========================
    */
    inline entity_manager::entity entity_manager::entity::bad()
    {
        return entity(nullptr, badID, badID);
    }

    /*
    =============================
    entity_manager::entity::is_ok
    =============================
    */
    inline bool entity_manager::entity::is_ok() const
    {
        return m_EntityID != badID && m_CollectionID != badID;
    }

    /*
    =========================================
    entity_manager::entity::get_collection_id
    =========================================
    */
    inline scene_id entity_manager::entity::get_collection_id() const
    {
        return m_CollectionID;
    }

    /*
    =====================================
    entity_manager::entity::get_entity_id
    =====================================
    */
    inline entity_id entity_manager::entity::get_entity_id() const
    {
        return m_EntityID;
    }

    inline string &entity_manager::entity::get_name()
    {
        return m_Name;
    }

}

#endif