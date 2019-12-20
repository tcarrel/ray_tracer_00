#pragma once



#include<optional>



#include "camera.h"
#include "image.h"
#include "global_constants.h"
#include "progress.h"
#include "scene.h"
#include<glm\glm.hpp>
#include<string>



#include "tracer.args.h"



class Tracer
{
public:

    Tracer(
        const Trace_args& trace_args,
        const Camera_ctor_args& camera_args,
        const Image_ctor_args& image_args,
        const Progress_bar_args& pb_args );

    void render( Scene& scene );
    std::optional<std::string> save( void );

private:

    glm::dvec3 miss_color_{};

    Camera camera_;
    Image image_;
    Progress_Bar p_bar_;

    size_t width_{};
    size_t height_{};
    unsigned rays_per_pixel_{};
    int max_depth_{};
};

