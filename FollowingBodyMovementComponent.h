#ifndef FOLLOWING_BODY_MOVEMENT_COMPONENT_H
#define FOLLOWING_BODY_MOVEMENT_COMPONENT_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/collision_object3d.hpp>

using namespace godot;

class FollowingBodyMovementComponent : public Node {

    GDCLASS(FollowingBodyMovementComponent, Node)

    private:
        bool _is_enabled = true;
        float speed = 3.0f;
        float distance = 3.0f;
        float min_height = 1.0f;

        CollisionObject3D *_body_to_follow = nullptr;
        NodePath _body_to_follow_path = NodePath("");

        Node3D *_parent_actor = nullptr;

    protected:
        static void _bind_methods();

    public:
        FollowingBodyMovementComponent();
        ~FollowingBodyMovementComponent();

        void _ready() override;
        void _physics_process(double delta) override;
        void _notification(int what);

        // Getters y Setters para las propiedades
        void set_is_enabled(bool value);
        bool get_is_enabled() const;

        void set_speed(float value);
        float get_speed() const;

        void set_distance(float value);
        float get_distance() const;

        void set_min_height(float value);
        float get_min_height() const;

        void set_body_to_follow(CollisionObject3D *value);
        CollisionObject3D *get_body_to_follow() const;

        void set_body_to_follow_path(NodePath value);
        NodePath get_body_to_follow_path() const;
};

#endif // FOLLOWING_BODY_MOVEMENT_COMPONENT_H