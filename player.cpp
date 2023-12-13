#include "player.h"

#include <iostream>
#include <stdexcept>

#include "input.h"

ID2D1Bitmap* Player::sprite = nullptr;

void Player::init(Graphics& g)
{
    g.LoadBitmapFromFile(L".\\images\\slime.png", &sprite);
}

Player::Player(const float x, const float y, const int left_limit, const int right_limit, const int top_limit, const int bottom_limit)
    : position(x, y), velocity(0.0f, 0.0f), starting_position(x, y), standing_on(nullptr), updates_since_falling(10), // arbritray number bigger than coyote frames
      left_limit(left_limit), right_limit(right_limit - WIDTH), top_limit(top_limit), bottom_limit(bottom_limit - HEIGHT), sprite_cycle(0), facing(Direction::LEFT)
    {
        std::cout << "PLAYER INITIALIZED\n";
    }


void Player::reset()
{
    position = starting_position;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    standing_on = nullptr;
    updates_since_falling = 10;
}


void Player::draw(Graphics& g) const
{
    if (sprite_cycle < 15)
        if (facing == Direction::LEFT)
            g.DrawBitmap(sprite, sprite_left_small, D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT));
        else
            g.DrawBitmap(sprite, sprite_right_small, D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT));
    else
        if (facing == Direction::LEFT)
            g.DrawBitmap(sprite, sprite_left_big, D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT));
        else
            g.DrawBitmap(sprite, sprite_right_big, D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT));
}


Vec2I Player::update(const float& multiplier, const std::vector<Collidable>& collidables)
{
    if (Input::GetKeyDown(SPACE) && Input::GetKey(SPACE).frame_number == 0)
    {
        reset();
        return Vec2I(0, 0);
    }
    // std::cout << "PLAYER: " << position.y << '\n';
    // if (standing_on != nullptr)
    //     std::cout << "PLATFORM: " << standing_on->get_y() << '\n';
    update_velocity(multiplier);
    position += velocity;

    update_collisions(collidables);

    Vec2I position_moved = shift_screen();

    update_cycles();

    return position_moved;
}

void Player::update_velocity(const float& multiplier)
{
    float old_vel_x = velocity.x;
    update_velocity_x(multiplier);
    
    update_direction_facing(old_vel_x);

    update_velocity_y(multiplier);

    clamp_velocity();

    // if (velocity.y != 0)
    //     std::cout << "Y VELOCITY: " << velocity.y << '\n';
}

void Player::update_velocity_x(const float& multiplier)
{
    if (Input::GetKeyDown(A) && !Input::GetKeyDown(D))
        velocity.x -= (velocity.x > 0 ? X_STOP_ACC : X_GO_ACC) * multiplier;

    else if (!Input::GetKeyDown(A) && Input::GetKeyDown(D))
        velocity.x += (velocity.x < 0 ? X_STOP_ACC : X_GO_ACC) * multiplier;
    
    else if (velocity.x > X_STOP_ACC)
        velocity.x -= X_STOP_ACC;

    else if (velocity.x < -X_STOP_ACC)
        velocity.x += X_STOP_ACC;

    else
        velocity.x = 0;
}

void Player::update_velocity_y(const float& multiplier)
{
    // std::cout << "PLAYER Y + HEIGHT + 1: " << position.y + HEIGHT + 1 << '\n';
    // if (standing_on != nullptr)
    // {
    //     std::cout << "PLATFORM Y: " << standing_on->get_y() << '\n';
    //     std::cout << "PY == PY: " << (standing_on->get_y() == position.y + HEIGHT + 1) << '\n';
    // }
    if (standing_on != nullptr && (position.x + WIDTH < standing_on->get_x() || position.x > standing_on->get_x() + standing_on->get_length()))
    { // if were no longer standing on the same thing we were eariler
        standing_on = nullptr;
        updates_since_falling = 0;
        // std::cout << "FALLING DECTECTED\n";
    }

    if ((standing_on != nullptr || updates_since_falling < coyote_frames) && Input::GetKeyDown(W) && Input::GetKey(W).frame_number < jump_buffer_frames)
    { // jump
        velocity.y = JUMP_ACC;
        standing_on = nullptr;
    }
    else if (standing_on == nullptr)
    { // gravity
        velocity.y += GRAV_ACC * multiplier;
    }

}

void Player::update_direction_facing(float old_velocity)
{
    if (velocity.x == 0)
        return;

    if (old_velocity == 0 && velocity.x != 0)
        facing = velocity.x > 0 ? Direction::RIGHT : Direction::LEFT;

    else if ((old_velocity > 0) != (velocity.x > 0))
        facing = velocity.x > 0 ? Direction::RIGHT : Direction::LEFT;
}

void Player::update_collisions(const std::vector<Collidable>& collidables)
{
    for (const Collidable& collidable : collidables)
    {
        if (collides(collidable))
        {
            move(collidable);
            // std::cout << "COLLIDES\n";
        }
    }
}

bool Player::collides(const Collidable& collidable) const
{
    switch (collidable.get_side())
    {
    case TOP:
        return position.x + WIDTH >= collidable.get_x() && position.x <= collidable.get_x() + collidable.get_length() &&
               position.y <= collidable.get_y() && position.y - velocity.y >= collidable.get_y();
    case BOTTOM:
        return position.x + WIDTH >= collidable.get_x() && position.x <= collidable.get_x() + collidable.get_length() &&
               position.y >= collidable.get_y() - HEIGHT && position.y - velocity.y <= collidable.get_y() - HEIGHT;
    case LEFT:
        return position.y + HEIGHT >= collidable.get_y() && position.y <= collidable.get_y() + collidable.get_length() &&
               position.x <= collidable.get_x() && position.x - velocity.x > collidable.get_x();
    case RIGHT:
        return position.y + HEIGHT >= collidable.get_y() && position.y <= collidable.get_y() + collidable.get_length() &&
               position.x + WIDTH >= collidable.get_x() && position.x + WIDTH - velocity.x < collidable.get_x();
    default:
        throw std::runtime_error("Soemthign went wrong");
    }
}

void Player::move(const Collidable& collidable)
{
    switch (collidable.get_side())
    {
    case TOP:
        position.y = collidable.get_y() + 1.0f;
        velocity.y = 0.0f;
        break;
    case BOTTOM:
        position.y = collidable.get_y() - HEIGHT - 1.0f;
        velocity.y = 0.0f;
        standing_on = const_cast<Collidable*>(&collidable);
        break;
    case LEFT:
        position.x = collidable.get_x() + 1.0f;
        velocity.x = 0.0f;
        break;
    case RIGHT:
        position.x = collidable.get_x() - WIDTH - 1.0f;
        velocity.x = 0.0f;
        break;
    }
}

Vec2I Player::shift_screen()
{
    Vec2I amount_to_shift;
    if (position.x < left_limit)
        amount_to_shift.x = (left_limit - position.x) * CAMERA_SPEED;

    if (position.x > right_limit)
        amount_to_shift.x = (right_limit - position.x) * CAMERA_SPEED;

    if (position.y > bottom_limit)
        amount_to_shift.y = (bottom_limit - position.y) * CAMERA_SPEED;

    if (position.y < top_limit)
        amount_to_shift.y = (top_limit - position.y) * CAMERA_SPEED;
    position += amount_to_shift;
    // std::cout << position.y << '\n';
    return amount_to_shift;
}