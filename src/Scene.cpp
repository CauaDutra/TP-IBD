#include "Scene.h"
#include <iostream>

Scene::Scene(int timestamp) {
    this->timestamp = timestamp;
    this->capacity = 10; // Initial capacity
    this->segment_count = 0;
    this->segments = new VisibleSegment[this->capacity];
}

Scene::~Scene() {
    delete[] segments; // Free the allocated memory
}

void Scene::resize() {
    capacity *= 2; // Double the capacity
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
    // Output format: S <tempo> <objeto> <inicio> <fim> [cite: 18]
    for (int i = 0; i < segment_count; ++i) {
        std::cout << "S " << timestamp << " "
                  << segments[i].object_id << " "
                  << segments[i].start_x << " "
                  << segments[i].end_x << std::endl;
    }
}