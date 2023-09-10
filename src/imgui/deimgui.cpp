#include <DE/imgui/deimgui.hpp>
#include <DE/memory/memory.hpp>
#include <DE/memory/list.hpp>
#include <DE/string_utils.hpp>
#include <DE/vec.hpp>
#include <DE/window.hpp>
#include <DE/ecs/scene.hpp>

#include <unordered_map>
#include <string>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

namespace de {

	bool ImGuiWindow::m_Initialized = false;

	static std::unordered_map<const Window *, ImGuiDebugPanelOptions> m_DebugPanelOptions;

	/*
	==============================================
	ImGuiDebugPanelOptions::ImGuiDebugPanelOptions
	==============================================
	*/
	ImGuiDebugPanelOptions::ImGuiDebugPanelOptions(ImGuiDebugMenuView _view)
		: view(_view)
	{ }

	/*
	=================
	ImGuiWindow::init
	=================
	*/
	void ImGuiWindow::init(Window &window)
	{
		// Initialise ImGui.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Défini ImGui sur le mode sombre.
		ImGui::StyleColorsDark();

		//SDL_Renderer *renderer = window.getRenderer().getRenderer();

		// Initialise ImGui pour utiliser le Renderer de SDL2.
		ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getRenderer().context());
		ImGui_ImplOpenGL3_Init("#version 330 core");

		m_Initialized = true;
	}

	/*
	=====================
	ImGuiWindow::shutdown
	=====================
	*/
	void ImGuiWindow::shutdown()
	{
		if(m_Initialized) {
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();

			m_Initialized = false;
		}
	}

	void scenes_enum_callback(scene_id id, Scene &scene)
	{
		std::string text("[" + std::to_string(id) + std::string("] ") + scene.getName());
		if(id == Scene::getActiveSceneID())
			text += " (active)";

		ImGui::AlignTextToFramePadding();
		ImGui::Text(text.c_str()); ImGui::SameLine();
		if(ImGui::Button(std::string("Supprimer##" + std::to_string(id)).c_str())) {
			Scene::mustBeDeleted(id);
		}
		ImGui::SameLine();
		if(ImGui::Button(std::string("Rendre active##" + std::to_string(id)).c_str())) {
			Scene::setActiveScene(id);
		}
	}

	void entities_enum_callback(entity_collection_id collection, entity_id entity)
	{
		std::string text("[" + std::to_string(entity) + "]");

		ImGui::AlignTextToFramePadding();
		ImGui::Text(text.c_str()); ImGui::SameLine();
		if(ImGui::Button(std::string("Supprimer##" + std::to_string(entity)).c_str())) {
			EntityManager::mustBeDeleted(collection, entity);
		}
	}

	/*
	======================
	ImGuiDebugMenu::render
	======================
	*/
	void ImGuiDebugMenu::render(const Window *window)
	{
		const auto &it = m_DebugPanelOptions.find(window);
		if(it == m_DebugPanelOptions.end())
			return;

		auto &options = it->second;

		scene_id scene = Scene::getActiveSceneID();
		entity_collection_id collection = Scene::getEntityCollection(scene);

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ 500, 700 });
		if(ImGui::Begin("DeepEngine Debug Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar)) {

			if(ImGui::BeginMenuBar()) {
				if(ImGui::BeginMenu("Vue")) {
					if(ImGui::MenuItem("Accueil", nullptr, false, true)) {
						options.view = ImGuiDebugMenuHomeView;
					}
					if(ImGui::MenuItem("Scènes", nullptr, false, true)) {
						options.view = ImGuiDebugMenuScenesView;
					}
					if(ImGui::MenuItem("Entités", nullptr, false, true)) {
						options.view = ImGuiDebugMenuEntitiesView;
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();

				// Affiche un panel différent selon la vue sélectionnée.
				switch(options.view) {
					default: break;
					// Accueil.
					case ImGuiDebugMenuHomeView: {
						ImGui::Text("Bienvenue dans le moteur profond !");
						ImGui::Spacing();

						if(ImGui::CollapsingHeader("Utilisation")) {
							ImGui::SeparatorText("Menu debug :");
							ImGui::BulletText("Insert : afficher / cacher le menu debug.");
						}
					} break;
					// Menu des scènes.
					case ImGuiDebugMenuScenesView: {
						static bool emptyNameBuffer = false;
						static char nameBuffer[128] = "";
						// Création d'une scène.
						ImGui::InputText("Nom: ", nameBuffer, sizeof(nameBuffer)); ImGui::SameLine();
						if(ImGui::Button("Créer une scène")) {
							if(strlen(nameBuffer) == 0)
								emptyNameBuffer = true;
							else {
								emptyNameBuffer = false;
								Scene::createScene(nameBuffer);
								nameBuffer[0] = '\0';
							}
						}

						if(emptyNameBuffer)
							ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Le nom de la scène ne peut pas être vide.");

						ImGui::Spacing();

						// Affiche toutes les scènes existantes.
						if(ImGui::CollapsingHeader("Liste des scènes")) {
							Scene::enumScenes(scenes_enum_callback);
						}
					} break;
					case ImGuiDebugMenuEntitiesView: {
						// Affiche le nombre d'entités de la scène.
						ImGui::Text(std::string("Nombre d'entités: " + std::to_string(EntityManager::getNumberOfEntities(collection))).c_str()); ImGui::SameLine();
						if(ImGui::Button("Tout supprimer")) {
							EntityManager::destroyAllEntities(collection);
						}

						ImGui::Spacing();

						// Affiche toutes les entités de la scène.
						if(ImGui::CollapsingHeader("Liste des entités de la scène")) {
							EntityManager::enumEntities(collection, entities_enum_callback);
						}
					} break;
				}
			}

			
			ImGui::End();
		}
	}

	/*
	=========================
	ImGuiDebugMenu::addWindow
	=========================
	*/
	void ImGuiDebugMenu::addWindow(const Window *window)
	{
		m_DebugPanelOptions.emplace(window, ImGuiDebugPanelOptions());
	}

	/*
	============================
	ImGuiDebugMenu::removeWindow
	============================
	*/
	void ImGuiDebugMenu::removeWindow(const Window *window)
	{
		m_DebugPanelOptions.erase(window);
	}

}
