#include "common.h"
#include <bw_note_queue.h>
#include <bw_phase_gen.h>
#include <bw_osc_saw.h>
#include <bw_osc_pulse.h>
#include <bw_osc_tri.h>
#include <bw_osc_sin.h>
#include <bw_osc_filt.h>
#include <bw_noise_gen.h>
#include <bw_pink_filt.h>
#include <bw_svf.h>
#include <bw_env_gen.h>
#include <bw_gain.h>
#include <bw_ppm.h>
#include <bw_buf.h>
#include <bw_voice_alloc.h>

#define BUFFER_SIZE	128
#define SYNC_RATE	1e-3f	// synchronous control rate, seconds
#define N_VOICES	8

typedef struct voice {
	bw_phase_gen_coeffs	vco1_phase_gen_coeffs;
	bw_phase_gen_coeffs	vco2_phase_gen_coeffs;
	bw_phase_gen_coeffs	vco3_phase_gen_coeffs;
	bw_svf_coeffs		vcf_coeffs;

	bw_phase_gen_state	vco1_phase_gen_state;
	bw_phase_gen_state	vco2_phase_gen_state;
	bw_phase_gen_state	vco3_phase_gen_state;
	bw_osc_filt_state	osc_filt_state;
	bw_pink_filt_state	pink_filt_state;
	bw_env_gen_state	vcf_env_gen_state;
	bw_svf_state		vcf_state;
	bw_env_gen_state	vca_env_gen_state;

	unsigned char		note;
	char			gate;
	float			mod_k;
	float			vcf_env_k;
	
	float			buf[5][BUFFER_SIZE];
} voice;

typedef struct plugin {
	bw_note_queue		note_queue;
	bw_osc_saw_coeffs	vco_saw_coeffs;
	bw_osc_pulse_coeffs	vco1_pulse_coeffs;
	bw_osc_tri_coeffs	vco1_tri_coeffs;
	bw_gain_coeffs		vco1_gain_coeffs;
	bw_osc_pulse_coeffs	vco2_pulse_coeffs;
	bw_osc_tri_coeffs	vco2_tri_coeffs;
	bw_gain_coeffs		vco2_gain_coeffs;
	bw_osc_pulse_coeffs	vco3_pulse_coeffs;
	bw_osc_tri_coeffs	vco3_tri_coeffs;
	bw_gain_coeffs		vco3_gain_coeffs;
	bw_noise_gen_coeffs	noise_gen_coeffs;
	bw_pink_filt_coeffs	pink_filt_coeffs;
	bw_pink_filt_state	pink_filt_state;
	bw_gain_coeffs		noise_gain_coeffs;
	bw_env_gen_coeffs	vcf_env_gen_coeffs;
	bw_env_gen_coeffs	vca_env_gen_coeffs;
	bw_phase_gen_coeffs	a440_phase_gen_coeffs;
	bw_phase_gen_state	a440_phase_gen_state;
	bw_gain_coeffs		gain_coeffs;
	bw_ppm_coeffs		ppm_coeffs;
	bw_ppm_state		ppm_state;

	voice			voices[N_VOICES];

	size_t			sync_count;

	uint64_t		rand_state;
	float			master_tune;
	float			modulation_mix;
	float			vco1_modulation;
	float			vco1_coarse;
	float			vco1_fine;
	char			vco1_waveform;
	float			vco2_modulation;
	float			vco2_coarse;
	float			vco2_fine;
	char			vco2_waveform;
	char			vco3_kbd_ctrl;
	float			vco3_coarse;
	float			vco3_fine;
	char			vco3_waveform;
	char			noise_color;
	float			vcf_modulation;
	char			vcf_kbd_ctrl;
	float			vcf_cutoff;
	float			vcf_contour;
	char			a440;

	float			pitch_bend;
	float			mod_wheel;
	size_t			sync_left;
	char			vco3_waveform_cur;
	char			noise_color_cur;
	char			vco1_waveform_cur;
	char			vco2_waveform_cur;

	float			buf[BUFFER_SIZE];
} plugin;

