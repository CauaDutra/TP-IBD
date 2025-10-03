#include <iostream>
#include <string>
#include <sstream>
#include "Object.h"
#include "Scene.h"

void swap(Object& a, Object& b) {
    Object temp = a;
    a = b;
    b = temp;
}

void generateScene(int timestamp, Object objects[], int object_count) {
    if (object_count == 0) return;

    Scene scene(timestamp);
    

    bool x_axis_covered[10000] = {false};
    
    for (int i = 0; i < object_count; ++i) {
        double obj_start = objects[i].getStartX();
        double obj_end = objects[i].getEndX();
        double visible_segment_start = -1;

        for (double x = obj_start; x < obj_end; x += 0.01) {
            int x_pos = static_cast<int>(x * 100);
            
            if (x_pos >= 0 && x_pos < 10000 && !x_axis_covered[x_pos]) {
                if (visible_segment_start < 0) {
                    visible_segment_start = x;
                }
                x_axis_covered[x_pos] = true;
            } else {
                if (visible_segment_start >= 0) {
                    scene.addSegment(objects[i].getId(), visible_segment_start, x);
                    visible_segment_start = -1;
                }
            }
        }

        if (visible_segment_start >= 0) {
            scene.addSegment(objects[i].getId(), visible_segment_start, obj_end);
        }
    }

    scene.writeScene();
}


int main() {
    int object_capacity = 10;
    int object_count = 0;
    Object* objects = new Object[object_capacity];

    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        char type;
        ss >> type;

        if (type == '0') {
            int id;
            double x, y, w;
            ss >> id >> x >> y >> w;

            if (object_count >= object_capacity) {
                object_capacity *= 2;
                Object* new_array = new Object[object_capacity];
                for(int i = 0; i < object_count; ++i) new_array[i] = objects[i];
                delete[] objects;
                objects = new_array;
            }
            
            Object new_object(id, x, y, w);

            int pos_inserir = 0;
            while (pos_inserir < object_count && objects[pos_inserir].getDepth() < new_object.getDepth()) {
                pos_inserir++;
            }