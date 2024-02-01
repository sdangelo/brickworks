#ifdef WASM
# define BW_NO_MATH_H
# define INFINITY (__builtin_inff())
#endif
#define BW_NO_DEBUG
