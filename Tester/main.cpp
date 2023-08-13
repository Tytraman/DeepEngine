#include <stdio.h>
#include <math.h>
#include <string>

#include <DE/def.h>
#include <DE/core.hpp>
#include <DE/error.hpp>
#include <DE/window.hpp>
#include <DE/key.hpp>
#include <DE/sys_win.hpp>
#include <DE/logger.hpp>
#include <DE/debug.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>
#include <DE/scene.hpp>
#include <DE/graphic/graphic.hpp>

#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

#include <DE/audio/audio.hpp>

#include <lua.hpp>

#define AUTHORS		"Tytraman"
#define BUILD_VER	"dev build 0.0.1"

#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	750
#define TARGET_MS		16
#define TARGET_FPS		300

#define SCROLL_SCALE_FACTOR_UP   1.10f
#define SCROLL_SCALE_FACTOR_DOWN 0.90f
#define SCROLL_ROTATE_FACTOR 3.0f

#define PLAYER_SPAWN_X 200.0f
#define PLAYER_SPAWN_Y 200.0f

#define RECT1_SPAWN_X  400.0f
#define RECT1_SPAWN_Y  200.0f

de::Logger *g_Logger = nullptr;

de::Entity g_Player = de::Entity::bad();

de::Entity g_Rect1 = de::Entity::bad();
de::ColliderComponent *g_Rect1ColliderComponent = nullptr;

void eventCallback(de::Window &window, de::devent e) {
	static bool mouseLeftClicked = false;
	static int lastMouseX, lastMouseY;

	uint32_t tt = e->getType();
	switch(tt) {
		default: break;
		case de::EventType::MouseButtonDown: {
			switch(e->getMouseButton()) {
				case de::MouseButton::Left: {
					mouseLeftClicked = true;
					de::Core::getMousePosition(&lastMouseX, &lastMouseY);
				} break;
				case de::MouseButton::Right: {
					de::scene_id sceneID = de::Scene::getActiveSceneID();
					de::Scene *scene = de::Scene::getScene(sceneID);
					de::entity_collection_id collectionID = de::Scene::getEntityCollection(sceneID);
					int mouseX, mouseY;
					de::Core::getMousePosition(&mouseX, &mouseY);

					de::fvec2 mouse = de::fvec2(mouseX, mouseY) - scene->getViewTranslation();

					printf("Right click (%f %f)\n", mouse.x, mouse.y);

					if(g_Rect1ColliderComponent->contour.isInside(mouse))
						de::EntityManager::destroyEntity(g_Rect1);
				} break;
			}
		} break;
		case de::EventType::MouseButtonUp: {
			switch(e->getMouseButton()) {
				case de::MouseButton::Left: {
					mouseLeftClicked = false;
				} break;
			}
		} break;
		case de::EventType::MouseMotion: {
			int mouseX, mouseY;
			de::Core::getMousePosition(&mouseX, &mouseY);

			// Si le clic gauche de la souris est pressé pendant que la souris est déplacée,
			// déplace la caméra.
			if(mouseLeftClicked) {
				de::scene_id sceneID = de::Scene::getActiveSceneID();
				if(sceneID != de::badID) {
					de::Scene *scene = de::Scene::getScene(sceneID);
					if(scene != nullptr) {
						int xDiff = mouseX - lastMouseX;
						int yDiff = mouseY - lastMouseY;

						de::fvec2 viewTranslation = scene->getViewTranslation();
						viewTranslation.x += xDiff;
						viewTranslation.y += yDiff;
						scene->setViewTranslation(viewTranslation);
					}
				}

				lastMouseX = mouseX;
				lastMouseY = mouseY;
			}else {
				//printf("Mouse: %d %d\n", mouseX, mouseY);
			}
		} break;
		case de::EventType::MouseWheel: {
			de::scene_id sceneID = de::Scene::getActiveSceneID();
			if(sceneID != de::badID) {
				de::Scene *scene = de::Scene::getScene(sceneID);
				if(scene != nullptr) {
					if(de::Key::isPressed(de::key::LShift)) {
						// Permet de tourner sur l'axe Z l'angle de vue de la scène.
						float viewAngle = scene->getViewAngle();
						viewAngle += e->getMouseScrollY() * SCROLL_ROTATE_FACTOR;
						scene->setViewAngle(viewAngle);
					}else {
						// Permet de zoomer sur la scène en modifiant le scale, mais ce n'est pas le meilleur moyen,
						// il faudrait plutôt avancer sur l'axe Z du plan mais actuellement le moteur ne le permet pas.
						de::fvec2 viewScale = scene->getViewScale();
						float scroll;
						if(e->getMouseScrollY() > 0)
							scroll = SCROLL_SCALE_FACTOR_UP;
						else
							scroll = SCROLL_SCALE_FACTOR_DOWN;

						viewScale *= scroll;
						scene->setViewScale(viewScale);
					}
				}
			}
		} break;
		case de::EventType::Window: {
			uint32_t type = e->getWindowEventType();
			switch(type) {
				default: break;
				case de::events::WindowResized: {
					
				} break;
			}
		} break;
	}

	// Appelle le callback par défaut
	de::Window::defaultInputCallback(window, e);
}

