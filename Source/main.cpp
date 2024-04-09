#include <stdio.h>

#include <deep_engine.hpp>
#include <drivers/gl3.hpp>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	480
#define TARGET_MS		16
#define TARGET_FPS		300

void event_callback(deep::window &window, deep::devent e)
{
    switch(e->getType())
    {
        default: break;
        case deep::event_type::MouseMotion:
        {
            if(!window.is_showing_debug_panel())
            {
                deep::scene_id sceneID = deep::scene::get_active_scene_id();
                if(sceneID == deep::badID)
                    break;

                deep::scene *scene = deep::scene::get_scene(sceneID);
                if(scene == nullptr)
                    break;

                deep::camera &camera = deep::scene::get_scene(deep::scene::get_active_scene_id())->get_attached_camera();
                float yaw   = camera.get_yaw();
                float pitch = camera.get_pitch();

                float sensitivity = 0.1f;

                int x = e->getMouseX();
                int y = e->getMouseY();

                int xDiff = x - window.get_width() / 2;
                int yDiff = y - window.get_height() / 2;

                yaw += xDiff * sensitivity;
                pitch -= yDiff * sensitivity;

                if(pitch > 89.0f)
                    pitch = 89.0f;
                if(pitch < -89.0f)
                    pitch = -89.0f;

                camera.set_yaw(yaw);
                camera.set_pitch(pitch);

                window.set_cursor_position(window.get_width() / 2, window.get_height() / 2);

                camera.update_angle_of_view();
            }
        };
    }

    // Appelle le callback par défaut
    deep::window::default_input_callback(window, e);
}

// Fonction appelée en boucle dans la boucle du jeu,
// pour mettre à jour toutes les valeurs du jeu
void update_callback(deep::window & /* win */)
{
    deep::scene_id sceneID = deep::scene::get_active_scene_id();
    deep::entity_collection_id collectionID;
    deep::scene *scene;

    if(sceneID == deep::badID)
        return;

    scene = deep::scene::get_scene(sceneID);
    if(scene == nullptr)
        return;

    collectionID = deep::scene::get_entity_collection(sceneID);

    float cameraSpeed = 0.15f;

    deep::camera &camera = scene->get_attached_camera();
    deep::vec3<float> cameraPos = camera.get_position();
    deep::vec3<float> cameraFront = camera.get_front();
    deep::vec3<float> cameraRight = deep::vec3<float>::normalize(deep::vec3<float>::cross(cameraFront, camera.get_up()));

    deep::entity_manager::entity entity = deep::entity_manager::entity::bad();

    // Augmentation de la vitesse de déplacement de la caméra.
    if(deep::key::isPressed(deep::dkey::LShift))
    {
        cameraSpeed += 0.10f;
    }

    // Déplacer la caméra vers l'avant.
    if(deep::key::isPressed(deep::dkey::Z))
    {
        camera.move_forward(cameraSpeed);
    }

    // Déplacer la caméra vers la gauche.
    if(deep::key::isPressed(deep::dkey::Q))
    {
        camera.move_left(cameraSpeed);
    }

    // Déplacer la caméra vers la droite.
    if(deep::key::isPressed(deep::dkey::D))
    {
        camera.move_right(cameraSpeed);
    }

    // Déplacer la caméra vers l'arrière.
    if(deep::key::isPressed(deep::dkey::S))
    {
        camera.move_backward(cameraSpeed);
    }

    // Fait monter la caméra.
    if(deep::key::isPressed(deep::dkey::Space))
    {
        camera.move_up(cameraSpeed);
    }

    // Fait descendre la caméra.
    if(deep::key::isPressed(deep::dkey::LCtrl))
    {
        camera.move_down(cameraSpeed);
    }

    camera.update_angle_of_view();
}

