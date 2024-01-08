#include "DE/imgui/deimgui.hpp"
#include "DE/memory/memory.hpp"
#include "DE/memory/list.hpp"
#include "DE/string_utils.hpp"
#include "DE/vec.hpp"
#include "DE/window.hpp"
#include "DE/ecs/scene.hpp"
#include "DE/hardware/cpu.hpp"
#include "DE/memory/settings.hpp"
#include "DE/resources.hpp"

#include <unordered_map>
#include <string>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

namespace deep
{

    bool im_gui_window::m_Initialized = false;

    static std::unordered_map<const window *, im_gui_debug_panel_options> m_DebugPanelOptions;

    static ImFont *g_TitleFont = nullptr;
    static ImFont *g_NormalFont = nullptr;

    /*
    ======================================================
    im_gui_debug_panel_options::im_gui_debug_panel_options
    ======================================================
    */
    im_gui_debug_panel_options::im_gui_debug_panel_options(im_gui_debug_menu_view _view)
        : view(_view)
    { }

    /*
    ===================
    im_gui_window::init
    ===================
    */
    void im_gui_window::init(window &window)
    {
        // Initialise ImGui.
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Défini ImGui sur le mode sombre.
        ImGui::StyleColorsDark();

        // Initialise ImGui pour utiliser le Renderer de SDL2.
        ImGui_ImplSDL2_InitForOpenGL(window.get_window(), window.get_renderer().get_context());
        ImGui_ImplOpenGL3_Init("#version 330 core");

        engine_settings *settings = engine_settings::get_singleton();
        resource_manager *resources = resource_manager::get_singleton();

        string font = resources->get_fonts_folder();
        font.append(settings->get_imgui_font_filename().str());

        ImGuiIO &io = ImGui::GetIO();

        io.Fonts->AddFontDefault();
        g_TitleFont = io.Fonts->AddFontFromFileTTF(font.str(), 32.0f);
        g_NormalFont = io.Fonts->AddFontFromFileTTF(font.str(), 17.5f);

        m_Initialized = true;
    }

    /*
    =======================
    im_gui_window::shutdown
    =======================
    */
    void im_gui_window::shutdown()
    {
        if(m_Initialized)
        {
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();

            m_Initialized = false;
        }
    }

    void scenes_enum_callback(scene_id id, scene &scene)
    {
        std::string text("[" + std::to_string(id) + std::string("] ") + scene.getName());
        if(id == scene::getActiveSceneID())
            text += " (active)";

        ImGui::AlignTextToFramePadding();
        ImGui::Text(text.c_str()); ImGui::SameLine();
        if(ImGui::Button(std::string("Supprimer##" + std::to_string(id)).c_str())) {
            scene::mustBeDeleted(id);
        }
        ImGui::SameLine();
        if(ImGui::Button(std::string("Rendre active##" + std::to_string(id)).c_str())) {
            scene::setActiveScene(id);
        }
    }

    void entities_enum_callback(entity_collection_id collection, entity_id entity)
    {
        entity_manager *entityManager = entity_manager::get_singleton();

        std::string text("[" + std::to_string(entity) + "]");

        ImGui::AlignTextToFramePadding();
        ImGui::Text(text.c_str()); ImGui::SameLine();
        if(ImGui::Button(std::string("Supprimer##" + std::to_string(entity)).c_str())) {
            entityManager->mustBeDeleted(collection, entity);
        }
    }

