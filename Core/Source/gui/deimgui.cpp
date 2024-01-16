#include "DE/gui/deimgui.hpp"
#include "DE/memory/memory.hpp"
#include "DE/memory/list.hpp"
#include "DE/memory/pair.hpp"
#include "DE/string_utils.hpp"
#include "DE/vec.hpp"
#include "DE/window.hpp"
#include "DE/ecs/scene.hpp"
#include "DE/ecs/component.hpp"
#include "DE/hardware/cpu.hpp"
#include "DE/memory/settings.hpp"
#include "DE/resources.hpp"
#include "DE/drivers/opengl/vao.hpp"
#include "DE/drivers/opengl/shader.hpp"
#include "DE/drivers/opengl/texture.hpp"

#include <unordered_map>
#include <string>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

namespace deep
{

    struct vao_selection
    {
        GL3::vao_manager::vao_item *currentVao;
        GL3::gl_id currentVaoID;
        pair<size_t, size_t> selection;

        vao_selection();
    };

    vao_selection::vao_selection()
        : currentVao(nullptr),
          currentVaoID(0),
          selection(0, static_cast<size_t>(-1))
    { }

    struct shader_selection
    {
        GL3::program_manager::program_item *currentProgram;
        GL3::gl_id currentProgramID;
        pair<size_t, size_t> selection;

        shader_selection();
    };

    shader_selection::shader_selection()
        : currentProgram(nullptr),
          currentProgramID(0),
          selection(0, static_cast<size_t>(-1))
    { }

    struct texture_selection
    {
        GL3::texture_manager::texture_item *currentTexture;
        GL3::gl_id currentTextureID;
        pair<size_t, size_t> selection;

        texture_selection();
    };

    texture_selection::texture_selection()
        : currentTexture(nullptr),
          currentTextureID(0),
          selection(0, static_cast<size_t>(-1))
    { }

    struct edit_vao
    {
        GL3::gl_id vboID;
        GL3::gl_id vaoID;

        edit_vao();
    };

    edit_vao::edit_vao()
        : vboID(0),
          vaoID(0)
    { }

    struct edit_texture
    {
        GL3::gl_id textureID;
        GL3::texture_manager::texture_item *item;
    };

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
        std::string text("[" + std::to_string(id) + std::string("] ") + scene.get_name());
        if(id == scene::get_active_scene_id())
            text += " (active)";

