#pragma once

#include <algorithm>
#include <vector>

#include "graphics.h"
#include "collidable.h"
#include "object.h"
#include "vec2.h"

class Player
{

public:
    Player(const float x, const float y, const int left_limit, const int right_limit, const int top_limit, const int bottom_limit);
    Player(const Vec2F& position, const D2D1_RECT_F& player_bounds);
    // Player(const Player&& player);

    void reset();
    bool should_reset(const std::vector<Object*> objects) const;
    constexpr static int WIDTH = 2_hu, HEIGHT = 2_vu;
    static_assert(WIDTH == HEIGHT);

    Vec2I update(const float& multiplier, const std::vector<Object*>& objects);
    void draw(Graphics& g) const;

    inline const Vec2F& get_position() const { return position; }

    static HRESULT init(Graphics& g);

private:
    Vec2F position, velocity;
    const Vec2F starting_position;
    int updates_since_falling, sprite_cycle;
    int left_limit, right_limit, top_limit, bottom_limit; // the points where the camera will start to move

    constexpr static float CAMERA_SPEED = 0.2f; // 0 < CAMERA_SPEED < 1


    const Collidable* standing_on;
    const Collidable* sliding_left;
    const Collidable* sliding_right;

    void update_velocity(const float& multiplier);
    void update_velocity_x(const float& multiplier);
    void update_velocity_y(const float& multiplier);
    inline void clamp_velocity()
    {
        if (velocity.x < -MAX_X_VEL)
            velocity.x -= (velocity.x + MAX_X_VEL) * SLOW_FACTOR;

        if (velocity.x > MAX_X_VEL)
            velocity.x -= (velocity.x - MAX_X_VEL) * SLOW_FACTOR;

        if (velocity.y > MAX_Y_VEL)
            velocity.y -= (velocity.y - MAX_Y_VEL) * SLOW_FACTOR;
        
        if (velocity.y < -MAX_Y_VEL)
            velocity.y -= (velocity.y + MAX_Y_VEL) * SLOW_FACTOR;
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


    constexpr static float MAX_X_VEL = 0.25_hu, X_GO_ACC = 0.025_hu, X_STOP_ACC = 0.06_hu, SLOW_FACTOR = 0.3f; // slow factor is how fast you start slowing down when youre above max speed
    constexpr static float MAX_Y_VEL = 0.5_vu, JUMP_VELOCITY = -1_vu, GRAV_ACC = 0.025_vu;
    constexpr static float SLIDING_MULTIPLIER = 0.3f, MAX_Y_VEL_SLIDING = 0.4_vu;
    constexpr static float WALL_JUMP_VELOCITY = JUMP_VELOCITY * 0.5f;

    inline void clamp_y_sliding_velocity(Vec2F& vel)
    {
        vel.y = std::min(MAX_Y_VEL_SLIDING, vel.y);
    }
    constexpr static int COYOTE_FRAMES = 5, JUMP_BUFFER_FRAMES = 5;

    static ID2D1Bitmap* sprite;

    constexpr static D2D1_RECT_F sprite_boxes[] = { { 0.0f, 0.0f, 32.0f, 17.0f }, { 32.0f, 0.0f, 64.0f, 17.0f }, { 64.0f, 0.0f, 96.0f, 17.0f }, { 96.0f, 0.0f, 128.0f, 17.0f } };
    
    enum class Direction
    {
        RIGHT, LEFT
    };

    Direction facing;
};