deep::GL3::gl_id compile_program(const char *programName, deep::ref<deep::text> vertex, deep::ref<deep::text> fragment)
{
    deep::GL3::program_manager *programManager = deep::GL3::program_manager::get_singleton();
    deep::GL3::shader_manager *shaderManager = deep::GL3::shader_manager::get_singleton();

    deep::GL3::gl_id vertexShader = shaderManager->create("vertex_shader", deep::GL3::shader_manager::gl_shader_type::Vertex);
    deep::GL3::gl_id fragmentShader = shaderManager->create("fragment_shader", deep::GL3::shader_manager::gl_shader_type::Fragment);

    shaderManager->load(vertexShader, vertex->get_resource_data(), vertex->get_resource_size());
    shaderManager->load(fragmentShader, fragment->get_resource_data(), fragment->get_resource_size());

    if(!shaderManager->compile(vertexShader))
    {
        deep::core::err() << "Unable to compile default_vert shader\n";
    }

    if(!shaderManager->compile(fragmentShader))
    {
        deep::core::err() << "Unable to compile default_frag shader\n";
    }

    deep::GL3::gl_id program = programManager->create(programName);

    programManager->attach_shader(program, vertexShader);
    programManager->attach_shader(program, fragmentShader);

    if(!programManager->link(program))
    {
        deep::core::err() << "Unable to link shader\n";
    }
    
    shaderManager->destroy(vertexShader);
    shaderManager->destroy(fragmentShader);

    return program;
}

