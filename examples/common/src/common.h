#ifdef WASM
# define BW_CXX_NO_ARRAY
# define BW_NO_MATH_H
# define INFINITY (__builtin_inff())
#endif

#ifndef DEBUG
# define BW_NO_DEBUG
#endif
