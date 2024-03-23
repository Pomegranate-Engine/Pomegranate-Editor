#include "standard_ecs_physics.h"
namespace Pomegranate
{
    Vec2 PhysicsObject::gravity = Vec2(0.0f, 980.0f);
    std::vector<Entity*> PhysicsObject::objects = std::vector<Entity*>();
    int RigidBody::sub_steps = 2;

    PhysicsObject::PhysicsObject()
    {
        this->cur_pos = Vec2(0.0f, 0.0f);
        this->old_pos = Vec2(0.0f, 0.0f);
        this->cur_rot = 0.0f;
        this->old_rot = 0.0f;

        this->acceleration = Vec2(0,0);
        this->rotational_acceleration = 0.0f;

        this->mass = 1.0f;
        this->gravity_scale = 1.0f;
        this->use_collision = true;
        this->body_type = PHYSICS_BODY_TYPE_RIGID;
        register_component(PhysicsObject);
        push_data<Vec2>("acceleration", &this->acceleration);
        push_data<float>("mass", &this->mass);
        push_data<float>("gravity_scale", &this->gravity_scale);
        push_data<bool>("use_collision", &this->use_collision);
        push_data<int>("body_type", &this->body_type);
    }
    void PhysicsObject::add_force(Vec2 force)
    {
        this->acceleration += force / this->mass;
    }
    void PhysicsObject::add_impulse(Vec2 impulse)
    {
        this->acceleration += impulse / this->mass;
    }
    void PhysicsObject::set_velocity(Vec2 velocity)
    {
        this->old_pos = cur_pos - velocity;
    }
    Vec2 PhysicsObject::get_velocity() const
    {
        return cur_pos - old_pos;
    }
    void PhysicsObject::init(Entity * e)
    {
        e->require_component<Transform>();
        this->old_pos = e->get_component<Transform>()->pos;
        PhysicsObject::objects.push_back(e);
    }

    void KinematicBody::tick(Entity *entity)
    {

    }

    void RigidBody::tick(Entity *entity)
    {
        if(entity->has_component<PhysicsObject>() && entity->get_component<PhysicsObject>()->body_type == PHYSICS_BODY_TYPE_RIGID)
        {
            auto *t = entity->get_component<Transform>();
            auto *p = entity->get_component<PhysicsObject>();
            auto *c = entity->get_component<CollisionShape>();

            for (int i = 0; i < RigidBody::sub_steps; ++i)
            {
                float delta = 0.016f/(float)RigidBody::sub_steps;
                p->cur_pos = t->pos;
                p->cur_rot = t->rot;

                //Constraint
                Vec2 constraint_position = Vec2(0,0);
                float constraint_radius = 512;
                Vec2 to_obj = p->cur_pos - constraint_position;

                float distance = to_obj.length();
                if(distance > constraint_radius-c->radius*(abs(t->scale.x+t->scale.y)*0.5f))
                {
                    Vec2 normal = to_obj.normalized();
                    p->cur_pos = constraint_position + normal * (constraint_radius-c->radius*(abs(t->scale.x+t->scale.y)*0.5f));
                }

                //Collisions
                solve_collisions(entity);

                Vec2 linear_velocity = p->cur_pos - p->old_pos;
                float angular_velocity = p->cur_rot - p->old_rot;
                //Gravity
                p->acceleration += PhysicsObject::gravity * p->gravity_scale;

                //Move
                p->old_pos = p->cur_pos;
                if(!p->initialized)
                {
                    p->initialized = true;
                    continue;
                }

                t->pos = p->cur_pos + linear_velocity + p->acceleration * (delta * delta);
                t->rot = p->cur_rot + angular_velocity + p->rotational_acceleration * (delta * delta);


                //Apply gravity
                p->acceleration = Vec2(0,0);
                p->rotational_acceleration = 0.0f;
                //print_info("Velocity: " + std::to_string(p->cur_pos.x) + ", " + std::to_string(p->cur_pos.y));
            }
        }
    }

    CollisionShape::CollisionShape()
    {
        this->shape_type = COLLISION_SHAPE_TYPE_CIRCLE;
        this->radius = 16.0;
        this->restitution = 0.0;
        this->size = Vec2(32.0, 32.0);
        register_component(CollisionShape);

        push_data<int>("shape_type", &this->shape_type);
        push_data<float>("radius", &this->radius);
        push_data<float>("restitution", &this->restitution);
        push_data<Vec2>("size", &this->size);
    }

    RigidBody::RigidBody()
    {
        register_system<RigidBody>();
    }

    void RigidBody::solve_collisions(Entity *entity)
    {
        auto *t = entity->get_component<Transform>();
        auto *p = entity->get_component<PhysicsObject>();
        auto *c = entity->get_component<CollisionShape>();
#pragma omp parallel for
        for (int i = 0; i < PhysicsObject::objects.size(); ++i) //TODO: test range-based loop on windows may break OMP
        {
            Entity *object = PhysicsObject::objects[i];
            if (object != entity)
            {
                auto *other = object;
                auto* other_p = other->get_component<PhysicsObject>();
                auto *other_c = other->get_component<CollisionShape>();
                if (other_c->shape_type == COLLISION_SHAPE_TYPE_CIRCLE && c->shape_type == COLLISION_SHAPE_TYPE_CIRCLE)
                {
                    float our_radius = c->radius * (abs(t->scale.x + t->scale.y) * 0.5f);
                    float other_radius = other_c->radius * (abs(other->get_component<Transform>()->scale.x +
                                                                other->get_component<Transform>()->scale.y) * 0.5f);
                    Vec2 collision_axis = p->cur_pos - other_p->cur_pos;
                    float dist = collision_axis.length();

                    if (dist < our_radius + other_radius)
                    {
                        Vec2 normal = collision_axis.normalized();
                        float overlap = (our_radius + other_radius) - dist;

                        // Separate critical sections for each object
#pragma omp critical
                        {
                            p->cur_pos += normal * (overlap * 0.5f);
                            other_p->cur_pos -= normal * (overlap * 0.5f);
                        }
                    }
                }
            }
        }
#pragma omp barrier
    }
}
