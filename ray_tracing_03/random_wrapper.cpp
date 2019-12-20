


#include "random_wrapper.h"



unsigned long long RNG::Random::Seed::seed_ = 0UL;
bool RNG::Random::Seed::seeded_ = false;



unsigned long long RNG::get_seed( void )
{
    return Random::Seed::seed();
}