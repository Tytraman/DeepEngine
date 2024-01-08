#include <stdio.h>
#include <math.h>
#include <string>

#include <DE/def.hpp>
#include <DE/core.hpp>
#include <DE/error.hpp>
#include <DE/window.hpp>
#include <DE/key.hpp>
#include <DE/sys_win.hpp>
#include <DE/logger.hpp>
#include <DE/debug.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>
#include <DE/ecs/scene.hpp>
#include <DE/graphics/graphic.hpp>
#include <DE/image/png.hpp>
#include <DE/image/bmp.hpp>
#include <DE/resources.hpp>
#include <DE/memory/hash_table.hpp>
#include <DE/memory/memory.hpp>
#include <DE/string.hpp>
#include <DE/file/file_object.hpp>
#include <DE/memory/settings.hpp>
#include <DE/safe_integer.hpp>
#include <DE/os/COM.hpp>
#include <DE/hardware/cpu.hpp>

extern "C"
{

#include <DE/memory/windows_internal.h>

}

#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

#include <DE/imgui/deimgui.hpp>

#include <DE/drivers/opengl/core.hpp>
#include <DE/drivers/opengl/shader.hpp>
#include <DE/drivers/opengl/uniform.hpp>

#define WINDOW_WIDTH	1745
#define WINDOW_HEIGHT	980
#define TARGET_MS		16
#define TARGET_FPS		300

#define SCROLL_SCALE_FACTOR_UP   1.10f
#define SCROLL_SCALE_FACTOR_DOWN 0.90f
#define SCROLL_ROTATE_FACTOR 3.0f

#define PLAYER_SPAWN_X 200.0f
#define PLAYER_SPAWN_Y 200.0f
#define PLAYER_GRAVITY 0.30f

#define RECT1_SPAWN_X  400.0f
#define RECT1_SPAWN_Y  200.0f

