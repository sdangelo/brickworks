#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void * impl;

impl impl_new();
void impl_free(impl instance);
void impl_set_sample_rate(impl instance, float sample_rate);
void impl_reset(impl instance);
void impl_set_parameter(impl instance, size_t index, float value);
float impl_get_parameter(impl instance, size_t index);
void impl_process(impl instance, const float **inputs, float **outputs, size_t n_samples);

#ifdef __cplusplus
}
#endif
