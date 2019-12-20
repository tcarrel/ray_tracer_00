#pragma once



#include<vector>
#include<optional>
#include<glm\glm.hpp>
#include<string>

#include "global_constants.h"
#include "image.args.h"



enum Image_File_Format
{
    BMP_FMT,
    JPG_FMT,
    PNG_FMT
};



uint8_t fp_to_8bit_color( double color );



class Image
{
public:
    Image( const Image_ctor_args& args );
    Image( int width, int height, int rpp, const std::string& fname, int format, int jpg_q, bool incr, bool ovrw );

    void update_pixel( int x, int y, int pass, double red, double green, double blue );

    std::optional<std::string> save( void );
    std::optional<std::string> save( int n );

private:
    void constructor_body( int format, int jpg_q );
    void check_file_exists( std::string& file );
    std::optional<std::string> save( std::vector<uint8_t>& output, const std::string& nth = "" );

    glm::dvec3& pixel( int x, int y );
    std::vector<glm::dvec3> pixels_;

    int width_{};
    int height_{};
    double rpp_{};
    int pixel_qty_{};
    int jpg_quality_{ 100 };

    bool incremental_ = false;
    bool overwrite_ = false;
    Image_File_Format format_;
    std::string filename_;
    std::string extension_;
};