        ImGui::AlignTextToFramePadding();
        ImGui::Text(text.c_str()); ImGui::SameLine();
        if(ImGui::Button(std::string("Supprimer##" + std::to_string(id)).c_str())) {
            scene::must_be_deleted(id);
        }
        ImGui::SameLine();
        if(ImGui::Button(std::string("Rendre active##" + std::to_string(id)).c_str())) {
            scene::set_active_scene(id);
        }
    }

    void __edit_vao_enum_callback(GL3::gl_id vaoID, GL3::vao_manager::vao_item *vao, mem_ptr args)
    {
        edit_vao *edit = (edit_vao *) args;

        if(ImGui::Selectable(vao->name.str(), edit->vaoID == vaoID))
        {
            edit->vaoID = vaoID;
            edit->vboID = vao->attachedVbo;
        }
    }

    void __edit_texture_enum_callback(GL3::gl_id textureID, GL3::texture_manager::texture_item *texture, mem_ptr args)
    {
        edit_texture *edit = (edit_texture *) args;

        if(ImGui::Selectable(texture->name.str(), edit->textureID == textureID))
        {
            edit->textureID = textureID;
            edit->item = texture;
        }
    }

    void __edit_program_enum_callback(GL3::gl_id programID, GL3::program_manager::program_item *program, mem_ptr args)
    {
        GL3::gl_id *id = (GL3::gl_id *) args;

        if(ImGui::Selectable(program->name.str(), *id == programID))
        {
            *id = programID;
        }
    }

    void __entities_enum_callback(entity_manager::entity &ent)
    {
        entity_manager *entityManager = entity_manager::get_singleton();

        std::string text("[" + std::string(ent.get_name().str()) + "]");

        ImGui::AlignTextToFramePadding();

        bool attribMenuOpened = ImGui::TreeNode(text.c_str());

        ImGui::SameLine();

        string buttonText("Supprimer");
        ImVec2 textSize = ImGui::CalcTextSize(buttonText.str());
        textSize.y *= 1.5f;
        textSize.x *= 1.15f;

        buttonText.append("##");
        buttonText.append(std::to_string(ent.get_entity_id()).c_str());

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - textSize.x - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        if(ImGui::Button(buttonText.str(), textSize))
        {
            entityManager->must_be_deleted(ent.get_entity_id(), ent.get_collection_id());
        }

        if(attribMenuOpened)
        {
            string entityID("ID: ");
            entityID.append(std::to_string(ent.get_entity_id()).c_str());

            component_manager::component_type componentTypes = entityManager->get_component_types(ent.get_entity_id(), ent.get_collection_id());

            component_manager *componentManager = component_manager::get_singleton();
            GL3::program_manager *programManager = GL3::program_manager::get_singleton();
            GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();
            GL3::texture_manager *textureManager = GL3::texture_manager::get_singleton();

            ImGui::Text(entityID.str());
            ImGui::Text("Liste des composants attachés:");

            if((to_utype(componentTypes) & to_utype(component_manager::component_type::drawable)) > 0)
            {
                if(ImGui::TreeNode("drawable_component"))
                {
                    component_id id = entityManager->get_component_id(ent.get_entity_id(), ent.get_collection_id(), component_manager::component_type::drawable);
                    drawable_component *drawable = componentManager->get_drawable_component(id);

                    if(drawable != nullptr)
                    {
                        GL3::program_manager::program_item *program = programManager->get(drawable->program);
                        GL3::vao_manager::vao_item *vao = vaoManager->get(drawable->vao);
                        GL3::texture_manager::texture_item *texture = textureManager->get(drawable->texture);

                        if(ImGui::BeginCombo("VAO", vao != nullptr ? vao->name.str() : ""))
                        {
                            edit_vao edit;
                            edit.vboID = drawable->vbo;
                            edit.vaoID = drawable->vao;

                            vaoManager->enum_vaos(__edit_vao_enum_callback, &edit);

                            drawable->vbo = edit.vboID;
                            drawable->vao = edit.vaoID;

                            ImGui::EndCombo();
                        }

                        if(ImGui::BeginCombo("Texture", texture != nullptr ? texture->name.str() : ""))
                        {
                            edit_texture edit;
                            edit.textureID = drawable->texture;

                            if(texture != nullptr)
                                edit.item = texture;
                            else
                                edit.item = nullptr;

                            textureManager->enum_textures(__edit_texture_enum_callback, &edit);

                            drawable->texture = edit.textureID;

                            ImGui::EndCombo();
                        }

                        if(ImGui::BeginCombo("Shader", program != nullptr ? program->name.str() : ""))
                        {
                            programManager->enum_programs(__edit_program_enum_callback, &drawable->program);

                            ImGui::EndCombo();
                        }
                    }

                    ImGui::TreePop();
                }
            }
                
            if((to_utype(componentTypes) & to_utype(component_manager::component_type::transformation)) > 0)
            {
                if(ImGui::TreeNode("transformation_component"))
                {
                    component_id id = entityManager->get_component_id(ent.get_entity_id(), ent.get_collection_id(), component_manager::component_type::transformation);
                    transformation_component *transformation = componentManager->get_transformation_component(id);

                    if(transformation != nullptr)
                    {
                        vec3<float> translation = transformation->get_translation();
                        vec3<float> scaling = transformation->get_scaling();
                        float rotationX = transformation->get_rotation_X();
                        float rotationY = transformation->get_rotation_Y();
                        float rotationZ = transformation->get_rotation_Z();

                        ImGui::DragScalar("X", ImGuiDataType_Float, &translation.x, 0.01f);
                        ImGui::DragScalar("Y", ImGuiDataType_Float, &translation.y, 0.01f);
                        ImGui::DragScalar("Z", ImGuiDataType_Float, &translation.z, 0.01f);

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::DragScalar("Taille X", ImGuiDataType_Float, &scaling.x, 0.01f);
                        ImGui::DragScalar("Taille Y", ImGuiDataType_Float, &scaling.y, 0.01f);
                        ImGui::DragScalar("Taille Z", ImGuiDataType_Float, &scaling.z, 0.01f);

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::DragScalar("Rotation X", ImGuiDataType_Float, &rotationX, 0.01f);
                        ImGui::DragScalar("Rotation Y", ImGuiDataType_Float, &rotationY, 0.01f);
                        ImGui::DragScalar("Rotation Z", ImGuiDataType_Float, &rotationZ, 0.01f);

                        ImGui::Spacing();
                        ImGui::Spacing();

                        transformation->set_translation(translation);
                        transformation->set_scaling(scaling);
                        transformation->set_rotation_X(rotationX);
                        transformation->set_rotation_Y(rotationY);
                        transformation->set_rotation_Z(rotationZ);
                    }

                    ImGui::TreePop();
                }
            }

            if((to_utype(componentTypes) & to_utype(component_manager::component_type::velocity)) > 0)
                ImGui::BulletText("velocity_component");
            if((to_utype(componentTypes) & to_utype(component_manager::component_type::collider)) > 0)
                ImGui::BulletText("collider_component");
            if((to_utype(componentTypes) & to_utype(component_manager::component_type::acceleration)) > 0)
                ImGui::BulletText("acceleration_component");
            if((to_utype(componentTypes) & to_utype(component_manager::component_type::health)) > 0)
                ImGui::BulletText("health_component");

            ImGui::TreePop();
        }
    }

    void __vaos_enum_callback(GL3::gl_id vaoID, GL3::vao_manager::vao_item *_vao, mem_ptr args)
    {
        vao_selection *sel = (vao_selection *) args;

        if(ImGui::Selectable(_vao->name.str(), sel->selection.value1() == sel->selection.value2()))
        {
            sel->selection.value2() = sel->selection.value1();
            sel->currentVao = _vao;
            sel->currentVaoID = vaoID;
        }

        sel->selection.value1()++;
    }

    void __programs_enum_callback(GL3::gl_id programID, GL3::program_manager::program_item *program, mem_ptr args)
    {
        shader_selection *sel = (shader_selection *) args;

        const char *name = program->name.str();

        if(ImGui::Selectable((name != nullptr ? name : ""), sel->selection.value1() == sel->selection.value2()))
        {
            sel->selection.value2() = sel->selection.value1();
            sel->currentProgram = program;
            sel->currentProgramID = programID;
        }

        sel->selection.value1()++;
    }

    void __textures_enum_callback(GL3::gl_id textureID, GL3::texture_manager::texture_item *texture, mem_ptr args)
    {
        texture_selection *sel = (texture_selection *) args;

        const char *name = texture->name.str();

        if(ImGui::Selectable((name != nullptr ? name : ""), sel->selection.value1() == sel->selection.value2()))
        {
            sel->selection.value2() = sel->selection.value1();
            sel->currentTexture = texture;
            sel->currentTextureID = textureID;
        }

        sel->selection.value1()++;
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

        scene_id scene = scene::get_active_scene_id();
        entity_collection_id collection = scene::get_entity_collection(scene);

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
                                scene::create_scene(nameBuffer);
                                nameBuffer[0] = '\0';
                            }
                        }

                        if(emptyNameBuffer)
                            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Le nom de la scène ne peut pas être vide.");

                        ImGui::Spacing();

                        // Affiche toutes les scènes existantes.
                        if(ImGui::CollapsingHeader("Liste des scènes"))
                        {
                            scene::enum_scenes(scenes_enum_callback);
                        }
                    } break;
                    // Menu des entités.
                    case ImGuiDebugMenuEntitiesView:
                    {
                        ImGui::PushFont(g_TitleFont);
                        ImGui::Text("Entités");
                        ImGui::PopFont();
                        ImGui::Separator();

                        ImGui::Spacing();

                        // Menu pour ajouter une entité.
                        if(ImGui::CollapsingHeader("Ajouter une entité"))
                        {
                            static char nameBuffer[128] = "";
                            static float x = 0.0f, y = 0.0f, z = 0.0f;
                            static float sx = 1.0f, sy = 1.0f, sz = 1.0f;
                            static float rotation = 0.0f;

                            static bool addDrawableComponent = false;
                            static bool addTransformationComponent = false;

                            static vao_selection vaoSel;
                            static shader_selection shaderSel;
                            static texture_selection textureSel;

                            if(ImGui::TreeNode("drawable_component"))
                            {
                                ImGui::Checkbox("Attacher le composant", &addDrawableComponent);

                                if(ImGui::BeginCombo("VAO", vaoSel.currentVao != nullptr ? vaoSel.currentVao->name.str() : ""))
                                {
                                    GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();
                                    vaoSel.selection.value1() = 0;

                                    vaoManager->enum_vaos(__vaos_enum_callback, &vaoSel);

                                    ImGui::EndCombo();
                                }

                                if(ImGui::BeginCombo("Texture", textureSel.currentTexture != nullptr ? textureSel.currentTexture->name.str() : ""))
                                {
                                    GL3::texture_manager *textureManager = GL3::texture_manager::get_singleton();
                                    textureSel.selection.value1() = 0;

                                    textureManager->enum_textures(__textures_enum_callback, &textureSel);

                                    ImGui::EndCombo();
                                }

                                if(ImGui::BeginCombo("Shader", shaderSel.currentProgram != nullptr ? shaderSel.currentProgram->name.str() : ""))
                                {
                                    GL3::program_manager *programManager = GL3::program_manager::get_singleton();
                                    shaderSel.selection.value1() = 0;

                                    programManager->enum_programs(__programs_enum_callback, &shaderSel);

                                    ImGui::EndCombo();
                                }

                                ImGui::TreePop();
                            }

                            if(ImGui::TreeNode("transformation_component"))
                            {
                                ImGui::Checkbox("Attacher le composant", &addTransformationComponent);

                                ImGui::DragScalar("X", ImGuiDataType_Float, &x);
                                ImGui::DragScalar("Y", ImGuiDataType_Float, &y);
                                ImGui::DragScalar("Z", ImGuiDataType_Float, &z);

                                ImGui::Spacing();
                                ImGui::Spacing();

                                ImGui::DragScalar("Taille X", ImGuiDataType_Float, &sx);
                                ImGui::DragScalar("Taille Y", ImGuiDataType_Float, &sy);
                                ImGui::DragScalar("Taille Z", ImGuiDataType_Float, &sz);

                                ImGui::Spacing();
                                ImGui::Spacing();

                                ImGui::DragScalar("Rotation", ImGuiDataType_Float, &rotation);

                                ImGui::TreePop();
                            }

                            ImGui::Spacing();

                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Nom:"); ImGui::SameLine(); ImGui::InputText("##", nameBuffer, sizeof(nameBuffer)); ImGui::SameLine();
                            if(ImGui::Button("Créer"))
                            {
                                component_manager *componentManager = component_manager::get_singleton();

                                entity_manager::entity ent = entityManager->create_entity(nameBuffer, scene::get_entity_collection(scene::get_active_scene_id()));

                                if(addDrawableComponent)
                                {
                                    if(vaoSel.currentVao == nullptr || shaderSel.currentProgram == nullptr)
                                        goto cancel_component;

                                    component_id id = componentManager->create_drawable_component(shaderSel.currentProgramID, vaoSel.currentVao->attachedVbo, vaoSel.currentVaoID, textureSel.currentTextureID);

                                    drawable_component *drawableComponent = componentManager->get_drawable_component(id);
                                    drawableComponent->renderCallback = drawable_component::classic_render_callback;

                                    entityManager->attach_component(ent.get_entity_id(), ent.get_collection_id(), id);
                                }

                                if(addTransformationComponent)
                                {
                                    component_id id = componentManager->create_transformation_component(vec3<float>(x, y, z), vec3<float>(sx, sy, sz), rotation);

                                    entityManager->attach_component(ent.get_entity_id(), ent.get_collection_id(), id);
                                }

                                nameBuffer[0] = '\0';
cancel_component: ;
                            }
                        }

                        ImGui::Spacing();

                        // Affiche toutes les entités de la scène.
                        if(ImGui::CollapsingHeader("Liste des entités de la scène"))
                        {
                            const char *textButton = "Tout supprimer";

                            ImGui::AlignTextToFramePadding();

                            ImVec2 textSize = ImGui::CalcTextSize(textButton);
                            textSize.y *= 1.5f;
                            textSize.x *= 1.2f;

                            // Affiche le nombre d'entités de la scène.
                            ImGui::Text(std::string("Nombre d'entités: " + std::to_string(entityManager->get_number_of_entities(collection))).c_str()); ImGui::SameLine();
                            
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - textSize.x - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
                            if(ImGui::Button(textButton, textSize))
                            {
                                entityManager->destroy_all_entities(collection);
                            }

                            ImGui::Spacing();

                            entityManager->enum_entities(collection, __entities_enum_callback);
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
    ==============================
    im_gui_debug_menu::render_info
    ==============================
    */
    void im_gui_debug_menu::render_info(const window *window)
    {
        ImGui::PushFont(g_NormalFont);

        ImVec2 windowSize(180, 120);
        ImVec2 windowPos(
            window->get_width() - windowSize.x,
            0
        );

        

        ImGui::SetNextWindowPos(windowPos);
        ImGui::SetNextWindowSize(windowSize);
        if(ImGui::Begin("##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            string frames("UPS: ");
            frames.append(std::to_string(window->get_UPS()).c_str());
            frames.append(" FPS: ");
            frames.append(std::to_string(window->get_FPS()).c_str());

            scene *currentScene = scene::get_scene(scene::get_active_scene_id());
            camera &camera = currentScene->get_attached_camera();

            vec3<float> camPos = camera.get_position();

            string positionX("X: ");
            positionX.append(std::to_string(camPos.x).c_str());

            string positionY("Y: ");
            positionY.append(std::to_string(camPos.y).c_str());

            string positionZ("Z: ");
            positionZ.append(std::to_string(camPos.z).c_str());

            ImGui::Text(frames.str());
            ImGui::Separator();
            ImGui::Text(positionX.str());
            ImGui::Text(positionY.str());
            ImGui::Text(positionZ.str());

            ImGui::End();
        }

        ImGui::PopFont();
    }

    /*
    =============================
    im_gui_debug_menu::add_window
    =============================
    */
    void im_gui_debug_menu::add_window(const window *window)
    {
        m_DebugPanelOptions.emplace(window, im_gui_debug_panel_options());
    }

    /*
    ================================
    im_gui_debug_menu::remove_window
    ================================
    */
    void im_gui_debug_menu::remove_window(const window *window)
    {
        m_DebugPanelOptions.erase(window);
    }

}
