#ifndef OBJECT_H
#define OBJECT_H

class Object {
private:
    int id;
    double x_center;
    double y_depth;
    double width;

public:
    Object();
    Object(int id, double x, double y, double w);
    void updatePosition(double new_x, double new_y);
    int getId() const;
    double getDepth() const;
    double getStartX() const;
    double getEndX() const;
};

#endif