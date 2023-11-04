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

extern "C" {

#include <DE/memory/windows_internal.h>

}

#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

#include <DE/imgui/deimgui.hpp>

#include <DE/rendering/opengl_utils.hpp>

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

void event_callback(de::window &window, de::devent e)
{
	switch(e->getType())
    {
		default: break;
		case de::event_type::MouseMotion:
        {
			if(!window.isShowingDebugPanel())
            {
				de::scene_id sceneID = de::scene::getActiveSceneID();
				if(sceneID == de::badID)
					break;

				de::scene *scene = de::scene::getScene(sceneID);
				if(scene == nullptr)
					break;

				de::Camera &camera = de::scene::getScene(de::scene::getActiveSceneID())->getCamera();
				float yaw   = camera.yaw();
				float pitch = camera.pitch();

				float sensitivity = 0.1f;

				int x = e->getMouseX();
				int y = e->getMouseY();

				int xDiff = x - window.getWidth() / 2;
				int yDiff = y - window.getHeight() / 2;

				yaw += xDiff * sensitivity;
				pitch -= yDiff * sensitivity;

				if(pitch > 89.0f)
					pitch = 89.0f;
				if(pitch < -89.0f)
					pitch = -89.0f;

				camera.setYaw(yaw);
				camera.setPitch(pitch);

				window.setCursorPos(window.getWidth() / 2, window.getHeight() / 2);

				camera.updateAngleOfView();
			}
		};
	}

	// Appelle le callback par défaut
	de::window::defaultInputCallback(window, e);
}

// Fonction appelée en boucle dans la boucle du jeu,
// pour mettre à jour toutes les valeurs du jeu
void update_callback(de::window &win)
{
	de::debug::addFunctionToCallbackList(DE_FUNCTION_NAME);

	de::scene_id sceneID = de::scene::getActiveSceneID();
	de::entity_collection_id collectionID;
	de::scene *scene;

	if(sceneID == de::badID)
		goto end;

	scene = de::scene::getScene(sceneID);
	if(scene == nullptr)
		goto end;

	collectionID = de::scene::getEntityCollection(sceneID);

	float cameraSpeed = 0.05f;

	de::Camera &camera = scene->getCamera();
	de::fvec3 cameraPos = camera.position();
	de::fvec3 cameraFront = camera.front();
	de::fvec3 cameraRight = de::fvec3::normalize(de::fvec3::cross(cameraFront, camera.up()));

	size_t entityIndex;
	de::entity entity = de::entity::bad();

	// Augmentation de la vitesse de déplacement de la caméra.
	if(de::key::isPressed(de::dkey::LShift))
    {
		cameraSpeed += 0.10f;
	}

	// Déplacer la caméra vers l'avant.
	if(de::key::isPressed(de::dkey::Z))
    {
		camera.moveForward(cameraSpeed);
	}

	// Déplacer la caméra vers la gauche.
	if(de::key::isPressed(de::dkey::Q))
    {
		camera.moveLeft(cameraSpeed);
	}

	// Déplacer la caméra vers la droite.
	if(de::key::isPressed(de::dkey::D))
    {
		camera.moveRight(cameraSpeed);
	}

	// Déplacer la caméra vers l'arrière.
	if(de::key::isPressed(de::dkey::S))
    {
		camera.moveBackward(cameraSpeed);
	}

	// Fait monter la caméra.
	if(de::key::isPressed(de::dkey::Space))
    {
		camera.moveUp(cameraSpeed);
	}

	// Fait descendre la caméra.
	if(de::key::isPressed(de::dkey::LCtrl))
    {
		camera.moveDown(cameraSpeed);
	}

	camera.updateAngleOfView();

end:

	de::debug::removeFunctionFromCallbackList();
}

