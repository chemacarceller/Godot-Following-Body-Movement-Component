#include "register_types.h"
#include "FollowingBodyMovementComponent.h"

using namespace godot;

void initialize_FollowingBodyMovementComponent(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // We registered our personalized class
    ClassDB::register_class<FollowingBodyMovementComponent>();
}

void uninitialize_FollowingBodyMovementComponent(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
    GDExtensionBool GDE_EXPORT
    FollowingBodyMovementComponent_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization*r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address,p_library, r_initialization);
        init_obj.register_initializer(initialize_FollowingBodyMovementComponent);
        init_obj.register_terminator(uninitialize_FollowingBodyMovementComponent);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        return init_obj.init();
    }
}