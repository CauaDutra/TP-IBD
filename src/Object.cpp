#include "Object.h"

Object::Object() : id(0), x_center(0), y_depth(0), width(0) {}

Object::Object(int id, double x, double y, double w) {
    this->id = id;
    this->x_center = x;
    this->y_depth = y;
    this->width = w;
}

void Object::updatePosition(double new_x, double new_y) {
    this->x_center = new_x;
    this->y_depth = new_y;
}

int Object::getId() const {
    return id;
}

double Object::getDepth() const {
    return y_depth;
}

double Object::getStartX() const {
    return x_center - (width / 2.0);
}

double Object::getEndX() const {
    return x_center + (width / 2.0);
}