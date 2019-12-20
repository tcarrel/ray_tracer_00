#include "image.h"
#include "epsilon.h"



#include<filesystem>
#include<iostream>
#include<optional>
#include<stb_image_write.h>
#include<string>



Image::Image( const Image_ctor_args& args )
    :
    width_( args.width ),
    height_( args.height ),
    rpp_( args.rpp ),
    incremental_( args.incremental_saves ),
    overwrite_( args.overwrite ), 
    filename_( OUTPUT_FILE_SUBFOLDER + args.filename)
{
    constructor_body( args.format, args.jpg_quality );
}



Image::Image( int width, int height, int rpp, const std::string& fname, int format, int jpg_q, bool incr, bool ovrw )
    :
    width_( width ),
    height_( height ),
    rpp_( static_cast<double>( rpp ) ),
    incremental_( incr ),
    overwrite_( ovrw ), 
    filename_( fname )
{
    constructor_body( format, jpg_q );
}



void Image::update_pixel( int x, int y, int pass, double red, double green, double blue )
{
    auto& p = pixel( x, y );

    if( pass != 0 )
    {
        p.r += red;
        p.g += green;
        p.b += blue;

        return;
    }

    p.r = red;
    p.g = green;
    p.b = blue;
}

std::optional<std::string> Image::save( void )
{
    std::cout << "  SAVING.";

    std::vector<uint8_t> output( pixels_.size() * 3 );
    size_t i = 0;
    for( auto pxl : pixels_ )
    {
        output[ i++ ] = fp_to_8bit_color( pxl.r / rpp_ );
        output[ i++ ] = fp_to_8bit_color( pxl.g / rpp_ );
        output[ i++ ] = fp_to_8bit_color( pxl.b / rpp_ );
    }

    if( incremental_ )
    {
        return save( output, " FINAL" );
    }
    else
    {
        return save( output, "" );
    }
}

std::optional<std::string> Image::save( int n )
{
    if( incremental_ )
    {
        std::cout << "  SAVING.";

        std::vector<uint8_t> output( pixels_.size() * 3 );
        size_t i = 0;
        for( auto pxl : pixels_ )
        {
            output[ i++ ] = fp_to_8bit_color( pxl.r / ( static_cast<double>( n ) + 1.0 ) );
            output[ i++ ] = fp_to_8bit_color( pxl.g / ( static_cast<double>( n ) + 1.0 ) );
            output[ i++ ] = fp_to_8bit_color( pxl.b / ( static_cast<double>( n ) + 1.0 ) );
        }

        return save( output, " (pass: " + std::to_string( n + 1 ) + ')' );
    }

    return {};
}



void Image::constructor_body( int format, int jpg_q )
{
    pixel_qty_ = static_cast<size_t>( width_ )* static_cast<size_t>( height_ );
    pixels_.resize( static_cast<size_t>( pixel_qty_ ) );

    switch( format )
    {
    default:
        //fallthrough;
    case BMP_FMT:
        format_ = BMP_FMT;
        break;
    case JPG_FMT:
        format_ = JPG_FMT;
        jpg_quality_ = jpg_q;
        break;
    case PNG_FMT:
        format_ = PNG_FMT;
        break;
    }
}

void Image::check_file_exists( std::string& file )
{
    auto ext = []( const Image_File_Format& fmt )->std::string
    {
        switch( fmt )
        {
        default:
            //fallthrough.
        case BMP_FMT:
            return ".bmp";
        case JPG_FMT:
            return ".jpg";
        case PNG_FMT:
            return ".png";
        }
    };
    
    if( std::filesystem::exists( std::filesystem::current_path() / ( file + ext( format_ ) ) ) )
    {
        int i = 1;
        std::string app = "";
        bool exists = true;
        while( exists )
        {
            app = " (copy " + std::to_string( i++ ) + ')';

            exists = std::filesystem::exists( std::filesystem::current_path() / ( file + app + ext( format_ ) ) );
        }

        file += app;
    }
}



glm::dvec3& Image::pixel( int x, int y )
{
    return pixels_[ ( static_cast<size_t>( y )* static_cast<size_t>( width_ ) ) + static_cast<size_t>( x ) ];
}



std::optional<std::string> Image::save( std::vector<uint8_t>& output, const std::string& nth )
{
    std::string fname = filename_ + nth;
    if( !overwrite_ )
    {
        check_file_exists( fname );
    }

    int ret_val = 0;
    switch( format_ )
    {

    default:
        //fallthrough.
    case BMP_FMT:
        fname += ".bmp";
        ret_val = stbi_write_bmp(
            fname.c_str(),
            width_, height_,
            3,
            output.data()
        );
        break;
    case JPG_FMT:
        fname += ".jpg";
        ret_val = stbi_write_jpg(
            fname.c_str(),
            width_, height_,
            3,
            output.data(),
            jpg_quality_
        );
        break;
    case PNG_FMT:
        fname += ".png";
        ret_val = stbi_write_png(
            fname.c_str(),
            width_, height_,
            3,
            output.data(),
            width_ * 3
        );
    }
    std::cout << "\033[10D" << std::string( 10, ' ' ) << "\033[10D";

    std::optional<std::string> out{};
    if( ret_val )
    {
        out = fname;
    }

    return out;
}




uint8_t fp_to_8bit_color( double color )
{
    if( color > 1.0 )
    {
        return 0xff;
    }

    if( color <= k_EPSILON )
    {
        return 0x00;
    }

    return static_cast<uint8_t>( color * 255.0 );
}
