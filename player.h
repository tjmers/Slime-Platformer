#pragma once

#include <algorithm>
#include <vector>

#include "graphics.h"
#include "vec2.h"
#ifndef DRAW_COLLIDABLES_
#define DRAW_COLLIDABLES_
#endif // DRAW_COLLIDABLES_
#include "collidable.h"

class Player
{

public:
    Player(const float x, const float y, const int left_limit, const int right_limit, const int top_limit, const int bottom_limit);
    // Player(const Player&& player);

    void reset();
    constexpr static int WIDTH = 1.75_hu, HEIGHT = 3_vu;

    Vec2I update(const float& multiplier, const std::vector<Collidable>& collidables);
    void draw(Graphics& g) const;

    inline Vec2F get_position() const { return position; }
    inline Vec2F get_velocity() const { return velocity; }

    inline void set_left_limit(int left_limit) { this->left_limit = left_limit; }
    inline void set_right_limit(int right_limit) { this->right_limit = right_limit; }

    static void init(Graphics& g);

private:
    Vec2F position, velocity;
    const Vec2F starting_position;
    int updates_since_falling, sprite_cycle;
    int left_limit, right_limit, top_limit, bottom_limit; // the points where the camera will start to move

    constexpr static float CAMERA_SPEED = 0.2f; // 0 < CAMERA_SPEED < 1


    Collidable* standing_on;
    Collidable* sliding_left;
    Collidable* sliding_right;

    void update_velocity(const float& multiplier);
    void update_velocity_x(const float& multiplier);
    void update_velocity_y(const float& multiplier);
    inline void clamp_velocity()
    {
        if (velocity.x < -MAX_X_VEL)
            velocity.x -= (velocity.x + MAX_X_VEL) * SLOW_FACTOR;

        if (velocity.x > MAX_X_VEL)
            velocity.x -= (velocity.x - MAX_X_VEL) * SLOW_FACTOR;

        velocity.y = std::min(velocity.y, MAX_Y_VEL);
        velocity.y = std::max(velocity.y, -MAX_Y_VEL);
    }
    void update_sliding();
    void wall_jump();
    void update_direction_facing(float old_velocity);
    void update_collisions(const std::vector<Collidable>& collidables);
    bool collides(const Collidable& collidable) const;
    void move(const Collidable& collidable);
    Vec2I shift_screen();
    inline void update_cycles()
    {
        ++updates_since_falling;
        if (++sprite_cycle == 30)
            sprite_cycle = 0;
    }


    constexpr static float MAX_X_VEL = 0.25_hu, X_GO_ACC = 0.03125_hu, X_STOP_ACC = 0.0625_hu, SLOW_FACTOR = 0.3f; // slow factor is how fast you start slowing down when youre above max speed
    constexpr static float MAX_Y_VEL = 0.5_vu, JUMP_VELOCITY = -0.5_vu, GRAV_ACC = 0.025_vu;
    constexpr static float SLIDING_MULTIPLIER = 0.3f, MAX_Y_VEL_SLIDING = 0.4_vu;
    constexpr static float WALL_JUMP_VELOCITY = JUMP_VELOCITY * 0.5f;

    inline void clamp_y_sliding_velocity(Vec2F& vel)
    {
        vel.y = std::min(MAX_Y_VEL_SLIDING, vel.y);
    }
    constexpr static int COYOTE_FRAMES = 5, JUMP_BUFFER_FRAMES = 5;

    static ID2D1Bitmap* sprite;
    constexpr static D2D1_RECT_F sprite_right_small = { 0.0f, 0.0f, 32.0f, 32.0f };
    constexpr static D2D1_RECT_F sprite_right_big = { 32.0f, 0.0f, 64.0f, 32.0f };
    
    constexpr static D2D1_RECT_F sprite_left_small = { 64.0f, 0.0f, 96.0f, 32.0f };
    constexpr static D2D1_RECT_F sprite_left_big = { 96.0f, 0.0f, 128.0f, 32.0f };
    
    enum class Direction
    {
        LEFT, RIGHT
    };

    Direction facing;
};