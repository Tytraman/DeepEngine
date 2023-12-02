#ifndef __DEEP_ENGINE_RESOURCES_HPP__
#define __DEEP_ENGINE_RESOURCES_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/rendering/opengl_utils.hpp>
#include <DE/string.hpp>

namespace de
{

	class DE_API resource_manager
    {

		public:
			static bool init(const char *filepath);
			static void shutdown();

			static texture_id loadTexture(const char *name, uint8_t unit);

		private:
			static string m_ResourcesFolder;
			static string m_ShadersFolder;
			static string m_TexturesFolder;
			static string m_SoundsFolder;

        public:
            resource_manager() = delete;
            resource_manager(const resource_manager &) = delete;
            resource_manager(resource_manager &&) = delete;

	};

}


#endif