void collider_callback(de::entity_collection_id collectionID, de::entity_id entity1, de::entity_id entity2, const de::fvec2 &difference, const de::rect &collision)
{
	de::scene_id sceneID = de::scene::getActiveSceneID();

//	// Diff permet de savoir dans quelle direction le joueur va.
//	de::fvec2 diff = difference;
//
//	// Vérifie si une des entités est le joueur ou le rectangle 1.
//	if(entity1 == g_Player.getEntityID()) {
//		playerID = entity1;
//
//		if(entity2 == g_Rect1.getEntityID()) {
//			rect1ID = entity2;
//		}else
//			rect1ID = de::badID;
//	}else if(entity2 == g_Player.getEntityID()) {
//		playerID = entity2;
//
//		if(entity1 == g_Rect1.getEntityID())
//			rect1ID = entity1;
//		else
//			rect1ID = de::badID;
//
//		diff = de::fvec2::inv(diff);
//	}else {
//		playerID = de::badID;
//
//		if(entity1 == g_Rect1.getEntityID()) {
//			rect1ID = entity1;
//		}else if(entity2 == g_Rect1.getEntityID()) {
//			rect1ID = entity2;
//			diff = de::fvec2::inv(diff);
//		}else
//			rect1ID = de::badID;
//	}
//
//rewatch:
//
//
//	// Collision entre le joueur et le rectangle 1.
//	if(playerID != de::badID && rect1ID != de::badID) {
//		if(g_CollisionPlayerRectEnabled) {
//			if(diff.y <= 0 || diff.y <= fabs(diff.x)) {
//				playerID = de::badID;
//				diff = de::fvec2::inv(diff);
//				goto rewatch;
//			}else
//				g_CollisionPlayerRectEnabled = false;
//		}
//	}
//	// Si le joueur est entré en collision avec autre chose.
//	else if(playerID != de::badID) {
//		de::component_id playerTransformationComponentID = de::EntityManager::getComponentID(g_Player, de::TransformationComponentType);
//		de::component_id playerColliderComponentID = de::EntityManager::getComponentID(g_Player, de::ColliderComponentType);
//		de::component_id playerVelCpnID = de::EntityManager::getComponentID(g_Player, de::VelocityComponentType);
//		de::component_id playerAccCpnID = de::EntityManager::getComponentID(g_Player, de::AccelerationComponentType);
//
//		de::TransformationComponent *playerTransformationComponent = de::ComponentManager::getTransformationComponent(playerTransformationComponentID);
//		de::ColliderComponent *playerColliderComponent = de::ComponentManager::getColliderComponent(playerColliderComponentID);
//		de::VelocityComponent *playerVelCpn = de::ComponentManager::getVelocityComponent(playerVelCpnID);
//		de::AccelerationComponent *playerAccCpn = de::ComponentManager::getAccelerationComponent(playerAccCpnID);
//
//		de::fvec3 playerTranslation  = playerTransformationComponent->getTranslation();
//		de::fvec3 playerVel = playerVelCpn->getVelocity();
//
//		float colW = fabs(collision.w);
//		float colH = fabs(collision.h);
//
//		// Une pratique assez courante de repositionner un objet pouvant se déplacer lorsqu'il touche un objet statique
//		// est de le déplacer par rapport à l'axe le plus petit du rectangle de collision,
//		// ainsi, si la largeur du rectangle de collision est plus petite que la hauteur, l'objet sera déplacé sur l'axe X, inversemment dans le cas
//		// où la hauteur est plus petite que la largeur.
//
//		// Si la collision est plus petite en largeur, on déplace le joueur sur l'axe X.
//		if(colW < colH) {
//			playerVel.x = 0.0f;
//			playerAccCpn->acceleration.x = 0.0f;
//			// Si le joueur se déplace vers la droite.
//			if(diff.x > 0) {
//				playerTranslation.x -= colW;
//				playerTransformationComponent->setTranslation(playerTranslation);
//				playerColliderComponent->contour.pos.x -= colW;
//			// Si le joueur se déplace vers la gauche.
//			}else {
//				playerTranslation.x += colW;
//				playerTransformationComponent->setTranslation(playerTranslation);
//				playerColliderComponent->contour.pos.x += colW;
//			}
//		}
//		// Sinon on déplace le joueur sur l'axe Y.
//		else {
//			// Si le joueur se déplace vers le bas.
//			if(diff.y > 0) {
//				playerTranslation.y -= colH;
//				playerTransformationComponent->setTranslation(playerTranslation);
//				playerColliderComponent->contour.pos.y -= colH;
//				playerVel.y = 0.0f;
//				playerAccCpn->acceleration.y = 0.0f;
//			// Si le joueur se déplace vers le haut.
//			}else {
//				playerTranslation.y += colH;
//				playerTransformationComponent->setTranslation(playerTranslation);
//				playerColliderComponent->contour.pos.y += colH;
//			}
//		}
//
//		playerVelCpn->setVelocity(playerVel);
//	}
//	// Si le rectangle 1 est entré en collision avec autre chose.
//	else if(rect1ID != de::badID) {
//		de::component_id rect1TransfCpnID = de::EntityManager::getComponentID(g_Rect1, de::TransformationComponentType);
//		de::component_id rect1CollCpnID = de::EntityManager::getComponentID(g_Rect1, de::ColliderComponentType);
//		de::component_id rect1VelCpnID = de::EntityManager::getComponentID(g_Rect1, de::VelocityComponentType);
//
//		de::TransformationComponent *rect1TransfCpn = de::ComponentManager::getTransformationComponent(rect1TransfCpnID);
//		de::ColliderComponent *rect1CollCpn = de::ComponentManager::getColliderComponent(rect1CollCpnID);
//		de::VelocityComponent *rect1VelCpn = de::ComponentManager::getVelocityComponent(rect1VelCpnID);
//
//		de::fvec3 rect1Transl = rect1TransfCpn->getTranslation();
//		de::fvec3 rect1Vel = rect1VelCpn->getVelocity();
//
//		float colW = fabs(collision.w);
//		float colH = fabs(collision.h);
//
//		//== g_AudioSource.play();
//
//		// Une pratique assez courante de repositionner un objet pouvant se déplacer lorsqu'il touche un objet statique
//		// est de le déplacer par rapport à l'axe le plus petit du rectangle de collision,
//		// ainsi, si la largeur du rectangle de collision est plus petite que la hauteur, l'objet sera déplacé sur l'axe X, inversemment dans le cas
//		// où la hauteur est plus petite que la largeur.
//
//		// Si la collision est plus petite en largeur, on déplace le joueur sur l'axe X.
//		if(colW < colH) {
//			// Si le joueur se déplace vers la droite.
//			if(diff.x > 0) {
//				rect1Transl.x -= colW;
//				rect1TransfCpn->setTranslation(rect1Transl);
//				rect1CollCpn->contour.pos.x -= colW;
//			// Si le joueur se déplace vers la gauche.
//			}else {
//				rect1Transl.x += colW;
//				rect1TransfCpn->setTranslation(rect1Transl);
//				rect1CollCpn->contour.pos.x += colW;
//			}
//
//			rect1Vel.x = -rect1Vel.x;
//			rect1VelCpn->setVelocity(rect1Vel);
//		}
//		// Sinon on déplace le joueur sur l'axe Y.
//		else {
//			// Si le joueur se déplace vers le bas.
//			if(diff.y > 0) {
//				rect1Transl.y -= colH;
//				rect1TransfCpn->setTranslation(rect1Transl);
//				rect1CollCpn->contour.pos.y -= colH;
//			// Si le joueur se déplace vers le haut.
//			}else {
//				rect1Transl.y += colH;
//				rect1TransfCpn->setTranslation(rect1Transl);
//				rect1CollCpn->contour.pos.y += colH;
//			}
//			rect1Vel.y = -rect1Vel.y;
//			rect1VelCpn->setVelocity(rect1Vel);
//		}
//	}
}

