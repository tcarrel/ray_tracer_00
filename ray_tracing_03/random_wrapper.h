#ifndef _x________RANDOM_AFAGU34YEIUKSGYHIKSUGYRTIAKISHU_H
#define _x________RANDOM_AFAGU34YEIUKSGYHIKSUGYRTIAKISHU_H



#include<random>
#include<chrono>
#include<type_traits>
#include<limits>


#ifdef _DEBUG
#include<iostream>
#endif

#ifndef RANDOM_WRAPPER_ALLOW_IMPLICIT_CONVERSION
# define RANDOM_WRAPPER_ALLOW_IMPLICIT_CONVERSION (false)
#else
# undef RANDOM_WRAPPER_ALLOW_IMPLICIT_CONVERSION
# define RANDOM_WRAPPER_ALLOW_IMPLICIT_CONVERSION (true)
#endif




namespace RNG
{
    template<typename TYPE>
    class Rand;

    namespace Random
    {
        ///<summary>Instatiating the Mersenne Twister is slow, it's better to
        ///for the entire program to treat it almost like a singleton.</summary>
        class Seed
        {
        public:

            static void set_seed( unsigned long long ciid )
            {
                seed_ = ciid;
                seeded_ = true;
            }

            auto static seed()
            {
                return get_seed();
            }

        protected:
            static auto& gen( void )
            {
                static std::mt19937 mtwister = std::mt19937( static_cast<unsigned>( get_seed() ) );
                return mtwister;
            }

            static unsigned long long get_seed( void )
            {
                if( !seeded_ )
                {
                    seed_ = std::chrono::system_clock::now().time_since_epoch().count();
#ifdef _DEBUG
                    std::cout << "  Seed value is:\t" << seed << '.' << '\n';
#endif
                    seeded_ = true;
                }
                return seed_;
            }

        private:

            static unsigned long long seed_;
            static bool seeded_;
        };



        template<typename TYPE, bool IS_FP>
        class Rand_distribution : protected Seed
        {
        public:
            TYPE get_val() {}
        };


        template<typename TYPE>
        class Rand_distribution<TYPE, true> : protected Seed
        {
        public:
            TYPE get_val() { return dist( gen() ); }

        protected:
            
            Rand_distribution( void ) : dist( std::numeric_limits<TYPE>::min(), std::numeric_limits<TYPE>::max() ) {}
            Rand_distribution( TYPE max ) : dist( 0, max ) {}
            Rand_distribution( TYPE min, TYPE max ) : dist( min, max ) {}

        private:
            std::uniform_real_distribution<TYPE> dist;
        };


        template<typename TYPE>
        class Rand_distribution<TYPE, false> : protected Seed
        {
        public:
            TYPE get_val() { return dist( gen() ); }

        protected:

            Rand_distribution( void ) : dist( std::numeric_limits<TYPE>::min(), std::numeric_limits<TYPE>::max() ) {}
            Rand_distribution( TYPE max ) : dist( 0, max ) {}
            Rand_distribution( TYPE min, TYPE max ) : dist( min, max ) {}

        private:
            std::uniform_int_distribution<TYPE> dist;
        };



        template<>
        class Rand_distribution<bool, true> : protected Seed
        {
        public:
            bool get_val() { return dist( gen() ); }

        protected:
            Rand_distribution() : dist( 0.5 ) {}
            Rand_distribution( double probability_of_true ) : dist( probability_of_true ) {}
            Rand_distribution( double numerator, double denominator ) : dist( numerator / denominator ) {}

        private:
            std::bernoulli_distribution dist;
        };



        template<>
        class Rand_distribution<bool, false> : protected Seed
        {
        public:
            bool get_val() { return dist( gen() ); }

        protected:
            Rand_distribution() : dist( 0.5 ) {}
            Rand_distribution( double probability_of_true ) : dist( 1.0 - probability_of_true ) {}
            Rand_distribution( double numerator, double denominator ) : dist( 1.0 - (numerator / denominator) ) {}

        private:
            std::bernoulli_distribution dist;
        };
    }


    template<typename TYPE>
    class Rand : private Random::Rand_distribution<TYPE, std::is_floating_point<TYPE>::value>
    {
    public:

        Rand() : Random::Rand_distribution<TYPE, is_fp>() {}
        Rand( TYPE max ) : Random::Rand_distribution<TYPE, is_fp>( max ) {}
        Rand( TYPE min, TYPE max ) : Random::Rand_distribution<TYPE, is_fp>( min, max ) {}

#if RANDOM_WRAPPER_ALLOW_IMPLICIT_CONVERSION
        operator TYPE() { return Random::Rand_distribution<TYPE, is_fp>::get_val(); }
#endif
        TYPE value( void ) { return static_cast<TYPE>(Random::Rand_distribution<TYPE, is_fp>::get_val()); }

        auto get_seed()
        {
            return ::RNG::Random::Seed::get_seed();
        }

    private:
        static constexpr auto is_fp = std::is_floating_point<TYPE>::value;
    };



    template<>
    class Rand<bool> : private Random::Rand_distribution<bool, true>
    {
    public:

        Rand() : Random::Rand_distribution<bool, true>() {}
        Rand( double max ) : Random::Rand_distribution<bool, true>( max ) {}
        Rand( double min, double max ) : Random::Rand_distribution<bool, true>( min, max ) {}

#if RANDOM_WRAPPER_ALLOW_IMPLICIT_CONVERSION
        operator bool() { return Random::Rand_distribution<bool, true>::get_val(); }
#endif
        bool value( void ) { return Random::Rand_distribution<bool, true>::get_val(); }

        auto get_seed()
        {
            return ::RNG::Random::Seed::seed();
        }
    };

    template<typename T>
    void set_seed( T nseed )
    {
        Random::Seed::set_seed( static_cast<unsigned long long>( nseed ) );
    }

    unsigned long long get_seed( void );
}
#endif // !_________RANDOM_AFAGU34YEIUKSGYHIKSUGYRTIAKISHU_H
