#pragma once

#include <functional>


class Action
{
public:
    // can be used for interpretation by the action
    struct Param
    {
        union
        {
            struct
            {
                int four_byte1, four_byte2;
            };
            
            long long eight_byte; 
            static_assert(sizeof(four_byte1) + sizeof(four_byte2) == sizeof(eight_byte));
        };
        Param(int n1, int n2) : four_byte1(n1), four_byte2(n2) {}
        Param(long long l) : eight_byte(l) {}
    };
    
    Action();

    Action(const std::function<void(Param&)>& undo);
    Action(const std::function<void(Param&)>& undo, const Param& p);
    Action(const std::function<void(Param&)>& undo, const std::function<void(Param&)>& cleanup);
    Action(const std::function<void(Param&)>& undo, const std::function<void(Param&)>& cleanup, const Param& param);

    Action(std::function<void(Param&)>&& undo);
    Action(std::function<void(Param&)>&& undo, const Param& p);
    Action(std::function<void(Param&)>&& undo, std::function<void(Param&)>&& cleanup);
    Action(std::function<void(Param&)>&& undo, std::function<void(Param&)>&& cleanup, const Param& param);

    Action(const Action& other);
    Action(Action&& other);

    ~Action();

    Action& operator=(Action&& other) noexcept;

    Action& operator=(const Action& other) noexcept;

    std::function<void(Param&)> undo;
    std::function<void(Param&)> cleanup;

    Param param;

    static std::function<void(Param&)> default_function;
};