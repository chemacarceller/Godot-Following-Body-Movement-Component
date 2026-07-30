#include "FollowingBodyMovementComponent.h"

// It provides the interface to ClassDB, Godot's internal database of all registered classes
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void FollowingBodyMovementComponent::_bind_methods() {

    // Record of methods for Godot to see
    ClassDB::bind_method(D_METHOD("set_enabled", "value"), &FollowingBodyMovementComponent::set_enabled);
    ClassDB::bind_method(D_METHOD("is_enabled"), &FollowingBodyMovementComponent::is_enabled);
    
    ClassDB::bind_method(D_METHOD("set_speed", "value"), &FollowingBodyMovementComponent::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &FollowingBodyMovementComponent::get_speed);

    ClassDB::bind_method(D_METHOD("set_distance", "value"), &FollowingBodyMovementComponent::set_distance);
    ClassDB::bind_method(D_METHOD("get_distance"), &FollowingBodyMovementComponent::get_distance);

    ClassDB::bind_method(D_METHOD("set_minHeight", "value"), &FollowingBodyMovementComponent::set_minHeight);
    ClassDB::bind_method(D_METHOD("get_minHeight"), &FollowingBodyMovementComponent::get_minHeight);

    ClassDB::bind_method(D_METHOD("set_bodyToFollow", "value"), &FollowingBodyMovementComponent::set_bodyToFollow);
    ClassDB::bind_method(D_METHOD("get_bodyToFollow"), &FollowingBodyMovementComponent::get_bodyToFollow);

    ClassDB::bind_method(D_METHOD("set_bodyToFollowPath", "value"), &FollowingBodyMovementComponent::set_bodyToFollowPath);
    ClassDB::bind_method(D_METHOD("get_bodyToFollowPath"), &FollowingBodyMovementComponent::get_bodyToFollowPath);

    // Exporting properties (equivalent to @export)
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_isEnabled"), "set_enabled", "is_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_distance"), "set_distance", "get_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_minHeight"), "set_minHeight", "get_minHeight");
    
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "_bodytoFollowPath"), "set_bodyToFollowPath", "get_bodyToFollowPath");
}


// Constructor and Destructor
FollowingBodyMovementComponent::FollowingBodyMovementComponent() {}

FollowingBodyMovementComponent::~FollowingBodyMovementComponent() {}


// Called when the node enters the scene tree for the first time.
void FollowingBodyMovementComponent::_ready() {

    // Equivalent to @onready var parentActor = get_parent()
    parentActor = Object::cast_to<Node3D>(get_parent());

    // Get the Node3D object to follow, in case a NodePath has been specified.
    if (!_bodyToFollowPath.is_empty()) {
        bodyToFollow = get_node<Node3D>(_bodyToFollowPath);
    }
}

void FollowingBodyMovementComponent::_physics_process(double delta) {

    // If the component is enabled and we have the necessary Node3D objects
    if (_isEnabled && parentActor != nullptr && bodyToFollow != nullptr) {
        
        // If the nodes are within the scene
        if (bodyToFollow->is_inside_tree() && parentActor->is_inside_tree()) {

            // Get the positions of both Node3D objects
            Vector3 body_to_follow_position = bodyToFollow->get_global_position();
            Vector3 parent_position = parentActor->get_global_position();
            
            // We calculate the direction in which it should move and the distance between both Node3D objects.
            Vector3 movement_direction = body_to_follow_position - parent_position;
            float distance_between_actor_and_pawn = Math::abs(movement_direction.length());

            // Do not enter the stopping distance range
            if (distance_between_actor_and_pawn > _distance) {
                movement_direction = movement_direction.normalized();
                parent_position += movement_direction * _speed * static_cast<float>(delta);
                parentActor->set_global_position(parent_position);
            }

            // Do not drop below min_height before entering the distance range
            if (parent_position.y < (body_to_follow_position.y + _minHeight)) {
                parent_position.y = body_to_follow_position.y + _minHeight;
                parentActor->set_global_position(parent_position);
            }

        }
    }
}


// This method is called when the node receives a notification. In this case, it listens for the NOTIFICATION_WM_CLOSE_REQUEST notification, which is sent when the window is requested to close. When this notification is received, the component calls queue_free() to free itself from memory, ensuring proper cleanup.
void FollowingBodyMovementComponent::_notification(int p_what) {

    // Handle the close request notification to ensure that the node is properly freed when the window is closed. This prevents potential memory leaks and ensures that the component is cleaned up correctly when the game is exited.
    if (p_what == NOTIFICATION_WM_CLOSE_REQUEST) {

        if (parentActor != nullptr) {

            // Get the unique ID that Godot assigned to this instance
            uint64_t instance_id = parentActor->get_instance_id();

            // Check with ObjectDB if that instance still exists in memory
            if (ObjectDB::get_instance(ObjectID(instance_id)) != nullptr) {

                // Free the node when the window close request is received. This ensures that the component is properly cleaned up and does not persist in memory after the game is closed.
                parentActor->queue_free();
                parentActor = nullptr;
            }
        }
    }
}