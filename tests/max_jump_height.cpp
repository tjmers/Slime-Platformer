// compilation command: g++ tests\max_jump_height.cpp main\vec2.cpp -o tests\max_jump_height.exe -std=c++20

// we use this so that the maximum jump height is close to an even number of V_UNITS, making the game feel better

#include <iostream>

#include "../main/vec2.h"

#pragma region COPY_PASTE_FROM_GRAPHICS_H
constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 640;
constexpr int NUM_H_UNIT = 48;
constexpr int NUM_V_UNIT = 32;
constexpr int H_UNIT = SCREEN_WIDTH / NUM_H_UNIT;
constexpr int V_UNIT = SCREEN_HEIGHT / NUM_V_UNIT;

static_assert(V_UNIT == H_UNIT);

constexpr int operator"" _vu(unsigned long long val)
{
	return static_cast<int>(val) * V_UNIT;
}
constexpr int operator"" _hu(unsigned long long val)
{
	return static_cast<int>(val) * H_UNIT;
}
constexpr float operator"" _vu(long double val)
{
	return static_cast<float>(val) * V_UNIT;
}
constexpr float operator"" _hu(long double val)
{
	return static_cast<float>(val) * V_UNIT;
}

#pragma endregion // COPY_PASTE_FROM_GRAPHICS_H

class ModifiedPlayer
{
public:
    ModifiedPlayer(const Vec2F position, float grav_acc) : position(position), velocity(0, 0), GRAV_ACC(grav_acc) {}
    void jump()
    {
        velocity.y = JUMP_VELOCITY;

        if (velocity.y > MAX_Y_VEL)
            velocity.y -= (velocity.y - MAX_Y_VEL) * SLOW_FACTOR;
        
        if (velocity.y < -MAX_Y_VEL)
            velocity.y -= (velocity.y + MAX_Y_VEL) * SLOW_FACTOR;

        position += velocity;
    }
    void update()
    {
        velocity.y += GRAV_ACC;

        if (velocity.y > MAX_Y_VEL)
            velocity.y -= (velocity.y - MAX_Y_VEL) * SLOW_FACTOR;
        
        if (velocity.y < -MAX_Y_VEL)
            velocity.y -= (velocity.y + MAX_Y_VEL) * SLOW_FACTOR;

        position += velocity;
    }

    inline float get_y() { return position.y; }
private:
    Vec2F position, velocity;
    constexpr static float SLOW_FACTOR = 0.3f;
    constexpr static float MAX_Y_VEL = 0.5_vu, JUMP_VELOCITY = -1_vu;
    const float GRAV_ACC;
};

int main(void)
{
    ModifiedPlayer m_p({0, 0}, 0.029_vu);

    m_p.jump();

    float prev_y = m_p.get_y();
    m_p.update();
    while (m_p.get_y() <= prev_y)
    {
        prev_y = m_p.get_y();
        m_p.update();
    }



    std::cout << "Maximum height (V_UNITS): " << (prev_y / V_UNIT * -1) << '\n';



    return 0;
}