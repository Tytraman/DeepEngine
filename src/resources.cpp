#include <DE/resources.hpp>
#include <DE/string_utils.hpp>
#include <DE/memory/memory.hpp>
#include <DE/file.hpp>
#include <DE/stream.hpp>
#include <DE/rendering/opengl_utils.hpp>

#include <stdio.h>

#if DE_WINDOWS
// Permet de récupérer les dossiers spéciaux sous Windows.
#include <shlobj_core.h>
#endif

namespace de {

	struct ShaderRelation {
		wchar_t *name;
		size_t vertexShaderIndex;
		size_t fragmentShaderIndex;
	};

	wchar_t *Resources::m_DeepDataFolder      = nullptr;
	wchar_t *Resources::m_DeepResourcesFolder = nullptr;
	wchar_t *Resources::m_DeepShadersFolder   = nullptr;
	wchar_t *Resources::m_DeepTexturesFolder  = nullptr;
	wchar_t *Resources::m_DeepSoundsFolder    = nullptr;

	bool enum_shader_files_callback(wchar_t *filename, bool isDirectory, void *args)
	{
		if(!isDirectory) {
			List *list = (List *) args;

			if(
				StringUtils::endsWith(filename, L".vert") ||
				StringUtils::endsWith(filename, L".frag")
				) {

				wchar_t *p = StringUtils::copy(filename);
				list->addCopy(&p);
			}
		}

		return true;
	}

	/*
	===============
	Resources::init
	===============
	*/
	bool Resources::init()
	{
		bool ret = true;
		WCHAR *filepath;
		HRESULT hResult = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &filepath);
		if(hResult != S_OK)
			return false;

		m_DeepDataFolder = StringUtils::copy(filepath);

		// Libère la mémoire allouée par la fonction 'SHGetKnownFolderPath'.
		CoTaskMemFree(filepath);

		StringUtils::append(&m_DeepDataFolder, L"\\CreationUI\\");

		m_DeepResourcesFolder = StringUtils::copy(m_DeepDataFolder);
		StringUtils::append(&m_DeepResourcesFolder, L"resources\\");

		m_DeepShadersFolder = StringUtils::copy(m_DeepResourcesFolder);
		StringUtils::append(&m_DeepShadersFolder, L"shaders\\");

		m_DeepTexturesFolder = StringUtils::copy(m_DeepResourcesFolder);
		StringUtils::append(&m_DeepTexturesFolder, L"textures\\");

		m_DeepSoundsFolder = StringUtils::copy(m_DeepResourcesFolder);
		StringUtils::append(&m_DeepSoundsFolder, L"sounds\\");

		// Création des dossiers.
		CreateDirectoryW(m_DeepDataFolder, NULL);
		CreateDirectoryW(m_DeepResourcesFolder, NULL);
		CreateDirectoryW(m_DeepShadersFolder, NULL);
		CreateDirectoryW(m_DeepTexturesFolder, NULL);
		CreateDirectoryW(m_DeepSoundsFolder, NULL);

		List shaderFiles(sizeof(wchar_t *));

		// Récupère tous les "Vertex Shaders" et les "Fragment Shaders".
		File::enumFiles(m_DeepShadersFolder, enum_shader_files_callback, &shaderFiles);
		size_t numberOfShaders = shaderFiles.getNumberOfElements();
		size_t i, j;
		wchar_t *filename1, *filename2;

		printf("Found %llu shaders.\n", numberOfShaders);

		List shadersRelation(sizeof(ShaderRelation));
		
		ShaderRelation *existingRelation;

		// Parcourt tous les shaders et fait le lien entre ceux qui ont le même nom.
		for(i = 0; i < numberOfShaders; ++i) {
			if(shaderFiles.getCopy(i, &filename1)) {
				// Retire l'extension du nom du fichier.
				size_t dotIndex = StringUtils::lastIndex(filename1, L'.');
				wchar_t *extension = filename1 + dotIndex + 1;
				if(dotIndex > 0)
					filename1[dotIndex] = L'\0';

				size_t numberOfRelations = shadersRelation.getNumberOfElements();
				bool foundRelation = false;

				// Parcourt la liste des relations pour voir si un shader a déjà ce nom.
				for(j = 0; j < numberOfRelations; ++j) {
					existingRelation = (ShaderRelation *) shadersRelation.getPtr(j);
					if(existingRelation != nullptr) {
						// Si une relation possède déjà le nom du fichier.
						if(StringUtils::equals(filename1, existingRelation->name)) {
							foundRelation = true;

							if(StringUtils::equals(extension, L"vert"))
								existingRelation->vertexShaderIndex   = i;
							else if(StringUtils::equals(extension, L"frag"))
								existingRelation->fragmentShaderIndex = i;

							break;
						}
					}
				}

				// Si aucune relation ne possède le nom du fichier, alors il faut la créer.
				if(!foundRelation) {
					ShaderRelation relation;
					wchar_t *filenameCopy = StringUtils::copy(filename1);
					relation.name = filenameCopy;
					if(StringUtils::equals(extension, L"vert")) {
						relation.vertexShaderIndex   = i;
						relation.fragmentShaderIndex = -1;
					}else if(StringUtils::equals(extension, L"frag")) {
						relation.fragmentShaderIndex = i;
						relation.vertexShaderIndex   = -1;
					}else {
						relation.vertexShaderIndex   = -1;
						relation.fragmentShaderIndex = -1;
					}

					shadersRelation.addCopy(&relation);
				}

				if(dotIndex > 0)
					filename1[dotIndex] = L'.';
			}
		}

