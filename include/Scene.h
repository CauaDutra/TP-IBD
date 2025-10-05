#ifndef SCENE_H
#define SCENE_H

struct VisibleSegment {
    int object_id;
    double start_x;
    double end_x;
};

class Scene {
private:
    VisibleSegment* segments;
    int segment_count;
    int capacity;
    int timestamp;
    void resize();

public:
    Scene(int timestamp);
    ~Scene();
    void addSegment(int id, double start, double end);
    void writeScene();
};

#endif