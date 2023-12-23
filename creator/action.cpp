#include "action.h"

// #include <iostream>

std::function<void(Action::Param&)> Action::default_function = [](Param&){};

Action::Action()
    : undo(default_function), cleanup(default_function), param(0) {}


Action::Action(const std::function<void(Param&)>& undo)
    : undo(undo), cleanup(default_function), param(0) {}

Action::Action(const std::function<void(Param&)>& undo, const Param& param)
    : undo(undo), cleanup(default_function), param(param) {}

Action::Action(const std::function<void(Param&)>& undo, const std::function<void(Param&)>& cleanup)
    : undo(undo), cleanup(cleanup), param(0) {}

Action::Action(const std::function<void(Param&)>& undo, const std::function<void(Param&)>& cleanup, const Param& param)
    : undo(undo), cleanup(cleanup), param(param) {}


Action::Action(std::function<void(Param&)>&& undo)
    : undo(std::move(undo)), cleanup(default_function), param(0) {}

Action::Action(std::function<void(Param&)>&& undo, const Param& param)
    : undo(std::move(undo)), cleanup(default_function), param(param) {}

Action::Action(std::function<void(Param&)>&& undo, std::function<void(Param&)>&& cleanup)
    : undo(std::move(undo)), cleanup(std::move(cleanup)), param(0) {}

Action::Action(std::function<void(Param&)>&& undo, std::function<void(Param&)>&& cleanup, const Param& param)
    : undo(std::move(undo)), cleanup(std::move(cleanup)), param(param) {}

Action::Action(const Action& other)
    : undo(other.undo), cleanup(other.cleanup), param(other.param) {}


Action::Action(Action&& other)
    : undo(std::move(other.undo)), cleanup(other.cleanup), param(other.param)
{
    other.undo = default_function;
    other.cleanup = default_function;
    other.param = 0;
}

Action::~Action()
{
    // std::cout << 6.1 << '\n';
    if (cleanup)
        cleanup(param);
    // std::cout << 6.2 << '\n';
}

Action& Action::operator=(const Action& other) noexcept
{
    
    cleanup(param);
    undo = other.undo;
    cleanup = other.cleanup;
    param = other.param;
    return *this;
}


Action& Action::operator=(Action&& other) noexcept
{
    cleanup(param);
    undo = std::move(other.undo);
    cleanup = std::move(other.cleanup);
    param = other.param;
    other.default_function = default_function;
    other.cleanup = default_function;
    other.param = 0;
    return *this;
}