		wchar_t *vertexShaderFilename;
		wchar_t *fragmentShaderFilename;

		// Maintenant que la liaison entre les différents shaders est faite, il faut les compiler et les linker ensembles.
		for(i = 0; i < shadersRelation.getNumberOfElements(); ++i) {
			existingRelation = (ShaderRelation *) shadersRelation.getPtr(i);
			if(existingRelation != nullptr) {
				// Si un élément n'a pas de relation alors on l'ignore, triste pour les célibataires :'(
				if(existingRelation->vertexShaderIndex == -1 || existingRelation->fragmentShaderIndex == -1)
					continue;

				if(shaderFiles.getCopy(existingRelation->vertexShaderIndex, &filename1)) {
					if(shaderFiles.getCopy(existingRelation->fragmentShaderIndex, &filename2)) {
						vertexShaderFilename = StringUtils::copy(m_DeepShadersFolder);
						StringUtils::append(&vertexShaderFilename, filename1);

						fragmentShaderFilename = StringUtils::copy(m_DeepShadersFolder);
						StringUtils::append(&fragmentShaderFilename, filename2);

						printf("%(\n  %ls\n  %ls\n  Opening files...\n  ", vertexShaderFilename, fragmentShaderFilename);

						InputFileStream vis(vertexShaderFilename);
						if(!vis.openW()) {
							fprintf(stderr, "Unable to open vertex shader file.\n");
							printf(")\n");

							mem::free(vertexShaderFilename);
							mem::free(fragmentShaderFilename);

							ret = false;
							goto end;
						}

						InputFileStream fis(fragmentShaderFilename);
						if(!fis.openW()) {
							fprintf(stderr, "Unable to open fragment shader file.\n");
							printf(")\n");

							vis.close();

							mem::free(vertexShaderFilename);
							mem::free(fragmentShaderFilename);

							ret = false;
							goto end;
						}

						MemoryChunk vertChk;
						MemoryChunk::alloc(vertChk, vis.getFileSize() + 1);
						MemoryChunk fragChk;
						MemoryChunk::alloc(fragChk, fis.getFileSize() + 1);
						size_t bytesRead;

						printf("Reading files...\n  ");

						if(!vis.readAll(vertChk.data(), &bytesRead)) {
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

						if(!fis.readAll(fragChk.data(), &bytesRead)) {
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

						char *convName = (char *) mem::alloc(StringUtils::length(existingRelation->name) + 1);
						StringUtils::wcharToChar(convName, existingRelation->name);

						mem::free(vertexShaderFilename);
						mem::free(fragmentShaderFilename);

						gl_shader vertShader = GLShader::create(GLShaderType::Vertex);
						gl_shader fragShader = GLShader::create(GLShaderType::Fragment);

						GLShader::load(vertShader, vertChk);
						GLShader::load(fragShader, fragChk);

						vertChk.free();
						fragChk.free();


						printf("Compiling shaders...\n  ");
						if(!GLShader::compile(vertShader)) {
							GLShader::destroy(vertShader);
							GLShader::destroy(fragShader);

							mem::free(convName);
							
							ret = false;
							goto end;
						}

						if(!GLShader::compile(fragShader)) {
							GLShader::destroy(vertShader);
							GLShader::destroy(fragShader);

							mem::free(convName);
							
							ret = false;
							goto end;
						}
						
						gl_program program = GLProgram::create(convName);

						mem::free(convName);

						GLProgram::attachShader(program, vertShader);
						GLProgram::attachShader(program, fragShader);

						printf("Linking shaders...\n");
						if(!GLProgram::link(program)) {
							printf(")\n");
							GLShader::destroy(vertShader);
							GLShader::destroy(fragShader);
							
							ret = false;
							goto end;
						}

						GLShader::destroy(vertShader);
						GLShader::destroy(fragShader);

						printf("  Done.\n)\n");
					}
				}
			}
		}

end:
		for(i = 0; i < shaderFiles.getNumberOfElements(); ++i) {
			if(shaderFiles.getCopy(i, &filename1))
				mem::free(filename1);
		}
		shaderFiles.free();

		for(i = 0; i < shadersRelation.getNumberOfElements(); ++i) {
			existingRelation = (ShaderRelation *) shadersRelation.getPtr(i);
			if(existingRelation != nullptr)
				mem::free(existingRelation->name);
		}
		shadersRelation.free();

		return ret;
	}

	/*
	===================
	Resources::shutdown
	===================
	*/
	void Resources::shutdown()
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

}
