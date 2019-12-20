#include "intersection.h"
#include "tracer.h"

#include<iostream>
#include<string>



Tracer::Tracer(
    const Trace_args& trace_args,
    const Camera_ctor_args& camera_args,
    const Image_ctor_args& image_args,
    const Progress_bar_args& pb_args )
    :
    miss_color_( trace_args.miss_color ),

    camera_( camera_args ),
    image_( image_args ),
    p_bar_( pb_args ),

    width_( trace_args.width ),
    height_( trace_args.height ),
    rays_per_pixel_( static_cast<unsigned>( trace_args.rpp ) ),
    max_depth_( trace_args.max_depth )
{}



void Tracer::render( Scene& scene )
{
    size_t count = 0;
    for( size_t primary_ray = 0; primary_ray < rays_per_pixel_; primary_ray++ )
    {
        for( size_t y = 0; y < height_; y++ )
        {
            for( size_t x = 0; x < width_; x++ )
            {
                auto ray = camera_.make_ray(
                    static_cast<int>( x ),
                    static_cast<int>( y ),
                    true );

                auto color = scene.cast_ray( ray );

                image_.update_pixel(
                    static_cast<int>( x ),
                    static_cast<int>( y ),
                    static_cast<int>( primary_ray ),
                    color.r, color.g, color.b );

                p_bar_.print( ++count );
            }
        }
        image_.save( static_cast<int>( primary_ray ) );
    }
}



std::optional<std::string> Tracer::save( void )
{
    return image_.save();
}
