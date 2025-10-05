#include "Scene.h"
#include <iostream>
#include <iomanip>

Scene::Scene(int timestamp) {
    this->timestamp = timestamp;
    this->capacity = 10;
    this->segment_count = 0;
    this->segments = new VisibleSegment[this->capacity];
}

Scene::~Scene() {
    delete[] segments;
}

void Scene::resize() {
    capacity *= 2;
    VisibleSegment* new_array = new VisibleSegment[capacity];
    for (int i = 0; i < segment_count; ++i) {
        new_array[i] = segments[i];
    }
    delete[] segments;
    segments = new_array;
}

void Scene::addSegment(int id, double start, double end) {
    if (segment_count >= capacity) {
        resize();
    }
    segments[segment_count].object_id = id;
    segments[segment_count].start_x = start;
    segments[segment_count].end_x = end;
    segment_count++;
}

void Scene::writeScene() {
    for (int i = 0; i < segment_count; ++i) {
        std::cout << "S " << timestamp << " "
                  << segments[i].object_id << " "
                  << std::fixed << std::setprecision(2)
                  << segments[i].start_x << " "
                  << segments[i].end_x << std::endl;
    }
}