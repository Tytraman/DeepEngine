#include "core/resources/resource_manager.hpp"
#include "core/core.hpp"
#include "core/memory.hpp"
#include "core/templates/pair.hpp"
#include "core/templates/hash_table.hpp"

#include "core/resources/resource_loader.hpp"
#include "core/resources/bmp_loader.hpp"
#include "core/resources/text_loader.hpp"

namespace deep
{

    resource_manager *resource_manager::g_ResourceManager = nullptr;

    /*struct shader_fusion
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
    { }*/

    //bool enum_shaders_config_fobj(file_object_container &container, string & /* currentPath */, size_t /*subContainerNumber*/, mem_ptr args)
    //{
    //    // Contient la liste des fusions Vertex Fragment shaders.
    //    hash_table<shader_fusion> *table = static_cast<hash_table<shader_fusion>*>(args);

    //    pair<string, string> *name = container.search_element("name");
    //    if(name == nullptr)
    //        return true;

    //    pair<string, string> *vertex = container.search_element("vertex");
    //    if(vertex == nullptr)
    //        return true;

    //    pair<string, string> *fragment = container.search_element("fragment");
    //    if(fragment == nullptr)
    //        return true;

    //    // Ajoute à la liste la fusion Vertex Fragment shaders.
    //    table->insert(name->value2().str(), shader_fusion(name->value2().str(), vertex->value2().str(), fragment->value2().str()));

    //    return true;
    //}

    /*bool enum_shader_files_callback_ascii(const char *filename, bool isDirectory, void *args)
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
    }*/

    /*
    ==================================
    resource_manager::resource_manager
    ==================================
    */
    resource_manager::resource_manager()
        : m_FileListing("resources_file_listing")
    { }

