#ifndef __DEEP_ENGINE_RESOURCES_HPP__
#define __DEEP_ENGINE_RESOURCES_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>

namespace de {

	class DE_API Resources {

		public:
			static bool init();
			static void shutdown();


		private:
			Resources() = delete;

			static wchar_t *m_DeepDataFolder;
			static wchar_t *m_DeepResourcesFolder;
			static wchar_t *m_DeepShadersFolder;
			static wchar_t *m_DeepTexturesFolder;
			static wchar_t *m_DeepSoundsFolder;

	};

}


#endif