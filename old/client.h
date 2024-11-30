#pragma once
#include "ent_camera.h"
#include "ent_controller.h"
#include "level_editor_controller.h"

struct Client : public ent
{
    level_editor_controller controller;
    ent_camera* camera = nullptr;


    virtual std::string get_member(std::string member_name) { return "Not Implemented"; }

    // Default constructor
    Client()
    {
        camera = new ent_camera();
        controller.mControlledEntity = camera;
    }
    //static std::string get_member(void* me, std::string member_name) { return "Not implemented"; }

    static void parallel_init() {};
    static void parallel_tick() {};

    // Implement a custom copy constructor if necessary
    Client(const Client& other)
    {
        // If controller is copyable
        controller = other.controller;

        // Deep copy the camera if needed (assuming it's a raw pointer)
        if (other.camera) {
            camera = new ent_camera(*other.camera);
        }
        else {
            camera = nullptr;
        }

        // Also update controller's controlled entity
        controller.mControlledEntity = camera;
    }

    // Implement a move constructor to allow transfer of ownership
    Client(Client&& other) noexcept
        : controller(std::move(other.controller)), camera(other.camera)
    {
        other.camera = nullptr; // Nullify the other camera pointer to prevent double deletion
        controller.mControlledEntity = camera;
    }

    // Implement a move assignment operator
    Client& operator=(Client&& other) noexcept
    {
        if (this != &other) {
            // Transfer ownership of the camera
            delete camera; // Delete current camera if necessary
            camera = other.camera;
            other.camera = nullptr;

            // Move controller
            controller = std::move(other.controller);

            // Update the controlled entity
            controller.mControlledEntity = camera;
        }
        return *this;
    }

    // Clean up camera resource
    ~Client()
    {
        delete camera;
    }

    static const char* get_name() { return "Client"; }


    void init() override {}
    void tick() override {}
    void exec(const ent_command& command) override {}


};