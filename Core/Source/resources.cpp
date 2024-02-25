#include "DE/resources.hpp"
#include "DE/core.hpp"
#include "DE/string_utils.hpp"
#include "DE/memory/memory.hpp"
#include "DE/memory/pair.hpp"
#include "DE/file.hpp"
#include "DE/io/stream.hpp"
#include "DE/image/png.hpp"
#include "DE/common.hpp"
#include "DE/file/file_object.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/drivers/opengl/shader.hpp"
#include "DE/drivers/opengl/texture.hpp"
#include "DE/io/file_stream.hpp"

#include <stdio.h>

namespace deep
{

    struct shader_fusion
    {
        string name;
        string vertexFilename;
        string fragmentFilename;

        shader_fusion(const char *first, const char *second, const char *third);
    };

    shader_fusion::shader_fusion(const char *first, const char *second, const char *third)
        : name(first),
          vertexFilename(second),
          fragmentFilename(third)
    { }

    bool enum_shaders_config_fobj(file_object_container &container, string & /* currentPath */, mem_ptr args)
    {
        // Contient la liste des fusions Vertex Fragment shaders.
        hash_table<shader_fusion> *table = static_cast<hash_table<shader_fusion>*>(args);

        pair<string, string> *name = container.search_element("name");
        if(name == nullptr)
            return true;

        pair<string, string> *vertex = container.search_element("vertex");
        if(vertex == nullptr)
            return true;

        pair<string, string> *fragment = container.search_element("fragment");
        if(fragment == nullptr)
            return true;

        // Ajoute à la liste la fusion Vertex Fragment shaders.
        table->insert(name->value2().str(), shader_fusion(name->value2().str(), vertex->value2().str(), fragment->value2().str()));

        return true;
    }

    bool enum_shader_files_callback_ascii(const char *filename, bool isDirectory, void *args)
    {
        if(!isDirectory)
        {
            list<pair<string, GL3::shader_manager::gl_shader_type>> *ls = static_cast<list<pair<string, GL3::shader_manager::gl_shader_type>>*>(args);

            if(string_utils::ends_with(filename, ".vert"))
            {
                ls->add(pair<string, GL3::shader_manager::gl_shader_type>(filename, GL3::shader_manager::gl_shader_type::Vertex));
            }
            else if(string_utils::ends_with(filename, ".frag"))
            {
                ls->add(pair<string, GL3::shader_manager::gl_shader_type>(filename, GL3::shader_manager::gl_shader_type::Fragment));
            }
        }

        return true;
    }

    /*
    ==================================
    resource_manager::resource_manager
    ==================================
    */
    resource_manager::resource_manager()
    { }

    /*
    ===============================
    resource_manager::get_singleton
    ===============================
    */
    resource_manager *resource_manager::get_singleton()
    {
        static resource_manager singleton;

        return &singleton;
    }

