#pragma once



#include "progress.args.h"



class Progress_Bar
{
public:

    Progress_Bar( const Progress_bar_args& args );
    Progress_Bar( size_t total );

    void print( size_t completed );

private:

    size_t total_work_;
};

