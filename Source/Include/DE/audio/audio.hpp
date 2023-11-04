#ifndef __DEEP_ENGINE_AUDIO_HPP__
#define __DEEP_ENGINE_AUDIO_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/vec.hpp>

#include <AL/al.h>
#include <AL/alc.h>

namespace de
{

	class audio_buffer;
	class audio_source;
	class audio_listener;
	class audio_device;

	constexpr uint32_t RiffChunkID    = (('F' << 24) | ('F' << 16) | ('I' << 8) | ('R'));    ///< ID qu'un fichier RIFF doit posséder au tout début du premier chunk.
	constexpr uint32_t WaveChunkID    = (('E' << 24) | ('V' << 16) | ('A' << 8) | ('W'));    ///< ID du format WAVE qu'un fichier RIFF peut contenir.
	constexpr uint32_t FmtSubChunkID  = ((' ' << 24) | ('t' << 16) | ('m' << 8) | ('f'));    ///< ID du sous chunk "fmt" qu'un fichier WAVE doit contenir.
	constexpr uint32_t DataSubChunkID = (('a' << 24) | ('t' << 16) | ('a' << 8) | ('d'));    ///< ID du sous chunk "data" qu'un fichier WAVE doit contenir.
	constexpr uint32_t ListSubChunkID = (('T' << 24) | ('S' << 16) | ('I' << 8) | ('L'));    ///< ID du sous chunk "LIST" qu'un fichier WAVE peut contenir.
	constexpr uint32_t InfoSubChunkID = (('O' << 24) | ('F' << 16) | ('N' << 8) | ('I'));    ///< ID du sous chunk "INFO" qu'un fichier WAVE peut contenir.

	constexpr uint16_t WaveFormatPCM        = 0x0001;
	constexpr uint16_t WaveFormatIEEEFloat  = 0x0003;
	constexpr uint16_t WaveFormatAlaw       = 0x0006;
	constexpr uint16_t WaveFormatMulaw      = 0x0007;
	constexpr uint16_t WaveFormatExtensible = 0xFFFE;

	
	DE_PACK(
		/// @brief Structure décrivant l'en-tête d'un fichier RIFF.
        struct DE_API riff_master_header
        {
		    uint32_t chunkID;    ///< ID du chunk, doit être égale à \ref RiffChunkID.
		    uint32_t chunkSize;  ///< La taille du chunk.
		    uint32_t formatID;   ///< L'ID du format que le RIFF englobe.
	    }
	);

	DE_PACK(
        struct DE_API wave_fmt_chunk
        {
		    uint16_t audioFormat;
		    uint16_t channelsNumber;
		    uint32_t sampleRate;
		    uint32_t byteRate;
		    uint16_t blockAlign;
		    uint16_t bitsPerSample;
	    }
    );

	DE_PACK(
        struct DE_API wave_fmt_extend_chunk
        {
		    wave_fmt_chunk fmtStandardChunk;
		    uint16_t extensionSize;
		    uint16_t validBitsPerSample;
		    uint32_t channelMask;
		    uint8_t guid[16];
	    }
    );

    

	DE_PACK(
        struct DE_API wave_list_chunk
        {
		    uint32_t typeID;
		    uint32_t size;
	    }
    );

	/*
			Sources pour développer le parseur de fichier WAVE:
			- https://www.recordingblogs.com/wiki/list-chunk-of-a-wave-file
			- https://www.videoproc.com/resource/wav-file.htm
			- https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
	
	*/

	DE_PACK(
		/// @brief Structure décrivant l'en-tête d'un fichier WAVE.
        struct DE_API wave_header
        {
		    riff_master_header riffHeader;   ///< En-tête RIFF du fichier.
		    uint32_t subChunkID;           ///< ID du sous chunk numéro 1, doit être égale à \ref FmtSubChunkID.
		    uint32_t subChunkSize;         ///< Taille du sous chunk numéro 1.
	    }
	);

	class DE_API audio_buffer
    {

		public:
			audio_buffer();

			bool create(const char *filename);
			void destroy();

			int getFrequency() const;
			int getBits() const;
			int getChannels() const;
			int getSize() const;

		private:
			ALuint m_Buffer;

			friend audio_source;
			friend audio_listener;
			friend audio_device;

	};

	class DE_API audio_source
    {

		public:
			audio_source();

			bool create();
			bool attachBuffer(const audio_buffer &buffer);
			bool play();
			void destroy();

		private:
			ALuint m_Source;

			friend audio_buffer;
			friend audio_listener;
			friend audio_device;

	};

	class DE_API audio_listener
    {

		public:
			bool setGain(float value);
			bool setPosition(const fvec3 &pos);
			bool setVelocity(const fvec3 &vel);
			// bool setOrientation();

			float getGain() const;
			fvec3 getPosition() const;
			fvec3 getVelocity() const;

		private:

			friend audio_buffer;
			friend audio_source;
			friend audio_device;

	};

	class DE_API audio_device
    {

		public:
			static bool init();
			static void shutdown();

			static audio_listener &getListener();

		private:
			static ALCdevice     *m_ALCDevice;
			static ALCcontext    *m_ALCContext;
			static audio_listener  m_Listener;

			friend audio_buffer;
			friend audio_listener;
			friend audio_source;

        public:
            audio_device() = delete;

	};

	inline audio_listener &audio_device::getListener()
	{
		return m_Listener;
	}

}


#endif