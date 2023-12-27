#include "player.h"

#include <iostream>
#include <stdexcept>

#include "input.h"

ID2D1Bitmap* Player::sprite = nullptr;

HRESULT Player::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\slime.png", &sprite);
}

Player::Player(const float x, const float y, const int left_limit, const int right_limit, const int top_limit, const int bottom_limit)
    : position(x, y), velocity(0.0f, 0.0f), starting_position(x, y), standing_on(nullptr), updates_since_falling(10), // arbritray number bigger than coyote frames
      left_limit(left_limit), right_limit(right_limit - WIDTH), top_limit(top_limit), bottom_limit(bottom_limit - HEIGHT), sprite_cycle(0), facing(Direction::LEFT),
      sliding_left(nullptr), sliding_right(nullptr), jumping(false) {}

Player::Player(const Vec2F& position, const D2D1_RECT_F& player_bounds)
    : position(position), velocity(0.0f, 0.0f), starting_position(position), standing_on(nullptr), updates_since_falling(10), // arbritray number bigger than coyote frames
      left_limit(player_bounds.left), right_limit(player_bounds.right - WIDTH), top_limit(player_bounds.top), bottom_limit(player_bounds.bottom - HEIGHT), sprite_cycle(0), facing(Direction::LEFT),
      sliding_left(nullptr), sliding_right(nullptr), jumping(false) {}


void Player::reset()
{
    position = starting_position;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    standing_on = nullptr;
    sliding_left = nullptr;
    sliding_right = nullptr;
    updates_since_falling = 10;
    jumping = false;
}


void Player::draw(Graphics& g) const
{
    g.DrawBitmap(sprite, sprite_boxes[(sprite_cycle < 15) + static_cast<int>(facing) * 2], D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT));
#ifdef DRAW_HITBOXES
    g.SetColor(D2D1::ColorF::Red);
    g.DrawRect(D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT), 3.0f);
#endif
}


Vec2I Player::update(const float& multiplier, const std::vector<Object*>& objects)
{

    update_velocity(multiplier);

    if (sliding_left != nullptr || sliding_right != nullptr)
        update_sliding();

    else
        position += velocity;

    for (const Object* object : objects)
        update_collisions(object->get_collidables());
        

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

    clamp_velocity(multiplier);

    // if (velocity.y != 0)
    //     std::cout << "Y VELOCITY: " << velocity.y << '\n';
}

void Player::update_velocity_x(const float& multiplier)
{
    if (sliding_left == nullptr && Input::GetKeyDown(Key::A) && !Input::GetKeyDown(Key::D))
        velocity.x -= (velocity.x > 0 ? X_STOP_ACC : X_GO_ACC) * multiplier;

    else if (sliding_right == nullptr && !Input::GetKeyDown(Key::A) && Input::GetKeyDown(Key::D))
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

    if (standing_on != nullptr && (position.x + WIDTH < standing_on->get_x() || position.x > standing_on->get_x() + standing_on->get_length()))
    { // if were no longer standing on the same thing we were eariler
        standing_on = nullptr;
        updates_since_falling = 0;
    }

    if ((standing_on != nullptr || updates_since_falling < COYOTE_FRAMES) && Input::GetKeyDown(Key::W) && Input::GetKeyFrame(Key::W) < JUMP_BUFFER_FRAMES)
    { // jump
        velocity.y = JUMP_VELOCITY;
        standing_on = nullptr;
        jumping = true;
        current_jumping_velocity = JUMP_VELOCITY;
    }
    else if (jumping)
    {
        current_jumping_velocity *= JUMP_MULTI;
        velocity.y += current_jumping_velocity;
        if (!Input::GetKeyDown(Key::W))
            jumping = false;
    }

    if (standing_on == nullptr)
    { // gravity
        velocity.y += GRAV_ACC * multiplier;
    }

}

void Player::update_sliding() // returns the actual velocity that we will use to update
{
    Vec2F actual_velocity = velocity * SLIDING_MULTIPLIER;
    clamp_y_sliding_velocity(actual_velocity);
    position += actual_velocity;

    if (Input::GetKeyDown(Key::W) && Input::GetKeyFrame(Key::W) < JUMP_BUFFER_FRAMES)
    {
        wall_jump();
        return;
    }

    if (!Input::GetKeyDown(Key::A) || (sliding_left != nullptr && sliding_left->get_y() + sliding_left->get_length() < position.y))
        sliding_left = nullptr;
    
    if (!Input::GetKeyDown(Key::D) || (sliding_right != nullptr && sliding_right->get_y() + sliding_right->get_length() < position.y))
        sliding_right = nullptr;
}

void Player::wall_jump()
{
    velocity.y = WALL_JUMP_VELOCITY;

    if (sliding_left != nullptr)
    {
        velocity.x = MAX_X_VEL * 3;
        sliding_left = nullptr;
    }
    else
    {
        velocity.x = -MAX_X_VEL * 3;
        sliding_right = nullptr;
    }

    jumping = true;
    current_jumping_velocity = WALL_JUMP_VELOCITY;
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
    case Side::TOP:
        return position.x + WIDTH >= collidable.get_x() && position.x <= collidable.get_x() + collidable.get_length() &&
               position.y <= collidable.get_y() && position.y - velocity.y >= collidable.get_y();
    case Side::BOTTOM:
        return position.x + WIDTH > collidable.get_x() && position.x < collidable.get_x() + collidable.get_length() &&
               position.y >= collidable.get_y() - HEIGHT && position.y - velocity.y <= collidable.get_y() - HEIGHT;
    case Side::LEFT:
        return sliding_left == nullptr &&
              (position.y + HEIGHT >= collidable.get_y() && position.y <= collidable.get_y() + collidable.get_length() &&
               position.x < collidable.get_x() && position.x - velocity.x >= collidable.get_x());
    case Side::RIGHT:
        return sliding_right == nullptr &&
              (position.y + HEIGHT >= collidable.get_y() && position.y <= collidable.get_y() + collidable.get_length() &&
               position.x + WIDTH > collidable.get_x() && position.x + WIDTH - velocity.x <= collidable.get_x());
    default:
        throw std::runtime_error("Soemthign went wrong");
    }
}

void Player::move(const Collidable& collidable)
{
    switch (collidable.get_side())
    {
    case Side::TOP:
        position.y = collidable.get_y() + 1.0f;
        velocity.y = 0.0f;
        break;
    case Side::BOTTOM:
        position.y = collidable.get_y() - HEIGHT - 1.0f;
        velocity.y = 0.0f;
        standing_on = &collidable;
        sliding_left = nullptr;
        sliding_right = nullptr;
        break;
    case Side::LEFT:
        position.x = collidable.get_x();
        velocity.x = 0.0f;
        sliding_left = &collidable;
        break;
    case Side::RIGHT:
        position.x = collidable.get_x() - WIDTH;
        velocity.x = 0.0f;
        sliding_right = &collidable;
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


bool Player::should_reset(const std::vector<Object*> objects) const
{
    for (Object* o : objects)
    {
        for (Collidable c : o->get_killables())
        {
            if (collides(c))
                return true;
        }
    }
    return false;
}