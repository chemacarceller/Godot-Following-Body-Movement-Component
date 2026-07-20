#ifndef FOLLOWING_BODY_MOVEMENT_COMPONENT_H
#define FOLLOWING_BODY_MOVEMENT_COMPONENT_H

// This is a custom movement component that allows an object (Node3D) to which it is attached to follow another object (Node3D) that can be specified
// The object to be followed can be specified via the NodePath (Editor + runtime) or the object itself (runtime)
// You can also specify the movement speed, the distance, and the height difference at which it will stop
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>

// Using the godot namespace to avoid prefixing all Godot classes with 'godot::'
using namespace godot;

// The class inherits from Node, allowing it to be added as a child to any Node in the scene tree
class FollowingBodyMovementComponent : public Node {

    // Macro to register the class with Godot's type system
    GDCLASS(FollowingBodyMovementComponent, Node)

    private:

        // Underscored private variable for exported variables (following Godot's convention)

        // Indicates if the component is enabled or not
        bool _isEnabled = true;

        // Specifies the speed at which the Node3D linked to this component will move.
        float _speed = 3.0f;

        // Distance at which the object being pursued will stop
        float _distance = 3.0f;

        // Height at which the object being pursued will remain
        float _minHeight = 1.0f;

        // The NodePath of the object that the object holding this component should follow
        // Designed to be specified in the editor
        NodePath _body_to_follow_path = NodePath("");

        // Private variables for internal use without exposing to Godot not underscored 

        // The Node3D object that the object holding this component should follow
        // Can only be specified at runtime; intended for prefabs instantiated at runtime.
        Node3D *body_to_follow = nullptr;

        // Node3D object to which this component has been linked
        Node3D *parentActor = nullptr;

    protected:

        // Registers the class, its methods, properties, and signals with the engine
        static void _bind_methods();

    public:

        // Constructor and destructor
        FollowingBodyMovementComponent();
        ~FollowingBodyMovementComponent();

        // Overridden methods from Node
        void _ready() override;
        void _physics_process(double delta) override;
        void _notification(int what);

        // Getters and setters for the properties
        void set_enabled(bool value);
        bool is_enabled() const;

        void set_speed(float value);
        float get_speed() const;

        void set_distance(float value);
        float get_distance() const;

        void set_min_height(float value);
        float get_min_height() const;

        void set_body_to_follow(Node3D *value);
        Node3D *get_body_to_follow() const;

        void set_body_to_follow_path(NodePath value);
        NodePath get_body_to_follow_path() const;
};

#endif // FOLLOWING_BODY_MOVEMENT_COMPONENT_H