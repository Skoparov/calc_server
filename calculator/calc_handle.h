#ifndef CALC_HANDLE_H
#define CALC_HANDLE_H

#include "calculator.h"

#ifdef SHOW_TIME
    #include "logger.h"
#endif

#include <boost/lexical_cast.hpp>

namespace calc
{

// Used to provide type independent interface for session-calculator interraction
class abstract_calc_handle
{
public:
    virtual ~abstract_calc_handle() = default;

    virtual void on_data( const char* data, uint64_t size, bool end = false ) = 0;
    virtual bool running() const noexcept = 0;
    virtual bool finished() const noexcept = 0;
    virtual bool error_occured() const noexcept = 0;
    virtual void reset() = 0;
    virtual void abort() = 0;
    virtual std::string get_result() = 0;
};

template< typename type >
class calc_handle : public abstract_calc_handle
{
public:
    void on_data( const char* data, uint64_t size, bool end = false ) override
    {
        assert( data );

        std::string new_data{ data, size };
        if( end && new_data.back() != '\n' )
        {
            new_data += "\n"; // just in case to avoid unnecessary hanging
        }

        if( m_calculator.running() )
        {
            m_calculator.add_expr_part( std::move( new_data ) );
        }
        else
        {
#ifdef SHOW_TIME
            m_start = std::chrono::high_resolution_clock::now();
#endif
            m_result = m_calculator.start( std::move( new_data ) );
        }
    }

    bool running() const noexcept override{ return m_calculator.running(); }
    bool finished() const noexcept override{ return m_calculator.finished(); }
    bool error_occured() const noexcept override{ return m_calculator.error_occured(); }
    void abort() override{ return m_calculator.abort(); }
    void reset() override{ return m_calculator.reset(); }

    std::string get_result() override
    {
        std::string result{ "No calculation was done" };

        try
        {
            if( m_result.valid() )
            {
                result = boost::lexical_cast< std::string >( m_result.get() );
#ifdef SHOW_TIME
                auto end = std::chrono::high_resolution_clock::now();
                uint64_t msec = std::chrono::duration_cast< std::chrono::milliseconds >( end - m_start ).count();
                logger::log( std::string{ "Calculation done in " } + std::to_string( msec ) );
#endif
            }
        }
        catch( const calc::calculation_aborted& )
        {
            result = "Calculation aborted";
        }
        catch( const std::exception& e )
        {
            result = e.what();
        }

        return result;
    }

private:
    calc::async_calculator< type > m_calculator;
    std::future< type > m_result;

#ifdef SHOW_TIME
    std::chrono::high_resolution_clock::time_point m_start;
#endif
};

} // calc

#endif
