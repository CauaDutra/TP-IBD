#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include "Object.h"
#include "Scene.h"

void swap(Object& a, Object& b) {
    Object temp = a; a = b; b = temp;
}

int partition(Object arr[], int low, int high) {
    if (low < 0 || high < 0 || low > high || arr == nullptr) return -1;
    double pivot = arr[high].getDepth();
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].getDepth() < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(Object arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        if (pi == -1) return;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void generateScene(int timestamp, Object objects[], int object_count) {
    if (object_count == 0) return;
    Scene scene(timestamp);
    
    const int ARRAY_SIZE = 200000;
    const int OFFSET = 100000;
    static bool x_axis_covered[ARRAY_SIZE];
    memset(x_axis_covered, 0, sizeof(x_axis_covered));
    
    for (int i = 0; i < object_count; ++i) {
        double obj_start_d = objects[i].getStartX();
        double obj_end_d = objects[i].getEndX();

        double visible_segment_start_d = -200000.0;

        int start_pos = floor(obj_start_d * 100);
        int end_pos = floor(obj_end_d * 100);

        for (int x_pos_conceptual = start_pos; x_pos_conceptual < end_pos; ++x_pos_conceptual) {
            int x_pos_array = x_pos_conceptual + OFFSET;

            if (x_pos_array < 0 || x_pos_array >= ARRAY_SIZE) {
                continue; 
            }

            if (!x_axis_covered[x_pos_array]) {
                if (visible_segment_start_d < -100000.0) {
                    visible_segment_start_d = static_cast<double>(x_pos_conceptual) / 100.0;
                }
                x_axis_covered[x_pos_array] = true;
            } else {
                if (visible_segment_start_d > -100000.0) {
                    double current_x_d = static_cast<double>(x_pos_conceptual) / 100.0;
                    scene.addSegment(objects[i].getId(), visible_segment_start_d, current_x_d);
                    visible_segment_start_d = -200000.0;
                }
            }
        }

        if (visible_segment_start_d > -100000.0) {
            scene.addSegment(objects[i].getId(), visible_segment_start_d, obj_end_d);
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

        if (type == '0' || type == 'O') {
            int id; double x, y, w; ss >> id >> x >> y >> w;
            if (object_count >= object_capacity) { 
                object_capacity *= 2;
                Object* new_array = new Object[object_capacity];
                for(int i = 0; i < object_count; ++i) new_array[i] = objects[i];
                delete[] objects;
                objects = new_array;
             }
            objects[object_count++] = Object(id, x, y, w);

        } else if (type == 'M') {
            int timestamp, obj_id; double x, y; ss >> timestamp >> obj_id >> x >> y;
            int target_index = -1;
            for (int i = 0; i < object_count; ++i) {
                if (objects[i].getId() == obj_id) {
                    target_index = i;
                    break;
                }
            }
            if (target_index != -1) {
                objects[target_index].updatePosition(x, y);
            }
        } else if (type == 'C') {
            int timestamp; ss >> timestamp;
            quickSort(objects, 0, object_count - 1);
            generateScene(timestamp, objects, object_count);
        }
    }

    delete[] objects;
    return 0;
}