static void plugin_init(plugin *instance) {
	bw_osc_saw_init(&instance->vco_saw_coeffs);
	bw_osc_pulse_init(&instance->vco1_pulse_coeffs);
	bw_osc_tri_init(&instance->vco1_tri_coeffs);
	bw_gain_init(&instance->vco1_gain_coeffs);
	bw_osc_pulse_init(&instance->vco2_pulse_coeffs);
	bw_osc_tri_init(&instance->vco2_tri_coeffs);
	bw_gain_init(&instance->vco2_gain_coeffs);
	bw_osc_pulse_init(&instance->vco3_pulse_coeffs);
	bw_osc_tri_init(&instance->vco3_tri_coeffs);
	bw_gain_init(&instance->vco3_gain_coeffs);
	bw_noise_gen_init(&instance->noise_gen_coeffs, &instance->rand_state);
	bw_pink_filt_init(&instance->pink_filt_coeffs);
	bw_gain_init(&instance->noise_gain_coeffs);
	bw_env_gen_init(&instance->vcf_env_gen_coeffs);
	bw_env_gen_init(&instance->vca_env_gen_coeffs);
	bw_phase_gen_init(&instance->a440_phase_gen_coeffs);
	bw_gain_init(&instance->gain_coeffs);
	bw_ppm_init(&instance->ppm_coeffs);

	for (int i = 0; i < N_VOICES; i++) {
		bw_phase_gen_init(&instance->voices[i].vco1_phase_gen_coeffs);
		bw_phase_gen_init(&instance->voices[i].vco2_phase_gen_coeffs);
		bw_phase_gen_init(&instance->voices[i].vco3_phase_gen_coeffs);
		bw_svf_init(&instance->voices[i].vcf_coeffs);
	}
	
	bw_osc_saw_set_antialiasing(&instance->vco_saw_coeffs, 1);
	bw_osc_pulse_set_antialiasing(&instance->vco1_pulse_coeffs, 1);
	bw_osc_tri_set_antialiasing(&instance->vco1_tri_coeffs, 1);
	bw_osc_pulse_set_antialiasing(&instance->vco2_pulse_coeffs, 1);
	bw_osc_tri_set_antialiasing(&instance->vco2_tri_coeffs, 1);
	bw_osc_pulse_set_antialiasing(&instance->vco3_pulse_coeffs, 1);
	bw_osc_tri_set_antialiasing(&instance->vco3_tri_coeffs, 1);
	bw_phase_gen_set_frequency(&instance->a440_phase_gen_coeffs, 440.f);
	
	instance->rand_state = 0xbaddecaf600dfeed;
}

static void plugin_fini(plugin *instance) {
	(void)instance;
}

static void plugin_set_sample_rate(plugin *instance, float sample_rate) {
	bw_osc_saw_set_sample_rate(&instance->vco_saw_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->vco1_pulse_coeffs, sample_rate);
	bw_osc_tri_set_sample_rate(&instance->vco1_tri_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->vco1_gain_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->vco2_pulse_coeffs, sample_rate);
	bw_osc_tri_set_sample_rate(&instance->vco2_tri_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->vco2_gain_coeffs, sample_rate);
	bw_osc_pulse_set_sample_rate(&instance->vco3_pulse_coeffs, sample_rate);
	bw_osc_tri_set_sample_rate(&instance->vco3_tri_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->vco3_gain_coeffs, sample_rate);
	bw_noise_gen_set_sample_rate(&instance->noise_gen_coeffs, sample_rate);
	bw_pink_filt_set_sample_rate(&instance->pink_filt_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->noise_gain_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->vcf_env_gen_coeffs, sample_rate);
	bw_env_gen_set_sample_rate(&instance->vca_env_gen_coeffs, sample_rate);
	bw_phase_gen_set_sample_rate(&instance->a440_phase_gen_coeffs, sample_rate);
	bw_gain_set_sample_rate(&instance->gain_coeffs, sample_rate);
	bw_ppm_set_sample_rate(&instance->ppm_coeffs, sample_rate);

	for (int i = 0; i < N_VOICES; i++) {
		bw_phase_gen_set_sample_rate(&instance->voices[i].vco1_phase_gen_coeffs, sample_rate);
		bw_phase_gen_set_sample_rate(&instance->voices[i].vco2_phase_gen_coeffs, sample_rate);
		bw_phase_gen_set_sample_rate(&instance->voices[i].vco3_phase_gen_coeffs, sample_rate);
		bw_svf_set_sample_rate(&instance->voices[i].vcf_coeffs, sample_rate);
	}

	instance->sync_count = (size_t)bw_roundf(sample_rate * SYNC_RATE);
}