    /*
    ======================
    resource_manager::init
    ======================
    */
    bool resource_manager::init(ref<stream> resourcesStream)
    {
        if(!resourcesStream->is_opened())
        {
            if(!resourcesStream->open())
            {
                return false;
            }
        }

        if(!m_ZipResource.init_base(resourcesStream))
        {
            return false;
        }

        if(!m_ZipResource.init())
        {
            return false;
        }

        int64_t fileListingIndex = m_ZipResource.get_file_index("file_listing.fobj");
        if(fileListingIndex != -1)
        {
            ref<memory_stream> fileListingStrm = mem::alloc_type<memory_stream>();

            m_ZipResource.read_file(fileListingIndex, fileListingStrm.get());

            fileListingStrm->seek(0, stream::seek_origin::Begin);

            m_FileListing.load(fileListingStrm.get());
        }

        // Ajoute les loaders supportés par défaut.
        ref<bmp_loader> bmpLoader = mem::alloc_type<bmp_loader>(&m_ZipResource);
        ref<text_loader> textLoader = mem::alloc_type<text_loader>(&m_ZipResource);

        m_Loaders.insert("bmp", bmpLoader.get());
        m_Loaders.insert("txt", textLoader.get());

        /*GL3::shader_manager *shaderManager = GL3::shader_manager::get_singleton();
        GL3::program_manager *programManager = GL3::program_manager::get_singleton();*/

        bool ret = true;

        string scenePath = m_ExportFolder;
        scenePath.append("scenes\\");

        // Création des dossiers.
        CreateDirectoryA(m_ScreenshotsFolder.str(), NULL);
        CreateDirectoryA(m_ExportFolder.str(), NULL);
        CreateDirectoryA(scenePath.str(), NULL);

        //string shadersConfig(m_ShadersFolder);
        //shadersConfig.append("shaders.fobj");

        //ref<file_stream> ifs(mem::alloc_type<file_stream>(shadersConfig.str(), file_stream::file_mode::Open, file_stream::file_access::Read, file_stream::file_share::Read));
        //if(!ifs->open())
        //{
        //    return false;
        //}

        //file_object shadersFobj("shaders");
        //if(!shadersFobj.load(ifs.get()))
        //{
        //    ifs->close();

        //    return false;
        //}

        //ifs->close();

        //hash_table<shader_fusion> shaderFiles;
        //shadersFobj.enumerate(nullptr, enum_shaders_config_fobj, nullptr, static_cast<mem_ptr>(&shaderFiles), 1);
        //size_t numberOfShaders = shaderFiles.getNumberOfElements();

        //core::out() << "Found " << numberOfShaders << " shaders.\n";

        //hash_table_iterator<shader_fusion> shaderIterator = shaderFiles.begin();
        //hash_table_iterator<shader_fusion> shaderEndIterator = shaderFiles.end();

        //// Compile et link les shaders.
        //for(; shaderIterator != shaderEndIterator; ++shaderIterator)
        //{   
        //    string vertexShaderFilename(m_ShadersFolder);
        //    string fragmentShaderFilename(m_ShadersFolder);
        //    vertexShaderFilename.append(shaderIterator->value.vertexFilename.str());
        //    fragmentShaderFilename.append(shaderIterator->value.fragmentFilename.str());

        //    core::out() << "(\n  " << vertexShaderFilename.str() << "\n  " << fragmentShaderFilename.str() << "\n  Opening files...\n  ";

        //    input_file_stream vis(vertexShaderFilename.str());
        //    if(!vis.open())
        //    {
        //        fprintf(stderr, "Unable to open vertex shader file.\n");
        //        core::out() << ")\n";

        //        ret = false;
        //        goto end;
        //    }

        //    input_file_stream fis(fragmentShaderFilename.str());
        //    if(!fis.open())
        //    {
        //        fprintf(stderr, "Unable to open fragment shader file.\n");
        //        core::out() << ")\n";

        //        vis.close();

        //        ret = false;
        //        goto end;
        //    }

        //    memory_chunk vertexShaderChunk;
        //    memory_chunk fragmentShaderChunk;
        //    memory_chunk::alloc(vertexShaderChunk, vis.getFileSize() + 1);
        //    memory_chunk::alloc(fragmentShaderChunk, fis.getFileSize() + 1);

        //    size_t bytesRead;

        //    core::out() << "Reading files...\n  ";

        //    if(!vis.readAll(vertexShaderChunk.data(), &bytesRead))
        //    {
        //        fprintf(stderr, "Can't read vertex shader.\n");
        //        core::out() << ")\n";

        //        vertexShaderChunk.free();
        //        fragmentShaderChunk.free();

        //        vis.close();
        //        fis.close();

        //        ret = false;
        //        goto end;
        //    }

        //    if(!fis.readAll(fragmentShaderChunk.data(), &bytesRead))
        //    {
        //        fprintf(stderr, "Can't read fragment shader.\n");
        //        core::out() << ")\n";

        //        vertexShaderChunk.free();
        //        fragmentShaderChunk.free();

        //        vis.close();
        //        fis.close();

        //        ret = false;
        //        goto end;
        //    }

        //    ((char *) vertexShaderChunk.data())[vertexShaderChunk.size() - 1] = '\0';
        //    ((char *) fragmentShaderChunk.data())[fragmentShaderChunk.size() - 1] = '\0';

        //    vis.close();
        //    fis.close();

        //    string vertexShaderName = shaderIterator->value.name;
        //    string fragmentShaderName = shaderIterator->value.name;
        //    vertexShaderName.append("_vert");
        //    fragmentShaderName.append("_frag");

        //    GL3::gl_id vertexShader = shaderManager->create(vertexShaderName.str(), GL3::shader_manager::gl_shader_type::Vertex);
        //    GL3::gl_id fragmentShader = shaderManager->create(fragmentShaderName.str(), GL3::shader_manager::gl_shader_type::Fragment);

        //    shaderManager->load(vertexShader, vertexShaderChunk);
        //    shaderManager->load(fragmentShader, fragmentShaderChunk);

        //    vertexShaderChunk.free();
        //    fragmentShaderChunk.free();

        //    core::out() << "Compiling shaders...\n";
        //    if(!shaderManager->compile(vertexShader))
        //    {
        //        shaderManager->destroy(vertexShader);
        //        shaderManager->destroy(fragmentShader);
        //                    
        //        ret = false;
        //        goto end;
        //    }

        //    if(!shaderManager->compile(fragmentShader))
        //    {
        //        shaderManager->destroy(vertexShader);
        //        shaderManager->destroy(fragmentShader);
        //                    
        //        ret = false;
        //        goto end;
        //    }
        //                
        //    GL3::gl_id program = programManager->create(shaderIterator->value.name.str());

        //    programManager->attach_shader(program, vertexShader);
        //    programManager->attach_shader(program, fragmentShader);

        //    core::out() << "  Linking shaders...\n";
        //    if(!programManager->link(program))
        //    {
        //        core::out() << ")\n";
        //        shaderManager->destroy(vertexShader);
        //        shaderManager->destroy(fragmentShader);
        //                    
        //        ret = false;
        //        goto end;
        //    }

        //    shaderManager->destroy(vertexShader);
        //    shaderManager->destroy(fragmentShader);

        //    core::out() << "  " DE_TERM_FG_GREEN "Done." DE_TERM_RESET "\n)\n";
        //}

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
    ==============================
    resource_manager::get_resource
    ==============================
    */
    ref<resource> resource_manager::get_resource(const char *name)
    {
        hash_entry<ref<resource>> *entry = m_Resources[name];

        if(entry == nullptr)
        {
            // Vérifie si la ressource est dans la liste des ressources et récupère son format.
            file_object_container *container = m_FileListing.search_container(name);

            if(container == nullptr)
            {
                return ref<resource>();
            }

            pair<string, string> *typeElement = container->search_element("type");

            if(typeElement == nullptr)
            {
                return ref<resource>();
            }

            pair<string, string> *locationElement = container->search_element("location");

            if(locationElement == nullptr)
            {
                return ref<resource>();
            }

            string &type = typeElement->value2();
            string &location = locationElement->value2();

            hash_entry<ref<resource_loader>> *entryLoader = m_Loaders[type.str()];

            if(entryLoader == nullptr)
            {
                return ref<resource>();
            }

            ref<resource> res = entryLoader->value->load_resource(location.str());

            if(res.is_null())
            {
                return res;
            }

            m_Resources.insert(name, res);

            core::out() << DE_TERM_RESET "[" DE_TERM_FG_RED "resource_manager" DE_TERM_RESET "] " DE_TERM_FG_CYAN << name << DE_TERM_RESET " loaded into cache.\n";

            return res;
        }

        return entry->value;
    }

    /*
    ============================
    resource_manager::add_loader
    ============================
    */
    void resource_manager::add_loader(const char *type, ref<resource_loader> loader)
    {
        hash_entry<ref<resource_loader>> *entry = m_Loaders[type];

        if(entry != nullptr)
        {
            entry->value = loader;

            return;
        }

        m_Loaders.insert(type, loader);
    }

}
