#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>
#include <typeindex>

#include <overkill/graphics_internal/UniformBuffer.hpp>
#include <overkill/graphics_internal/FrameBuffer.hpp>

#include "GameObject.hpp"
#include "Components/BaseComponents.hpp"
#include "Components/LightSource.hpp"
#include "Components/Transform.hpp"
#include "Components/Camera.hpp"
#include "../RenderSystem.hpp"
#include <overkill/scene/Viewport.hpp>

namespace OK
{
    struct Transform;
    struct SimpleMeshRenderer;


    struct ComponentRegistryBase
    {
    };
    template<typename T>
    struct ComponentRegistry : public ComponentRegistryBase
    {
    public:
        std::vector<T*> registered_components;
    };

    class Scene
    {
        friend class GameObject;
        std::string name;
        
        Viewport viewport;

        std::vector<GameObject*> objects;
        std::unordered_map<std::type_index, ComponentRegistryBase*> components;

        ComponentRegistry<GraphicsComponent>* renderers;
        ComponentRegistry<LightSource>* light_sources;

        UniformBuffer* shared_matrices_buffer;
        UniformBuffer* light_buffer;

        Transform* scene_graph;
        Camera* camera;

    public:
        OK::FrameBuffer* main_framebuffer = nullptr;
        //RenderSystem renderSystem;


        Scene(const char* name);

        constexpr const std::vector<GameObject*>& Scene::get_objects() const;
        
        Transform* const & root() const;
        Camera* const& active_camera() const;
        GameObject *& add_game_object(const char* name);
        void propagate_scene_graph(); 

        template<typename T>
        const std::vector<T*>& get_components()
        {
            return ((ComponentRegistry<T>*)components[std::type_index(typeid(T))])->registered_components;
        }

        void start_scene();
        void render();
        void update(float dt);
        void late_update(float dt);
        
        void set_viewport_shader(ShaderProgram* program);
        void print_scene_graph() const;

    private:

        template<typename T>
        void register_component(T* component) {
            std::cout << "DEBUG: Register component " << component->game_object->get_name() << "::" << typeid(*component).name() << std::endl;
            auto v = component->component_type_signatures();
            for (const auto& typeIndex : v)
            {
                std::cout << "\tRegistering as " << typeIndex.name() << std::endl;

                const auto registry = components.find(typeIndex);
                if (registry != components.end())
                {
                    ((ComponentRegistry<Component>*)registry->second)->registered_components.push_back(component);
                }
                else
                {
                    ComponentRegistry<T>* registry = new ComponentRegistry<T>();
                    registry->registered_components.push_back(component);
                    components[typeIndex] = registry;
                }
            }
        }

        void propagate_trs(Transform* transform, glm::mat4 parent_trs);
        void print_scene_graph_internal(Transform* transform = nullptr, int indent=1) const;

        ~Scene();
    };
}
