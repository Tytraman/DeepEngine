#include <DE/audio/audio.hpp>
#include <DE/stream.hpp>

#include <stdio.h>

namespace de {

	ALCdevice  *AudioDevice::m_ALCDevice  = nullptr;
	ALCcontext *AudioDevice::m_ALCContext = nullptr;
	AudioListener AudioDevice::m_Listener;

	/*
	========================
	AudioBuffer::AudioBuffer
	========================
	*/
	AudioBuffer::AudioBuffer()
		: m_Buffer(0)
	{ }

	/*
	===================
	AudioBuffer::create
	===================
	*/
	bool AudioBuffer::create(const char *filename)
	{
		InputFileStream s(filename);

		if(!s.open())
			return false;

		size_t filesize = s.getFileSize();
		if(filesize == 0)
			return false;

		// L'offset 20 est le début des données du chunk "fmt".
		size_t offset = sizeof(WaveHeader);

		WaveHeader *soundBuffer = (WaveHeader *) mem::allocNoTrack(filesize);
		size_t bytesRead;

		if(!s.readAll(soundBuffer, &bytesRead)) {
			s.close();
			mem::free(soundBuffer);
			return false;
		}

		s.close();

		// Vérifie que le fichier chargé soit bien dans le format WAVE.
		if(soundBuffer->riffHeader.chunkID != RiffChunkID || soundBuffer->riffHeader.formatID != WaveChunkID || soundBuffer->subChunkID != FmtSubChunkID) {
			mem::free(soundBuffer);
			return false;
		}

		WaveFmtChunk *fmtChunk = (WaveFmtChunk *) ((uint8_t *) soundBuffer + offset);

		offset += soundBuffer->subChunkSize;

		if(*((uint32_t *) ((uint8_t *) soundBuffer + offset)) == ListSubChunkID) {
			WaveListChunk *listChunk = (WaveListChunk *) ((uint8_t *) soundBuffer + offset);
			offset += listChunk->size + 8;
			if(listChunk->typeID == InfoSubChunkID) {
				
			}
		}

		if(*((uint32_t *) ((uint8_t *) soundBuffer + offset)) != DataSubChunkID) {
			mem::free(soundBuffer);
			return false;
		}

		offset += 4;

		ALsizei size = *((uint32_t *) ((uint8_t *) soundBuffer + offset));

		offset += 4;

		// Nettoie le code d'erreur d'OpenAL.
		alGetError();

		// Génère un buffer qui permettra de stocker par la suite les données du fichier WAV
		alGenBuffers(1, &m_Buffer);
		if(alGetError() != AL_NO_ERROR) {
			mem::free(soundBuffer);
			return false;
		}

		int alFormat;

		if(fmtChunk->channelsNumber == 1) {
			if(fmtChunk->bitsPerSample == 8)
				alFormat = AL_FORMAT_MONO8;
			else if(fmtChunk->bitsPerSample == 16)
				alFormat = AL_FORMAT_MONO16;
			else {
				mem::free(soundBuffer);
				return false;
			}
		}else {
			if(fmtChunk->bitsPerSample == 8)
				alFormat = AL_FORMAT_STEREO8;
			else if(fmtChunk->bitsPerSample == 16)
				alFormat = AL_FORMAT_STEREO16;
			else {
				mem::free(soundBuffer);
				return false;
			}
		}

		// Rempli le buffer des données du son.
		alBufferData(m_Buffer, alFormat, (uint8_t *) soundBuffer + offset, size, fmtChunk->sampleRate);

		mem::freeNoTrack(soundBuffer);

		// Si une erreur survint alors on détruit le buffer.
		if(alGetError() != AL_NO_ERROR) {
			// Détruit le buffer.
			alDeleteBuffers(1, &m_Buffer);
			return false;
		}

		return true;
	}

	/*
	====================
	AudioBuffer::destroy
	====================
	*/
	void AudioBuffer::destroy()
	{
		alDeleteBuffers(1, &m_Buffer);
	}

	/*
	=========================
	AudioBuffer::getFrequency
	=========================
	*/
	int AudioBuffer::getFrequency() const
	{
		alGetError();

		int frequency;
		alGetBufferi(m_Buffer, AL_FREQUENCY, &frequency);
		if(alGetError() != AL_NO_ERROR)
			return 0;

		return frequency;
	}

	/*
	====================
	AudioBuffer::getBits
	====================
	*/
	int AudioBuffer::getBits() const
	{
		alGetError();

		int bits;
		alGetBufferi(m_Buffer, AL_BITS, &bits);
		if(alGetError() != AL_NO_ERROR)
			return 0;

		return bits;
	}

	/*
	========================
	AudioBuffer::getChannels
	========================
	*/
	int AudioBuffer::getChannels() const
	{
		alGetError();

		int channels;
		alGetBufferi(m_Buffer, AL_CHANNELS, &channels);
		if(alGetError() != AL_NO_ERROR)
			return 0;

		return channels;
	}

	/*
	====================
	AudioBuffer::getSize
	====================
	*/
	int AudioBuffer::getSize() const
	{
		alGetError();

		int size;
		alGetBufferi(m_Buffer, AL_SIZE, &size);
		if(alGetError() != AL_NO_ERROR)
			return 0;

		return size;
	}