// Fonction appelée en boucle dans la boucle du jeu,
// pour mettre à jour toutes les valeurs du jeu
void updateCallback(de::Window &window) {
	de::Debug::addFunctionToCallbackList(DE_FUNCTION_NAME);

	de::scene_id sceneID = de::Scene::getActiveSceneID();
	de::entity_collection_id collectionID;
	de::component_id playerTransformationComponentID;
	de::component_id playerColliderComponentID;
	de::TransformationComponent *playerTransformationComponent;
	de::ColliderComponent *playerColliderComponent;
	de::Scene *scene;

	if(sceneID == de::badID)
		goto end;

	scene = de::Scene::getScene(sceneID);

	collectionID = de::Scene::getEntityCollection(sceneID);

	playerTransformationComponentID = de::EntityManager::getComponentID(g_Player, de::TransformationComponentType);
	playerColliderComponentID       = de::EntityManager::getComponentID(g_Player, de::ColliderComponentType);

	playerTransformationComponent = de::ComponentManager::getTransformationComponent(playerTransformationComponentID);
	playerColliderComponent       = de::ComponentManager::getColliderComponent(playerColliderComponentID);

	if(de::Key::isPressed(de::key::LeftArrow)) {
		float rotation = playerTransformationComponent->getRotation();
		rotation -= 1.0f;
		playerTransformationComponent->setRotation(rotation);
	}

	if(de::Key::isPressed(de::key::RightArrow)) {
		float rotation = playerTransformationComponent->getRotation();
		rotation += 1.0f;
		playerTransformationComponent->setRotation(rotation);
	}

	if(de::Key::isPressed(de::key::UpArrow)) {
		de::fvec2 scaling = playerTransformationComponent->getScaling();
		scaling += 2.0f;
		playerTransformationComponent->setScaling(scaling);
	}

	if(de::Key::isPressed(de::key::DownArrow)) {
		de::fvec2 scaling = playerTransformationComponent->getScaling();
		scaling -= 2.0f;
		playerTransformationComponent->setScaling(scaling);
	}

	if(de::Key::isPressed(de::key::O)) {
		de::fvec2 viewTranslation = scene->getViewTranslation();
		viewTranslation.y -= 5.0f;
		scene->setViewTranslation(viewTranslation);
	}

	if(de::Key::isPressed(de::key::K)) {
		de::fvec2 viewTranslation = scene->getViewTranslation();
		viewTranslation.x -= 5.0f;
		scene->setViewTranslation(viewTranslation);
	}

	if(de::Key::isPressed(de::key::L)) {
		de::fvec2 viewTranslation = scene->getViewTranslation();
		viewTranslation.y += 5.0f;
		scene->setViewTranslation(viewTranslation);
	}

	if(de::Key::isPressed(de::key::M)) {
		de::fvec2 viewTranslation = scene->getViewTranslation();
		viewTranslation.x += 5.0f;
		scene->setViewTranslation(viewTranslation);
	}

	// Déplacer le rectangle vers le haut
	if(de::Key::isPressed(de::key::Z)) {
		de::fvec2 translation = playerTransformationComponent->getTranslation();
		translation.y -= 5.0f;
		playerTransformationComponent->setTranslation(translation);
		if(playerColliderComponent != nullptr)
			playerColliderComponent->contour.pos.y -= 5.0f;
	}

	// Déplacer le rectangle vers la gauche
	if(de::Key::isPressed(de::key::Q)) {
		de::fvec2 translation = playerTransformationComponent->getTranslation();
		translation.x -= 5.0f;
		playerTransformationComponent->setTranslation(translation);
		if(playerColliderComponent != nullptr)
			playerColliderComponent->contour.pos.x -= 5.0f;
	}

	// Déplacer le rectangle vers le bas
	if(de::Key::isPressed(de::key::S)) {
		de::fvec2 translation = playerTransformationComponent->getTranslation();
		translation.y += 5.0f;
		playerTransformationComponent->setTranslation(translation);
		if(playerColliderComponent != nullptr)
			playerColliderComponent->contour.pos.y += 5.0f;
	}

	// Déplacer le rectangle vers la droite
	if(de::Key::isPressed(de::key::D)) {
		de::fvec2 translation = playerTransformationComponent->getTranslation();
		translation.x += 5.0f;
		playerTransformationComponent->setTranslation(translation);
		if(playerColliderComponent != nullptr)
			playerColliderComponent->contour.pos.x += 5.0f;
	}

	{
		de::component_id rect1TransformationComponentID = de::EntityManager::getComponentID(g_Rect1, de::TransformationComponentType);

		if(rect1TransformationComponentID != de::badID) {
			de::TransformationComponent *rect1TransformationCpn = de::ComponentManager::getTransformationComponent(rect1TransformationComponentID);

			de::component_id rect1VelocityCpnID = de::EntityManager::getComponentID(g_Rect1, de::VelocityComponentType);
			if(rect1VelocityCpnID != de::badID) {
				de::VelocityComponent *rect1VelocityCpn = de::ComponentManager::getVelocityComponent(rect1VelocityCpnID);
			}
		}
	}

end:
	de::Debug::removeFunctionFromCallbackList();
}