static size_t plugin_mem_req(plugin *instance) {
	(void)instance;
	return 0;
}

static void plugin_mem_set(plugin *instance, void *mem) {
	(void)instance;
	(void)mem;
}

static void plugin_reset(plugin *instance) {
	for (int i = 0; i < N_VOICES; i++)
		bw_svf_set_cutoff(&instance->voices[i].vcf_coeffs, instance->vcf_cutoff);

	bw_note_queue_reset(&instance->note_queue);
	bw_osc_saw_reset_coeffs(&instance->vco_saw_coeffs);
	bw_osc_pulse_reset_coeffs(&instance->vco1_pulse_coeffs);
	bw_osc_tri_reset_coeffs(&instance->vco1_tri_coeffs);
	bw_gain_reset_coeffs(&instance->vco1_gain_coeffs);
	bw_osc_pulse_reset_coeffs(&instance->vco2_pulse_coeffs);
	bw_osc_tri_reset_coeffs(&instance->vco2_tri_coeffs);
	bw_gain_reset_coeffs(&instance->vco2_gain_coeffs);
	bw_osc_pulse_reset_coeffs(&instance->vco3_pulse_coeffs);
	bw_osc_tri_reset_coeffs(&instance->vco3_tri_coeffs);
	bw_gain_reset_coeffs(&instance->vco3_gain_coeffs);
	bw_noise_gen_reset_coeffs(&instance->noise_gen_coeffs);
	bw_pink_filt_reset_coeffs(&instance->pink_filt_coeffs);
	bw_gain_reset_coeffs(&instance->noise_gain_coeffs);
	bw_env_gen_reset_coeffs(&instance->vcf_env_gen_coeffs);
	bw_env_gen_reset_coeffs(&instance->vca_env_gen_coeffs);
	bw_phase_gen_reset_coeffs(&instance->a440_phase_gen_coeffs);
	float p, pi;
	bw_phase_gen_reset_state(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, 0.f, &p, &pi);
	bw_gain_reset_coeffs(&instance->gain_coeffs);
	bw_ppm_reset_coeffs(&instance->ppm_coeffs);
	bw_ppm_reset_state(&instance->ppm_coeffs, &instance->ppm_state, 0.f);

	for (int i = 0; i < N_VOICES; i++) {
		bw_phase_gen_reset_coeffs(&instance->voices[i].vco1_phase_gen_coeffs);
		bw_phase_gen_reset_coeffs(&instance->voices[i].vco2_phase_gen_coeffs);
		bw_phase_gen_reset_coeffs(&instance->voices[i].vco3_phase_gen_coeffs);
		bw_svf_reset_coeffs(&instance->voices[i].vcf_coeffs);
		
		bw_phase_gen_reset_state(&instance->voices[i].vco1_phase_gen_coeffs, &instance->voices[i].vco1_phase_gen_state, 0.f, &p, &pi);
		bw_phase_gen_reset_state(&instance->voices[i].vco2_phase_gen_coeffs, &instance->voices[i].vco2_phase_gen_state, 0.f, &p, &pi);
		bw_phase_gen_reset_state(&instance->voices[i].vco3_phase_gen_coeffs, &instance->voices[i].vco3_phase_gen_state, 0.f, &p, &pi);
		bw_osc_filt_reset_state(&instance->voices[i].osc_filt_state, 0.f);
		bw_pink_filt_reset_state(&instance->pink_filt_coeffs, &instance->voices[i].pink_filt_state, 0.f);
		float lp, bp, hp;
		bw_svf_reset_state(&instance->voices[i].vcf_coeffs, &instance->voices[i].vcf_state, 0.f, &lp, &bp, &hp);
		bw_env_gen_reset_state(&instance->vcf_env_gen_coeffs, &instance->voices[i].vcf_env_gen_state, 0.f);
		bw_env_gen_reset_state(&instance->vca_env_gen_coeffs, &instance->voices[i].vca_env_gen_state, 0.f);
		
		instance->voices[i].note = 60;
		instance->voices[i].gate = 0;
	}
	instance->pitch_bend = 0.f;
	instance->mod_wheel = 0.f;
	instance->sync_left = instance->sync_count;
	instance->vco3_waveform_cur = instance->vco3_waveform;
	instance->noise_color_cur = instance->noise_color;
	instance->vco1_waveform_cur = instance->vco1_waveform;
	instance->vco2_waveform_cur = instance->vco2_waveform;
}

