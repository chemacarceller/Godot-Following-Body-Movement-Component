#include "FollowingBodyMovementComponent.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

FollowingBodyMovementComponent::FollowingBodyMovementComponent() {}

FollowingBodyMovementComponent::~FollowingBodyMovementComponent() {}

void FollowingBodyMovementComponent::_bind_methods() {

    // Vincular métodos de propiedades (Getters/Setters)
    ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &FollowingBodyMovementComponent::set_is_enabled);
    ClassDB::bind_method(D_METHOD("get_is_enabled"), &FollowingBodyMovementComponent::get_is_enabled);
    
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

    // Exportar variables al Inspector de Godot
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_IsEnabled"), "set_is_enabled", "get_is_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance"), "set_distance", "get_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "minHeight"), "set_min_height", "get_min_height");
    
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_bodyToFollow", PROPERTY_HINT_RESOURCE_TYPE, "CollisionObject3D"), "set_body_to_follow", "get_body_to_follow");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "_bodytoFollowPath"), "set_body_to_follow_path", "get_body_to_follow_path");
}

void FollowingBodyMovementComponent::_ready() {
    // Equivalente a @onready var _parentActor = get_parent()
    _parent_actor = Object::cast_to<Node3D>(get_parent());

    if (!_body_to_follow_path.is_empty()) {
        _body_to_follow = get_node<CollisionObject3D>(_body_to_follow_path);
    }
}

void FollowingBodyMovementComponent::_notification(int what) {
    if (what == NOTIFICATION_WM_CLOSE_REQUEST) {
        queue_free();
    }
}

void FollowingBodyMovementComponent::_physics_process(double delta) {

    if (_is_enabled && _parent_actor != nullptr && _body_to_follow != nullptr) {
            
        if (_body_to_follow->is_inside_tree() && _parent_actor->is_inside_tree()) {

            Vector3 body_to_follow_position = _body_to_follow->get_global_position();
            Vector3 parent_position = _parent_actor->get_global_position();
            
            Vector3 movement_direction = body_to_follow_position - parent_position;
            float distance_between_actor_and_pawn = Math::abs(movement_direction.length());

            // No bajar de min_height antes de entrar en distancia
            if (parent_position.y < (body_to_follow_position.y + min_height)) {
                parent_position.y = body_to_follow_position.y + min_height;
                _parent_actor->set_position(parent_position);
            }

            // No entrar en el rango de distancia de parada
            if (distance_between_actor_and_pawn > distance) {
                movement_direction = movement_direction.normalized();
                parent_position += movement_direction * speed * static_cast<float>(delta);
                _parent_actor->set_position(parent_position);
               
                // Volver a comprobar no bajar de min_height tras el movimiento
                if (parent_position.y < (body_to_follow_position.y + min_height)) {
                    parent_position.y = body_to_follow_position.y + min_height;
                    _parent_actor->set_position(parent_position);
                }
            }
        }
    }
}

// Implementación de Getters y Setters
void FollowingBodyMovementComponent::set_is_enabled(bool value) { _is_enabled = value; }
bool FollowingBodyMovementComponent::get_is_enabled() const { return _is_enabled; }

void FollowingBodyMovementComponent::set_speed(float value) { speed = value; }
float FollowingBodyMovementComponent::get_speed() const { return speed; }

void FollowingBodyMovementComponent::set_distance(float value) { distance = value; }
float FollowingBodyMovementComponent::get_distance() const { return distance; }

void FollowingBodyMovementComponent::set_min_height(float value) { min_height = value; }
float FollowingBodyMovementComponent::get_min_height() const { return min_height; }

void FollowingBodyMovementComponent::set_body_to_follow(CollisionObject3D *value) { _body_to_follow = value; }
CollisionObject3D *FollowingBodyMovementComponent::get_body_to_follow() const { return _body_to_follow; }

void FollowingBodyMovementComponent::set_body_to_follow_path(NodePath value) { _body_to_follow_path = value; }
NodePath FollowingBodyMovementComponent::get_body_to_follow_path() const { return _body_to_follow_path; }