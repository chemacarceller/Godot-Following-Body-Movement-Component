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
        NodePath _bodyToFollowPath = NodePath("");

        // Private variables for internal use without exposing to Godot not underscored 

        // The Node3D object that the object holding this component should follow
        // Can only be specified at runtime; intended for prefabs instantiated at runtime.
        Node3D *bodyToFollow = nullptr;

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
        void set_enabled(bool value) { _isEnabled = value; }
        bool is_enabled() const { return _isEnabled; }

        void set_speed(float value) { _speed = value; }
        float get_speed() const { return _speed; }

        void set_distance(float value) { _distance = value; }
        float get_distance() const { return _distance; }

        void set_minHeight(float value) { _minHeight = value; }
        float get_minHeight() const { return _minHeight; }

        void set_bodyToFollow(Node3D *value) { bodyToFollow = value; }
        Node3D *get_bodyToFollow() const { return bodyToFollow; }

        void set_bodyToFollowPath(NodePath value) { _bodyToFollowPath = value; }
        NodePath get_bodyToFollowPath() const { return _bodyToFollowPath; }
};

#endif