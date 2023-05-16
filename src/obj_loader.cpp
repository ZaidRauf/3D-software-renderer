#include "obj_loader.h"

std::vector<std::string> break_up_by_space(const std::string &input_str){
    std::string input_line(input_str);

    auto first_space_index = input_line.find_first_of(' ');
    input_line = input_line.substr(first_space_index + 1);

    // Now find number of components n spaces + 1
    int num_componenets = 1;
    for(const auto &c : input_line){
        if(c == ' '){
            num_componenets += 1;
        }
    }

    std::vector<std::string> componenet_strings;
    for(auto i = 0; i < num_componenets; i++){
        auto end_idx = input_line.find_first_of(' ');

        if(first_space_index != std::string::npos){
            componenet_strings.push_back(input_line.substr(0, end_idx));
        }
        else {
            end_idx = input_line.find_first_of('\n');
            componenet_strings.push_back(input_line.substr(0, end_idx));
        }

        input_line = input_line.substr(end_idx + 1);
    }

    return componenet_strings;
}

OBJLoader::OBJLoader(){}

OBJLoader::OBJLoader(const std::string &filename){
    std::ifstream obj_file;

    obj_file.open(filename, std::ifstream::in);

    std::string input_line;

    while(!obj_file.eof()){
        std::getline(obj_file, input_line);

        if(input_line[0] == 'v' && input_line[1] == ' '){
            vertex_count++;
            std::vector<std::string> vertex_componenet_strings = break_up_by_space(input_line);

            std::array<float, 3> parsed_vertex_componenets;
            for(auto i = 0; i < 3; i++){
                parsed_vertex_componenets[i] = std::stof(vertex_componenet_strings[i]);
            }

            vertices.emplace_back(parsed_vertex_componenets[0], parsed_vertex_componenets[1], parsed_vertex_componenets[2]);
        }

        else if(input_line[0] == 'v' && input_line[1] == 't'){
            uv_count++;
            std::vector<std::string> uv_componenet_strings = break_up_by_space(input_line);

            std::array<float, 2> parsed_uv_componenets;
            for(auto i = 0; i < 2; i++){
                parsed_uv_componenets[i] = std::stof(uv_componenet_strings[i]);
            }

            uvs.emplace_back(parsed_uv_componenets[0], parsed_uv_componenets[1]);
        }

        else if(input_line[0] == 'f'){
            face_count++;
            std::vector<std::string> face_componenet_strings = break_up_by_space(input_line);

            std::array<int, 3> parsed_face_idx_componenets;
            std::array<int, 3> parsed_uv_idx_componenets;
            bool uv_data_found = false;

            for(auto i = 0; i < 3; i++){
                face_idx_count++;
                auto first_slash_idx = face_componenet_strings[i].find_first_of('/');

                // First handle the vertex index
                parsed_face_idx_componenets[i] = std::stoi(face_componenet_strings[i].substr(0, first_slash_idx));
                parsed_face_idx_componenets[i] -= 1; // OBJ files are 1 indexed so turn it to zero indexed here

                // Check if there is UV index information
                if(face_componenet_strings[i][first_slash_idx + 1] != '/'){
                    uv_idx_count++;
                    uv_data_found = true;
                    std::string trunc_uv_component(face_componenet_strings[i].substr(first_slash_idx + 1));
                    trunc_uv_component = trunc_uv_component.substr(0, trunc_uv_component.find_first_of('/'));

                    parsed_uv_idx_componenets[i] = std::stoi(trunc_uv_component);
                    parsed_uv_idx_componenets[i] -= 1;
                }
            }

            face_indices.emplace_back(parsed_face_idx_componenets[0], parsed_face_idx_componenets[1], parsed_face_idx_componenets[2]);

            if(uv_data_found){
                uv_indices.emplace_back(parsed_uv_idx_componenets[0], parsed_uv_idx_componenets[1], parsed_uv_idx_componenets[2]);
            }
        }
    }

    // for(const auto &x : vertices){
    //     std::cout << "VERTEX: " << x << std::endl;
    // }

    // for(const auto &x : uvs){
    //     std::cout << "UV: " << x << std::endl;
    // }
    
    // for(const auto &x : face_indices){
    //     std::cout << "VERTEX FACE IDX: " << Vector3(x.a, x.b, x.c) << std::endl;
    // }

    // for(const auto &x : uv_indices){
    //     std::cout << "VERTEX UV IDX: " << Vector3(x.a, x.b, x.c) << std::endl;
    // }
    
    std::cout << vertex_count << " " << uv_count << " " << face_count << " " << face_idx_count << " " << uv_idx_count << std::endl;
}

OBJLoader::~OBJLoader(){}