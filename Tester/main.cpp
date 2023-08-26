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

#include <DE/imgui/deimgui.hpp>

#include <DE/rendering/opengl_utils.hpp>

#define AUTHORS		"Tytraman"
#define BUILD_VER	"dev build 0.0.1"

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

de::Logger *g_Logger = nullptr;

de::Entity g_Player = de::Entity::bad();

de::Entity g_Rect1 = de::Entity::bad();
de::ColliderComponent *g_Rect1ColliderComponent = nullptr;

static bool g_CollisionPlayerRectEnabled = true;

void eventCallback(de::Window &window, de::devent e)
{
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

	// Déplacer le joueur vers la gauche.
	if(de::Key::isPressed(de::key::Q)) {
		de::fvec2 translation = playerTransformationComponent->getTranslation();
		translation.x -= 5.0f;
		playerTransformationComponent->setTranslation(translation);
		if(playerColliderComponent != nullptr)
			playerColliderComponent->contour.pos.x -= 5.0f;
	}

	// Déplacer le joueur vers la droite.
	if(de::Key::isPressed(de::key::D)) {
		de::fvec2 translation = playerTransformationComponent->getTranslation();
		translation.x += 5.0f;
		playerTransformationComponent->setTranslation(translation);
		if(playerColliderComponent != nullptr)
			playerColliderComponent->contour.pos.x += 5.0f;
	}

	// Faire sauter le joueur.
	if(de::Key::isPressed(de::key::Space)) {
		de::component_id playerAccCpnID = de::EntityManager::getComponentID(g_Player, de::AccelerationComponentType);
		de::component_id playerVelCpnID = de::EntityManager::getComponentID(g_Player, de::VelocityComponentType);

		de::AccelerationComponent *playerAccCpn = de::ComponentManager::getAccelerationComponent(playerAccCpnID);
		de::VelocityComponent *playerVelCpn = de::ComponentManager::getVelocityComponent(playerVelCpnID);

		if(playerAccCpn->acceleration.y == 0.0f) {
			de::fvec2 playerVel = playerVelCpn->getVelocity();
			playerVel.y -= 10.0f;
			playerVelCpn->setVelocity(playerVel);
			playerAccCpn->acceleration.y = PLAYER_GRAVITY;
		}
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
		if(g_CollisionPlayerRectEnabled) {
			if(diff.y <= 0 || diff.y <= fabs(diff.x)) {
				playerID = de::badID;
				diff = de::fvec2::inv(diff);
				goto rewatch;
			}else
				g_CollisionPlayerRectEnabled = false;
		}
	}
	// Si le joueur est entré en collision avec autre chose.
	else if(playerID != de::badID) {
		de::component_id playerTransformationComponentID = de::EntityManager::getComponentID(g_Player, de::TransformationComponentType);
		de::component_id playerColliderComponentID = de::EntityManager::getComponentID(g_Player, de::ColliderComponentType);
		de::component_id playerVelCpnID = de::EntityManager::getComponentID(g_Player, de::VelocityComponentType);
		de::component_id playerAccCpnID = de::EntityManager::getComponentID(g_Player, de::AccelerationComponentType);

		de::TransformationComponent *playerTransformationComponent = de::ComponentManager::getTransformationComponent(playerTransformationComponentID);
		de::ColliderComponent *playerColliderComponent = de::ComponentManager::getColliderComponent(playerColliderComponentID);
		de::VelocityComponent *playerVelCpn = de::ComponentManager::getVelocityComponent(playerVelCpnID);
		de::AccelerationComponent *playerAccCpn = de::ComponentManager::getAccelerationComponent(playerAccCpnID);

		de::fvec2 playerTranslation  = playerTransformationComponent->getTranslation();
		de::fvec2 playerVel = playerVelCpn->getVelocity();

		float colW = fabs(collision.w);
		float colH = fabs(collision.h);

		// Une pratique assez courante de repositionner un objet pouvant se déplacer lorsqu'il touche un objet statique
		// est de le déplacer par rapport à l'axe le plus petit du rectangle de collision,
		// ainsi, si la largeur du rectangle de collision est plus petite que la hauteur, l'objet sera déplacé sur l'axe X, inversemment dans le cas
		// où la hauteur est plus petite que la largeur.

		// Si la collision est plus petite en largeur, on déplace le joueur sur l'axe X.
		if(colW < colH) {
			playerVel.x = 0.0f;
			playerAccCpn->acceleration.x = 0.0f;
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
				playerVel.y = 0.0f;
				playerAccCpn->acceleration.y = 0.0f;
			// Si le joueur se déplace vers le haut.
			}else {
				playerTranslation.y += colH;
				playerTransformationComponent->setTranslation(playerTranslation);
				playerColliderComponent->contour.pos.y += colH;
			}
		}

		playerVelCpn->setVelocity(playerVel);
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

		//== g_AudioSource.play();

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

void collider_out_callback(de::entity_collection_id collectionID, de::entity_id entity1, de::entity_id entity2, const de::fvec2 &difference)
{
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

	if(playerID != de::badID && rect1ID != de::badID) {
		g_CollisionPlayerRectEnabled = true;
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
		de::Core::shutdown();
		return 1;
	}
	g_Logger = &logger;

	de::Debug::addFunctionToCallbackList(DE_FUNCTION_NAME);
	de::Debug::writeToStream(logger);

	printf("pwd: %s\n", de::Core::getPwd());

	de::Window window(TARGET_MS, TARGET_FPS);
	window.setEventCallback(eventCallback);
	window.setUpdateCallback(updateCallback);
	errorStatus = de::Window::create(window, "DeepEngine Tester by " AUTHORS " [" BUILD_VER "]", de::size(WINDOW_WIDTH, WINDOW_HEIGHT));
	if(errorStatus != de::ErrorStatus::NoError) {
		de::DError::printError(errorStatus);
		de::Core::shutdown();
		return 1;
	}

	de::ImGuiWindow::init(window);

	de::scene_id sceneID = de::Scene::createScene("scn_main");
	de::Scene *scene = de::Scene::getScene(sceneID);
	scene->setColliderCallback(collider_callback);
	scene->setColliderOutCallback(collider_out_callback);

	de::entity_collection_id collectionID = de::Scene::getEntityCollection(sceneID);

	printf(
		"====================[ OpenGL ]====================\n"
		"Version: %s\n"
		"Max vertex attribs: %d\n"
		"==================================================\n",
		
		de::GLCore::version(),
		de::GLCore::maxVertexAttribs()
	);

	de::Entity ent1 = de::Graphic::createRectangle(collectionID, de::fvec2(0.0f, 0.0f), 1.0f, 1.0f, de::colora(0, 0, 255, 255));
	de::component_id ent1TransID = de::EntityManager::getComponentID(ent1, de::TransformationComponentType);
	de::TransformationComponent *ent1Trans = de::ComponentManager::getTransformationComponent(ent1TransID);
	ent1Trans->setRotation(10.0f);

	//de::Entity ent2 = de::Graphic::createTriangle(collectionID, de::fvec2(50.0f, 25.0f), 70.0f, de::colora(255, 255, 0, 255));



	de::InputFileStream vertIfs("..\\glsl\\default.vert");
	if(!vertIfs.open()) {
		fprintf(stderr, "Unable to open vertex shader file.\n");
		return 1;
	}

	de::InputFileStream fragIfs("..\\glsl\\default.frag");
	if(!fragIfs.open()) {
		fprintf(stderr, "Unable to open fragment shader file.\n");
		return 1;
	}

	de::MemoryChunk vertShader1;
	de::MemoryChunk::alloc(vertShader1, vertIfs.getFileSize());
	size_t bytesRead;

	if(!vertIfs.readAll(vertShader1.data(), &bytesRead)) {
		fprintf(stderr, "Can't read shader.\n");
		vertIfs.close();
		fragIfs.close();
		return 1;
	}

	de::MemoryChunk fragShader1;
	de::MemoryChunk::alloc(fragShader1, fragIfs.getFileSize());

	if(!fragIfs.readAll(fragShader1.data(), &bytesRead)) {
		fprintf(stderr, "Can't read shader.\n");
		vertShader1.free();
		vertIfs.close();
		fragIfs.close();
		return 1;
	}
	
	de::GLShader vertShader(de::GLShaderType::Vertex);
	de::GLShader fragShader(de::GLShaderType::Fragment);

	vertShader.load(vertShader1);
	fragShader.load(fragShader1);

	vertShader1.free();
	fragShader1.free();

	if(!vertShader.compile()) {
		vertIfs.close();
		fragIfs.close();
		return 1;
	}

	if(!fragShader.compile()) {
		vertIfs.close();
		fragIfs.close();
		return 1;
	}

	vertIfs.close();
	fragIfs.close();

	de::GLProgram program;
	program.attachShader(vertShader);
	program.attachShader(fragShader);
	if(!program.link()) {
		vertShader.destroy();
		fragShader.destroy();
		return 1;
	}

	program.use();

	vertShader.destroy();
	fragShader.destroy();

	/*g_Rect1 = de::Graphic::createRectangle(sceneID, de::fvec2(RECT1_SPAWN_X, RECT1_SPAWN_Y), 25.0f, 25.0f, de::colora(0, 255, 0, 255), true);

	de::component_id rect1VelocityComponentID = de::ComponentManager::createVelocityComponent();
	de::VelocityComponent *rect1VelocityComponent = de::ComponentManager::getVelocityComponent(rect1VelocityComponentID);
	rect1VelocityComponent->setVelocity(de::fvec2(15.0f, 15.0f));
	de::EntityManager::attachComponent(g_Rect1, rect1VelocityComponentID);

	de::component_id rect1TransformationComponentID = de::EntityManager::getComponentID(g_Rect1, de::TransformationComponentType);
	de::component_id rect1ColliderComponentID = de::EntityManager::getComponentID(g_Rect1, de::ColliderComponentType);*/

	/*de::TransformationComponent *rect1TransformationComponent = de::ComponentManager::getTransformationComponent(rect1TransformationComponentID);
	g_Rect1ColliderComponent = de::ComponentManager::getColliderComponent(rect1ColliderComponentID);*/

	/*de::fvec2 rect1Translation = rect1TransformationComponent->getTranslation();
	de::fvec2 rect1Scaling     = rect1TransformationComponent->getScaling();*/

	/*g_Player = de::Graphic::createRectangle(collectionID, de::fvec2(PLAYER_SPAWN_X, PLAYER_SPAWN_Y), 50.0f, 50.0f, de::colora(255, 0, 0, 255), true);
	de::component_id playerVelCpnID = de::ComponentManager::createVelocityComponent();
	de::component_id playerAccCpnID = de::ComponentManager::createAccelerationComponent(de::fvec2(0.0f, PLAYER_GRAVITY));
	
	de::EntityManager::attachComponent(g_Player, playerVelCpnID);
	de::EntityManager::attachComponent(g_Player, playerAccCpnID);*/

	de::Scene::setActiveScene(sceneID);

	window.setShowingDebugPanel(true);

	// Lance la boucle du jeu, bloquant.
	window.run();

	// Détruit tous les composants internes de la fenêtre puis la fenêtre elle-même.
	window.destroy();

	logger.close();

	de::Core::shutdown();

	printf("Good-bye!\n");

	return 0;
}
