#include "invisible_boundry.h"

InvisibleBoundry::InvisibleBoundry(Side side, int x, int y, int length)
    : Object({Collidable(side, x, y, length)}, {}) {}