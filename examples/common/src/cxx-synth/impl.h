#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void * impl;

impl impl_new(void);
void impl_free(impl handle);
void impl_set_sample_rate(impl handle, float sample_rate);
void impl_reset(impl handle);
void impl_set_parameter(impl handle, size_t index, float value);
float impl_get_parameter(impl handle, size_t index);
void impl_process(impl handle, const float ** inputs, float ** outputs, size_t n_samples);
void impl_midi_msg_in(impl handle, size_t index, const uint8_t * data);

#ifdef __cplusplus
}
#endif