static void plugin_set_parameter(plugin *instance, size_t index, float value) {
	switch (index) {
	case 0:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->gain_coeffs, v * v * v);
	}
		break;
	case 1:
		instance->master_tune = value;
		break;
	case 2:
	{
		// using portamento time 0% -> 90%: tau = portamento time / log(10)
		const float v = (0.001f * 0.4342944819032517f) * value;
		for (int i = 0; i < N_VOICES; i++) {
			bw_phase_gen_set_portamento_tau(&instance->voices[i].vco1_phase_gen_coeffs, v);
			bw_phase_gen_set_portamento_tau(&instance->voices[i].vco2_phase_gen_coeffs, v);
			bw_phase_gen_set_portamento_tau(&instance->voices[i].vco3_phase_gen_coeffs, v);
		}
		break;
	}
	case 3:
		instance->modulation_mix = 0.01f * value;
		break;
	case 4:
		instance->vco1_modulation = 0.01f * value;
		break;
	case 5:
		instance->vco1_coarse = value;
		break;
	case 6:
		instance->vco1_fine = value;
		break;
	case 7:
		instance->vco1_waveform = value;
		break;
	case 8:
	{
		const float v = 0.01f * value;
		bw_osc_pulse_set_pulse_width(&instance->vco1_pulse_coeffs, v);
		bw_osc_tri_set_slope(&instance->vco1_tri_coeffs, bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 9:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->vco1_gain_coeffs, v * v * v);
		break;
	}
	case 10:
		instance->vco2_modulation = 0.01f * value;
		break;
	case 11:
		instance->vco2_coarse = value;
		break;
	case 12:
		instance->vco2_fine = value;
		break;
	case 13:
		instance->vco2_waveform = value;
		break;
	case 14:
	{
		const float v = 0.01f * value;
		bw_osc_pulse_set_pulse_width(&instance->vco2_pulse_coeffs, v);
		bw_osc_tri_set_slope(&instance->vco2_tri_coeffs, bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 15:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->vco2_gain_coeffs, v * v * v);
		break;
	}
	case 16:
		instance->vco3_kbd_ctrl = value >= 0.5f;
		break;
	case 17:
		instance->vco3_coarse = value;
		break;
	case 18:
		instance->vco3_fine = value;
		break;
	case 19:
		instance->vco3_waveform = value;
		break;
	case 20:
	{
		const float v = 0.01f * value;
		bw_osc_pulse_set_pulse_width(&instance->vco3_pulse_coeffs, v);
		bw_osc_tri_set_slope(&instance->vco3_tri_coeffs, bw_clipf(v, 0.001f, 0.999f));
		break;
	}
	case 21:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->vco3_gain_coeffs, v * v * v);
		break;
	}
	case 22:
		instance->noise_color = value;
		break;
	case 23:
	{
		const float v = 0.01f * value;
		bw_gain_set_gain_lin(&instance->noise_gain_coeffs, v * v * v);
		break;
	}
	case 24:
		instance->vcf_modulation = 0.01f * value;
		break;
	case 25:
		instance->vcf_kbd_ctrl = value;
		break;
	case 26:
		instance->vcf_cutoff = value;
		break;
	case 27:
	{
		const float v = 0.5f + (0.01f * 9.5f) * value;
		for (int i = 0; i < N_VOICES; i++)
			bw_svf_set_Q(&instance->voices[i].vcf_coeffs, v);
		break;
	}
	case 28:
		instance->vcf_contour = 0.01f * value;
		break;
	case 29:
		bw_env_gen_set_attack(&instance->vcf_env_gen_coeffs, 0.001f * value);
		break;
	case 30:
		bw_env_gen_set_decay(&instance->vcf_env_gen_coeffs, 0.001f * value);
		break;
	case 31:
		bw_env_gen_set_sustain(&instance->vcf_env_gen_coeffs, 0.01f * value);
		break;
	case 32:
		bw_env_gen_set_release(&instance->vcf_env_gen_coeffs, 0.001f * value);
		break;
	case 33:
		bw_env_gen_set_attack(&instance->vca_env_gen_coeffs, 0.001f * value);
		break;
	case 34:
		bw_env_gen_set_decay(&instance->vca_env_gen_coeffs, 0.001f * value);
		break;
	case 35:
		bw_env_gen_set_sustain(&instance->vca_env_gen_coeffs, 0.01f * value);
		break;
	case 36:
		bw_env_gen_set_release(&instance->vca_env_gen_coeffs, 0.001f * value);
		break;
	case 37:
		instance->a440 = value >= 0.5f;
		break;
	}
}

