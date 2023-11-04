#include <DE/resources.hpp>
#include <DE/string_utils.hpp>
#include <DE/memory/memory.hpp>
#include <DE/file.hpp>
#include <DE/stream.hpp>
#include <DE/rendering/opengl_utils.hpp>
#include <DE/image/png.hpp>

#include <stdio.h>

#if DE_WINDOWS
// Permet de récupérer les dossiers spéciaux sous Windows.
#include <shlobj_core.h>
#endif

namespace de
{

	struct shader_relation
    {
		wchar_t *name;
		size_t vertexShaderIndex;
		size_t fragmentShaderIndex;
	};

	wchar_t *resources::m_DeepDataFolder      = nullptr;
	wchar_t *resources::m_DeepResourcesFolder = nullptr;
	wchar_t *resources::m_DeepShadersFolder   = nullptr;
	wchar_t *resources::m_DeepTexturesFolder  = nullptr;
	wchar_t *resources::m_DeepSoundsFolder    = nullptr;

	bool enum_shader_files_callback(wchar_t *filename, bool isDirectory, void *args)
	{
		if(!isDirectory)
        {
			list<wchar_t *> *ls = static_cast<list<wchar_t *>*>(args);

			if(
				string_utils::endsWith(filename, L".vert") ||
				string_utils::endsWith(filename, L".frag")
				) {

				wchar_t *p = string_utils::copy(filename);
				ls->add(p);
			}
		}

		return true;
	}

	/*
	===============
	resources::init
	===============
	*/
	bool resources::init()
	{
		bool ret = true;
		WCHAR *filepath;

		// Récupère le chemin d'accès "C:\ProgramData\".
		HRESULT hResult = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &filepath);
		if(hResult != S_OK)
			return false;

		m_DeepDataFolder = string_utils::copy(filepath);

		// Libère la mémoire allouée par la fonction 'SHGetKnownFolderPath'.
		CoTaskMemFree(filepath);


		// Crée tous les noms de dossiers ressources.
		string_utils::append(&m_DeepDataFolder, L"\\CreationUI\\");

		m_DeepResourcesFolder = string_utils::copy(m_DeepDataFolder);
		string_utils::append(&m_DeepResourcesFolder, L"resources\\");

		m_DeepShadersFolder = string_utils::copy(m_DeepResourcesFolder);
		string_utils::append(&m_DeepShadersFolder, L"shaders\\");

		m_DeepTexturesFolder = string_utils::copy(m_DeepResourcesFolder);
		string_utils::append(&m_DeepTexturesFolder, L"textures\\");

		m_DeepSoundsFolder = string_utils::copy(m_DeepResourcesFolder);
		string_utils::append(&m_DeepSoundsFolder, L"sounds\\");

		// Création des dossiers.
		CreateDirectoryW(m_DeepDataFolder, NULL);
		CreateDirectoryW(m_DeepResourcesFolder, NULL);
		CreateDirectoryW(m_DeepShadersFolder, NULL);
		CreateDirectoryW(m_DeepTexturesFolder, NULL);
		CreateDirectoryW(m_DeepSoundsFolder, NULL);

		list<wchar_t *> shaderFiles;

		// Récupère tous les "Vertex Shaders" et les "Fragment Shaders".
		file::enumFiles(m_DeepShadersFolder, enum_shader_files_callback, &shaderFiles);
		size_t numberOfShaders = shaderFiles.getNumberOfElements();
		size_t i, j;

		// Des wchar sont utilisés car la fonction 'SHGetKnownFolderPath' n'existe pas en version char.
		wchar_t *filename1, *filename2;

		printf("Found %llu shaders.\n", numberOfShaders);

		list<shader_relation> shadersRelation;

