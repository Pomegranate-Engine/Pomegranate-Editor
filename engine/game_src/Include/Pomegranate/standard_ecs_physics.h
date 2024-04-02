#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_PHYSICS_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_PHYSICS_H

#include "ecs.h"
#include "vec_math.h"
#include "standard_ecs.h"
#if defined(_OPENMP)
#include <omp.h>
#endif
namespace Pomegranate
{
    enum PhysicsBodyType
    {
        PHYSICS_BODY_TYPE_KINEMATIC,
        PHYSICS_BODY_TYPE_RIGID,
        PHYSICS_BODY_TYPE_STATIC
    };
    enum CollisionShapeType
    {
        COLLISION_SHAPE_TYPE_RECTANGLE,
        COLLISION_SHAPE_TYPE_CIRCLE
    };

    class PhysicsObject : public Component
    {
    public:
        Vec2 cur_pos;
        Vec2 old_pos;
        float cur_rot;
        float old_rot;
        Vec2 acceleration;
        float rotational_acceleration;
        float mass;
        float gravity_scale;
        float drag = 0.0;
        bool use_collision;
        bool initialized = false;
        PhysicsBodyType body_type;
        PhysicsObject();
        void init(Entity *) override;

        static Vec2 gravity;
        static std::vector<Entity*> objects;
        void add_force(Vec2 force);
        void add_impulse(Vec2 impulse);
        void set_velocity(Vec2 velocity);
        Vec2 get_velocity() const;
    };

    class CollisionShape : public Component
    {
    public:
        CollisionShapeType shape_type;
        float radius;
        Vec2 size;
        float restitution;
        CollisionShape();
    };

    class KinematicBody: public System
    {
    public:
        void tick(Entity* entity) override;
    };

    class RigidBody: public System
    {
    public:
        RigidBody();
        void tick(Entity* entity) override;
        static void solve_collisions(Entity* entity);
        static int sub_steps;
    };
}

#endif //POMEGRANATE_ENGINE_STANDARD_ECS_PHYSICS_H