    /*
    ======================
    resource_manager::init
    ======================
    */
    bool resource_manager::init(const char *filepath)
    {
        GL3::shader_manager *shaderManager = GL3::shader_manager::get_singleton();
        GL3::program_manager *programManager = GL3::program_manager::get_singleton();

        bool ret = true;

        m_ResourcesFolder = filepath;

        m_ShadersFolder = m_ResourcesFolder;
        m_ShadersFolder.append("shaders\\");

        m_TexturesFolder = m_ResourcesFolder;
        m_TexturesFolder.append("textures\\");

        m_SoundsFolder = m_ResourcesFolder;
        m_SoundsFolder.append("sounds\\");

        m_ScreenshotsFolder = m_ResourcesFolder;
        m_ScreenshotsFolder.append("screenshots\\");

        m_FontsFolder = m_ResourcesFolder;
        m_FontsFolder.append("fonts\\");

        // Création des dossiers.
        CreateDirectoryA(m_ResourcesFolder.str(), NULL);
        CreateDirectoryA(m_ShadersFolder.str(), NULL);
        CreateDirectoryA(m_TexturesFolder.str(), NULL);
        CreateDirectoryA(m_SoundsFolder.str(), NULL);
        CreateDirectoryA(m_ScreenshotsFolder.str(), NULL);
        CreateDirectoryA(m_FontsFolder.str(), NULL);

        string shadersConfig(m_ShadersFolder);
        shadersConfig.append("shaders.fobj");

        file_stream ifs(shadersConfig.str(), file_stream::file_mode::Open, file_stream::file_access::Read, file_stream::file_share::Read);
        if(!ifs.open())
        {
            return false;
        }

        file_object shadersFobj(&ifs, "shaders");
        if(!shadersFobj.load())
        {
            ifs.close();

            return false;
        }

        ifs.close();

        hash_table<shader_fusion> shaderFiles;
        shadersFobj.enumerate(nullptr, enum_shaders_config_fobj, static_cast<mem_ptr>(&shaderFiles), 1);
        size_t numberOfShaders = shaderFiles.getNumberOfElements();

        core::out() << "Found " << numberOfShaders << " shaders.\n";

        hash_table_iterator<shader_fusion> shaderIterator = shaderFiles.begin();
        hash_table_iterator<shader_fusion> shaderEndIterator = shaderFiles.end();

        // Compile et link les shaders.
        for(; shaderIterator != shaderEndIterator; ++shaderIterator)
        {   
            string vertexShaderFilename(m_ShadersFolder);
            string fragmentShaderFilename(m_ShadersFolder);
            vertexShaderFilename.append(shaderIterator->value.vertexFilename.str());
            fragmentShaderFilename.append(shaderIterator->value.fragmentFilename.str());

            core::out() << "(\n  " << vertexShaderFilename.str() << "\n  " << fragmentShaderFilename.str() << "\n  Opening files...\n  ";

            input_file_stream vis(vertexShaderFilename.str());
            if(!vis.open())
            {
                fprintf(stderr, "Unable to open vertex shader file.\n");
                core::out() << ")\n";

                ret = false;
                goto end;
            }

            input_file_stream fis(fragmentShaderFilename.str());
            if(!fis.open())
            {
                fprintf(stderr, "Unable to open fragment shader file.\n");
                core::out() << ")\n";

                vis.close();

                ret = false;
                goto end;
            }

            memory_chunk vertexShaderChunk;
            memory_chunk fragmentShaderChunk;
            memory_chunk::alloc(vertexShaderChunk, vis.getFileSize() + 1);
            memory_chunk::alloc(fragmentShaderChunk, fis.getFileSize() + 1);

            size_t bytesRead;

            core::out() << "Reading files...\n  ";

            if(!vis.readAll(vertexShaderChunk.data(), &bytesRead))
            {
                fprintf(stderr, "Can't read vertex shader.\n");
                core::out() << ")\n";

                vertexShaderChunk.free();
                fragmentShaderChunk.free();

                vis.close();
                fis.close();

                ret = false;
                goto end;
            }

            if(!fis.readAll(fragmentShaderChunk.data(), &bytesRead))
            {
                fprintf(stderr, "Can't read fragment shader.\n");
                core::out() << ")\n";

                vertexShaderChunk.free();
                fragmentShaderChunk.free();

                vis.close();
                fis.close();

                ret = false;
                goto end;
            }

            ((char *) vertexShaderChunk.data())[vertexShaderChunk.size() - 1] = '\0';
            ((char *) fragmentShaderChunk.data())[fragmentShaderChunk.size() - 1] = '\0';

            vis.close();
            fis.close();

            string vertexShaderName = shaderIterator->value.name;
            string fragmentShaderName = shaderIterator->value.name;
            vertexShaderName.append("_vert");
            fragmentShaderName.append("_frag");

            GL3::gl_id vertexShader = shaderManager->create(vertexShaderName.str(), GL3::shader_manager::gl_shader_type::Vertex);
            GL3::gl_id fragmentShader = shaderManager->create(fragmentShaderName.str(), GL3::shader_manager::gl_shader_type::Fragment);

            shaderManager->load(vertexShader, vertexShaderChunk);
            shaderManager->load(fragmentShader, fragmentShaderChunk);

            vertexShaderChunk.free();
            fragmentShaderChunk.free();

            core::out() << "Compiling shaders...\n";
            if(!shaderManager->compile(vertexShader))
            {
                shaderManager->destroy(vertexShader);
                shaderManager->destroy(fragmentShader);
                            
                ret = false;
                goto end;
            }

            if(!shaderManager->compile(fragmentShader))
            {
                shaderManager->destroy(vertexShader);
                shaderManager->destroy(fragmentShader);
                            
                ret = false;
                goto end;
            }
                        
            GL3::gl_id program = programManager->create(shaderIterator->value.name.str());

            programManager->attach_shader(program, vertexShader);
            programManager->attach_shader(program, fragmentShader);

            core::out() << "  Linking shaders...\n";
            if(!programManager->link(program))
            {
                core::out() << ")\n";
                shaderManager->destroy(vertexShader);
                shaderManager->destroy(fragmentShader);
                            
                ret = false;
                goto end;
            }

            shaderManager->destroy(vertexShader);
            shaderManager->destroy(fragmentShader);

            core::out() << "  " DE_TERM_FG_GREEN "Done." DE_TERM_RESET "\n)\n";
        }

end:
        return ret;
    }

    /*
    ==========================
    resource_manager::shutdown
    ==========================
    */
    void resource_manager::shutdown()
    {

    }

    /*
    =============================
    resource_manager::loadTexture
    =============================
    */
    GL3::gl_id resource_manager::loadTexture(const char *name, uint8_t unit)
    {
        png png;
        string filename = m_TexturesFolder;
        filename.append(name);

        if(!png.loadAndRead(filename.str()))
            return 0;

        printf("Texture \"%s\" loaded: %s\n", name, png::colorTypeName(png.colorType()));

        GL3::texture_manager *textureManager = GL3::texture_manager::get_singleton();

        png.applyVerticalMirrorEffect();

        GL3::gl_id texture = textureManager->create_2D(name);
        textureManager->bind(texture, unit);

        textureManager->set_texture_wrapping_s(GL3::texture_manager::gl_texture_wrap::ClampToEdge);
        textureManager->set_texture_wrapping_t(GL3::texture_manager::gl_texture_wrap::ClampToEdge);
        textureManager->set_texture_filtering(GL3::texture_manager::gl_texture_filter::Nearest);

        mem_ptr image = png.rawImage();
        textureManager->transmit_texture(image, png.width(), png.height(), png.colorType());
        mem::free(image);

        return texture;
    }

    /*
    =========================
    resource_manager::loadBMP
    =========================
    */
    bool resource_manager::loadBMP(const char *name, bmp &dest)
    {
        string filename = m_TexturesFolder;
        filename.append(name);

        if(!dest.create_from_file(filename.str()))
            return false;

        return true;
    }

}