    /*
    =========================
    im_gui_debug_menu::render
    =========================
    */
    void im_gui_debug_menu::render(const window *window)
    {
        const auto &it = m_DebugPanelOptions.find(window);
        if(it == m_DebugPanelOptions.end())
            return;

        auto &options = it->second;

        scene_id scene = scene::getActiveSceneID();
        entity_collection_id collection = scene::getEntityCollection(scene);

        entity_manager *entityManager = entity_manager::get_singleton();

        ImGui::PushFont(g_NormalFont);

        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ 500, 700 });
        if(ImGui::Begin("DeepEngine Debug Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
        {
            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("Vue"))
                {
                    if(ImGui::MenuItem("Accueil", nullptr, false, true))
                    {
                        options.view = ImGuiDebugMenuHomeView;
                    }
                    if(ImGui::MenuItem("Scènes", nullptr, false, true))
                    {
                        options.view = ImGuiDebugMenuScenesView;
                    }
                    if(ImGui::MenuItem("Entités", nullptr, false, true))
                    {
                        options.view = ImGuiDebugMenuEntitiesView;
                    }
                    if(ImGui::MenuItem("CPU", nullptr, false, true))
                    {
                        options.view = ImGuiDebugMenuCPUView;
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();

                // Affiche un panel différent selon la vue sélectionnée.
                switch(options.view)
                {
                    default: break;
                    // Accueil.
                    case ImGuiDebugMenuHomeView:
                    {
                        ImGui::PushFont(g_TitleFont);
                        ImGui::Text("Accueil");
                        ImGui::PopFont();
                        ImGui::Separator();

                        ImGui::Text("Bienvenue dans le moteur profond !");
                        ImGui::Spacing();

                        if(ImGui::CollapsingHeader("Utilisation"))
                        {
                            ImGui::SeparatorText("Menu debug :");
                            ImGui::BulletText("Insert : afficher / cacher le menu debug.");
                        }
                    } break;
                    // Menu des scènes.
                    case ImGuiDebugMenuScenesView:
                    {
                        static bool emptyNameBuffer = false;
                        static char nameBuffer[128] = "";

                        ImGui::PushFont(g_TitleFont);
                        ImGui::Text("Scènes");
                        ImGui::PopFont();
                        ImGui::Separator();

                        // Création d'une scène.
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Nom:"); ImGui::SameLine(); ImGui::InputText("##", nameBuffer, sizeof(nameBuffer)); ImGui::SameLine();
                        if(ImGui::Button("Créer une scène"))
                        {
                            if(strlen(nameBuffer) == 0)
                                emptyNameBuffer = true;
                            else
                            {
                                emptyNameBuffer = false;
                                scene::createScene(nameBuffer);
                                nameBuffer[0] = '\0';
                            }
                        }

                        if(emptyNameBuffer)
                            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Le nom de la scène ne peut pas être vide.");

                        ImGui::Spacing();

                        // Affiche toutes les scènes existantes.
                        if(ImGui::CollapsingHeader("Liste des scènes"))
                        {
                            scene::enumScenes(scenes_enum_callback);
                        }
                    } break;
                    // Menu des entités.
                    case ImGuiDebugMenuEntitiesView:
                    {
                        ImGui::PushFont(g_TitleFont);
                        ImGui::Text("Entités");
                        ImGui::PopFont();
                        ImGui::Separator();

                        // Affiche le nombre d'entités de la scène.
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text(std::string("Nombre d'entités: " + std::to_string(entityManager->get_number_of_entities(collection))).c_str()); ImGui::SameLine();
                        if(ImGui::Button("Tout supprimer"))
                        {
                            entityManager->destroyAllEntities(collection);
                        }

                        ImGui::Spacing();

                        // Affiche toutes les entités de la scène.
                        if(ImGui::CollapsingHeader("Liste des entités de la scène"))
                        {
                            entityManager->enum_entities(collection, entities_enum_callback);
                        }
                    } break;
                    case ImGuiDebugMenuCPUView:
                    {
                        CPU *cpu = CPU::get_singleton();
                        uint16_t numberOfPerformanceCores = cpu->get_number_of_performance_cores();
                        uint16_t numberOfEfficiencyCores = cpu->get_number_of_efficiency_cores();
                        list<CPU::logical_processor> &logicalProcessors = cpu->get_logical_processors();

                        string numberOfCores("Nombre de coeurs: ");
                        numberOfCores.append(std::to_string(cpu->get_number_of_cores()).c_str());
                        if(numberOfEfficiencyCores > 0)
                        {
                            numberOfCores.append(" (");
                            numberOfCores.append(std::to_string(numberOfPerformanceCores).c_str());
                            numberOfCores.append("P + ");
                            numberOfCores.append(std::to_string(numberOfEfficiencyCores).c_str());
                            numberOfCores.append("E)");
                        }

                        string numberOfLogicalProcessors("Nombre de threads: ");
                        numberOfLogicalProcessors.append(std::to_string(logicalProcessors.count()).c_str());

                        string name = cpu->get_name();

                        string architecture("Architecture: ");
                        architecture.append(cpu->get_architecture_str());

                        string addressWidth("Taille d'une adresse: ");
                        addressWidth.append(std::to_string(cpu->get_address_width()).c_str());
                        addressWidth.append("-bit");

                        ImGui::PushFont(g_TitleFont);
                        ImGui::Text("CPU");
                        ImGui::PopFont();
                        ImGui::Separator();

                        ImGui::Spacing();

                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Nom:");
                        ImGui::SameLine();
                        
                        ImGui::InputText("##", rm_const<char *>(name.str()), name.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
                        ImGui::Text(numberOfCores.str());
                        ImGui::Text(numberOfLogicalProcessors.str());
                        ImGui::Text(architecture.str());
                        ImGui::Text(addressWidth.str());

                        if(ImGui::CollapsingHeader("Caches"))
                        {
                            string numberOfL1Caches("Nombre de caches L1: ");
                            numberOfL1Caches.append(std::to_string(cpu->get_number_of_L1_caches()).c_str());

                            string numberOfL2Caches("Nombre de caches L2: ");
                            numberOfL2Caches.append(std::to_string(cpu->get_number_of_L2_caches()).c_str());

                            string numberOfL3Caches("Nombre de caches L3: ");
                            numberOfL3Caches.append(std::to_string(cpu->get_number_of_L3_caches()).c_str());

                            ImGui::Text(numberOfL1Caches.str());
                            ImGui::Text(numberOfL2Caches.str());
                            ImGui::Text(numberOfL3Caches.str());

                            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

                            if(ImGui::BeginTable("cpu_caches", 2, flags))
                            {
                                list<CPU::cache_entry> &cacheEntries = cpu->get_cache_entries();
                                size_t i;

                                ImGui::TableSetupColumn("Niveau");
                                ImGui::TableSetupColumn("Taille");
                                ImGui::TableHeadersRow();
                                for(i = 0; i < cacheEntries.count(); ++i)
                                {
                                    ImGui::TableNextRow();
                                    ImGui::TableNextColumn();
                                    switch(cacheEntries[i].level)
                                    {
                                        default:
                                        {
                                            ImGui::Text("N/A");
                                        } break;
                                        case CPU::cache_level::L1:
                                        {
                                            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f) , "L1");
                                        } break;
                                        case CPU::cache_level::L2:
                                        {
                                            ImGui::TextColored(ImVec4(0.91764705882352941176470588235294f, 0.85490196078431372549019607843137f, 0.14509803921568627450980392156863f, 1.0f), "L2");
                                        } break;
                                        case CPU::cache_level::L3:
                                        {
                                            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "L3");
                                        } break;
                                    }

                                    ImGui::TableNextColumn();
                                    uint64_t size = cacheEntries[i].size;
                                    if(size > 999 && size < 1000000)
                                    {
                                        ImGui::Text("%.2f ko", size / 1000.0f);
                                    }
                                    else if(size > 999999)
                                    {
                                        ImGui::Text("%.2f mo", size / 1000000.0f);
                                    }
                                    else
                                    {
                                        ImGui::Text("%llu o", size);
                                    }
                                
                                }

                                ImGui::EndTable();
                            }
                        }

                        

                        
                    } break;
                }
            }

            
            ImGui::End();
        }

        ImGui::PopFont();
    }

    /*
    =========================
    im_gui_debug_menu::addWindow
    =========================
    */
    void im_gui_debug_menu::addWindow(const window *window)
    {
        m_DebugPanelOptions.emplace(window, im_gui_debug_panel_options());
    }

    /*
    ============================
    im_gui_debug_menu::removeWindow
    ============================
    */
    void im_gui_debug_menu::removeWindow(const window *window)
    {
        m_DebugPanelOptions.erase(window);
    }

}