void collider_callback(de::entity_collection_id collectionID, de::entity_id entity1, de::entity_id entity2, const de::fvec2 &difference, const de::Rect &collision)
{
	de::scene_id sceneID = de::Scene::getActiveSceneID();

	de::entity_id playerID;
	de::entity_id rect1ID;

	// Diff permet de savoir dans quelle direction le joueur va.
	de::fvec2 diff = difference;

	// Vérifie si une des entités est le joueur ou le rectangle 1.
	if(entity1 == g_Player.getEntityID()) {
		playerID = entity1;

		if(entity2 == g_Rect1.getEntityID()) {
			rect1ID = entity2;
		}else
			rect1ID = de::badID;
	}else if(entity2 == g_Player.getEntityID()) {
		playerID = entity2;

		if(entity1 == g_Rect1.getEntityID())
			rect1ID = entity1;
		else
			rect1ID = de::badID;

		diff = de::fvec2::inv(diff);
	}else {
		playerID = de::badID;

		if(entity1 == g_Rect1.getEntityID()) {
			rect1ID = entity1;
		}else if(entity2 == g_Rect1.getEntityID()) {
			rect1ID = entity2;
			diff = de::fvec2::inv(diff);
		}else
			rect1ID = de::badID;
	}

rewatch:


	// Collision entre le joueur et le rectangle 1.
	if(playerID != de::badID && rect1ID != de::badID) {
		playerID = de::badID;
		diff = de::fvec2::inv(diff);
		goto rewatch;
	}
	// Si le joueur est entré en collision avec autre chose.
	else if(playerID != de::badID) {
		de::component_id playerTransformationComponentID = de::EntityManager::getComponentID(g_Player, de::TransformationComponentType);
		de::component_id playerColliderComponentID = de::EntityManager::getComponentID(g_Player, de::ColliderComponentType);

		de::TransformationComponent *playerTransformationComponent = de::ComponentManager::getTransformationComponent(playerTransformationComponentID);
		de::ColliderComponent *playerColliderComponent = de::ComponentManager::getColliderComponent(playerColliderComponentID);

		de::fvec2 playerTranslation  = playerTransformationComponent->getTranslation();

		float colW = fabs(collision.w);
		float colH = fabs(collision.h);

		// Une pratique assez courante de repositionner un objet pouvant se déplacer lorsqu'il touche un objet statique
		// est de le déplacer par rapport à l'axe le plus petit du rectangle de collision,
		// ainsi, si la largeur du rectangle de collision est plus petite que la hauteur, l'objet sera déplacé sur l'axe X, inversemment dans le cas
		// où la hauteur est plus petite que la largeur.

		// Si la collision est plus petite en largeur, on déplace le joueur sur l'axe X.
		if(colW < colH) {
			// Si le joueur se déplace vers la droite.
			if(diff.x > 0) {
				playerTranslation.x -= colW;
				playerTransformationComponent->setTranslation(playerTranslation);
				playerColliderComponent->contour.pos.x -= colW;
			// Si le joueur se déplace vers la gauche.
			}else {
				playerTranslation.x += colW;
				playerTransformationComponent->setTranslation(playerTranslation);
				playerColliderComponent->contour.pos.x += colW;
			}
		}
		// Sinon on déplace le joueur sur l'axe Y.
		else {
			// Si le joueur se déplace vers le bas.
			if(diff.y > 0) {
				playerTranslation.y -= colH;
				playerTransformationComponent->setTranslation(playerTranslation);
				playerColliderComponent->contour.pos.y -= colH;
			// Si le joueur se déplace vers le haut.
			}else {
				playerTranslation.y += colH;
				playerTransformationComponent->setTranslation(playerTranslation);
				playerColliderComponent->contour.pos.y += colH;
			}
		}
	}
	// Si le rectangle 1 est entré en collision avec autre chose.
	else if(rect1ID != de::badID) {
		de::component_id rect1TransfCpnID = de::EntityManager::getComponentID(g_Rect1, de::TransformationComponentType);
		de::component_id rect1CollCpnID = de::EntityManager::getComponentID(g_Rect1, de::ColliderComponentType);
		de::component_id rect1VelCpnID = de::EntityManager::getComponentID(g_Rect1, de::VelocityComponentType);

		de::TransformationComponent *rect1TransfCpn = de::ComponentManager::getTransformationComponent(rect1TransfCpnID);
		de::ColliderComponent *rect1CollCpn = de::ComponentManager::getColliderComponent(rect1CollCpnID);
		de::VelocityComponent *rect1VelCpn = de::ComponentManager::getVelocityComponent(rect1VelCpnID);

		de::fvec2 rect1Transl = rect1TransfCpn->getTranslation();
		de::fvec2 rect1Vel = rect1VelCpn->getVelocity();

		float colW = fabs(collision.w);
		float colH = fabs(collision.h);

		// Une pratique assez courante de repositionner un objet pouvant se déplacer lorsqu'il touche un objet statique
		// est de le déplacer par rapport à l'axe le plus petit du rectangle de collision,
		// ainsi, si la largeur du rectangle de collision est plus petite que la hauteur, l'objet sera déplacé sur l'axe X, inversemment dans le cas
		// où la hauteur est plus petite que la largeur.

		// Si la collision est plus petite en largeur, on déplace le joueur sur l'axe X.
		if(colW < colH) {
			// Si le joueur se déplace vers la droite.
			if(diff.x > 0) {
				rect1Transl.x -= colW;
				rect1TransfCpn->setTranslation(rect1Transl);
				rect1CollCpn->contour.pos.x -= colW;
			// Si le joueur se déplace vers la gauche.
			}else {
				rect1Transl.x += colW;
				rect1TransfCpn->setTranslation(rect1Transl);
				rect1CollCpn->contour.pos.x += colW;
			}

			rect1Vel.x = -rect1Vel.x;
			rect1VelCpn->setVelocity(rect1Vel);
		}
		// Sinon on déplace le joueur sur l'axe Y.
		else {
			// Si le joueur se déplace vers le bas.
			if(diff.y > 0) {
				rect1Transl.y -= colH;
				rect1TransfCpn->setTranslation(rect1Transl);
				rect1CollCpn->contour.pos.y -= colH;
			// Si le joueur se déplace vers le haut.
			}else {
				rect1Transl.y += colH;
				rect1TransfCpn->setTranslation(rect1Transl);
				rect1CollCpn->contour.pos.y += colH;
			}
			rect1Vel.y = -rect1Vel.y;
			rect1VelCpn->setVelocity(rect1Vel);
		}
	}
}