#undef main
int main()
{
    switch(deep::core::init("DeepEngine [" DE_VERSION "]", 0, 0, 0))
    {
        default:
            break;
        case deep::core_init_status::CannotInstantiateObjects:
        {
            fprintf(stderr, "Cannot instanciate objects.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::InstanceAlreadyExists:
        {
            fprintf(stderr, "Process already running.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::NoEnoughDiskSpace:
        {
            fprintf(stderr, "No enough disk space.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::NoEnoughMemory:
        {
            fprintf(stderr, "No enough memory.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotInitCOM:
        {
            fprintf(stderr, "Cannot initialize COM.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotQueryCpuInfo:
        {
            fprintf(stderr, "Cannot query CPU info.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotInitNtDll:
        {
            fprintf(stderr, "Cannot load NTDLL.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotLoadStdStream:
        {
            fprintf(stderr, "Cannot load STD stream.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotLoadEngineSettings:
        {
            fprintf(stderr, "Cannot load engine settings.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotInitResources:
        {
            fprintf(stderr, "Cannot initialize resources.\n");
        } return EXIT_FAILURE;
    }

    deep::core::out() << "pwd: " << deep::core::get_pwd().str() << "\n";

    deep::window win(TARGET_MS, TARGET_FPS);
    win.set_event_callback(event_callback);
    win.set_update_callback(update_callback);

    if(!deep::window::create(win, "DeepEngine [" DE_VERSION "]", deep::size(WINDOW_WIDTH, WINDOW_HEIGHT)))
    {
        deep::core::shutdown();

        return EXIT_FAILURE;
    }

    deep::resource_manager *resourceManager = deep::resource_manager::get_singleton();
    deep::GL3::texture_manager *textureManager = deep::GL3::texture_manager::get_singleton();

    deep::im_gui_window::init(win);

    deep::scene_id sceneID = deep::scene::create_scene("scn_main");

    deep::entity_collection_id collectionID = deep::scene::get_entity_collection(sceneID);

    deep::core::out() <<
        DE_TERM_FG_YELLOW "====================[ " DE_TERM_FG_RED "\\OpenGL/" DE_TERM_FG_YELLOW " ]====================\n" DE_TERM_RESET
        "Version: "                  << deep::GL3::core::query_version() << "\n"
        "Max vertex attribs: "       << deep::GL3::core::query_max_vertex_attribs() << "\n"
        "Max texture image units: "  << deep::GL3::core::query_max_texture_image_units() << "\n"
        "Max texture size: "         << deep::GL3::core::query_max_texture_size() << "\n"
        "Max 3D texture size: "      << deep::GL3::core::query_max_3D_texture_size() << "\n"
        "Max array texture layers: " << deep::GL3::core::query_max_array_texture_layers() << "\n"
        DE_TERM_FG_YELLOW "====================================================\n" DE_TERM_RESET;

    
    deep::ref<deep::image> dirtRes = deep::ref_cast<deep::image>(resourceManager->get_resource("textures.dirt"));
    deep::ref<deep::image> grassBlockSideRes = deep::ref_cast<deep::image>(resourceManager->get_resource("textures.grass_block_side"));
    deep::ref<deep::image> grassBlockTopRes = deep::ref_cast<deep::image>(resourceManager->get_resource("textures.grass_block_top"));

    grassBlockSideRes->add(dirtRes, deep::vec2<uint64_t>(16, 0));
    grassBlockSideRes->add(grassBlockTopRes, deep::vec2<uint64_t>(0, 16));

    deep::ref<deep::text> defaultShaderFragRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.default_frag"));
    deep::ref<deep::text> defaultShaderVertRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.default_vert"));
    deep::ref<deep::text> lightedColoredObjectShaderFragRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.lighted_colored_object_frag"));
    deep::ref<deep::text> lightedObjectShaderVertRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.lighted_object_vert"));
    deep::ref<deep::text> lightedTexturedObjectShaderFragRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.lighted_textured_object_frag"));
    deep::ref<deep::text> lightSourceObjectShaderFragRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.light_source_object_frag"));
    deep::ref<deep::text> postProcessingShaderFragRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.post_processing_frag"));
    deep::ref<deep::text> postProcessingShaderVertRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.post_processing_vert"));
    deep::ref<deep::text> skyboxShaderFragRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.skybox_frag"));
    deep::ref<deep::text> skyboxShaderVertRes = deep::ref_cast<deep::text>(resourceManager->get_resource("shaders.skybox_vert"));

    deep::GL3::gl_id mcTexture = textureManager->create_2D("mc_texture");
    textureManager->bind(mcTexture, 0);
    textureManager->set_texture_wrapping_s(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_t(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_filtering(deep::GL3::texture_manager::gl_texture_filter::Nearest);
    textureManager->transmit_texture(grassBlockSideRes->get_resource_data(), static_cast<int>(grassBlockSideRes->get_width()), static_cast<int>(grassBlockSideRes->get_height()), grassBlockSideRes->get_color_model());

    deep::vec4<float> black(0.0f, 0.0f, 0.0f, 1.0);
    deep::vec4<float> lightCubeColor(1.0f, 1.0f, 1.0f, 1.0);
    deep::vec4<float> lightedCubeColor(1.0f, 0.5f, 0.31f, 1.0f);

    deep::GL3::gl_id defaultProgram = compile_program("default", defaultShaderVertRes, defaultShaderFragRes);
    deep::GL3::gl_id skyboxProgram  = compile_program("skybox", skyboxShaderVertRes, skyboxShaderFragRes);
    deep::GL3::gl_id postProcessProgram = compile_program("post_processing", postProcessingShaderVertRes, postProcessingShaderFragRes);
    deep::GL3::gl_id lightedObjectProgram = compile_program("lighted_colored_object", lightedObjectShaderVertRes, lightedColoredObjectShaderFragRes);
    deep::GL3::gl_id lightSourceObjectProgram = compile_program("light_source_object", lightedObjectShaderVertRes, lightSourceObjectShaderFragRes);
    deep::GL3::gl_id lightedTexturedObjectProgram = compile_program("lighted_textured_object", lightedObjectShaderVertRes, lightedTexturedObjectShaderFragRes);

    deep::GL3::program_manager *programManager = deep::GL3::program_manager::get_singleton();

    programManager->use(defaultProgram);

    int mTrs = deep::GL3::uniform_manager::find(defaultProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    int mRotX = deep::GL3::uniform_manager::find(defaultProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    int mRotY = deep::GL3::uniform_manager::find(defaultProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    int mRotZ = deep::GL3::uniform_manager::find(defaultProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    int mScl = deep::GL3::uniform_manager::find(defaultProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    int view = deep::GL3::uniform_manager::find(defaultProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::mat4x4(0.0f));

    int proj = deep::GL3::uniform_manager::find(defaultProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::mat4x4(0.0f));

    int myTex = deep::GL3::uniform_manager::find(defaultProgram, "myTex");
    if(myTex != -1)
        programManager->add_uniform("myTex", myTex, 0);

    programManager->use(skyboxProgram);

    int skyboxView = deep::GL3::uniform_manager::find(skyboxProgram, "view");
    if(skyboxView != -1)
        programManager->add_uniform("view", skyboxView, deep::mat4x4(0.0f));

    int skyboxProj = deep::GL3::uniform_manager::find(skyboxProgram, "proj");
    if(skyboxProj != -1)
        programManager->add_uniform("proj", skyboxProj, deep::mat4x4(0.0f));

    int skyboxCube = deep::GL3::uniform_manager::find(skyboxProgram, "skybox");
    if(skyboxCube != -1)
        programManager->add_uniform("skybox", skyboxCube, 0);

    // ===== LIGHTED OBJECT ===== //

    deep::vec3<float> lightSourcePos = deep::vec3<float>(-3.5f, 4.0f, -1.0f);

    programManager->use(lightedObjectProgram);
    
    mTrs = deep::GL3::uniform_manager::find(lightedObjectProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    mRotX = deep::GL3::uniform_manager::find(lightedObjectProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    mRotY = deep::GL3::uniform_manager::find(lightedObjectProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    mRotZ = deep::GL3::uniform_manager::find(lightedObjectProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    mScl = deep::GL3::uniform_manager::find(lightedObjectProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    view = deep::GL3::uniform_manager::find(lightedObjectProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::mat4x4(0.0f));

    proj = deep::GL3::uniform_manager::find(lightedObjectProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::mat4x4(0.0f));

    int objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.ambient");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.ambient", objectColorLoc, deep::vec3<float>(1.0f, 0.5f, 0.31f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.diffuse", objectColorLoc, deep::vec3<float>(1.0f, 0.5f, 0.31f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.specular", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.shininess");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.shininess", objectColorLoc, 32.0f);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.position");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.position", objectColorLoc, lightSourcePos);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.ambient");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.ambient", objectColorLoc, deep::vec3<float>(0.2f, 0.2f, 0.2f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.diffuse", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.specular", objectColorLoc, lightCubeColor.rgb());

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deViewPos");
    if(objectColorLoc != -1)
        programManager->add_uniform("deViewPos", objectColorLoc, deep::vec3<float>());

    // ===========================================

    programManager->use(lightedTexturedObjectProgram);
    
    mTrs = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    mRotX = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    mRotY = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    mRotZ = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    mScl = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    view = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::mat4x4(0.0f));

    proj = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::mat4x4(0.0f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deMaterial.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.diffuse", objectColorLoc, 0);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deMaterial.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.specular", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deMaterial.shininess");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.shininess", objectColorLoc, 32.0f);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.position");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.position", objectColorLoc, lightSourcePos);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.ambient");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.ambient", objectColorLoc, deep::vec3<float>(0.2f, 0.2f, 0.2f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.diffuse", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.specular", objectColorLoc, lightCubeColor.rgb());

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deViewPos");
    if(objectColorLoc != -1)
        programManager->add_uniform("deViewPos", objectColorLoc, deep::vec3<float>());

    // ===========================================


    programManager->use(lightSourceObjectProgram);

    mTrs = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    mRotX = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    mRotY = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    mRotZ = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    mScl = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    view = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::mat4x4(0.0f));

    proj = deep::GL3::uniform_manager::find(lightSourceObjectProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::mat4x4(0.0f));

    programManager->use(postProcessProgram);

    int postScreenTexture = deep::GL3::uniform_manager::find(postProcessProgram, "screenTexture");
    if(postScreenTexture != -1)
        programManager->add_uniform("screenTexture", postScreenTexture, 0);

    deep::entity_manager *entityManager = deep::entity_manager::get_singleton();

    deep::graphic::cube_texture_points points;
    points.frontTopLeft.x     = points.backTopLeft.x     = points.rightTopLeft.x     = points.leftTopLeft.x     = 0.0f;
    points.frontTopLeft.y     = points.backTopLeft.y     = points.rightTopLeft.y     = points.leftTopLeft.y     = 1.0f;
    points.frontTopRight.x    = points.backTopRight.x    = points.rightTopRight.x    = points.leftTopRight.x    = 1.0f;
    points.frontTopRight.y    = points.backTopRight.y    = points.rightTopRight.y    = points.leftTopRight.y    = 1.0f;
    points.frontBottomLeft.x  = points.backBottomLeft.x  = points.rightBottomLeft.x  = points.leftBottomLeft.x  = 0.0f;
    points.frontBottomLeft.y  = points.backBottomLeft.y  = points.rightBottomLeft.y  = points.leftBottomLeft.y  = 0.0f;
    points.frontBottomRight.x = points.backBottomRight.x = points.rightBottomRight.x = points.leftBottomRight.x = 1.0f;
    points.frontBottomRight.y = points.backBottomRight.y = points.rightBottomRight.y = points.leftBottomRight.y = 0.0f;

    points.bottomTopLeft.x     = 0.0f;
    points.bottomTopLeft.y     = 1.0f;
    points.bottomTopRight.x    = 1.0f;
    points.bottomTopRight.y    = 1.0f;
    points.bottomBottomLeft.x  = 0.0f;
    points.bottomBottomLeft.y  = 0.0f;
    points.bottomBottomRight.x = 1.0f;
    points.bottomBottomRight.y = 0.0f;

    points.topTopLeft.x     = 0.0f;
    points.topTopLeft.y     = 1.0f;
    points.topTopRight.x    = 1.0f;
    points.topTopRight.y    = 1.0f;
    points.topBottomLeft.x  = 0.0f;
    points.topBottomLeft.y  = 0.0f;
    points.topBottomRight.x = 1.0f;
    points.topBottomRight.y = 0.0f;

    deep::polygon lightedCube = deep::graphic::create_cube("lighted_cube", lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor);
    deep::polygon testLightedCube = deep::graphic::create_cube("test_lighted_cube", lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, points);
    deep::polygon lightSourceCube = deep::graphic::create_cube("light_source_cube", lightCubeColor, lightCubeColor, lightCubeColor, lightCubeColor, lightCubeColor, lightCubeColor);

    deep::color_material *lightedMaterial1 = deep::mem::alloc_type<deep::color_material>(lightedObjectProgram, deep::vec3<float>(0.75f, 0.32f, 0.67f), deep::vec3<float>(0.75f, 0.32f, 0.67f), deep::vec3<float>(0.5f, 0.5f, 0.5f), 32.0f);
    deep::texture_material *lightedMaterial2 = deep::mem::alloc_type<deep::texture_material>(lightedTexturedObjectProgram, mcTexture, deep::vec3<float>(0.5f, 0.5f, 0.5f), 64.0f);
    deep::color_material *lightSourceMaterial = deep::mem::alloc_type<deep::color_material>(lightSourceObjectProgram, deep::vec3<float>(1.0f, 1.0f, 1.0f), deep::vec3<float>(1.0f, 1.0f, 1.0f), deep::vec3<float>(0.5f, 0.5f, 0.5f), 32.0f);

    entityManager->create_entity("lighted_entity", collectionID, lightedCube, deep::vec3<float>(0.0f, 0.0f, -5.0f), deep::vec3<float>(3.0f, 3.0f, 3.0f), lightedMaterial1);
    entityManager->create_entity("test_lighted_entity", collectionID, testLightedCube, deep::vec3<float>(0.0f, 6.0f, -5.0f), deep::vec3<float>(3.0f, 3.0f, 3.0f), lightedMaterial2);
    entityManager->create_entity("light_source_entity", collectionID, lightSourceCube, lightSourcePos, deep::vec3<float>(1.0f, 1.0f, 1.0f), lightSourceMaterial);

    //== deep::component_id testDrawableComponentID 

    // Affiche la skybox à la fin pour optimiser les appelles au fragment shader = FPS++
    //deep::entity_manager::entity skyboxEnt = deep::graphic::create_cubemap("skybox", skyboxProgram, collectionID, deep::fvec3(0.0f, 0.0f, 0.0f), 5005.0f, 105.0f, 2005.0f, skybox, 0);

    deep::scene::set_active_scene(sceneID);

    win.set_showing_debug_panel(true);

    // Lance la boucle du jeu, bloquant.
    win.run();

    programManager->destroy_all_programs();
    resourceManager->shutdown();
    deep::core::shutdown();

    win.destroy();

    _CrtDumpMemoryLeaks();

    return EXIT_SUCCESS;
}
