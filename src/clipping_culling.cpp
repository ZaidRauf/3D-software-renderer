#include "clipping_culling.h"

bool cull::should_backface_cull(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &viewPos){
    // Back face culling
    
    // First get the Face Normal
    // Be wary of winding order and handedness here
    // since this is a left handed coordinate system with clockwise winding order keep that in mind for cross product
    Vector3 normal = Vector3::Cross(v1 - v2, v1 - v3).Normalized();
    
    // Calculate ray from face to camera
    Vector3 face_view_ray = (viewPos - Vector3(v1)).Normalized();
    
    if(Vector3::Dot(face_view_ray, normal) <= 0){
        return true;
    }

    return false;
}

// TODO: Move away from vectors and use static arrays instead
void clip::clip_vertices(Vector4 v1, Vector4 v2, Vector4 v3, std::vector<Vector4> &keep_vertex_list){
    std::vector<Vector4> test_vertex_list;
    std::vector<Vector4> temp_keep_vertex_list;

    test_vertex_list.push_back(v1);
    test_vertex_list.push_back(v2);
    test_vertex_list.push_back(v3);

    using ClipFnPair = std::pair<bool(*)(const Vector4&), float(*)(const Vector4&, const Vector4&)>;

    ClipFnPair right_clip_fns{
        [](const Vector4 &v){ return v.x >= v.w; },
        [](const Vector4 &v1, const Vector4 &v2){ return (v1.w - v1.x)/((v1.w - v1.x) - (v2.w - v2.x)); }
    };

    ClipFnPair left_clip_fns{
        [](const Vector4 &v){ return v.x <= -v.w; },
        [](const Vector4 &v1, const Vector4 &v2){ return (v1.w + v1.x)/((v1.w + v1.x) - (v2.w + v2.x)); }
    };

    ClipFnPair bottom_clip_fns{
        [](const Vector4 &v){ return v.y >= v.w; },
        [](const Vector4 &v1, const Vector4 &v2){ return (v1.w - v1.y)/((v1.w - v1.y) - (v2.w - v2.y)); }
    };

    ClipFnPair top_clip_fns{
        [](const Vector4 &v){ return v.y <= -v.w; },
        [](const Vector4 &v1, const Vector4 &v2){ return (v1.w + v1.y)/((v1.w + v1.y) - (v2.w + v2.y)); }
    };

    ClipFnPair far_clip_fns{
        [](const Vector4 &v){ return v.z >= v.w; },
        [](const Vector4 &v1, const Vector4 &v2){ return (v1.w - v1.z)/((v1.w - v1.z) - (v2.w - v2.z)); }
    };

    ClipFnPair near_clip_fns{
        [](const Vector4 &v){ return v.z <= 0; },
        [](const Vector4 &v1, const Vector4 &v2){ return (v1.w + v1.z)/((v1.w + v1.z) - (v2.w + v2.z)); }
    };
    
    // Not sure if I need this    
    //ClipFnPair negative_clip_fns{
    //    [](const Vector4 &v){ return v.w <= 0; },
    //    [](const Vector4 &v1, const Vector4 &v2){ return (v1.w)/(v1.w - v2.w); }
    //};

    std::array<ClipFnPair, 6> clip_test_fns = {
        right_clip_fns,
        left_clip_fns,
        bottom_clip_fns,
        top_clip_fns,
        far_clip_fns,
        near_clip_fns
        //negative_clip_fns
    };
    
    for (auto clip_fn_pair : clip_test_fns){
        for(auto i = 0; i < test_vertex_list.size(); i++){
            Vector4 test_v1 = test_vertex_list[i];
            Vector4 test_v2 = test_vertex_list[(i + 1) % test_vertex_list.size()];
        
            bool v1_outside = clip_fn_pair.first(test_v1);
            bool v2_outside = clip_fn_pair.first(test_v2);
        
            if(!v1_outside){
                temp_keep_vertex_list.push_back(test_v1);
            }
        
            if(v1_outside != v2_outside){
                float t = clip_fn_pair.second(test_v1, test_v2);
                auto result = interpolation::lerp<Vector4>(test_v1, test_v2, t);

                temp_keep_vertex_list.push_back(result);
            }
        }

        test_vertex_list.erase(test_vertex_list.begin(), test_vertex_list.end());

        for(Vector4 v : temp_keep_vertex_list){
           test_vertex_list.push_back(v); 
        }

        temp_keep_vertex_list.erase(temp_keep_vertex_list.begin(), temp_keep_vertex_list.end());
    }

    for (Vector4 v : test_vertex_list){
        keep_vertex_list.push_back(v);
    }
}


void clip::retriangulate_clipped_vertices(std::vector<Vector4> &clipped_vertex_list, std::vector<Triangle> &rebuilt_triangles){
    for(int i = 0; i <static_cast<int>(clipped_vertex_list.size()) - 2; i++){
        Triangle t(clipped_vertex_list.front(), clipped_vertex_list[i+1], clipped_vertex_list[i+2]);
        rebuilt_triangles.push_back(t);
    }
}