void event_callback(deep::window &window, deep::devent e)
{
    switch(e->getType())
    {
        default: break;
        case deep::event_type::MouseMotion:
        {
            if(!window.is_showing_debug_panel())
            {
                deep::scene_id sceneID = deep::scene::getActiveSceneID();
                if(sceneID == deep::badID)
                    break;

                deep::scene *scene = deep::scene::getScene(sceneID);
                if(scene == nullptr)
                    break;

                deep::Camera &camera = deep::scene::getScene(deep::scene::getActiveSceneID())->getCamera();
                float yaw   = camera.yaw();
                float pitch = camera.pitch();

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

                camera.setYaw(yaw);
                camera.setPitch(pitch);

                window.set_cursor_position(window.get_width() / 2, window.get_height() / 2);

                camera.updateAngleOfView();
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
    deep::scene_id sceneID = deep::scene::getActiveSceneID();
    deep::entity_collection_id collectionID;
    deep::scene *scene;

    if(sceneID == deep::badID)
        return;

    scene = deep::scene::getScene(sceneID);
    if(scene == nullptr)
        return;

    collectionID = deep::scene::getEntityCollection(sceneID);

    float cameraSpeed = 0.05f;

    deep::Camera &camera = scene->getCamera();
    deep::fvec3 cameraPos = camera.position();
    deep::fvec3 cameraFront = camera.front();
    deep::fvec3 cameraRight = deep::fvec3::normalize(deep::fvec3::cross(cameraFront, camera.up()));

    deep::entity entity = deep::entity::bad();

    // Augmentation de la vitesse de déplacement de la caméra.
    if(deep::key::isPressed(deep::dkey::LShift))
    {
        cameraSpeed += 0.10f;
    }

    // Déplacer la caméra vers l'avant.
    if(deep::key::isPressed(deep::dkey::Z))
    {
        camera.moveForward(cameraSpeed);
    }

    // Déplacer la caméra vers la gauche.
    if(deep::key::isPressed(deep::dkey::Q))
    {
        camera.moveLeft(cameraSpeed);
    }

    // Déplacer la caméra vers la droite.
    if(deep::key::isPressed(deep::dkey::D))
    {
        camera.moveRight(cameraSpeed);
    }

    // Déplacer la caméra vers l'arrière.
    if(deep::key::isPressed(deep::dkey::S))
    {
        camera.moveBackward(cameraSpeed);
    }

    // Fait monter la caméra.
    if(deep::key::isPressed(deep::dkey::Space))
    {
        camera.moveUp(cameraSpeed);
    }

    // Fait descendre la caméra.
    if(deep::key::isPressed(deep::dkey::LCtrl))
    {
        camera.moveDown(cameraSpeed);
    }

    camera.updateAngleOfView();
}

#include <iostream>

#undef main
int main()
{
    deep::error_status errorStatus;

    switch(deep::core::init("DeepEngine [" DE_VERSION "]", 0, 0, 0))
    {
        default:
            break;
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
    }

    printf("pwd: %s\n", deep::core::getPwd());

    deep::window win(TARGET_MS, TARGET_FPS);
    win.set_event_callback(event_callback);
    win.set_update_callback(update_callback);
    errorStatus = deep::window::create(win, "DeepEngine [" DE_VERSION "]", deep::size(WINDOW_WIDTH, WINDOW_HEIGHT));

    if(errorStatus != deep::error_status::NoError)
    {
        deep::error::printError(errorStatus);
        deep::core::shutdown();
        return 1;
    }

    deep::engine_settings *engineSettings = deep::engine_settings::get_singleton();
    deep::resource_manager *resourcesManager = deep::resource_manager::get_singleton();
    deep::GL3::texture_manager *textureManager = deep::GL3::texture_manager::get_singleton();

    // Charge les ressources nécessaires au jeu.
    if(!resourcesManager->init(engineSettings->get_resources_directory().str()))
    {
        deep::core::shutdown();
        return 1;
    }

    deep::im_gui_window::init(win);

    deep::scene_id sceneID = deep::scene::createScene("scn_main");

    deep::entity_collection_id collectionID = deep::scene::getEntityCollection(sceneID);

    printf(
        DE_TERM_FG_YELLOW "====================[ " DE_TERM_FG_RED "\\OpenGL/" DE_TERM_FG_YELLOW " ]====================\n" DE_TERM_RESET
        "Version: %s\n"
        "Max vertex attribs: %d\n"
        "Max texture image units: %d\n"
        DE_TERM_FG_YELLOW "====================================================\n" DE_TERM_RESET,
        
        deep::GL3::core::query_version(),
        deep::GL3::core::query_max_vertex_attribs(),
        deep::GL3::core::query_max_texture_image_units()
    );

    deep::bmp mcGrass, mcGrassTop, mcDirt;
    if(!resourcesManager->loadBMP("grass_block_side.bmp", mcGrass))
    {
        fprintf(stderr, "Unable to load resource.\n");
        return EXIT_FAILURE;
    }

    if(!resourcesManager->loadBMP("grass_block_top.bmp", mcGrassTop))
    {
        fprintf(stderr, "Unable to load resource.\n");
        return EXIT_FAILURE;
    }

    if(!resourcesManager->loadBMP("dirt.bmp", mcDirt))
    {
        fprintf(stderr, "Unable to load resource.\n");
        return EXIT_FAILURE;
    }

    if(!mcGrass.add(mcGrassTop))
    {
        fprintf(stderr, "Unable to add bmp image.\n");
        return EXIT_FAILURE;
    }

    if(!mcGrass.add(mcDirt, deep::vec2(0, mcGrass.get_height())))
    {
        fprintf(stderr, "Unable to add bmp image.\n");
        return EXIT_FAILURE;
    }

    deep::GL3::gl_id mcTexture = textureManager->create_2D("mc_texture");
    textureManager->bind(mcTexture, 0);
    textureManager->set_texture_wrapping_s(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_t(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_filtering(deep::GL3::texture_manager::gl_texture_filter::Nearest);
    textureManager->transmit_texture(mcGrass.get_image(), mcGrass.get_width(), mcGrass.get_height(), mcGrass.get_color_type());

    deep::bmp skyboxLeft;
    skyboxLeft.create_from_file("..\\resources\\textures\\skybox_left.bmp");
    skyboxLeft.vertical_flip();
    skyboxLeft.horizontal_flip();

    deep::bmp skyboxFront;
    skyboxFront.create_from_file("..\\resources\\textures\\skybox_front.bmp");
    skyboxFront.vertical_flip();
    skyboxFront.horizontal_flip();

    deep::bmp skyboxRight;
    skyboxRight.create_from_file("..\\resources\\textures\\skybox_right.bmp");
    skyboxRight.vertical_flip();
    skyboxRight.horizontal_flip();

    deep::bmp skyboxBack;
    skyboxBack.create_from_file("..\\resources\\textures\\skybox_back.bmp");
    skyboxBack.vertical_flip();
    skyboxBack.horizontal_flip();

    deep::bmp skyboxBottom;
    skyboxBottom.create_from_file("..\\resources\\textures\\skybox_bottom.bmp");

    deep::bmp skyboxTop;
    skyboxTop.create_from_file("..\\resources\\textures\\skybox_top.bmp");

    deep::GL3::gl_id skybox = textureManager->create_2D("skybox");
    textureManager->bind_cubemaps(skybox);
    textureManager->set_texture_wrapping_s_cubemaps(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_t_cubemaps(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_r_cubemaps(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_filtering_cubemaps(deep::GL3::texture_manager::gl_texture_filter::Linear);
    textureManager->transmit_texture_cubemaps(skyboxLeft.get_image(), skyboxFront.get_image(), skyboxRight.get_image(), skyboxBack.get_image(), skyboxBottom.get_image(), skyboxTop.get_image(), skyboxTop.get_width(), skyboxTop.get_height(), skyboxTop.get_color_type());

    deep::GL3::program_manager *programManager = deep::GL3::program_manager::get_singleton();

    deep::hash_function hash = programManager->get_hash_function();
    deep::GL3::gl_id defaultProgram = hash("default");
    deep::GL3::gl_id skyboxProgram  = hash("skybox");
    deep::GL3::gl_id postProcessProgram = hash("post_processing");

    programManager->use(defaultProgram);

    int mTrs = deep::GL3::uniform_manager::find(defaultProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::fvec3(0.0f));

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
        programManager->add_uniform("mScl", mScl, deep::fvec3(0.0f));

    int view = deep::GL3::uniform_manager::find(defaultProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::fmat4x4(0.0f));

    int proj = deep::GL3::uniform_manager::find(defaultProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::fmat4x4(0.0f));

    int myTex = deep::GL3::uniform_manager::find(defaultProgram, "myTex");
    if(myTex != -1)
        programManager->add_uniform("myTex", myTex, 0);

    programManager->use(skyboxProgram);

    int skyboxView = deep::GL3::uniform_manager::find(skyboxProgram, "view");
    if(skyboxView != -1)
        programManager->add_uniform("view", skyboxView, deep::fmat4x4(0.0f));

    int skyboxProj = deep::GL3::uniform_manager::find(skyboxProgram, "proj");
    if(skyboxProj != -1)
        programManager->add_uniform("proj", skyboxProj, deep::fmat4x4(0.0f));

    int skyboxCube = deep::GL3::uniform_manager::find(skyboxProgram, "skybox");
    if(skyboxCube != -1)
        programManager->add_uniform("skybox", skyboxCube, 0);

    programManager->use(postProcessProgram);

    int postScreenTexture = deep::GL3::uniform_manager::find(postProcessProgram, "screenTexture");
    if(postScreenTexture != -1)
        programManager->add_uniform("screenTexture", postScreenTexture, 0);

    deep::colora white(255, 255, 255, 255);

    size_t i, j;

    /*for(i = 0; i < 50; ++i)
    {
        for(j = 0; j < 50; ++j)
        {
            de::string name = "3d_rect_";
            name.append(std::to_string(i + j).c_str());

            de::entity ent = de::graphic::create_3D_rectangle_texture(
                name.str(),
                defaultProgram,
                collectionID,
                de::fvec3(i, 0.0f, j),
                1.0f,
                1.0f,
                1.0f,
                white,
                white,
                white,
                white,
                white,
                white,
                mcGrassSide,
                0
            );
        }
    }*/

    deep::entity_manager *entityManager = deep::entity_manager::get_singleton();

    deep::graphic::cube_texture_points points;
    points.frontTopLeft.x     = points.backTopLeft.x     = points.rightTopLeft.x     = points.leftTopLeft.x     = 0.0f;
    points.frontTopLeft.y     = points.backTopLeft.y     = points.rightTopLeft.y     = points.leftTopLeft.y     = 0.5f;
    points.frontTopRight.x    = points.backTopRight.x    = points.rightTopRight.x    = points.leftTopRight.x    = 0.5f;
    points.frontTopRight.y    = points.backTopRight.y    = points.rightTopRight.y    = points.leftTopRight.y    = 0.5f;
    points.frontBottomLeft.x  = points.backBottomLeft.x  = points.rightBottomLeft.x  = points.leftBottomLeft.x  = 0.0f;
    points.frontBottomLeft.y  = points.backBottomLeft.y  = points.rightBottomLeft.y  = points.leftBottomLeft.y  = 0.0f;
    points.frontBottomRight.x = points.backBottomRight.x = points.rightBottomRight.x = points.leftBottomRight.x = 0.5f;
    points.frontBottomRight.y = points.backBottomRight.y = points.rightBottomRight.y = points.leftBottomRight.y = 0.0f;

    points.bottomTopLeft.x     = 0.0f;
    points.bottomTopLeft.y     = 1.0f;
    points.bottomTopRight.x    = 0.5f;
    points.bottomTopRight.y    = 1.0f;
    points.bottomBottomLeft.x  = 0.0f;
    points.bottomBottomLeft.y  = 0.5f;
    points.bottomBottomRight.x = 0.5f;
    points.bottomBottomRight.y = 0.5f;

    points.topTopLeft.x     = 0.5f;
    points.topTopLeft.y     = 0.5f;
    points.topTopRight.x    = 1.0f;
    points.topTopRight.y    = 0.5f;
    points.topBottomLeft.x  = 0.5f;
    points.topBottomLeft.y  = 0.0f;
    points.topBottomRight.x = 1.0f;
    points.topBottomRight.y = 0.0f;

    deep::polygon pol = deep::graphic::create_cube("grass_cube", white, white, white, white, white, white, points);

    for(i = 0; i < 50; ++i)
    {
        for(j = 0; j < 50; ++j)
        {
            deep::entity entTest = entityManager->create_entity(collectionID, pol, defaultProgram, deep::fvec3(i * 5.0f, 0.0f, j * 5.0f), deep::fvec3(3.0f, 3.0f, 3.0f), mcTexture);
        }
    }

    // Affiche la skybox à la fin pour optimiser les appelles au fragment shader = FPS++
    deep::entity skyboxEnt = deep::graphic::create_cubemap("skybox", skyboxProgram, collectionID, deep::fvec3(0.0f, 0.0f, 0.0f), 5005.0f, 105.0f, 2005.0f, skybox, 0);

    deep::scene::setActiveScene(sceneID);

    win.set_showing_debug_panel(true);

    // Lance la boucle du jeu, bloquant.
    win.run();

    programManager->destroy_all_programs();
    resourcesManager->shutdown();
    deep::core::shutdown();

    win.destroy();

    _CrtDumpMemoryLeaks();

    printf(DE_TERM_FG_RED "~Good-bye~" DE_TERM_RESET "\n");

    return EXIT_SUCCESS;
}
