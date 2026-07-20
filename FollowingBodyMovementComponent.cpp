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

    ClassDB::bind_method(D_METHOD("set_min_height", "value"), &FollowingBodyMovementComponent::set_min_height);
    ClassDB::bind_method(D_METHOD("get_min_height"), &FollowingBodyMovementComponent::get_min_height);

    ClassDB::bind_method(D_METHOD("set_body_to_follow", "value"), &FollowingBodyMovementComponent::set_body_to_follow);
    ClassDB::bind_method(D_METHOD("get_body_to_follow"), &FollowingBodyMovementComponent::get_body_to_follow);

    ClassDB::bind_method(D_METHOD("set_body_to_follow_path", "value"), &FollowingBodyMovementComponent::set_body_to_follow_path);
    ClassDB::bind_method(D_METHOD("get_body_to_follow_path"), &FollowingBodyMovementComponent::get_body_to_follow_path);

    // Exporting properties (equivalent to @export)
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_isEnabled"), "set_enabled", "is_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_distance"), "set_distance", "get_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_minHeight"), "set_min_height", "get_min_height");
    
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "_bodytoFollowPath"), "set_body_to_follow_path", "get_body_to_follow_path");
}


// Constructor and Destructor
FollowingBodyMovementComponent::FollowingBodyMovementComponent() {}

FollowingBodyMovementComponent::~FollowingBodyMovementComponent() {}


// Called when the node enters the scene tree for the first time.
void FollowingBodyMovementComponent::_ready() {

    // Equivalent to @onready var parentActor = get_parent()
    parentActor = Object::cast_to<Node3D>(get_parent());

    // Get the Node3D object to follow, in case a NodePath has been specified.
    if (!_body_to_follow_path.is_empty()) {
        body_to_follow = get_node<Node3D>(_body_to_follow_path);
    }
}

void FollowingBodyMovementComponent::_physics_process(double delta) {

    // If the component is enabled and we have the necessary Node3D objects
    if (_isEnabled && parentActor != nullptr && body_to_follow != nullptr) {
        
        // If the nodes are within the scene
        if (body_to_follow->is_inside_tree() && parentActor->is_inside_tree()) {

            // Get the positions of both Node3D objects
            Vector3 body_to_follow_position = body_to_follow->get_global_position();
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


// Implementation of Getters and Setters
void FollowingBodyMovementComponent::set_enabled(bool value) { _isEnabled = value; }
bool FollowingBodyMovementComponent::is_enabled() const { return _isEnabled; }

void FollowingBodyMovementComponent::set_speed(float value) { _speed = value; }
float FollowingBodyMovementComponent::get_speed() const { return _speed; }

void FollowingBodyMovementComponent::set_distance(float value) { _distance = value; }
float FollowingBodyMovementComponent::get_distance() const { return _distance; }

void FollowingBodyMovementComponent::set_min_height(float value) { _minHeight = value; }
float FollowingBodyMovementComponent::get_min_height() const { return _minHeight; }

void FollowingBodyMovementComponent::set_body_to_follow(Node3D *value) { body_to_follow = value; }
Node3D *FollowingBodyMovementComponent::get_body_to_follow() const { return body_to_follow; }

void FollowingBodyMovementComponent::set_body_to_follow_path(NodePath value) { _body_to_follow_path = value; }
NodePath FollowingBodyMovementComponent::get_body_to_follow_path() const { return _body_to_follow_path; }


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