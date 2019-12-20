


#include "intersection.h"



#include<cassert>



Intersection& Intersection::operator=( const Intersection& rhs )
{
    this->is_a_hit = rhs.is_a_hit;
    this->distance_ = rhs.distance_;
    this->position_ = rhs.position_;
    this->normal_ = rhs.normal_;

    return *this;
}



bool Intersection::operator<( const Intersection& rhs )
{
    return this->distance_ < rhs.distance_;
}

bool Intersection::operator>( const Intersection& rhs )
{
    return this->distance_ > rhs.distance_;
}

bool Intersection::operator==( const Intersection& rhs )
{
    return this->distance_ == rhs.distance_;
}

bool Intersection::operator!=( const Intersection& rhs )
{
    return this->distance_ != rhs.distance_;
}

bool Intersection::operator<=( const Intersection& rhs )
{
    return this->distance_ <= rhs.distance_;
}

bool Intersection::operator>=( const Intersection& rhs )
{
    return this->distance_ >= rhs.distance_;
}