	/*
	========================
	AudioSource::AudioSource
	========================
	*/
	AudioSource::AudioSource()
		: m_Source(0)
	{ }

	/*
	===================
	AudioSource::create
	===================
	*/
	bool AudioSource::create()
	{
		// Nettoie le code d'erreur d'OpenAL.
		alGetError();

		// Génère une source audio qui jouera le son d'un buffer.
		// Une source peut être placée dans un monde 3D pour donner un effet spatial.
		// Un buffer doit être attaché à cette source.
		alGenSources(1, &m_Source);
		if(alGetError() != AL_NO_ERROR)
			return false;

		return true;
	}

	/*
	=========================
	AudioSource::attachBuffer
	=========================
	*/
	bool AudioSource::attachBuffer(const AudioBuffer &buffer)
	{
		alGetError();

		alSourcei(m_Source, AL_BUFFER, buffer.m_Buffer);
		if(alGetError() != AL_NO_ERROR)
			return false;

		return true;
	}

	/*
	=================
	AudioSource::play
	=================
	*/
	bool AudioSource::play()
	{
		alGetError();

		alSourcePlay(m_Source);
		if(alGetError() != AL_NO_ERROR)
			return false;

		return true;
	}

	/*
	====================
	AudioSource::destroy
	====================
	*/
	void AudioSource::destroy()
	{
		alDeleteSources(1, &m_Source);
	}


	/*
	======================
	AudioListener::setGain
	======================
	*/
	bool AudioListener::setGain(float value)
	{
		alGetError();

		// Modifie les valeurs de l'attribut de position du listener.
		alListenerf(AL_GAIN, value);
		if(alGetError() != AL_NO_ERROR)
			return false;

		return true;
	}

	/*
	==========================
	AudioListener::setPosition
	==========================
	*/
	bool AudioListener::setPosition(const fvec3 &pos)
	{
		alGetError();

		// Modifie les valeurs de l'attribut de position du listener.
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		if(alGetError() != AL_NO_ERROR)
			return false;

		return true;
	}

	/*
	==========================
	AudioListener::setVelocity
	==========================
	*/
	bool AudioListener::setVelocity(const fvec3 &pos)
	{
		alGetError();

		// Modifie les valeurs de l'attribut de position du listener.
		alListener3f(AL_VELOCITY, pos.x, pos.y, pos.z);
		if(alGetError() != AL_NO_ERROR)
			return false;

		return true;
	}

	/*
	=============================
	AudioListener::setOrientation
	=============================
	*/
	//bool AudioListener::setOrientation()
	//{
	//	alGetError();

	//	// Modifie les valeurs de l'attribut de position du listener.
	//	alListener3f(AL_ORIENTATION, pos.x, pos.y, pos.z);
	//	if(alGetError() != AL_NO_ERROR)
	//		return false;

	//	return true;
	//}

	/*
	======================
	AudioListener::getGain
	======================
	*/
	float AudioListener::getGain() const
	{
		alGetError();

		float value;
		alGetListenerf(AL_GAIN, &value);
		if(alGetError() != AL_NO_ERROR)
			value = 0;

		return value;
	}

	/*
	==========================
	AudioListener::getPosition
	==========================
	*/
	fvec3 AudioListener::getPosition() const
	{
		alGetError();

		float x, y, z;
		alGetListener3f(AL_POSITION, &x, &y, &z);
		if(alGetError() != AL_NO_ERROR) {
			x = 0;
			y = 0;
			z = 0;
		}

		return { x, y, z };
	}

	/*
	==========================
	AudioListener::getVelocity
	==========================
	*/
	fvec3 AudioListener::getVelocity() const
	{
		alGetError();

		float x, y, z;
		alGetListener3f(AL_VELOCITY, &x, &y, &z);
		if(alGetError() != AL_NO_ERROR) {
			x = 0;
			y = 0;
			z = 0;
		}

		return { x, y, z };
	}

	/*
	=================
	AudioDevice::init
	=================
	*/
	bool AudioDevice::init()
	{
		// Sélectionne le "dispositif par défaut"
		m_ALCDevice = alcOpenDevice(nullptr);
		if(m_ALCDevice == nullptr)
			return false;

		// Crée un contexte pour le dispositif précédemment créé.
		m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);
		if(m_ALCContext == nullptr) {
			m_ALCDevice = nullptr;
			return false;
		}

		// Fonctionne de la même manière qu'OpenGL, c'est une machine à états,
		// on indique donc que le contexte actuellement actif est celui qu'on
		// vient de créer.
		alcMakeContextCurrent(m_ALCContext);

		// NOTE: Il est obligatoire d'exécuter cette succession de fonctions avant d'utiliser les autres fonctions d'OpenAL
		//       car elles ont besoin d'un contexte actif sinon elles retourneront une erreur. (Oui c'est du vécu).

		return true;
	}

	/*
	=====================
	AudioDevice::shutdown
	=====================
	*/
	void AudioDevice::shutdown()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_ALCContext);
		alcCloseDevice(m_ALCDevice);

		m_ALCDevice  = nullptr;
		m_ALCContext = nullptr;
	}

}
