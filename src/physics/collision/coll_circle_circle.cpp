#include "coll_circle_circle.h"

CircleCircleCollision::CircleCircleCollision(sb_Circle *first, sb_Circle *second) :
Collision(first, second)
{}

Collision *CircleCircleCollision::collide(sb_Circle *first, sb_Circle *second) {
    if (do_collide(first, second)) {
        return new CircleCircleCollision(first, second);
    } else {
        return nullptr;
    }
}

void CircleCircleCollision::solve() {
    std::swap(first, second);
}