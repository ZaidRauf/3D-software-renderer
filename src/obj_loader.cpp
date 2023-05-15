#include "obj_loader.h"

OBJLoader::OBJLoader(){}

OBJLoader::OBJLoader(const std::string &filename){
    std::ifstream obj_file;

    obj_file.open(filename, std::ifstream::in);

    std::string input_line;

    while(!obj_file.eof()){
        std::getline(obj_file, input_line);

        if(input_line[0] == 'v' && input_line[1] != 'n'){
            vertex_count++;

            auto first_space_index = input_line.find_first_of(' ');
            input_line = input_line.substr(first_space_index + 1);

            std::vector<std::string> vertex_componenet_strings;
            for(auto i = 0; i < 3; i++){
                auto end_idx = input_line.find_first_of(' ');

                if(first_space_index != std::string::npos){
                    vertex_componenet_strings.push_back(input_line.substr(0, end_idx));
                }
                else {
                    end_idx = input_line.find_first_of('\n');
                    vertex_componenet_strings.push_back(input_line.substr(0, end_idx));
                }

                input_line = input_line.substr(end_idx + 1);
            }

            std::array<float, 3> parsed_vertex_componenets;

            for(auto i = 0; i < 3; i++){
                parsed_vertex_componenets[i] = std::stof(vertex_componenet_strings[i]);
            }

            vertices.emplace_back(parsed_vertex_componenets[0], parsed_vertex_componenets[1], parsed_vertex_componenets[2]);
        }

        else if(input_line[0] == 'f'){
            face_count++;

            auto first_space_index = input_line.find_first_of(' ');
            input_line = input_line.substr(first_space_index + 1);

            std::vector<std::string> face_componenet_strings;
            for(auto i = 0; i < 3; i++){
                auto end_idx = input_line.find_first_of(' ');

                if(first_space_index != std::string::npos){
                    face_componenet_strings.push_back(input_line.substr(0, end_idx));
                }
                else {
                    end_idx = input_line.find_first_of('\n');
                    face_componenet_strings.push_back(input_line.substr(0, end_idx));
                }

                input_line = input_line.substr(end_idx + 1);
            }

            std::array<float, 3> parsed_face_componenets;
            for(auto i = 0; i < 3; i++){
                parsed_face_componenets[i] = std::stoi(face_componenet_strings[i].substr(0, face_componenet_strings[1].find_first_of('/')));
            }
        }
    }
    
}

OBJLoader::~OBJLoader(){}