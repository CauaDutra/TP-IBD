#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Object.h"
#include "Scene.h"

void swap(Object& a, Object& b) {
    Object temp = a;
    a = b;
    b = temp;
}

int partition(Object arr[], int low, int high) {
    if (low < 0 || high < 0 || low > high || arr == nullptr) {
        return -1;
    }
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
    
    static bool x_axis_covered[10000];
    memset(x_axis_covered, 0, sizeof(x_axis_covered));
    
    for (int i = 0; i < object_count; ++i) {
        double obj_start_d = objects[i].getStartX();
        double obj_end_d = objects[i].getEndX();

        double visible_segment_start_d = -1.0;

        int start_pos = static_cast<int>(obj_start_d * 100);
        int end_pos = static_cast<int>(obj_end_d * 100);

        for (int x_pos = start_pos; x_pos < end_pos; ++x_pos) {
            if (x_pos < 0 || x_pos >= 10000) {
                continue; 
            }

            double current_x_d = static_cast<double>(x_pos) / 100.0;

            if (!x_axis_covered[x_pos]) {
                if (visible_segment_start_d < 0) {
                    visible_segment_start_d = current_x_d;
                }
                x_axis_covered[x_pos] = true;
            } else {
                if (visible_segment_start_d >= 0) {
                    scene.addSegment(objects[i].getId(), visible_segment_start_d, current_x_d);
                    visible_segment_start_d = -1.0;
                }
            }
        }

        if (visible_segment_start_d >= 0) {
            scene.addSegment(objects[i].getId(), visible_segment_start_d, obj_end_d);
        }
    }
    scene.writeScene();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo de entrada fornecido." << std::endl;
        return 1;
    }
    
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << argv[1] << std::endl;
        return 1;
    }

    std::string strategy = "lazy";
    if (argc > 2 && (std::string(argv[2]) == "eager")) {
        strategy = "eager";
    }
    
    int object_capacity = 10;
    int object_count = 0;
    Object* objects = new Object[object_capacity];

    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        char type;
        ss >> type;

        if (type == '0') {
            int id; double x, y, w; ss >> id >> x >> y >> w;
            if (object_count >= object_capacity) { 
                object_capacity *= 2;
                Object* new_array = new Object[object_capacity];
                for(int i = 0; i < object_count; ++i) {
                    new_array[i] = objects[i];
                }
                delete[] objects;
                objects = new_array;
             }
            
            if (strategy == "lazy") {
                objects[object_count++] = Object(id, x, y, w);
            } else {
                Object new_object(id, x, y, w);
                int pos_inserir = 0;
                while (pos_inserir < object_count && objects[pos_inserir].getDepth() < new_object.getDepth()) {
                    pos_inserir++;
                }
                for (int i = object_count; i > pos_inserir; --i) {
                    objects[i] = objects[i - 1];
                }
                objects[pos_inserir] = new_object;
                object_count++;
            }
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
                if (strategy == "lazy") {
                    objects[target_index].updatePosition(x, y);
                } else {
                    double old_depth = objects[target_index].getDepth();
                    objects[target_index].updatePosition(x, y);
                    double new_depth = objects[target_index].getDepth();
                    if (old_depth != new_depth) {
                         if (new_depth < old_depth) {
                            int i = target_index;
                            while (i > 0 && objects[i].getDepth() < objects[i - 1].getDepth()) {
                                swap(objects[i], objects[i - 1]);
                                i--;
                            }
                        } else {
                            int i = target_index;
                            while (i < object_count - 1 && objects[i].getDepth() > objects[i + 1].getDepth()) {
                                swap(objects[i], objects[i + 1]);
                                i++;
                            }
                        }
                    }
                }
            }
        } else if (type == 'C') {
            int timestamp; ss >> timestamp;
            if (strategy == "lazy") {
                quickSort(objects, 0, object_count - 1);
            }
            generateScene(timestamp, objects, object_count);
        }
    }

    delete[] objects;
    return 0;
}