		// Parcourt tous les shaders et fait le lien entre ceux qui ont le même nom.
		for(i = 0; i < numberOfShaders; ++i)
        {
            filename1 = shaderFiles[i];

			// Retire l'extension du nom du fichier.
			size_t dotIndex = string_utils::lastIndex(filename1, L'.');
			wchar_t *extension = filename1 + dotIndex + 1;
			if(dotIndex > 0)
				filename1[dotIndex] = L'\0';

			size_t numberOfRelations = shadersRelation.getNumberOfElements();
			bool foundRelation = false;

			// Parcourt la liste des relations pour voir si un shader a déjà ce nom.
			for(j = 0; j < numberOfRelations; ++j)
            {
				shader_relation &existingRelation = shadersRelation[j];
				// Si une relation possède déjà le nom du fichier.
				if(string_utils::equals(filename1, existingRelation.name))
                {
					foundRelation = true;

					if(string_utils::equals(extension, L"vert"))
						existingRelation.vertexShaderIndex   = i;
					else if(string_utils::equals(extension, L"frag"))
						existingRelation.fragmentShaderIndex = i;

					break;
				}
			}

			// Si aucune relation ne possède le nom du fichier, alors il faut la créer.
			if(!foundRelation)
            {
				shader_relation relation;
				wchar_t *filenameCopy = string_utils::copy(filename1);
				relation.name = filenameCopy;
				if(string_utils::equals(extension, L"vert"))
                {
					relation.vertexShaderIndex   = i;
					relation.fragmentShaderIndex = -1;
				}
                else if(string_utils::equals(extension, L"frag"))
                {
					relation.fragmentShaderIndex = i;
					relation.vertexShaderIndex   = -1;
				}
                else
                {
					relation.vertexShaderIndex   = -1;
					relation.fragmentShaderIndex = -1;
				}

				shadersRelation.add(relation);
			}

			if(dotIndex > 0)
				filename1[dotIndex] = L'.';
		}

		wchar_t *vertexShaderFilename;
		wchar_t *fragmentShaderFilename;

		// Maintenant que la liaison entre les différents shaders est faite, il faut les compiler et les linker ensembles.
		for(i = 0; i < shadersRelation.getNumberOfElements(); ++i)
        {
			shader_relation &existingRelation = shadersRelation[i];

			// Si un élément n'a pas de relation alors on l'ignore, triste pour les célibataires :'(
			if(existingRelation.vertexShaderIndex == -1 || existingRelation.fragmentShaderIndex == -1)
				continue;

            filename1 = shaderFiles[existingRelation.vertexShaderIndex];
            filename2 = shaderFiles[existingRelation.fragmentShaderIndex];

			vertexShaderFilename = string_utils::copy(m_DeepShadersFolder);
			string_utils::append(&vertexShaderFilename, filename1);

			fragmentShaderFilename = string_utils::copy(m_DeepShadersFolder);
			string_utils::append(&fragmentShaderFilename, filename2);

			printf("%(\n  %ls\n  %ls\n  Opening files...\n  ", vertexShaderFilename, fragmentShaderFilename);

			input_file_stream vis(vertexShaderFilename);
			if(!vis.openW())
            {
				fprintf(stderr, "Unable to open vertex shader file.\n");
				printf(")\n");

				mem::free(vertexShaderFilename);
				mem::free(fragmentShaderFilename);

				ret = false;
				goto end;
			}

			input_file_stream fis(fragmentShaderFilename);
			if(!fis.openW())
            {
				fprintf(stderr, "Unable to open fragment shader file.\n");
				printf(")\n");

				vis.close();

				mem::free(vertexShaderFilename);
				mem::free(fragmentShaderFilename);

				ret = false;
				goto end;
			}

			memory_chunk vertChk;
			memory_chunk::alloc(vertChk, vis.getFileSize() + 1);
			memory_chunk fragChk;
			memory_chunk::alloc(fragChk, fis.getFileSize() + 1);
			size_t bytesRead;

			printf("Reading files...\n  ");

			if(!vis.readAll(vertChk.data(), &bytesRead))
            {
				fprintf(stderr, "Can't read vertex shader.\n");
				printf(")\n");

				vertChk.free();
				fragChk.free();

				vis.close();
				fis.close();

				mem::free(vertexShaderFilename);
				mem::free(fragmentShaderFilename);

				ret = false;
				goto end;
			}

			if(!fis.readAll(fragChk.data(), &bytesRead))
            {
				fprintf(stderr, "Can't read fragment shader.\n");
				printf(")\n");

				vertChk.free();
				fragChk.free();

				vis.close();
				fis.close();

				mem::free(vertexShaderFilename);
				mem::free(fragmentShaderFilename);

				ret = false;
				goto end;
			}

			((char *) vertChk.data())[vertChk.size() - 1] = '\0';
			((char *) fragChk.data())[fragChk.size() - 1] = '\0';

			vis.close();
			fis.close();

			char *convName = static_cast<char *>(mem::alloc(string_utils::length(existingRelation.name) + 1));
			string_utils::wcharToChar(convName, existingRelation.name);

			mem::free(vertexShaderFilename);
			mem::free(fragmentShaderFilename);

			gl_shader_int vertShader = gl_shader::create(gl_shader_type::Vertex);
			gl_shader_int fragShader = gl_shader::create(gl_shader_type::Fragment);

			gl_shader::load(vertShader, vertChk);
			gl_shader::load(fragShader, fragChk);

			vertChk.free();
			fragChk.free();


			printf("Compiling shaders...\n  ");
			if(!gl_shader::compile(vertShader))
            {
				gl_shader::destroy(vertShader);
				gl_shader::destroy(fragShader);

				mem::free(convName);
							
				ret = false;
				goto end;
			}

			if(!gl_shader::compile(fragShader))
            {
				gl_shader::destroy(vertShader);
				gl_shader::destroy(fragShader);

				mem::free(convName);
							
				ret = false;
				goto end;
			}
						
			gl_program_int program = gl_program::create(convName);

			mem::free(convName);

			gl_program::attachShader(program, vertShader);
			gl_program::attachShader(program, fragShader);

			printf("Linking shaders...\n");
			if(!gl_program::link(program))
            {
				printf(")\n");
				gl_shader::destroy(vertShader);
				gl_shader::destroy(fragShader);
							
				ret = false;
				goto end;
			}

			gl_shader::destroy(vertShader);
			gl_shader::destroy(fragShader);

			printf("  Done.\n)\n");
		}

end:
		for(i = 0; i < shaderFiles.getNumberOfElements(); ++i)
        {
            filename1 = shaderFiles[i];
			mem::free(filename1);
		}
		shaderFiles.free();