void collider_out_callback(de::entity_collection_id collectionID, de::entity_id entity1, de::entity_id entity2, const de::fvec2 &difference)
{
	//de::entity_id playerID;
	//de::entity_id rect1ID;

	//// Diff permet de savoir dans quelle direction le joueur va.
	//de::fvec2 diff = difference;

	//// Vérifie si une des entités est le joueur ou le rectangle 1.
	//if(entity1 == g_Player.getEntityID()) {
	//	playerID = entity1;

	//	if(entity2 == g_Rect1.getEntityID()) {
	//		rect1ID = entity2;
	//	}else
	//		rect1ID = de::badID;
	//}else if(entity2 == g_Player.getEntityID()) {
	//	playerID = entity2;

	//	if(entity1 == g_Rect1.getEntityID())
	//		rect1ID = entity1;
	//	else
	//		rect1ID = de::badID;

	//	diff = de::fvec2::inv(diff);
	//}else {
	//	playerID = de::badID;

	//	if(entity1 == g_Rect1.getEntityID()) {
	//		rect1ID = entity1;
	//	}else if(entity2 == g_Rect1.getEntityID()) {
	//		rect1ID = entity2;
	//		diff = de::fvec2::inv(diff);
	//	}else
	//		rect1ID = de::badID;
	//}

	//if(playerID != de::badID && rect1ID != de::badID) {
	//	g_CollisionPlayerRectEnabled = true;
	//}
}

