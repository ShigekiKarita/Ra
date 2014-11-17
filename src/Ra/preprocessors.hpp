// preprocessors.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef PREPROCESSORS_H
#define PREPROCESSORS_H 1

#include "generated_preprocessors.hpp"

// def nth with default : typename T1 = p, typename T2 = p, ... typename Tn = p
#define pp_typename_T_def_n( n, p ) typename T##n = p, 
#define pp_typename_T_def_n_end( n, p ) typename T##n = p
#define pp_typename_T_def( n, p ) pp_repeat( n, pp_typename_T_def_n, pp_typename_T_def_n_end, p )

// shift args left : T1, ..., Tn -> T2, ..., Tn+1
#define pp_args_lshift_t_n( n, t ) pp_cat( t, pp_inc( n ) ),
#define pp_args_lshift_t_n_end( n, t ) pp_cat( t, pp_inc( n ) )
#define pp_args_lshift_T( n ) pp_repeat( n, pp_args_lshift_t_n, pp_args_lshift_t_n_end, T )

// repeat same type nth
#define pp_repeat_t_n( n, t ) t,
#define pp_repeat_t_n_end( n, t ) t
#define pp_repeat_t( n, t ) pp_repeat( n, pp_repeat_t_n, pp_repeat_t_n_end, t )

// concat token
#define pp_cat( a, b ) pp_cat_( a, b )
#define pp_cat_( a, b ) a##b

#endif // PREPROCESSORS_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
