#include <DE/resources.hpp>
#include <DE/string_utils.hpp>
#include <DE/memory/memory.hpp>
#include <DE/memory/pair.hpp>
#include <DE/file.hpp>
#include <DE/stream.hpp>
#include <DE/rendering/opengl_utils.hpp>
#include <DE/image/png.hpp>
#include <DE/common.hpp>
#include <DE/file/file_object.hpp>
#include <DE/memory/hash_table.hpp>

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

    string resource_manager::m_ResourcesFolder;
    string resource_manager::m_ShadersFolder;
    string resource_manager::m_TexturesFolder;
    string resource_manager::m_SoundsFolder;
    string resource_manager::m_ScreenshotsFolder;

    bool enum_shaders_config_fobj(file_object_container &container, string &currentPath, mem_ptr args)
    {
        // Contient la liste des fusions Vertex Fragment shaders.
        hash_table<shader_fusion> *table = static_cast<hash_table<shader_fusion>*>(args);

        pair<string, string> *name = container.searchElement("name");
        if(name == nullptr)
            return true;

        pair<string, string> *vertex = container.searchElement("vertex");
        if(vertex == nullptr)
            return true;

        pair<string, string> *fragment = container.searchElement("fragment");
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
			list<pair<string, gl_shader_type>> *ls = static_cast<list<pair<string, gl_shader_type>>*>(args);

            if(string_utils::endsWith(filename, ".vert"))
            {
                ls->add(pair<string, gl_shader_type>(filename, gl_shader_type::Vertex));
            }
            else if(string_utils::endsWith(filename, ".frag"))
            {
                ls->add(pair<string, gl_shader_type>(filename, gl_shader_type::Fragment));
            }
		}

		return true;
	}

	/*
	======================
	resource_manager::init
	======================
	*/
	bool resource_manager::init(const char *filepath)
	{
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

		// Création des dossiers.
		CreateDirectoryA(m_ResourcesFolder.str(), NULL);
		CreateDirectoryA(m_ShadersFolder.str(), NULL);
		CreateDirectoryA(m_TexturesFolder.str(), NULL);
		CreateDirectoryA(m_SoundsFolder.str(), NULL);
        CreateDirectoryA(m_ScreenshotsFolder.str(), NULL);

        string shadersConfig(m_ShadersFolder);
        shadersConfig.append("shaders.fobj");

        file_object shadersFobj(shadersConfig.str());

        if(!shadersFobj.open())
            return false;

        if(!shadersFobj.load())
        {
            shadersFobj.close();
            return false;
        }

        hash_table<shader_fusion> shaderFiles;
        shadersFobj.enumerate(nullptr, enum_shaders_config_fobj, static_cast<mem_ptr>(&shaderFiles), 1);
		size_t numberOfShaders = shaderFiles.getNumberOfElements();

		printf("Found %llu shaders.\n", numberOfShaders);

        auto &currentShader = shaderFiles.begin();
        auto &endShader = shaderFiles.end();

        // Compile et link les shaders.
        for(; currentShader != endShader; ++currentShader)
        {   
            string vertexShaderFilename(m_ShadersFolder);
            string fragmentShaderFilename(m_ShadersFolder);
            vertexShaderFilename.append(currentShader->value.vertexFilename.str());
            fragmentShaderFilename.append(currentShader->value.fragmentFilename.str());

			printf("(\n  %s\n  %s\n  Opening files...\n  ", vertexShaderFilename.str(), fragmentShaderFilename.str());

			input_file_stream vis(vertexShaderFilename.str());
			if(!vis.open())
            {
				fprintf(stderr, "Unable to open vertex shader file.\n");
				printf(")\n");

				ret = false;
				goto end;
			}

            input_file_stream fis(fragmentShaderFilename.str());
			if(!fis.open())
            {
				fprintf(stderr, "Unable to open fragment shader file.\n");
				printf(")\n");

                vis.close();

				ret = false;
				goto end;
			}

			memory_chunk vertexShaderChunk;
            memory_chunk fragmentShaderChunk;
			memory_chunk::alloc(vertexShaderChunk, vis.getFileSize() + 1);
            memory_chunk::alloc(fragmentShaderChunk, fis.getFileSize() + 1);

			size_t bytesRead;

			printf("Reading files...\n  ");

			if(!vis.readAll(vertexShaderChunk.data(), &bytesRead))
            {
				fprintf(stderr, "Can't read vertex shader.\n");
				printf(")\n");

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
				printf(")\n");

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

            string vertexShaderName = currentShader->value.name;
            string fragmentShaderName = currentShader->value.name;
            vertexShaderName.append("_vert");
            fragmentShaderName.append("_frag");

			shader_id vertexShader = shader_manager::create(vertexShaderName.str(), gl_shader_type::Vertex);
            shader_id fragmentShader = shader_manager::create(fragmentShaderName.str(), gl_shader_type::Fragment);

			shader_manager::load(vertexShader, vertexShaderChunk);
            shader_manager::load(fragmentShader, fragmentShaderChunk);

			vertexShaderChunk.free();
            fragmentShaderChunk.free();

			printf("Compiling shaders...\n");
			if(!shader_manager::compile(vertexShader))
            {
				shader_manager::destroy(vertexShader);
                shader_manager::destroy(fragmentShader);
							
				ret = false;
				goto end;
			}

            if(!shader_manager::compile(fragmentShader))
            {
				shader_manager::destroy(vertexShader);
                shader_manager::destroy(fragmentShader);
							
				ret = false;
				goto end;
			}
						
			program_id program = program_manager::create(currentShader->value.name.str());

			program_manager::attachShader(program, vertexShader);
            program_manager::attachShader(program, fragmentShader);

			printf("  Linking shaders...\n");
			if(!program_manager::link(program))
            {
				printf(")\n");
				shader_manager::destroy(vertexShader);
                shader_manager::destroy(fragmentShader);
							
				ret = false;
				goto end;
			}

			shader_manager::destroy(vertexShader);
            shader_manager::destroy(fragmentShader);

			printf("  " DE_TERM_FG_GREEN "Done." DE_TERM_RESET "\n)\n");
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
	texture_id resource_manager::loadTexture(const char *name, uint8_t unit)
	{
		png png;
		string filename = m_TexturesFolder;
		filename.append(name);

		if(!png.loadAndRead(filename.str()))
			return 0;

		printf("Texture \"%s\" loaded: %s\n", name, png::colorTypeName(png.colorType()));

		png.applyVerticalMirrorEffect();

		texture_id texture = texture_manager::create2D(name);
		texture_manager::bind(texture, unit);

		texture_manager::setTextureWrappingS(gl_texture_wrap::ClampToEdge);
		texture_manager::setTextureWrappingT(gl_texture_wrap::ClampToEdge);
		texture_manager::setTextureFiltering(gl_texture_filter::Nearest);

		mem_ptr image = png.rawImage();
		texture_manager::transmitTexture(image, png.width(), png.height(), png.colorType());
		mem::free(image);

		return texture;
	}

}
