#ifndef __DEEP_ENGINE_AUDIO_HPP__
#define __DEEP_ENGINE_AUDIO_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/vec.hpp>

#include <AL/al.h>
#include <AL/alc.h>

namespace de {

	class AudioBuffer;
	class AudioSource;
	class AudioListener;
	class AudioDevice;

	constexpr uint32_t RiffChunkID    = (('F' << 24) | ('F' << 16) | ('I' << 8) | ('R'));    ///< ID qu'un fichier RIFF doit posséder au tout début du premier chunk.
	constexpr uint32_t WaveChunkID    = (('E' << 24) | ('V' << 16) | ('A' << 8) | ('W'));    ///< ID du format WAVE qu'un fichier RIFF peut contenir.
	constexpr uint32_t FmtSubChunkID  = ((' ' << 24) | ('t' << 16) | ('m' << 8) | ('f'));    ///< ID du sous chunk "fmt" qu'un fichier WAVE doit contenir.
	constexpr uint32_t DataSubChunkID = (('a' << 24) | ('t' << 16) | ('a' << 8) | ('d'));    ///< ID du sous chunk "data" qu'un fichier WAVE doit contenir.
	constexpr uint32_t ListSubChunkID = (('T' << 24) | ('S' << 16) | ('I' << 8) | ('L'));    ///< ID du sous chunk "LIST" qu'un fichier WAVE peut contenir.
	constexpr uint32_t InfoSubChunkID = (('O' << 24) | ('F' << 16) | ('N' << 8) | ('I'));    ///< ID du sous chunk "INFO" qu'un fichier WAVE peut contenir.

	
	DE_PACK(
		/// @brief Structure décrivant l'en-tête d'un fichier RIFF.
		struct DE_API RiffMasterHeader {
		uint32_t chunkID;    ///< ID du chunk, doit être égale à \ref RiffChunkID.
		uint32_t chunkSize;  ///< La taille du chunk.
		uint32_t formatID;   ///< L'ID du format que le RIFF englobe.
	});

	DE_PACK(struct DE_API WaveFmtChunk {
		uint16_t audioFormat;
		uint16_t channelsNumber;
		uint32_t sampleRate;
		uint32_t byteRate;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
		uint32_t nextChunkID;
		uint32_t nextChunkSize;
	});

	DE_PACK(struct DE_API WaveListChunk {
		uint32_t typeID;
	});

	/*
			Sources pour développer le parseur de fichier WAVE:
			- https://www.recordingblogs.com/wiki/list-chunk-of-a-wave-file
			- https://www.videoproc.com/resource/wav-file.htm
			- https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
	
	*/

	DE_PACK(
		/// @brief Structure décrivant l'en-tête d'un fichier WAVE.
		struct DE_API WaveHeader {
		RiffMasterHeader riffHeader;   ///< En-tête RIFF du fichier.
		uint32_t subChunkID;           ///< ID du sous chunk numéro 1, doit être égale à \ref FmtSubChunkID.
		uint32_t subChunkSize;         ///< Taille du sous chunk numéro 1.
	});

	

	class DE_API AudioBuffer {

		public:
			AudioBuffer();

			bool create(const char *filename);
			void destroy();

		private:
			ALuint m_Buffer;

			friend AudioSource;
			friend AudioListener;
			friend AudioDevice;

	};

	class DE_API AudioSource {

		public:
			AudioSource();

			bool create();
			bool attachBuffer(const AudioBuffer &buffer);
			bool play();
			void destroy();

		private:
			ALuint m_Source;

			friend AudioBuffer;
			friend AudioListener;
			friend AudioDevice;

	};

	class DE_API AudioListener {

		public:
			bool setGain(float value);
			bool setPosition(const fvec3 &pos);
			bool setVelocity(const fvec3 &vel);
			// bool setOrientation();

		private:

			friend AudioBuffer;
			friend AudioSource;
			friend AudioDevice;

	};

	class DE_API AudioDevice {

		public:
			static bool init();
			static void shutdown();

			static AudioListener &getListener();

		private:
			static ALCdevice     *m_ALCDevice;
			static ALCcontext    *m_ALCContext;
			static AudioListener  m_Listener;

			AudioDevice() = delete;

			friend AudioBuffer;
			friend AudioListener;
			friend AudioSource;

	};

	inline AudioListener &AudioDevice::getListener()
	{
		return m_Listener;
	}

}


#endif