static float plugin_get_parameter(plugin *instance, size_t index) {
	(void)index;
	return bw_clipf(bw_ppm_get_y_z1(&instance->ppm_state), -60.f, 0.f);
}

static void note_on(void *BW_RESTRICT handle, unsigned char note, float velocity) {
	(void)velocity;
	voice *v = (voice *)handle;
	v->note = note;
	v->gate = 1;
}

static void note_off(void *BW_RESTRICT handle, float velocity) {
	(void)velocity;
	voice *v = (voice *)handle;
	v->gate = 0;
}

static unsigned char get_note(const void *BW_RESTRICT handle) {
	voice *v = (voice *)handle;
	return v->note;
}

static char is_free(const void *BW_RESTRICT handle) {
	voice *v = (voice *)handle;
	bw_env_gen_phase phase = bw_env_gen_get_phase(&v->vca_env_gen_state);
	return !v->gate && phase == bw_env_gen_phase_off;
}

static void plugin_process(plugin *instance, const float **inputs, float **outputs, size_t n_samples) {
	(void)inputs;

	// voice allocation
	
	static bw_voice_alloc_opts alloc_opts = { bw_voice_alloc_priority_low, note_on, note_off, get_note, is_free };
	void *voices[N_VOICES];
	for (int i = 0; i < N_VOICES; i++)
		voices[i] = (void *)(instance->voices + i);
	bw_voice_alloc(&alloc_opts, &instance->note_queue, voices, N_VOICES);
	bw_note_queue_clear(&instance->note_queue);
	
	// asynchronous control-rate operations
	
	const float df1 = instance->vco1_coarse + instance->pitch_bend + (8.333333333333333e-2f * 0.01f) * instance->vco1_fine;
	const float df2 = instance->vco2_coarse + instance->pitch_bend + (8.333333333333333e-2f * 0.01f) * instance->vco2_fine;
	const float df3 = instance->vco3_coarse + instance->pitch_bend + (8.333333333333333e-2f * 0.01f) * instance->vco3_fine;
	for (int i = 0; i < N_VOICES; i++) {
		int n = instance->voices[i].note - 69;
		int n3 = instance->vco3_kbd_ctrl ? instance->voices[i].note - 69 : -69;
		bw_phase_gen_set_frequency(&instance->voices[i].vco1_phase_gen_coeffs, instance->master_tune * bw_pow2f(df1 + 8.333333333333333e-2f * n));
		bw_phase_gen_set_frequency(&instance->voices[i].vco2_phase_gen_coeffs, instance->master_tune * bw_pow2f(df2 + 8.333333333333333e-2f * n));
		bw_phase_gen_set_frequency(&instance->voices[i].vco3_phase_gen_coeffs, instance->master_tune * bw_pow2f(df3 + 8.333333333333333e-2f * n3));
	}

	if (instance->vco3_waveform_cur != instance->vco3_waveform) {
		switch (instance->vco3_waveform) {
		case 2:
			bw_osc_pulse_reset_coeffs(&instance->vco3_pulse_coeffs);
			break;
		case 3:
			bw_osc_tri_reset_coeffs(&instance->vco3_tri_coeffs);
			break;
		}
		instance->vco3_waveform_cur = instance->vco3_waveform;
	}

	if (instance->noise_color_cur != instance->noise_color) {
		if (instance->noise_color == 2)
			for (int j = 0; j < N_VOICES; j++)
				bw_pink_filt_reset_state(&instance->pink_filt_coeffs, &instance->voices[j].pink_filt_state, 0.f);
		instance->noise_color_cur = instance->noise_color;
	}

	if (instance->vco1_waveform_cur != instance->vco1_waveform) {
		switch (instance->vco1_waveform) {
		case 2:
			bw_osc_pulse_reset_coeffs(&instance->vco1_pulse_coeffs);
			break;
		case 3:
			bw_osc_tri_reset_coeffs(&instance->vco1_tri_coeffs);
			break;
		}
		instance->vco1_waveform_cur = instance->vco1_waveform;
	}

	if (instance->vco2_waveform_cur != instance->vco2_waveform) {
		switch (instance->vco2_waveform) {
		case 2:
			bw_osc_pulse_reset_coeffs(&instance->vco2_pulse_coeffs);
			break;
		case 3:
			bw_osc_tri_reset_coeffs(&instance->vco2_tri_coeffs);
			break;
		}
		instance->vco2_waveform_cur = instance->vco2_waveform;
	}

	// synchronous control-rate and audio-rate operations

	float *b0[N_VOICES], *b1[N_VOICES], *b2[N_VOICES], *b3[N_VOICES], *b4[N_VOICES];
	char gates[N_VOICES];
	bw_osc_filt_state *osc_filt_states[N_VOICES];
	bw_pink_filt_state *pink_filt_states[N_VOICES];
	bw_env_gen_state *vcf_env_gen_states[N_VOICES], *vca_env_gen_states[N_VOICES];
	for (int j = 0; j < N_VOICES; j++) {
		b0[j] = instance->voices[j].buf[0];
		b1[j] = instance->voices[j].buf[1];
		b2[j] = instance->voices[j].buf[2];
		b3[j] = instance->voices[j].buf[3];
		b4[j] = instance->voices[j].buf[4];
		gates[j] = instance->voices[j].gate;
		osc_filt_states[j] = &instance->voices[j].osc_filt_state;
		pink_filt_states[j] = &instance->voices[j].pink_filt_state;
		vcf_env_gen_states[j] = &instance->voices[j].vcf_env_gen_state;
		vca_env_gen_states[j] = &instance->voices[j].vca_env_gen_state;
	}

	for (size_t i = 0; i < n_samples; ) {
		float *out = outputs[0] + i;
		int n = bw_minf(bw_minf(n_samples - i, BUFFER_SIZE), instance->sync_left);

		const char sync = instance->sync_left == instance->sync_count;

		// osc 3

		for (int j = 0; j < N_VOICES; j++)
			bw_phase_gen_process(&instance->voices[j].vco3_phase_gen_coeffs, &instance->voices[j].vco3_phase_gen_state, NULL, b0[j], b1[j], n);
		switch (instance->vco3_waveform_cur) {
		case 1:
			bw_osc_saw_process_multi(&instance->vco_saw_coeffs, (const float **)b0, (const float **)b1, b0, N_VOICES, n);
			break;
		case 2:
			bw_osc_pulse_process_multi(&instance->vco3_pulse_coeffs, (const float **)b0, (const float **)b1, b0, N_VOICES, n);
			break;
		default:
			bw_osc_tri_process_multi(&instance->vco3_tri_coeffs, (const float **)b0, (const float **)b1, b0, N_VOICES, n);
			break;
		}

		// noise generator
		
		bw_noise_gen_process_multi(&instance->noise_gen_coeffs, b1, N_VOICES, n);
		if (instance->noise_color_cur == 2)
			bw_pink_filt_process_multi(&instance->pink_filt_coeffs, pink_filt_states, (const float **)b1, b1, N_VOICES, n);
		bw_buf_scale_multi((const float * const *)b1, 5.f, b1, N_VOICES, n);

		// modulation signals

		for (int j = 0; j < N_VOICES; j++) {
			for (int k = 0; k < n; k++)
				b2[j][k] = instance->mod_wheel * (b0[j][k] + instance->modulation_mix * (b1[j][k] - b0[j][k]));
		}
		if (sync)
			for (int j = 0; j < N_VOICES; j++)
				instance->voices[j].mod_k = b2[j][0];

		// osc 1

		for (int j = 0; j < N_VOICES; j++) {
			bw_buf_scale(b2[j], instance->vco1_modulation, b3[j], n);
			bw_phase_gen_process(&instance->voices[j].vco1_phase_gen_coeffs, &instance->voices[j].vco1_phase_gen_state, b3[j], b3[j], b4[j], n);
		}
		switch (instance->vco1_waveform_cur) {
		case 1:
			bw_osc_saw_process_multi(&instance->vco_saw_coeffs, (const float **)b3, (const float **)b4, b3, N_VOICES, n);
			break;
		case 2:
			bw_osc_pulse_process_multi(&instance->vco1_pulse_coeffs, (const float **)b3, (const float **)b4, b3, N_VOICES, n);
			break;
		default:
			bw_osc_tri_process_multi(&instance->vco1_tri_coeffs, (const float **)b3, (const float **)b4, b3, N_VOICES, n);
			break;
		}

		// osc 2

		for (int j = 0; j < N_VOICES; j++) {
			bw_buf_scale(b2[j], instance->vco2_modulation, b2[j], n);
			bw_phase_gen_process(&instance->voices[j].vco2_phase_gen_coeffs, &instance->voices[j].vco2_phase_gen_state, b2[j], b2[j], b4[j], n);
		}
		switch (instance->vco2_waveform_cur) {
		case 1:
			bw_osc_saw_process_multi(&instance->vco_saw_coeffs, (const float **)b2, (const float **)b4, b2, N_VOICES, n);
			break;
		case 2:
			bw_osc_pulse_process_multi(&instance->vco2_pulse_coeffs, (const float **)b2, (const float **)b4, b2, N_VOICES, n);
			break;
		default:
			bw_osc_tri_process_multi(&instance->vco2_tri_coeffs, (const float **)b2, (const float **)b4, b2, N_VOICES, n);
			break;
		}

		// mixer

		bw_gain_process_multi(&instance->vco1_gain_coeffs, (const float **)b3, b3, N_VOICES, n);
		bw_gain_process_multi(&instance->vco2_gain_coeffs, (const float **)b2, b2, N_VOICES, n);
		bw_gain_process_multi(&instance->vco3_gain_coeffs, (const float **)b0, b0, N_VOICES, n);
		bw_gain_process_multi(&instance->noise_gain_coeffs, (const float **)b1, b1, N_VOICES, n);
		bw_buf_mix_multi((const float * const *)b0, (const float * const *)b2, b0, N_VOICES, n);
		bw_buf_mix_multi((const float * const *)b0, (const float * const *)b3, b0, N_VOICES, n);

		bw_osc_filt_process_multi(osc_filt_states, (const float **)b0, b0, N_VOICES, n);

		const float k = instance->noise_color_cur == 2
			? 6.f * bw_noise_gen_get_scaling_k(&instance->noise_gen_coeffs) * bw_pink_filt_get_scaling_k(&instance->pink_filt_coeffs)
			: 0.1f * bw_noise_gen_get_scaling_k(&instance->noise_gen_coeffs);
		bw_buf_scale_multi((const float * const *)b1, k, b1, N_VOICES, n);
		bw_buf_mix_multi((const float * const *)b0, (const float * const *)b1, b0, N_VOICES, n);

		// vcf

		bw_env_gen_process_multi(&instance->vcf_env_gen_coeffs, vcf_env_gen_states, gates, NULL, N_VOICES, n);
		if (sync)
			for (int j = 0; j < N_VOICES; j++)
				instance->voices[j].vcf_env_k = bw_env_gen_get_y_z1(vcf_env_gen_states[j]);
		const float cutoff_unmapped = 0.1447648273010839f * bw_logf(0.05f * instance->vcf_cutoff);
		for (int j = 0; j < N_VOICES; j++) {
			const float cutoff_vpos =
				cutoff_unmapped
				+ instance->vcf_contour * instance->voices[j].vcf_env_k
				+ 0.3f * instance->vcf_modulation * instance->voices[j].mod_k;
			float cutoff = 20.f * bw_expf(6.907755278982137 * cutoff_vpos);
			switch (instance->vcf_kbd_ctrl) {
			case 2: // 1/3
				cutoff *= bw_pow2f((0.629960524947437f * 8.333333333333333e-2f) * (instance->voices[j].note - 60));
				break;
			case 3: // 2/3
				cutoff *= bw_pow2f((0.793700525984100f * 8.333333333333333e-2f) * (instance->voices[j].note - 60));
				break;
			case 4: // full
				cutoff *= bw_pow2f(8.333333333333333e-2f * (instance->voices[j].note - 60));
				break;
			default: // off, do nothing
				break;
			}
			bw_svf_set_cutoff(&instance->voices[j].vcf_coeffs, bw_clipf(cutoff, 20.f, 20e3f));
			bw_svf_process(&instance->voices[j].vcf_coeffs, &instance->voices[j].vcf_state, b0[j], b0[j], NULL, NULL, n);
		}

		// vca

		bw_env_gen_process_multi(&instance->vca_env_gen_coeffs, vca_env_gen_states, gates, b1, N_VOICES, n);
		bw_buf_mul_multi((const float * const *)b0, (const float * const *)b1, b0, N_VOICES, n);

		// mix voices

		bw_buf_fill(0.f, out, n);
		for (int j = 0; j < N_VOICES; j++)
			bw_buf_mix(out, b0[j], out, n);

		// A 440 Hz osc

		if (instance->a440) { // nobody will notice or care about phase issues here
			bw_phase_gen_process(&instance->a440_phase_gen_coeffs, &instance->a440_phase_gen_state, NULL, instance->buf, NULL, n);
			bw_osc_sin_process(instance->buf, instance->buf, n);
			bw_buf_mix(out, instance->buf, out, n);
		}

		// output
		
		bw_gain_process(&instance->gain_coeffs, out, out, n);
		bw_ppm_process(&instance->ppm_coeffs, &instance->ppm_state, out, NULL, n);

		instance->sync_left -= n;
		if (instance->sync_left == 0)
			instance->sync_left = instance->sync_count;

		i += n;
	}
}

static void plugin_midi_msg_in(plugin *instance, size_t index, const uint8_t * data) {
	(void)index;
	switch (data[0] & 0xf0) {
	case 0x90: // note on
		bw_note_queue_add(&instance->note_queue, data[1], data[2] != 0, (1.f / 127.f) * data[2], 0);
		break;
	case 0x80: // note off
		bw_note_queue_add(&instance->note_queue, data[1], 0, 0, 0);
		break;
	case 0xe0: // pitch bend
	{
		const uint16_t v = (data[2] << 7) | data[1];
		instance->pitch_bend = 2.f * bw_maxf((1.f / 16383.f) * (v - 0x2000), -1.f) - 1.f;
		break;
	}
	case 0xb0: // control change
		if (data[1] == 1) // mod wheel
			instance->mod_wheel = (1.f / 127.f) * data[2];
		break;
	}
}
