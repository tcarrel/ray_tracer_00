


#include<string>
#include<iostream>
#include<cstdlib>
#include<chrono>
#include<clara.hpp>



#include "ray.h"
#include "glm_prints.h"
#include "random_wrapper.h"
#include "render_settings.h"
#include "helpers.h"
#include "camera.h"
#include "image.h"
#include "tracer.h"
#include "scene.h"



bool parse_args( int argc, char* argv[], Render_Settings& settings, int& exit_status );
bool parse_color( std::string hex_str, double& r, double& g, double& b );



auto main( int argc, char* argv[] ) -> int
{

    std::cout << "\n\n\n" << std::endl;

    Render_Settings settings;
    int ret_val = 0;

    if( parse_args( argc, argv, settings, ret_val ) )
    {
        return ret_val;
    }

    double aspect_ratio = static_cast<double>( settings.image.width ) / static_cast<double>( settings.image.height );
    std::cout
        << "\tOutput image aspect ratio is: " << aspect_ratio << "\n"
        << "\tTotal number of pixels to process is: " << settings.image.width * settings.image.height << ".\n";


    
    Trace_args t_args{};

    t_args.max_depth = settings.max_depth;
    t_args.miss_color.r = settings.miss_color.red;
    t_args.miss_color.g = settings.miss_color.green;
    t_args.miss_color.b = settings.miss_color.blue;
    t_args.rpp = settings.rpp;
    t_args.width = settings.image.width;
    t_args.height = settings.image.height;

    Camera_ctor_args c_args{};

    c_args.eye = { 0, 0, 0 };
    c_args.lookat = { 0, 0, 1 };
    c_args.up = { 0, -1, 0 };
    c_args.vert_FOV = 50;
    c_args.width = static_cast<double>( settings.image.width );
    c_args.height = static_cast<double>( settings.image.height );

    Image_ctor_args i_args{};

    i_args.rpp = settings.rpp;
    i_args.width = settings.image.width;
    i_args.height = settings.image.height;
    i_args.filename = settings.image.filename;
    i_args.format = settings.image.format;
    i_args.jpg_quality = settings.image.jpg_quality;
    i_args.incremental_saves = settings.incremental_save;
    i_args.overwrite = settings.image.overwrite;

    Progress_bar_args p_args;

    p_args.rpp = settings.rpp;
    p_args.width = settings.image.width;
    p_args.height = settings.image.height;

    Camera cam( c_args );
    auto rand = RNG::Rand( 0, 100 );


    Tracer T( t_args, c_args, i_args, p_args );
    Scene scene( settings.max_depth );

    glm::dvec3 s1{ -30.0, -20.0, 100.0 };
    glm::dvec3 s2{ -36.0, -26.0,  75.0 };

    glm::dvec3 aa = glm::normalize( s1 );

    glm::dvec3 s3 = 50.0 * aa;


    scene.set_ambient( (uint8_t)50, (uint8_t)50, (uint8_t)50 );
    scene.set_bg( t_args.miss_color.r, t_args.miss_color.g, t_args.miss_color.b );
    scene.add_sphere( s1, 10.0 + sqrt(PI) );
    scene.add_sphere( s2, 1.0 );
    scene.add_sphere( s3, 0.55 );

    auto s4 = s1;
    auto by = 1.25 * ( 10.0 + sqrt( PI ) );

    s4.x -= by;
    s4.y -= by;
    s4.z -= by;

    scene.add_sphere( s4, ( 1.0 + sqrt( 5.0 ) ) / 2.0 );

    auto t2_0 = s1;
    t2_0.y -= 16.0;
    auto t2_1 = t2_0, t2_2 = t2_0;

    t2_0.z += 1.0;  t2_0.x += 1.0;
    t2_1.z += 1.0;  t2_1.x -= 1.0;
    t2_2.z -= sqrt( 3.0 ) / 2.0;

    auto XXXX = 7.0;

    t2_0.z -= XXXX;
    t2_1.z -= XXXX;
    t2_2.z -= XXXX;

    XXXX = 0.75;

    scene.add_sphere( t2_0, XXXX );
    scene.add_sphere( t2_1, XXXX / 2.0 );
    scene.add_sphere( t2_2, XXXX / 3.0 );
    scene.add_triangle( t2_0, t2_2, t2_1 );

    auto sa = s2 + glm::dvec3( 0, 10, 7.0 );
    auto sb = s2 + glm::dvec3( 10, 0, 0 );

    auto tri = scene.add_triangle( s2, sb, sa );
    auto tri_norm = glm::normalize( tri.normal() );
    auto tri_midp = tri.center();

    scene.add_sphere( tri_midp + ( -4.0 * tri_norm ), 1 );

    //auto d = glm::dvec3( s2.x, s2.y + 5, s2.z ) - s1;
    glm::dvec3 p1{ -30.0, 200.0, 100.0 };

   // scene.add_point_light( p1, glm::dvec3( 1.0, 0.0, 0.0 ), 1 );
    glm::dvec3 p2{ -30.0, -125.0, 100.0 };

    auto dirx = glm::normalize( s2 - s1 );
    auto p3 = s1 + ( 45.0 * dirx );
   // scene.add_point_light( p2, glm::dvec3( 0, 1, 0 ), 1 );
    scene.add_point_light( p1, { 1.0, 0.5, 1.0 }, 10 );
    scene.add_point_light( p2, { 1.0, 0.5, 0.0 }, 2 );
    scene.add_point_light( p3, { 0.9, 0.9, 0.2 }, 4 );

    std::cout << "\033[s";
    T.render( scene );
    auto file = T.save();

    std::cout << "\033[u";

    if( file.has_value() )
    {
        std::cout << "\tImage saved as \"" << file.value() << "\"\n";
    }
    else
    {
        std::cerr << "\tError saving file.\n";
    }

    std::cout << "\n\t" << Ray::count() << " rays processed.\n";

    return EXIT_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

bool parse_args( int argc, char* argv[], Render_Settings& settings, int& exit_status )
{
    constexpr bool QUIT = true;

    bool show_help = false;
    unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();

    using clara::Opt; using clara::Help;

    std::string hex_color = "";

    struct
    {
        // Has to be a better way to do this, but I'm not that familiar with Clara, yet.  It works for now.
        bool s8k = false;
        bool nikon_d5300 = false;
        bool s5k = false;
        bool s4k = false;
        bool s1440p = false;
        bool s1080p = false;
        bool s720p = false;
        bool s480p = false;
        bool s360p = false;
        bool s240p16x9 = false;
        bool s240p4x3 = false;
        bool s144p = false;

        bool any( void )
        {
            return s8k
                || nikon_d5300
                || s5k
                || s4k
                || s1440p
                || s1080p
                || s720p
                || s480p
                || s360p
                || s240p16x9
                || s240p4x3
                || s144p;
        }
    } size_presets;

    struct
    {
        bool bmp = false;
        int  jpg = 0;
        bool png = false;

        int qty()
        {
            return static_cast<int>( bmp ) + static_cast<int>( jpg != 0 ) + static_cast<int>( png );
        }
    } img_fmt;

    auto cli =
        Opt( settings.image.filename, "filename" )[ "-f" ][ "--filename" ]
        ( "Output image filename without the extension.\nDefault is <" +
            std::string( OUTPUT_FILENAME_default ) +
            ">.  Created files are bitmaps (.bmp)." ) |
        Opt( settings.image.width, "width" )[ "-w" ][ "--image_width" ]
        ( "Output image width in pixels.\nDefault is " +
            std::to_string( IMAGE_WIDTH_default ) +
            " pixels.  Must be greater than 0." ) |
        Opt( settings.image.height, "height" )[ "-h" ][ "--image_height" ]
        ( "Output image height in pixels.\nDefault is " +
            std::to_string( IMAGE_HEIGHT_default ) +
            " pixels.  Must be greater than 0." ) |
        Opt( size_presets.s8k )[ "--8k" ]["--4320p"]
        ( "Sets resolution of output image to 7680×4320.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.nikon_d5300 )[ "--d5300" ]
        ( "Sets resolution of output image to the max resolution of a Nikon D5300 camera, 6000×4000."
            "\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s5k )[ "--5k" ]["--2880p"]
        ( "Sets resolution of output image to 5120×2880.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s4k )[ "--4k" ]["--2160p"]
        ( "Sets resolution of output image to 4096×2160.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s1440p )[ "--1440p" ][ "--WQXGA" ]
        ( "Sets resolution of output image to 2560×1440.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s1080p )[ "--1080p" ][ "--UXGA" ]
        ( "Sets resolution of output image to 1920×1080.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s720p )[ "--720p" ][ "--HDTV" ]
        ( "Sets resolution of output image to 1280×720.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s480p )[ "--480p" ]
        ( "Sets resolution of output image to 720×480.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s360p )[ "--360p" ]
        ( "Sets resolution of output image to 480×360.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s240p16x9 )[ "--240p16:9" ][ "--240p" ]
        ( "Sets resolution of output image to 426×240.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s240p4x3 )[ "--240p4:3" ]
        ( "Sets resolution of output image to 320×240.\n-w, -h, and lower quality presets are ignored." ) |
        Opt( size_presets.s144p )[ "--144p" ]
        ( "Sets resolution of output image to 256×144.\n-w, -h are ignored." ) |
        Opt( img_fmt.bmp )["--bmp"]["--bitmap"]
        ( "Sets the output image format to Bitmap.  Cannot be used together with other format "
            "specifiers.  This is the default.") |
        Opt( img_fmt.jpg, "quality" )["--jpg"]["--jpeg"]
        ( "Sets the output image format to jpeg.  Cannot be used together with other format " 
            "specifiers.  Quality is an integer in the range 1-100, 100 is best quality.  "
            "The default is bitmap." ) |
        Opt( img_fmt.png )["--png"]
        ( "Sets the output image format to png.  Cannot be used together with other format "
            "specifiers.  The default is bitmap.") |
        Opt( settings.rpp, "rays/pixel" )[ "-r" ][ "--rays_per_pixel" ]
        ( "Sample rays per image pixel.\nDefault is " +
            std::to_string( RAYS_PER_PIXEL_default ) +
            " rays/pixel.  Must be greater than 0." ) |
        Opt( settings.max_depth, "max depth" )[ "-d" ][ "--max_depth" ]
        ( "Maximum number of bounces for each ray.\nDefault is " +
            std::to_string( MAX_DEPTH_default ) + " bounces.  Must be greater than 0." ) |
        Opt( hex_color, "hex color" )[ "-c" ][ "--color" ][ "--miss_color" ]
        ( "Color to be used for rays that do not intersect any geometry.  Has no effect is geometry "
            "is used to provide background coloring." ) |
        Opt( settings.incremental_save )[ "-i" ][ "--incremental" ]
        ( "Save image each time every pixel has be processed one [more] time.  "
            "An increment index will be appended to the filenames." ) |
        Opt( settings.image.overwrite )[ "-o" ][ "--overwrite" ]
        ( "Overwrite existing image files, if necessary.  If omitted and an overwrite is necessary,"
            " new files will be appended with numerically." ) |
        Opt( seed, "Seed for the random number generator." )[ "-s" ][ "--seed" ]
        ( "Supply a seed for the random number generator." ) |
        Help( show_help );

    using clara::Args;
    auto result = cli.parse( Args( argc, argv ) );

    if( !result )
    {
        std::cerr << "Command-line error: " << result.errorMessage() << '\n' << cli;
        exit_status = EXIT_FAILURE;
        return QUIT;
    }

    if( show_help )
    {
        std::cout << cli;
        exit_status = EXIT_SUCCESS;
        return QUIT;
    }


    if( settings.image.filename != OUTPUT_FILENAME_default )
    {
        if( settings.image.filename.empty() )
        {
            std::wcerr << "Output image filename cannot be empty.  Setting to default <"
                << OUTPUT_FILENAME_default << ">.\n";
            settings.image.filename = OUTPUT_FILENAME_default;
            return !QUIT;
        }
    }

    if( !settings.incremental_save )
    {
        std::cout << "\tOutput filename set to <" << settings.image.filename << ".???>.\n";
    }
    else
    {
        std::cout
            << "\tOutput filename set to <" << settings.image.filename << " (#).???>.\n";
    }

    if( settings.rpp != RAYS_PER_PIXEL_default )
    {
        if( settings.rpp < 1 )
        {
            std::cerr << "rpp, invalid value.\n" << cli;
            exit_status = EXIT_FAILURE;
            return QUIT;
        }
        std::cout << "\tRPP set to " << settings.rpp << " rays/pixel.\n";
    }

    if( settings.max_depth != MAX_DEPTH_default )
    {
        if( settings.max_depth < 1 )
        {
            std::cerr << "max depth, invalid value.\n" << cli;
            exit_status = EXIT_FAILURE;
            return QUIT;
        }
        std::cout << "\tMaximum depth set to " << settings.max_depth << " bounces.\n";
    }

    switch( img_fmt.qty() )
    {
    case 0:
        img_fmt.bmp = true;
        //fallthrough
    case 1:
        if( img_fmt.bmp )
        {
            settings.image.format = BMP_FMT;
            std::cout << "\tBitmap output format selected.\n";
            break;
        }
        if( img_fmt.png )
        {
            settings.image.format = PNG_FMT;
            std::cout << "\tPNG output format selected.\n";
            break;
        }
        if( img_fmt.jpg > 0 )
        {
            if( img_fmt.jpg > 100 )
            {
                std::cerr << "\tInvalid jpg quality requested.  Value must be 1-100.\n\tUsing 100.\n";
                img_fmt.jpg = 100;
            }
            settings.image.format = JPG_FMT;
            settings.image.jpg_quality = img_fmt.jpg;
            std::cout << "\tJPEG output format selected.  With quality == " << img_fmt.jpg << ".\n";
            break;
        }
    default:
        std::cerr << "\tMore than one file format specifier was provided.\n" << cli;
        exit_status = EXIT_FAILURE;
        return QUIT;
    }

    if( hex_color.empty() )
    {
        parse_color( "000000", settings.miss_color.red, settings.miss_color.green, settings.miss_color.blue );
    }
    else
    {
        if( !parse_color( hex_color, settings.miss_color.red, settings.miss_color.green, settings.miss_color.blue ) )
        {
            std::cerr << cli;
            exit_status = EXIT_FAILURE;
            return QUIT;
        }
    }

    RNG::set_seed( seed );
    std::cout << "\tRandom seed is: " << RNG::get_seed() << '\n';

    if( size_presets.any() )
    {
        if( size_presets.s144p )
        {
            settings.image.width = 256; settings.image.height = 144;
        }

        if( size_presets.s240p4x3 )
        {
            settings.image.width = 320; settings.image.height = 240;
        }

        if( size_presets.s240p16x9 )
        {
            settings.image.width = 426; settings.image.height = 240;
        }

        if( size_presets.s360p )
        {
            settings.image.width = 480; settings.image.height = 360;
        }

        if( size_presets.s480p )
        {
            settings.image.width = 720; settings.image.height = 480;
        }

        if( size_presets.s720p )
        {
            settings.image.width = 1280; settings.image.height = 720;
        }

        if( size_presets.s1080p )
        {
            settings.image.width = 1920; settings.image.height = 1080;
        }

        if( size_presets.s1440p )
        {
            settings.image.width = 2560; settings.image.height = 1440;
        }

        if( size_presets.s4k ) // 4096×2160
        {
            settings.image.width = 4096; settings.image.height = 2160;
        }

        if( size_presets.s5k ) // 5120×2880
        {
            settings.image.width = 5120; settings.image.height = 2880;
        }

        if( size_presets.nikon_d5300 )
        {
            settings.image.width = 6000; settings.image.height = 4000;
        }

        if( size_presets.s8k )
        {
            settings.image.width = 7680; settings.image.height = 4320;
        }

        std::wcout << "\tOutput image set to " << settings.image.width << "×" << settings.image.height << " pixels.\n";
        return !QUIT;
    }
    else
    {
        if( settings.image.width != IMAGE_WIDTH_default )
        {
            if( settings.image.width < 1 )
            {
                std::cerr << "Output image width, invalid value.\n" << cli;
                exit_status = EXIT_FAILURE;
                return QUIT;
            }
            std::cout << "\tOutput image width set to " << settings.image.width << " pixels.\n";
        }

        if( settings.image.height != IMAGE_HEIGHT_default )
        {
            if( settings.image.height < 1 )
            {
                std::cerr << "Output image height, invalid value.\n" << cli;
                exit_status = EXIT_FAILURE;
                return QUIT;
            }
            std::cout << "\tOutput image height set to " << settings.image.height << " pixels.\n";
        }
    }

    return !QUIT;
}



bool parse_color( std::string hex_str, double& r, double& g, double& b )
{
    if( hex_str.length() != 6 )
    {
        std::cerr << "Invalid color value string.\n";
        return false;
    }

    std::cout
        << "\tMiss Color:\n\t\thex:\t" << hex_str;

    std::string red = "00", green = "00", blue = "00";

    red[ 0 ] = hex_str[ 0 ];
    red[ 1 ] = hex_str[ 1 ];

    green[ 0 ] = hex_str[ 2 ];
    green[ 1 ] = hex_str[ 3 ];

    blue[ 0 ] = hex_str[ 4 ];
    blue[ 1 ] = hex_str[ 5 ];


    struct
    {
        int r;
        int g;
        int b;
    } c;

    c.r = std::stoi( red, nullptr, 16 );
    c.g = std::stoi( green, nullptr, 16 );
    c.b = std::stoi( blue, nullptr, 16 );

    std::cout
        << "\n\t\thex{r}:\t" << c.r
        << "\n\t\thex{g}:\t" << c.g
        << "\n\t\thex{b}:\t" << c.b;

    r = static_cast<double>( c.r ) / 255.0;
    g = static_cast<double>( c.g ) / 255.0;
    b = static_cast<double>( c.b ) / 255.0;

    std::cout
        << "\n\n\t\tdbl{r}:\t" << r
        << "\n\t\tdbl{g}:\t" << g
        << "\n\t\tdbl{b}:\t" << b << '\n';

    return true;
}
