#pragma once
#include <map>
#include <string>
#include <vcruntime_typeinfo.h>
#include <vector>


class Components
{
public:
    
};

struct i_component_handler 
{
    virtual ~i_component_handler() = default;
};

template <typename T> 
struct component_handler : i_component_handler
{
    std::vector<T> Components;
    std::map<int,int> index;  
};

struct component_manager
{
    std::map<std::string,i_component_handler*> component_map;

    template <typename T>
    void add_component(unsigned int id)
    { 
        if (component_map.contains(typeid(T).name()) == false)
        {
            component_map[typeid(T).name()] = new component_handler<T>();
        }
        component_handler<T>* handler = static_cast<component_handler<T>*>(component_map[typeid(T).name()]);
        handler->Components.emplace_back();
        handler->index[id] = handler->Components.size() - 1;
    }

    template <typename T>
    void remove_component(unsigned int id)
    {
        std::string component_name = typeid(T).name();
        if (component_map.contains(component_name) == false)
        {
            return;
        }
        component_handler<T>* handler = static_cast<component_handler<T>*>(component_map[component_name]);
        int idx = handler->index[id];
        
        handler->Components.erase(handler->Components.begin() + idx);
        handler->index.erase(id);

        for (auto& [key, value] : handler->index)
        {
            if (value > idx)
            {
                --value;
            }
        }
        
    }
    template <typename T>
    T& getComponent(unsigned int id)
    {
        component_handler<T>* compPtr = get_component_handler<T>();

        return compPtr->Components[compPtr->index[id]];
    }
    template <typename T>
    component_handler<T>* get_component_handler()
    {
        return static_cast<component_handler<T>*>(component_map[typeid(T).name()]);
    }
};