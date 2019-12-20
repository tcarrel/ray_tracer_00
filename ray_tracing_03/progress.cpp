


#include "progress.h"



#include<string>
#include<iostream>



Progress_Bar::Progress_Bar( const Progress_bar_args& args )
    :
    Progress_Bar( args.rpp * args.width * args.height )
{}



Progress_Bar::Progress_Bar( size_t total )
    :
    total_work_( total )
{}



void Progress_Bar::print( size_t completed )
{
    const double factor = 100.0 / total_work_;
    static size_t per_comp = 0;
    size_t new_per_comp = static_cast<size_t>( completed * factor );

    if( new_per_comp == per_comp )
    {
        return;
    }

    per_comp = new_per_comp;
    const size_t per_remain = 100 - per_comp;

    std::cout << "\033[2;4H\033[1mPROGRESS:║";

    std::cout << "\033[1;32m";
    for( unsigned i = 0; i < per_comp; i++ )
    {
        std::cout << "█";
    }
    std::cout << "\033[0;31m";
    for( unsigned i = 0; i < per_remain; i++ )
    {
        std::cout << "▒";
    }
    std::cout << "\033[0m\033[1m║ \033[1;33m" << per_comp << "%\033[0m\n";
}