#undef main
int main() {
	de::ErrorStatus errorStatus;

	errorStatus = de::Core::init();
	if(errorStatus != de::ErrorStatus::NoError) {
		de::DError::printError(errorStatus);
		return 1;
	}

	de::Logger logger("supralog.log");
	if(!logger.open()) {
		fprintf(stderr, "Error when creating logger\n");
		de::Core::quit();
		return 1;
	}
	g_Logger = &logger;

	de::Debug::addFunctionToCallbackList(DE_FUNCTION_NAME);
	de::Debug::writeToStream(logger);

	lua_State *L = luaL_newstate();

	printf("pwd: %s\n", de::Core::getPwd());

	if(!de::AudioDevice::init())
		fprintf(stderr, "Unable to load audio device.\n");

	de::AudioBuffer songBuffer;

	if(!songBuffer.create("song.wav")) {
		fprintf(stderr, "Unable to load sound.\n");
		de::AudioDevice::shutdown();
		return 1;
	}

	printf("Audio frequency: %d\nAudio bits: %d\nAudio channels: %d\nAudio size: %d\n", songBuffer.getFrequency(), songBuffer.getBits(), songBuffer.getChannels(), songBuffer.getSize());

	de::AudioSource audioSource;

	if(!audioSource.create()) {
		fprintf(stderr, "Unable to create audio source.\n");

		songBuffer.destroy();
		de::AudioDevice::shutdown();

		return 1;
	}

	if(!audioSource.attachBuffer(songBuffer)) {
		fprintf(stderr, "Unable to attach buffer to audio source.\n");

		songBuffer.destroy();
		audioSource.destroy();
		de::AudioDevice::shutdown();

		return 1;
	}

	de::AudioListener &audioListener = de::AudioDevice::getListener();
	if(!audioListener.setPosition(de::fvec3(0.0f, 0.0f, 0.0f))) {
		fprintf(stderr, "Unable to set listener position.\n");

		songBuffer.destroy();
		audioSource.destroy();
		de::AudioDevice::shutdown();

		return 1;
	}

	if(!audioListener.setVelocity(de::fvec3(0.0f, 0.0f, 0.0f))) {
		fprintf(stderr, "Unable to set listener velocity.\n");

		songBuffer.destroy();
		audioSource.destroy();
		de::AudioDevice::shutdown();

		return 1;
	}

	if(!audioSource.play()) {
		fprintf(stderr, "Unable to play sound.\n");

		songBuffer.destroy();
		audioSource.destroy();
		de::AudioDevice::shutdown();

		return 1;
	}

	de::Window window(TARGET_MS, TARGET_FPS);
	window.setEventCallback(eventCallback);
	window.setUpdateCallback(updateCallback);
	errorStatus = de::Window::create(window, "DeepEngine Tester by " AUTHORS " [" BUILD_VER "]", de::size(WINDOW_WIDTH, WINDOW_HEIGHT));
	if(errorStatus != de::ErrorStatus::NoError) {
		de::DError::printError(errorStatus);
		de::Core::quit();
		return 1;
	}

	de::scene_id sceneID = de::Scene::createScene();
	de::Scene *scene = de::Scene::getScene(sceneID);
	scene->setColliderCallback(collider_callback);

	de::entity_collection_id collectionID = de::Scene::getEntityCollection(sceneID);

	g_Rect1 = de::Graphic::createRectangle(sceneID, de::fvec2(RECT1_SPAWN_X, RECT1_SPAWN_Y), 25.0f, 25.0f, de::colora(0, 255, 0, 255), true);

	de::component_id rect1VelocityComponentID = de::ComponentManager::createVelocityComponent();
	de::VelocityComponent *rect1VelocityComponent = de::ComponentManager::getVelocityComponent(rect1VelocityComponentID);
	rect1VelocityComponent->setVelocity(de::fvec2(2.0f, 2.0f));
	de::EntityManager::attachComponent(g_Rect1, rect1VelocityComponentID);

	de::component_id rect1TransformationComponentID = de::EntityManager::getComponentID(g_Rect1, de::TransformationComponentType);
	de::component_id rect1ColliderComponentID = de::EntityManager::getComponentID(g_Rect1, de::ColliderComponentType);

	de::TransformationComponent *rect1TransformationComponent = de::ComponentManager::getTransformationComponent(rect1TransformationComponentID);
	g_Rect1ColliderComponent = de::ComponentManager::getColliderComponent(rect1ColliderComponentID);

	de::fvec2 rect1Translation = rect1TransformationComponent->getTranslation();
	de::fvec2 rect1Scaling     = rect1TransformationComponent->getScaling();

	de::Entity leftRect    = de::Graphic::createRectangle(collectionID, de::fvec2(40.0f, WINDOW_HEIGHT / 2.0f), 20.0f, WINDOW_HEIGHT - 100.0f, de::colora(255, 0, 255, 255), true);
	de::Entity rightRect   = de::Graphic::createRectangle(collectionID, de::fvec2(WINDOW_WIDTH - 40.0f, WINDOW_HEIGHT / 2.0f), 20.0f, WINDOW_HEIGHT - 100.0f, de::colora(255, 0, 255, 255), true);
	de::Entity topRect     = de::Graphic::createRectangle(collectionID, de::fvec2(WINDOW_WIDTH / 2.0f, 100.0f), WINDOW_WIDTH - 100.0f, 20.0f, de::colora(0, 0, 255, 255), true);
	de::Entity bottomRect  = de::Graphic::createRectangle(collectionID, de::fvec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 100.0f), WINDOW_WIDTH - 100.0f, 20.0f, de::colora(0, 0, 255, 255), true);

	g_Player = de::Graphic::createRectangle(collectionID, de::fvec2(PLAYER_SPAWN_X, PLAYER_SPAWN_Y), 50.0f, 50.0f, de::colora(255, 0, 0, 255), true);

	de::component_id playerDrawableComponentID = de::EntityManager::getComponentID(g_Player, de::DrawableComponentType);
	de::component_id playerTransformationComponentID = de::EntityManager::getComponentID(g_Player, de::TransformationComponentType);

	de::DrawableComponent *playerDrawableComponent = de::ComponentManager::getDrawableComponent(playerDrawableComponentID);
	de::TransformationComponent *playerTransformationComponent = de::ComponentManager::getTransformationComponent(playerTransformationComponentID);

	de::fvec2 playerTranslation = playerTransformationComponent->getTranslation();
	de::fvec2 playerScaling     = playerTransformationComponent->getScaling();

	de::Scene::setCurrentScene(sceneID);

	// Lance la boucle du jeu, bloquant.
	window.run();

	// Détruit tous les composants internes de la fenêtre puis la fenêtre elle-même.
	window.destroy();

	logger.close();

	lua_close(L);

	songBuffer.destroy();
	audioSource.destroy();
	de::AudioDevice::shutdown();

	de::Core::quit();

	printf("Good-bye!\n");

	return 0;
}
