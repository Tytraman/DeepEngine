#ifndef __DEEP_ENGINE_RESOURCES_HPP__
#define __DEEP_ENGINE_RESOURCES_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/rendering/opengl_utils.hpp>

namespace de
{

	class DE_API resources
    {

		public:
			static bool init();
			static void shutdown();

			static gl_texture_int loadTexture(const char *name, uint8_t unit);

		private:
			static wchar_t *m_DeepDataFolder;
			static wchar_t *m_DeepResourcesFolder;
			static wchar_t *m_DeepShadersFolder;
			static wchar_t *m_DeepTexturesFolder;
			static wchar_t *m_DeepSoundsFolder;

        public:
            resources() = delete;

	};

}


#endif