#include <type_traits>

#undef main
int main()
{
	de::error_status errorStatus;

	errorStatus = de::core::init();
	if(errorStatus != de::error_status::NoError)
    {
		de::error::printError(errorStatus);
		return 1;
	}

	printf("pwd: %s\n", de::core::getPwd());

    {
        de::list<de::string> l;

        l.add("test_string_list");
    }

	de::window win(TARGET_MS, TARGET_FPS);
	win.setEventCallback(event_callback);
	win.setUpdateCallback(update_callback);
	errorStatus = de::window::create(win, "Creation UI [" DE_VERSION "]", de::size(WINDOW_WIDTH, WINDOW_HEIGHT));

	if(errorStatus != de::error_status::NoError)
    {
		de::error::printError(errorStatus);
		de::core::shutdown();
		return 1;
	}

	de::im_gui_window::init(win);

	de::scene_id sceneID = de::scene::createScene("scn_main");
	de::scene *scene = de::scene::getScene(sceneID);
	scene->setColliderCallback(collider_callback);
	scene->setColliderOutCallback(collider_out_callback);

	de::entity_collection_id collectionID = de::scene::getEntityCollection(sceneID);

	printf(
		"====================[ OpenGL ]====================\n"
		"Version: %s\n"
		"Max vertex attribs: %d\n"
		"Max texture image units: %d\n"
		"==================================================\n",
		
		de::gl_core::version(),
		de::gl_core::maxVertexAttribs(),
		de::gl_core::maxTextureImageUnits()
	);

	de::resources::init();

	de::gl_texture_int mcGrassSide = de::resources::loadTexture("grass_block_side.png", 0);
	de::gl_texture_int mcGrassTop  = de::resources::loadTexture("grass_block_top.png", 0);

	de::png pngSkyboxLeft;
	de::png pngSkyboxFront;
	de::png pngSkyboxRight;
	de::png pngSkyboxBack;
	de::png pngSkyboxBottom;
	de::png pngSkyboxTop;

	// Charge les images de la skybox
	if(!pngSkyboxLeft.loadAndRead("..\\resources\\textures\\skybox_left.png"))
    {
		fprintf(stderr, "Unable to load 'skybox_left'.\n");
		return 1;
	}

	if(!pngSkyboxFront.loadAndRead("..\\resources\\textures\\skybox_front.png"))
    {
		fprintf(stderr, "Unable to load 'skybox_front'.\n");
		return 1;
	}

	if(!pngSkyboxRight.loadAndRead("..\\resources\\textures\\skybox_right.png"))
    {
		fprintf(stderr, "Unable to load 'skybox_right'.\n");
		return 1;
	}

	if(!pngSkyboxBack.loadAndRead("..\\resources\\textures\\skybox_back.png"))
    {
		fprintf(stderr, "Unable to load 'skybox_back'.\n");
		return 1;
	}

	if(!pngSkyboxBottom.loadAndRead("..\\resources\\textures\\skybox_bottom.png"))
    {
		fprintf(stderr, "Unable to load 'skybox_bottom'.\n");
		return 1;
	}

	if(!pngSkyboxTop.loadAndRead("..\\resources\\textures\\skybox_top.png"))
    {
		fprintf(stderr, "Unable to load 'skybox_top'.\n");
		return 1;
	}

	pngSkyboxLeft.applyVerticalMirrorEffect();
	pngSkyboxLeft.applyHorizontalMirrorEffect();

	pngSkyboxFront.applyVerticalMirrorEffect();
	pngSkyboxFront.applyHorizontalMirrorEffect();

	pngSkyboxRight.applyVerticalMirrorEffect();
	pngSkyboxRight.applyHorizontalMirrorEffect();

	pngSkyboxBack.applyVerticalMirrorEffect();
	pngSkyboxBack.applyHorizontalMirrorEffect();

	de::mem_ptr imageLeft   = pngSkyboxLeft.rawImage();
	de::mem_ptr imageFront  = pngSkyboxFront.rawImage();
	de::mem_ptr imageRight  = pngSkyboxRight.rawImage();
	de::mem_ptr imageBack   = pngSkyboxBack.rawImage();
	de::mem_ptr imageBottom = pngSkyboxBottom.rawImage();
	de::mem_ptr imageTop    = pngSkyboxTop.rawImage();

	de::gl_texture_int skybox = de::gl_texture::create();
	de::gl_texture::bindCubemaps(skybox);
	de::gl_texture::setTextureWrappingSCubemaps(de::gl_texture_wrap::ClampToEdge);
	de::gl_texture::setTextureWrappingTCubemaps(de::gl_texture_wrap::ClampToEdge);
	de::gl_texture::setTextureWrappingRCubemaps(de::gl_texture_wrap::ClampToEdge);
	de::gl_texture::setTextureFilteringCubemaps(de::gl_texture_filter::Linear);
	de::gl_texture::transmitTextureCubemaps(imageLeft, imageFront, imageRight, imageBack, imageBottom, imageTop, pngSkyboxLeft.width(), pngSkyboxLeft.height(), pngSkyboxLeft.colorType());

	de::mem::free(imageLeft);
	de::mem::free(imageFront);
	de::mem::free(imageRight);
	de::mem::free(imageBack);
	de::mem::free(imageBottom);
	de::mem::free(imageTop);

	de::gl_program_int defaultProgram = de::gl_program::get("default");
	de::gl_program_int skyboxProgram  = de::gl_program::get("skybox");

	de::colora white(255, 255, 255, 255);

	size_t i, j;
	for(i = 0; i < 50; ++i)
    {
		for(j = 0; j < 50; ++j)
        {
			de::entity ent = de::graphic::create3DRectangleTexture(
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
	}

	// Affiche la skybox à la fin pour optimiser les appelles au fragment shader = FPS++
	de::entity skyboxEnt = de::graphic::createCubemap(skyboxProgram, collectionID, de::fvec3(0.0f, 0.0f, 0.0f), 200.0f, 200.0f, 200.0f, skybox, 0);

	de::scene::setActiveScene(sceneID);

	win.setShowingDebugPanel(true);

	// Lance la boucle du jeu, bloquant.
	win.run();

	// Détruit tous les composants internes de la fenêtre puis la fenêtre elle-même.
	win.destroy();

	de::gl_program::destroyAllPrograms();

	de::resources::shutdown();
	de::core::shutdown();

	printf("Good-bye!\n");

	return 0;
}