		for(i = 0; i < shadersRelation.getNumberOfElements(); ++i)
        {
			shader_relation &existingRelation = shadersRelation[i];
			mem::free(existingRelation.name);
		}
		shadersRelation.free();

		return ret;
	}

	/*
	===================
	resources::shutdown
	===================
	*/
	void resources::shutdown()
	{
		if(m_DeepDataFolder != nullptr)
			mem::free(m_DeepDataFolder);

		if(m_DeepResourcesFolder != nullptr)
			mem::free(m_DeepResourcesFolder);

		if(m_DeepShadersFolder != nullptr)
			mem::free(m_DeepShadersFolder);

		if(m_DeepTexturesFolder != nullptr)
			mem::free(m_DeepTexturesFolder);

		if(m_DeepSoundsFolder != nullptr)
			mem::free(m_DeepSoundsFolder);
	}

	/*
	======================
	resources::loadTexture
	======================
	*/
	gl_texture_int resources::loadTexture(const char *name, uint8_t unit)
	{
		png png;
		wchar_t *filename = string_utils::copy(m_DeepTexturesFolder);
		string_utils::append(&filename, name);

		if(!png.loadAndRead(filename)) {
			mem::free(filename);
			return 0;
		}

		printf("Texture \"%s\" loaded: %s\n", name, png::colorTypeName(png.colorType()));

		mem::free(filename);

		png.applyVerticalMirrorEffect();

		gl_texture_int texture = gl_texture::create();
		gl_texture::bind(texture, unit);

		gl_texture::setTextureWrappingS(gl_texture_wrap::ClampToEdge);
		gl_texture::setTextureWrappingT(gl_texture_wrap::ClampToEdge);
		gl_texture::setTextureFiltering(gl_texture_filter::Nearest);

		mem_ptr image = png.rawImage();
		gl_texture::transmitTexture(image, png.width(), png.height(), png.colorType());
		mem::free(image);

		return texture;
	}

}
