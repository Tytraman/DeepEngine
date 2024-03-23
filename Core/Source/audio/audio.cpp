#include "audio/audio.hpp"
#include "io/stream.hpp"

#include <stdio.h>

namespace deep
{

    ALCdevice  *audio_device::m_ALCDevice  = nullptr;
    ALCcontext *audio_device::m_ALCContext = nullptr;
    audio_listener audio_device::m_Listener;

    /*
    ========================
    audio_buffer::audio_buffer
    ========================
    */
    audio_buffer::audio_buffer()
        : m_Buffer(0)
    { }

    /*
    ===================
    audio_buffer::create
    ===================
    */
    bool audio_buffer::create(const char *filename)
    {
        input_file_stream s(filename);

        if(!s.open())
            return false;

        size_t filesize = s.getFileSize();
        if(filesize == 0)
            return false;

        // L'offset 20 est le début des données du chunk "fmt".
        size_t offset = sizeof(wave_header);

        wave_header *soundBuffer = (wave_header *) mem::alloc_no_track(filesize);
        size_t bytesRead;

        if(!s.readAll(soundBuffer, &bytesRead))
        {
            s.close();
            mem::free(soundBuffer);
            return false;
        }

        s.close();

        // Vérifie que le fichier chargé soit bien dans le format WAVE.
        if(soundBuffer->riffHeader.chunkID != RiffChunkID || soundBuffer->riffHeader.formatID != WaveChunkID || soundBuffer->subChunkID != FmtSubChunkID)
        {
            mem::free(soundBuffer);
            return false;
        }

        wave_fmt_chunk *fmtChunk = (wave_fmt_chunk *) ((uint8_t *) soundBuffer + offset);

        offset += soundBuffer->subChunkSize;

        if(*((uint32_t *) ((uint8_t *) soundBuffer + offset)) == ListSubChunkID)
        {
            wave_list_chunk *listChunk = (wave_list_chunk *) ((uint8_t *) soundBuffer + offset);
            offset += listChunk->size + 8;
            if(listChunk->typeID == InfoSubChunkID) {
                
            }
        }

        if(*((uint32_t *) ((uint8_t *) soundBuffer + offset)) != DataSubChunkID)
        {
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
        if(alGetError() != AL_NO_ERROR)
        {
            mem::free(soundBuffer);
            return false;
        }

        int alFormat;

        if(fmtChunk->channelsNumber == 1)
        {
            if(fmtChunk->bitsPerSample == 8)
                alFormat = AL_FORMAT_MONO8;
            else if(fmtChunk->bitsPerSample == 16)
                alFormat = AL_FORMAT_MONO16;
            else
            {
                mem::free(soundBuffer);
                return false;
            }
        }
        else
        {
            if(fmtChunk->bitsPerSample == 8)
                alFormat = AL_FORMAT_STEREO8;
            else if(fmtChunk->bitsPerSample == 16)
                alFormat = AL_FORMAT_STEREO16;
            else
            {
                mem::free(soundBuffer);
                return false;
            }
        }

        // Rempli le buffer des données du son.
        alBufferData(m_Buffer, alFormat, (uint8_t *) soundBuffer + offset, size, fmtChunk->sampleRate);

        mem::free_no_track(soundBuffer);

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
    audio_buffer::destroy
    ====================
    */
    void audio_buffer::destroy()
    {
        alDeleteBuffers(1, &m_Buffer);
    }

    /*
    =========================
    audio_buffer::getFrequency
    =========================
    */
    int audio_buffer::getFrequency() const
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
    audio_buffer::getBits
    ====================
    */
    int audio_buffer::getBits() const
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
    audio_buffer::getChannels
    ========================
    */
    int audio_buffer::getChannels() const
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
    audio_buffer::getSize
    ====================
    */
    int audio_buffer::getSize() const
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
    audio_source::audio_source
    ========================
    */
    audio_source::audio_source()
        : m_Source(0)
    { }

    /*
    ===================
    audio_source::create
    ===================
    */
    bool audio_source::create()
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
    audio_source::attachBuffer
    =========================
    */
    bool audio_source::attachBuffer(const audio_buffer &buffer)
    {
        alGetError();

        alSourcei(m_Source, AL_BUFFER, buffer.m_Buffer);
        if(alGetError() != AL_NO_ERROR)
            return false;

        return true;
    }

    /*
    =================
    audio_source::play
    =================
    */
    bool audio_source::play()
    {
        alGetError();

        alSourcePlay(m_Source);
        if(alGetError() != AL_NO_ERROR)
            return false;

        return true;
    }

    /*
    ====================
    audio_source::destroy
    ====================
    */
    void audio_source::destroy()
    {
        alDeleteSources(1, &m_Source);
    }


    /*
    ======================
    audio_listener::setGain
    ======================
    */
    bool audio_listener::setGain(float value)
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
    audio_listener::setPosition
    ==========================
    */
    bool audio_listener::setPosition(const vec3<float> &pos)
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
    audio_listener::setVelocity
    ==========================
    */
    bool audio_listener::setVelocity(const vec3<float> &pos)
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
    audio_listener::setOrientation
    =============================
    */
    //bool audio_listener::setOrientation()
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
    audio_listener::getGain
    ======================
    */
    float audio_listener::getGain() const
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
    audio_listener::getPosition
    ==========================
    */
    vec3<float> audio_listener::getPosition() const
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
    audio_listener::getVelocity
    ==========================
    */
    vec3<float> audio_listener::getVelocity() const
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
    ==================
    audio_device::init
    ==================
    */
    bool audio_device::init()
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
    audio_device::shutdown
    =====================
    */
    void audio_device::shutdown()
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_ALCContext);
        alcCloseDevice(m_ALCDevice);

        m_ALCDevice  = nullptr;
        m_ALCContext = nullptr;
    }

}
