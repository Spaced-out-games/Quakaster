#pragma once
#include <vector>
#include <include/base/System.h>
#include <functional>


struct SystemManager {
public:
    // Access systems using the operator[]
    ISystem* operator[] (size_t index) {
        return systems.at(index);
    }



    //inline void init_all_systems()

    const std::vector<ISystem*>& get_systems() {
        return systems;
    }

    // Add a new system
    template <typename system_t, typename... Args>
    ISystem* add_system(Args&&... args) {
        systems.push_back(new system_t(std::forward<Args>(args)...)); // Allocate memory for the new system
        return systems.at(systems.size() - 1);
    }

    // Destructor to clean up allocated systems
    ~SystemManager() {
        for (auto system : systems) {
            delete system; // Free the allocated memory
        }
    }
protected:
    friend class Client;
    friend class Server;
    inline void init_system(int index, QKScene& scene) const {
        systems[index]->init(scene);
    }

    inline void tick_system(int index, QKScene& scene) const {
        systems[index]->tick(scene);
    }

    inline void destroy_system(int index, QKScene& scene) const {

    }
private:
    std::vector<ISystem*> systems; // Store raw pointers to systems
};