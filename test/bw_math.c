#include <stdlib.h>
#include <stdio.h>

#include <bw_math.h>

#include <math.h>

int n_ok = 0;
int n_ko = 0;

#define TEST(expr, expected) \
{ \
	union { float f; uint32_t u; } v_expr, v_expected; \
	v_expr.f = expr; \
	v_expected.f = expected; \
	if (expr == v_expected.f) { \
		printf("✔ %s = %g\n", #expr, v_expected.f); \
		n_ok++; \
	} else { \
		printf("✘ %s = %g [0x%x] (expected %g [0x%x]) - line %d\n", #expr, v_expr.f, v_expr.u, v_expected.f, v_expected.u, __LINE__); \
		n_ko++; \
	} \
}

#define TEST_BW_INTFRAC(val, expected_i, expected_f) \
{ \
	union { float f; uint32_t u; } v_res_i, v_res_f, v_expected_i, v_expected_f; \
	v_expected_i.f = expected_i; \
	v_expected_f.f = expected_f; \
	bw_intfracf(val, &v_res_i.f, &v_res_f.f); \
	if (v_res_i.f == v_expected_i.f && v_res_f.f == v_expected_f.f) { \
		printf("✔ bw_intfracf(%g) = %g, %g\n", val, expected_i, expected_f); \
		n_ok++; \
	} else { \
		printf("✘ bw_intfracf(%g) = %g [0x%x], %g [0x%x] (expected %g [0x%x], %g [0x%x]) - line %d\n", val, v_res_i.f, v_res_i.u, v_res_f.f, v_res_f.u, v_expected_i.f, v_expected_i.u, v_expected_f.f, v_expected_f.u, __LINE__); \
		n_ko++; \
	} \
}

#define TEST_REL(expr, expected, tol) \
{ \
	union { float f; uint32_t u; } v_expr, v_expected; \
	v_expr.f = expr; \
	v_expected.f = expected; \
	float err = fabsf((v_expr.f - v_expected.f) / v_expected.f); \
	if (err <= tol) { \
		printf("✔ %s = %g (expected %g, err %g%%, tol %g%%)\n", #expr, v_expr.f, v_expected.f, err * 100.f, tol * 100.f); \
		n_ok++; \
	} else { \
		printf("✘ %s = %g [0x%x] (expected %g [0x%x], err %g%%, tol %g%%) - line %d\n", #expr, v_expr.f, v_expr.u, v_expected.f, v_expected.u, err * 100.f, tol * 100.f, __LINE__); \
		n_ko++; \
	} \
}

#define TEST_ABS_REL(expr, expected, tol_abs, tol_rel) \
{ \
	union { float f; uint32_t u; } v_expr, v_expected; \
	v_expr.f = expr; \
	v_expected.f = expected; \
	float err_abs = fabsf(v_expr.f - v_expected.f); \
	float err_rel = fabsf((v_expr.f - v_expected.f) / v_expected.f); \
	if (err_abs <= tol_abs || err_rel <= tol_rel) { \
		printf("✔ %s = %g (expected %g, err %g|%g%%, tol %g|%g%%)\n", #expr, v_expr.f, v_expected.f, err_abs, err_rel * 100.f, tol_abs, tol_rel * 100.f); \
		n_ok++; \
	} else { \
		printf("✘ %s = %g [0x%x] (expected %g [0x%x], err %g|%g%%, tol %g|%g%%) - line %d\n", #expr, v_expr.f, v_expr.u, v_expected.f, v_expected.u, err_abs, err_rel * 100.f, tol_abs, tol_rel * 100.f, __LINE__); \
		n_ko++; \
	} \
}

int main() {
	printf("\nbw_math unit tests\n");
	printf("------------------\n\n");

	TEST(bw_copysignf(INFINITY, INFINITY), INFINITY);
	TEST(bw_copysignf(INFINITY, -INFINITY), -INFINITY);
	TEST(bw_copysignf(INFINITY, 1e3f), INFINITY);
	TEST(bw_copysignf(INFINITY, -1e3f), -INFINITY);
	TEST(bw_copysignf(INFINITY, 1.f), INFINITY);
	TEST(bw_copysignf(INFINITY, -1.f), -INFINITY);
	TEST(bw_copysignf(INFINITY, 1e-3f), INFINITY);
	TEST(bw_copysignf(INFINITY, -1e-3f), -INFINITY);
	TEST(bw_copysignf(INFINITY, 0.f), INFINITY);
	TEST(bw_copysignf(INFINITY, -0.f), -INFINITY);
	TEST(bw_copysignf(1e3f, INFINITY), 1e3f);
	TEST(bw_copysignf(1e3f, -INFINITY), -1e3f);
	TEST(bw_copysignf(1e3f, 1e3f), 1e3f);
	TEST(bw_copysignf(1e3f, -1e3f), -1e3f);
	TEST(bw_copysignf(1e3f, 1.f), 1e3f);
	TEST(bw_copysignf(1e3f, -1.f), -1e3f);
	TEST(bw_copysignf(1e3f, 1e-3f), 1e3f);
	TEST(bw_copysignf(1e3f, -1e-3f), -1e3f);
	TEST(bw_copysignf(1e3f, 0.f), 1e3f);
	TEST(bw_copysignf(1e3f, -0.f), -1e3f);
	TEST(bw_copysignf(1.f, INFINITY), 1.f);
	TEST(bw_copysignf(1.f, -INFINITY), -1.f);
	TEST(bw_copysignf(1.f, 1e3f), 1.f);
	TEST(bw_copysignf(1.f, -1e3f), -1.f);
	TEST(bw_copysignf(1.f, 1.f), 1.f);
	TEST(bw_copysignf(1.f, -1.f), -1.f);
	TEST(bw_copysignf(1.f, 1e-3f), 1.f);
	TEST(bw_copysignf(1.f, -1e-3f), -1.f);
	TEST(bw_copysignf(1.f, 0.f), 1.f);
	TEST(bw_copysignf(1.f, -0.f), -1.f);
	TEST(bw_copysignf(1e-3f, INFINITY), 1e-3f);
	TEST(bw_copysignf(1e-3f, -INFINITY), -1e-3f);
	TEST(bw_copysignf(1e-3f, 1e3f), 1e-3f);
	TEST(bw_copysignf(1e-3f, -1e3f), -1e-3f);
	TEST(bw_copysignf(1e-3f, 1.f), 1e-3f);
	TEST(bw_copysignf(1e-3f, -1.f), -1e-3f);
	TEST(bw_copysignf(1e-3f, 1e-3f), 1e-3f);
	TEST(bw_copysignf(1e-3f, -1e-3f), -1e-3f);
	TEST(bw_copysignf(1e-3f, 0.f), 1e-3f);
	TEST(bw_copysignf(1e-3f, -0.f), -1e-3f);
	TEST(bw_copysignf(0.f, INFINITY), 0.f);
	TEST(bw_copysignf(0.f, -INFINITY), -0.f);
	TEST(bw_copysignf(0.f, 1e3f), 0.f);
	TEST(bw_copysignf(0.f, -1e3f), -0.f);
	TEST(bw_copysignf(0.f, 1.f), 0.f);
	TEST(bw_copysignf(0.f, -1.f), -0.f);
	TEST(bw_copysignf(0.f, 1e-3f), 0.f);
	TEST(bw_copysignf(0.f, -1e-3f), -0.f);
	TEST(bw_copysignf(0.f, 0.f), 0.f);
	TEST(bw_copysignf(0.f, -0.f), -0.f);
	TEST(bw_copysignf(-INFINITY, INFINITY), INFINITY);
	TEST(bw_copysignf(-INFINITY, -INFINITY), -INFINITY);
	TEST(bw_copysignf(-INFINITY, 1e3f), INFINITY);
	TEST(bw_copysignf(-INFINITY, -1e3f), -INFINITY);
	TEST(bw_copysignf(-INFINITY, 1.f), INFINITY);
	TEST(bw_copysignf(-INFINITY, -1.f), -INFINITY);
	TEST(bw_copysignf(-INFINITY, 1e-3f), INFINITY);
	TEST(bw_copysignf(-INFINITY, -1e-3f), -INFINITY);
	TEST(bw_copysignf(-INFINITY, 0.f), INFINITY);
	TEST(bw_copysignf(-INFINITY, -0.f), -INFINITY);
	TEST(bw_copysignf(-1e3f, INFINITY), 1e3f);
	TEST(bw_copysignf(-1e3f, -INFINITY), -1e3f);
	TEST(bw_copysignf(-1e3f, 1e3f), 1e3f);
	TEST(bw_copysignf(-1e3f, -1e3f), -1e3f);
	TEST(bw_copysignf(-1e3f, 1.f), 1e3f);
	TEST(bw_copysignf(-1e3f, -1.f), -1e3f);
	TEST(bw_copysignf(-1e3f, 1e-3f), 1e3f);
	TEST(bw_copysignf(-1e3f, -1e-3f), -1e3f);
	TEST(bw_copysignf(-1e3f, 0.f), 1e3f);
	TEST(bw_copysignf(-1e3f, -0.f), -1e3f);
	TEST(bw_copysignf(-1.f, INFINITY), 1.f);
	TEST(bw_copysignf(-1.f, -INFINITY), -1.f);
	TEST(bw_copysignf(-1.f, 1e3f), 1.f);
	TEST(bw_copysignf(-1.f, -1e3f), -1.f);
	TEST(bw_copysignf(-1.f, 1.f), 1.f);
	TEST(bw_copysignf(-1.f, -1.f), -1.f);
	TEST(bw_copysignf(-1.f, 1e-3f), 1.f);
	TEST(bw_copysignf(-1.f, -1e-3f), -1.f);
	TEST(bw_copysignf(-1.f, 0.f), 1.f);
	TEST(bw_copysignf(-1.f, -0.f), -1.f);
	TEST(bw_copysignf(-1e-3f, INFINITY), 1e-3f);
	TEST(bw_copysignf(-1e-3f, -INFINITY), -1e-3f);
	TEST(bw_copysignf(-1e-3f, 1e3f), 1e-3f);
	TEST(bw_copysignf(-1e-3f, -1e3f), -1e-3f);
	TEST(bw_copysignf(-1e-3f, 1.f), 1e-3f);
	TEST(bw_copysignf(-1e-3f, -1.f), -1e-3f);
	TEST(bw_copysignf(-1e-3f, 1e-3f), 1e-3f);
	TEST(bw_copysignf(-1e-3f, -1e-3f), -1e-3f);
	TEST(bw_copysignf(-1e-3f, 0.f), 1e-3f);
	TEST(bw_copysignf(-1e-3f, -0.f), -1e-3f);
	TEST(bw_copysignf(-0.f, INFINITY), 0.f);
	TEST(bw_copysignf(-0.f, -INFINITY), -0.f);
	TEST(bw_copysignf(-0.f, 1e3f), 0.f);
	TEST(bw_copysignf(-0.f, -1e3f), -0.f);
	TEST(bw_copysignf(-0.f, 1.f), 0.f);
	TEST(bw_copysignf(-0.f, -1.f), -0.f);
	TEST(bw_copysignf(-0.f, 1e-3f), 0.f);
	TEST(bw_copysignf(-0.f, -1e-3f), -0.f);
	TEST(bw_copysignf(-0.f, 0.f), 0.f);
	TEST(bw_copysignf(-0.f, -0.f), -0.f);
	
	TEST(bw_signf(INFINITY), 1.f);
	TEST(bw_signf(-INFINITY), -1.f);
	TEST(bw_signf(1e3f), 1.f);
	TEST(bw_signf(-1e3f), -1.f);
	TEST(bw_signf(1.f), 1.f);
	TEST(bw_signf(-1.f), -1.f);
	TEST(bw_signf(1e-3f), 1.f);
	TEST(bw_signf(-1e-3f), -1.f);
	TEST(bw_signf(0.f), 0.f);
	TEST(bw_signf(-0.f), 0.f);
	
	TEST(bw_absf(INFINITY), INFINITY);
	TEST(bw_absf(-INFINITY), INFINITY);
	TEST(bw_absf(1e3f), 1e3f);
	TEST(bw_absf(-1e3f), 1e3f);
	TEST(bw_absf(1.f), 1.f);
	TEST(bw_absf(-1.f), 1.f);
	TEST(bw_absf(1e-3f), 1e-3f);
	TEST(bw_absf(-1e-3f), 1e-3f);
	TEST(bw_absf(0.f), 0.f);
	TEST(bw_absf(-0.f), 0.f);
	
	TEST(bw_min0f(INFINITY), 0.f);
	TEST(bw_min0f(-INFINITY), -INFINITY);
	TEST(bw_min0f(1e3f), 0.f);
	TEST(bw_min0f(-1e3f), -1e3f);
	TEST(bw_min0f(1.f), 0.f);
	TEST(bw_min0f(-1.f), -1.f);
	TEST(bw_min0f(1e-3f), 0.f);
	TEST(bw_min0f(-1e-3f), -1e-3f);
	TEST(bw_min0f(0.f), 0.f);
	TEST(bw_min0f(-0.f), -0.f);
	
	TEST(bw_max0f(INFINITY), INFINITY);
	TEST(bw_max0f(-INFINITY), 0.f);
	TEST(bw_max0f(1e3f), 1e3f);
	TEST(bw_max0f(-1e3f), 0.f);
	TEST(bw_max0f(1.f), 1.f);
	TEST(bw_max0f(-1.f), 0.f);
	TEST(bw_max0f(1e-3f), 1e-3f);
	TEST(bw_max0f(-1e-3f), 0.f);
	TEST(bw_max0f(0.f), 0.f);
	TEST(bw_max0f(-0.f), 0.f);
	
	TEST(bw_minf(INFINITY, INFINITY), INFINITY);
	TEST(bw_minf(INFINITY, 1e3f), 1e3f);
	TEST(bw_minf(INFINITY, 1.f), 1.f);
	TEST(bw_minf(INFINITY, 1e-3f), 1e-3f);
	TEST(bw_minf(INFINITY, 0.f), 0.f);
	TEST(bw_minf(INFINITY, -1e-3f), -1e-3f);
	TEST(bw_minf(INFINITY, -1.f), -1.f);
	TEST(bw_minf(INFINITY, -1e3f), -1e3f);
	TEST(bw_minf(INFINITY, -INFINITY), -INFINITY);
	TEST(bw_minf(1e3f, INFINITY), 1e3f);
	TEST(bw_minf(1e3f, 1e3f), 1e3f);
	TEST(bw_minf(1e3f, 1.f), 1.f);
	TEST(bw_minf(1e3f, 1e-3f), 1e-3f);
	TEST(bw_minf(1e3f, 0.f), 0.f);
	TEST(bw_minf(1e3f, -1e-3f), -1e-3f);
	TEST(bw_minf(1e3f, -1.f), -1.f);
	TEST(bw_minf(1e3f, -1e3f), -1e3f);
	TEST(bw_minf(1e3f, -INFINITY), -INFINITY);
	TEST(bw_minf(1.f, INFINITY), 1.f);
	TEST(bw_minf(1.f, 1e3f), 1.f);
	TEST(bw_minf(1.f, 1.f), 1.f);
	TEST(bw_minf(1.f, 1e-3f), 1e-3f);
	TEST(bw_minf(1.f, 0.f), 0.f);
	TEST(bw_minf(1.f, -1e-3f), -1e-3f);
	TEST(bw_minf(1.f, -1.f), -1.f);
	TEST(bw_minf(1.f, -1e3f), -1e3f);
	TEST(bw_minf(1.f, -INFINITY), -INFINITY);
	TEST(bw_minf(1e-3f, INFINITY), 1e-3f);
	TEST(bw_minf(1e-3f, 1e3f), 1e-3f);
	TEST(bw_minf(1e-3f, 1.f), 1e-3f);
	TEST(bw_minf(1e-3f, 1e-3f), 1e-3f);
	TEST(bw_minf(1e-3f, 0.f), 0.f);
	TEST(bw_minf(1e-3f, -1e-3f), -1e-3f);
	TEST(bw_minf(1e-3f, -1.f), -1.f);
	TEST(bw_minf(1e-3f, -1e3f), -1e3f);
	TEST(bw_minf(1e-3f, -INFINITY), -INFINITY);
	TEST(bw_minf(0.f, INFINITY), 0.f);
	TEST(bw_minf(0.f, 1e3f), 0.f);
	TEST(bw_minf(0.f, 1.f), 0.f);
	TEST(bw_minf(0.f, 1e-3f), 0.f);
	TEST(bw_minf(0.f, 0.f), 0.f);
	TEST(bw_minf(0.f, -1e-3f), -1e-3f);
	TEST(bw_minf(0.f, -1.f), -1.f);
	TEST(bw_minf(0.f, -1e3f), -1e3f);
	TEST(bw_minf(0.f, -INFINITY), -INFINITY);
	TEST(bw_minf(-1e-3f, INFINITY), -1e-3f);
	TEST(bw_minf(-1e-3f, 1e3f), -1e-3f);
	TEST(bw_minf(-1e-3f, 1.f), -1e-3f);
	TEST(bw_minf(-1e-3f, 1e-3f), -1e-3f);
	TEST(bw_minf(-1e-3f, 0.f), -1e-3f);
	TEST(bw_minf(-1e-3f, -1e-3f), -1e-3f);
	TEST(bw_minf(-1e-3f, -1.f), -1.f);
	TEST(bw_minf(-1e-3f, -1e3f), -1e3f);
	TEST(bw_minf(-1e-3f, -INFINITY), -INFINITY);
	TEST(bw_minf(-1.f, INFINITY), -1.f);
	TEST(bw_minf(-1.f, 1e3f), -1.f);
	TEST(bw_minf(-1.f, 1.f), -1.f);
	TEST(bw_minf(-1.f, 1e-3f), -1.f);
	TEST(bw_minf(-1.f, 0.f), -1.f);
	TEST(bw_minf(-1.f, -1e-3f), -1.f);
	TEST(bw_minf(-1.f, -1.f), -1.f);
	TEST(bw_minf(-1.f, -1e3f), -1e3f);
	TEST(bw_minf(-1.f, -INFINITY), -INFINITY);
	TEST(bw_minf(-1e3f, INFINITY), -1e3f);
	TEST(bw_minf(-1e3f, 1e3f), -1e3f);
	TEST(bw_minf(-1e3f, 1.f), -1e3f);
	TEST(bw_minf(-1e3f, 1e-3f), -1e3f);
	TEST(bw_minf(-1e3f, 0.f), -1e3f);
	TEST(bw_minf(-1e3f, -1e-3f), -1e3f);
	TEST(bw_minf(-1e3f, -1.f), -1e3f);
	TEST(bw_minf(-1e3f, -1e3f), -1e3f);
	TEST(bw_minf(-1e3f, -INFINITY), -INFINITY);
	TEST(bw_minf(-INFINITY, INFINITY), -INFINITY);
	TEST(bw_minf(-INFINITY, 1e3f), -INFINITY);
	TEST(bw_minf(-INFINITY, 1.f), -INFINITY);
	TEST(bw_minf(-INFINITY, 1e-3f), -INFINITY);
	TEST(bw_minf(-INFINITY, 0.f), -INFINITY);
	TEST(bw_minf(-INFINITY, -1e-3f), -INFINITY);
	TEST(bw_minf(-INFINITY, -1.f), -INFINITY);
	TEST(bw_minf(-INFINITY, -1e3f), -INFINITY);
	TEST(bw_minf(-INFINITY, -INFINITY), -INFINITY);
	
	TEST(bw_maxf(INFINITY, INFINITY), INFINITY);
	TEST(bw_maxf(INFINITY, 1e3f), INFINITY);
	TEST(bw_maxf(INFINITY, 1.f), INFINITY);
	TEST(bw_maxf(INFINITY, 1e-3f), INFINITY);
	TEST(bw_maxf(INFINITY, 0.f), INFINITY);
	TEST(bw_maxf(INFINITY, -1e-3f), INFINITY);
	TEST(bw_maxf(INFINITY, -1.f), INFINITY);
	TEST(bw_maxf(INFINITY, -1e3f), INFINITY);
	TEST(bw_maxf(INFINITY, -INFINITY), INFINITY);
	TEST(bw_maxf(1e3f, INFINITY), INFINITY);
	TEST(bw_maxf(1e3f, 1e3f), 1e3f);
	TEST(bw_maxf(1e3f, 1.f), 1e3f);
	TEST(bw_maxf(1e3f, 1e-3f), 1e3f);
	TEST(bw_maxf(1e3f, 0.f), 1e3f);
	TEST(bw_maxf(1e3f, -1e-3f), 1e3f);
	TEST(bw_maxf(1e3f, -1.f), 1e3f);
	TEST(bw_maxf(1e3f, -1e3f), 1e3f);
	TEST(bw_maxf(1e3f, -INFINITY), 1e3f);
	TEST(bw_maxf(1.f, INFINITY), INFINITY);
	TEST(bw_maxf(1.f, 1e3f), 1e3f);
	TEST(bw_maxf(1.f, 1.f), 1.f);
	TEST(bw_maxf(1.f, 1e-3f), 1.f);
	TEST(bw_maxf(1.f, 0.f), 1.f);
	TEST(bw_maxf(1.f, -1e-3f), 1.f);
	TEST(bw_maxf(1.f, -1.f), 1.f);
	TEST(bw_maxf(1.f, -1e3f), 1.f);
	TEST(bw_maxf(1.f, -INFINITY), 1.f);
	TEST(bw_maxf(1e-3f, INFINITY), INFINITY);
	TEST(bw_maxf(1e-3f, 1e3f), 1e3f);
	TEST(bw_maxf(1e-3f, 1.f), 1.f);
	TEST(bw_maxf(1e-3f, 1e-3f), 1e-3f);
	TEST(bw_maxf(1e-3f, 0.f), 1e-3f);
	TEST(bw_maxf(1e-3f, -1e-3f), 1e-3f);
	TEST(bw_maxf(1e-3f, -1.f), 1e-3f);
	TEST(bw_maxf(1e-3f, -1e3f), 1e-3f);
	TEST(bw_maxf(1e-3f, -INFINITY), 1e-3f);
	TEST(bw_maxf(0.f, INFINITY), INFINITY);
	TEST(bw_maxf(0.f, 1e3f), 1e3f);
	TEST(bw_maxf(0.f, 1.f), 1.f);
	TEST(bw_maxf(0.f, 1e-3f), 1e-3f);
	TEST(bw_maxf(0.f, 0.f), 0.f);
	TEST(bw_maxf(0.f, -1e-3f), 0.f);
	TEST(bw_maxf(0.f, -1.f), 0.f);
	TEST(bw_maxf(0.f, -1e3f), 0.f);
	TEST(bw_maxf(0.f, -INFINITY), 0.f);
	TEST(bw_maxf(-1e-3f, INFINITY), INFINITY);
	TEST(bw_maxf(-1e-3f, 1e3f), 1e3f);
	TEST(bw_maxf(-1e-3f, 1.f), 1.f);
	TEST(bw_maxf(-1e-3f, 1e-3f), 1e-3f);
	TEST(bw_maxf(-1e-3f, 0.f), 0.f);
	TEST(bw_maxf(-1e-3f, -1e-3f), -1e-3f);
	TEST(bw_maxf(-1e-3f, -1.f), -1e-3f);
	TEST(bw_maxf(-1e-3f, -1e3f), -1e-3f);
	TEST(bw_maxf(-1e-3f, -INFINITY), -1e-3f);
	TEST(bw_maxf(-1.f, INFINITY), INFINITY);
	TEST(bw_maxf(-1.f, 1e3f), 1e3f);
	TEST(bw_maxf(-1.f, 1.f), 1.f);
	TEST(bw_maxf(-1.f, 1e-3f), 1e-3f);
	TEST(bw_maxf(-1.f, 0.f), 0.f);
	TEST(bw_maxf(-1.f, -1e-3f), -1e-3f);
	TEST(bw_maxf(-1.f, -1.f), -1.f);
	TEST(bw_maxf(-1.f, -1e3f), -1.f);
	TEST(bw_maxf(-1.f, -INFINITY), -1.f);
	TEST(bw_maxf(-1e3f, INFINITY), INFINITY);
	TEST(bw_maxf(-1e3f, 1e3f), 1e3f);
	TEST(bw_maxf(-1e3f, 1.f), 1.f);
	TEST(bw_maxf(-1e3f, 1e-3f), 1e-3f);
	TEST(bw_maxf(-1e3f, 0.f), 0.f);
	TEST(bw_maxf(-1e3f, -1e-3f), -1e-3f);
	TEST(bw_maxf(-1e3f, -1.f), -1.f);
	TEST(bw_maxf(-1e3f, -1e3f), -1e3f);
	TEST(bw_maxf(-1e3f, -INFINITY), -1e3f);
	TEST(bw_maxf(-INFINITY, INFINITY), INFINITY);
	TEST(bw_maxf(-INFINITY, 1e3f), 1e3f);
	TEST(bw_maxf(-INFINITY, 1.f), 1.f);
	TEST(bw_maxf(-INFINITY, 1e-3f), 1e-3f);
	TEST(bw_maxf(-INFINITY, 0.f), 0.f);
	TEST(bw_maxf(-INFINITY, -1e-3f), -1e-3f);
	TEST(bw_maxf(-INFINITY, -1.f), -1.f);
	TEST(bw_maxf(-INFINITY, -1e3f), -1e3f);
	TEST(bw_maxf(-INFINITY, -INFINITY), -INFINITY);
	
	TEST(bw_clipf(INFINITY, -INFINITY, INFINITY), INFINITY);
	TEST(bw_clipf(INFINITY, -1e3f, 1e3f), 1e3f);
	TEST(bw_clipf(INFINITY, -1.f, 1.f), 1.f);
	TEST(bw_clipf(INFINITY, -1e-3f, 1e-3f), 1e-3f);
	TEST(bw_clipf(INFINITY, 0.f, 0.f), 0.f);
	TEST(bw_clipf(1e3f, -INFINITY, INFINITY), 1e3f);
	TEST(bw_clipf(1e3f, -1e3f, 1e3f), 1e3f);
	TEST(bw_clipf(1e3f, -1.f, 1.f), 1.f);
	TEST(bw_clipf(1e3f, -1e-3f, 1e-3f), 1e-3f);
	TEST(bw_clipf(1e3f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(1.f, -INFINITY, INFINITY), 1.f);
	TEST(bw_clipf(1.f, -1e3f, 1e3f), 1.f);
	TEST(bw_clipf(1.f, -1.f, 1.f), 1.f);
	TEST(bw_clipf(1.f, -1e-3f, 1e-3f), 1e-3f);
	TEST(bw_clipf(1.f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(1e-3f, -INFINITY, INFINITY), 1e-3f);
	TEST(bw_clipf(1e-3f, -1e3f, 1e3f), 1e-3f);
	TEST(bw_clipf(1e-3f, -1.f, 1.f), 1e-3f);
	TEST(bw_clipf(1e-3f, -1e-3f, 1e-3f), 1e-3f);
	TEST(bw_clipf(1e-3f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(0.f, -INFINITY, INFINITY), 0.f);
	TEST(bw_clipf(0.f, -1e3f, 1e3f), 0.f);
	TEST(bw_clipf(0.f, -1.f, 1.f), 0.f);
	TEST(bw_clipf(0.f, -1e-3f, 1e-3f), 0.f);
	TEST(bw_clipf(0.f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(-1e-3f, -INFINITY, INFINITY), -1e-3f);
	TEST(bw_clipf(-1e-3f, -1e3f, 1e3f), -1e-3f);
	TEST(bw_clipf(-1e-3f, -1.f, 1.f), -1e-3f);
	TEST(bw_clipf(-1e-3f, -1e-3f, 1e-3f), -1e-3f);
	TEST(bw_clipf(-1e-3f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(-1.f, -INFINITY, INFINITY), -1.f);
	TEST(bw_clipf(-1.f, -1e3f, 1e3f), -1.f);
	TEST(bw_clipf(-1.f, -1.f, 1.f), -1.f);
	TEST(bw_clipf(-1.f, -1e-3f, 1e-3f), -1e-3f);
	TEST(bw_clipf(-1.f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(-1e3f, -INFINITY, INFINITY), -1e3f);
	TEST(bw_clipf(-1e3f, -1e3f, 1e3f), -1e3f);
	TEST(bw_clipf(-1e3f, -1.f, 1.f), -1.f);
	TEST(bw_clipf(-1e3f, -1e-3f, 1e-3f), -1e-3f);
	TEST(bw_clipf(-1e3f, 0.f, 0.f), 0.f);
	TEST(bw_clipf(-INFINITY, -INFINITY, INFINITY), -INFINITY);
	TEST(bw_clipf(-INFINITY, -1e3f, 1e3f), -1e3f);
	TEST(bw_clipf(-INFINITY, -1.f, 1.f), -1.f);
	TEST(bw_clipf(-INFINITY, -1e-3f, 1e-3f), -1e-3f);
	TEST(bw_clipf(-INFINITY, 0.f, 0.f), 0.f);
	
	TEST(bw_truncf(1.234e38f), 1.234e38f);
	TEST(bw_truncf(1001.f), 1001.f);
	TEST(bw_truncf(1000.9f), 1000.f);
	TEST(bw_truncf(1000.5f), 1000.f);
	TEST(bw_truncf(1000.1f), 1000.f);
	TEST(bw_truncf(1000.0f), 1000.f);
	TEST(bw_truncf(999.9f), 999.f);
	TEST(bw_truncf(999.5f), 999.f);
	TEST(bw_truncf(999.1f), 999.f);
	TEST(bw_truncf(999.f), 999.f);
	TEST(bw_truncf(1.5f), 1.f);
	TEST(bw_truncf(1.f), 1.f);
	TEST(bw_truncf(0.9f), 0.f);
	TEST(bw_truncf(0.5f), 0.f);
	TEST(bw_truncf(0.1f), 0.f);
	TEST(bw_truncf(0.0f), 0.f);
	TEST(bw_truncf(-0.0f), 0.f);
	TEST(bw_truncf(-0.1f), 0.f);
	TEST(bw_truncf(-0.5f), 0.f);
	TEST(bw_truncf(-0.9f), 0.f);
	TEST(bw_truncf(-1.f), -1.f);
	TEST(bw_truncf(-1.5f), -1.f);
	TEST(bw_truncf(-999.f), -999.f);
	TEST(bw_truncf(-999.1f), -999.f);
	TEST(bw_truncf(-999.5f), -999.f);
	TEST(bw_truncf(-999.9f), -999.f);
	TEST(bw_truncf(-1000.0f), -1000.f);
	TEST(bw_truncf(-1000.1f), -1000.f);
	TEST(bw_truncf(-1000.5f), -1000.f);
	TEST(bw_truncf(-1000.9f), -1000.f);
	TEST(bw_truncf(-1001.f), -1001.f);
	TEST(bw_truncf(-1.234e38f), -1.234e38f);
	
	TEST(bw_roundf(1.234e38f), 1.234e38f);
	TEST(bw_roundf(1001.f), 1001.f);
	TEST(bw_roundf(1000.9f), 1001.f);
	TEST(bw_roundf(1000.5f), 1001.f);
	TEST(bw_roundf(1000.1f), 1000.f);
	TEST(bw_roundf(1000.0f), 1000.f);
	TEST(bw_roundf(999.9f), 1000.f);
	TEST(bw_roundf(999.5f), 1000.f);
	TEST(bw_roundf(999.1f), 999.f);
	TEST(bw_roundf(999.f), 999.f);
	TEST(bw_roundf(1.5f), 2.f);
	TEST(bw_roundf(1.f), 1.f);
	TEST(bw_roundf(0.9f), 1.f);
	TEST(bw_roundf(0.5f), 1.f);
	TEST(bw_roundf(0.1f), 0.f);
	TEST(bw_roundf(0.0f), 0.f);
	TEST(bw_roundf(-0.0f), 0.f);
	TEST(bw_roundf(-0.1f), 0.f);
	TEST(bw_roundf(-0.5f), -1.f);
	TEST(bw_roundf(-0.9f), -1.f);
	TEST(bw_roundf(-1.f), -1.f);
	TEST(bw_roundf(-1.5f), -2.f);
	TEST(bw_roundf(-999.f), -999.f);
	TEST(bw_roundf(-999.1f), -999.f);
	TEST(bw_roundf(-999.5f), -1000.f);
	TEST(bw_roundf(-999.9f), -1000.f);
	TEST(bw_roundf(-1000.0f), -1000.f);
	TEST(bw_roundf(-1000.1f), -1000.f);
	TEST(bw_roundf(-1000.5f), -1001.f);
	TEST(bw_roundf(-1000.9f), -1001.f);
	TEST(bw_roundf(-1001.f), -1001.f);
	TEST(bw_roundf(-1.234e38f), -1.234e38f);
	
	TEST(bw_floorf(1.234e38f), 1.234e38f);
	TEST(bw_floorf(1001.f), 1001.f);
	TEST(bw_floorf(1000.9f), 1000.f);
	TEST(bw_floorf(1000.5f), 1000.f);
	TEST(bw_floorf(1000.1f), 1000.f);
	TEST(bw_floorf(1000.0f), 1000.f);
	TEST(bw_floorf(999.9f), 999.f);
	TEST(bw_floorf(999.5f), 999.f);
	TEST(bw_floorf(999.1f), 999.f);
	TEST(bw_floorf(999.f), 999.f);
	TEST(bw_floorf(1.5f), 1.f);
	TEST(bw_floorf(1.f), 1.f);
	TEST(bw_floorf(0.9f), 0.f);
	TEST(bw_floorf(0.5f), 0.f);
	TEST(bw_floorf(0.1f), 0.f);
	TEST(bw_floorf(0.0f), 0.f);
	TEST(bw_floorf(-0.0f), 0.f);
	TEST(bw_floorf(-0.1f), -1.f);
	TEST(bw_floorf(-0.5f), -1.f);
	TEST(bw_floorf(-0.9f), -1.f);
	TEST(bw_floorf(-1.f), -1.f);
	TEST(bw_floorf(-1.5f), -2.f);
	TEST(bw_floorf(-999.f), -999.f);
	TEST(bw_floorf(-999.1f), -1000.f);
	TEST(bw_floorf(-999.5f), -1000.f);
	TEST(bw_floorf(-999.9f), -1000.f);
	TEST(bw_floorf(-1000.0f), -1000.f);
	TEST(bw_floorf(-1000.1f), -1001.f);
	TEST(bw_floorf(-1000.5f), -1001.f);
	TEST(bw_floorf(-1000.9f), -1001.f);
	TEST(bw_floorf(-1001.f), -1001.f);
	TEST(bw_floorf(-1.234e38f), -1.234e38f);
	
	TEST(bw_ceilf(1.234e38f), 1.234e38f);
	TEST(bw_ceilf(1001.f), 1001.f);
	TEST(bw_ceilf(1000.9f), 1001.f);
	TEST(bw_ceilf(1000.5f), 1001.f);
	TEST(bw_ceilf(1000.1f), 1001.f);
	TEST(bw_ceilf(1000.0f), 1000.f);
	TEST(bw_ceilf(999.9f), 1000.f);
	TEST(bw_ceilf(999.5f), 1000.f);
	TEST(bw_ceilf(999.1f), 1000.f);
	TEST(bw_ceilf(999.f), 999.f);
	TEST(bw_ceilf(1.5f), 2.f);
	TEST(bw_ceilf(1.f), 1.f);
	TEST(bw_ceilf(0.9f), 1.f);
	TEST(bw_ceilf(0.5f), 1.f);
	TEST(bw_ceilf(0.1f), 1.f);
	TEST(bw_ceilf(0.0f), 0.f);
	TEST(bw_ceilf(-0.0f), 0.f);
	TEST(bw_ceilf(-0.1f), 0.f);
	TEST(bw_ceilf(-0.5f), 0.f);
	TEST(bw_ceilf(-0.9f), 0.f);
	TEST(bw_ceilf(-1.f), -1.f);
	TEST(bw_ceilf(-1.5f), -1.f);
	TEST(bw_ceilf(-999.f), -999.f);
	TEST(bw_ceilf(-999.1f), -999.f);
	TEST(bw_ceilf(-999.5f), -999.f);
	TEST(bw_ceilf(-999.9f), -999.f);
	TEST(bw_ceilf(-1000.0f), -1000.f);
	TEST(bw_ceilf(-1000.1f), -1000.f);
	TEST(bw_ceilf(-1000.5f), -1000.f);
	TEST(bw_ceilf(-1000.9f), -1000.f);
	TEST(bw_ceilf(-1001.f), -1001.f);
	TEST(bw_ceilf(-1.234e38f), -1.234e38f);
	
	TEST_BW_INTFRAC(1.999e3f, 1.999e3f, 0.f);
	TEST_BW_INTFRAC(1.123e3f, 1.123e3f, 0.f);
	TEST_BW_INTFRAC(1.999f, 1.f, (1.999f - 1.f));
	TEST_BW_INTFRAC(1.123f, 1.f, (1.123f - 1.f));
	TEST_BW_INTFRAC(1.f, 1.f, 0.f);
	TEST_BW_INTFRAC(0.999f, 0.f, 0.999f);
	TEST_BW_INTFRAC(0.001f, 0.f, 0.001f);
	TEST_BW_INTFRAC(0.f, 0.f, 0.f);
	TEST_BW_INTFRAC(-0.001f, -1.f, 0.999f);
	TEST_BW_INTFRAC(-0.999f, -1.f, (1.f - 0.999f));
	TEST_BW_INTFRAC(-1.f, -1.f, 0.f);
	TEST_BW_INTFRAC(-1.123f, -2.f, (2.f - 1.123f));
	TEST_BW_INTFRAC(-1.999f, -2.f, (2.f - 1.999f));
	TEST_BW_INTFRAC(-1.123e3f, -1.123e3f, 0.f);
	TEST_BW_INTFRAC(-1.999e3f, -1.999e3f, 0.f);
	
	TEST_REL(bw_rcpf(1.2e27f), 8.333333333e-28f, 0.000013f);
	TEST_REL(bw_rcpf(1e20f), 1e-20f, 0.000013f);
	TEST_REL(bw_rcpf(1e10f), 1e-10f, 0.000013f);
	TEST_REL(bw_rcpf(1e3f), 0.001f, 0.000013f);
	TEST_REL(bw_rcpf(500.f), 0.002f, 0.000013f);
	TEST_REL(bw_rcpf(200.f), 0.005f, 0.000013f);
	TEST_REL(bw_rcpf(100.f), 0.01f, 0.000013f);
	TEST_REL(bw_rcpf(50.f), 0.02f, 0.000013f);
	TEST_REL(bw_rcpf(20.f), 0.05f, 0.000013f);
	TEST_REL(bw_rcpf(10.f), 0.1f, 0.000013f);
	TEST_REL(bw_rcpf(5.f), 0.2f, 0.000013f);
	TEST_REL(bw_rcpf(2.f), 0.5f, 0.000013f);
	TEST_REL(bw_rcpf(1.f), 1.f, 0.000013f);
	TEST_REL(bw_rcpf(0.5f), 2.f, 0.000013f);
	TEST_REL(bw_rcpf(0.2f), 5.f, 0.000013f);
	TEST_REL(bw_rcpf(0.1f), 10.f, 0.000013f);
	TEST_REL(bw_rcpf(0.05f), 20.f, 0.000013f);
	TEST_REL(bw_rcpf(0.02f), 50.f, 0.000013f);
	TEST_REL(bw_rcpf(0.01f), 100.f, 0.000013f);
	TEST_REL(bw_rcpf(0.005f), 200.f, 0.000013f);
	TEST_REL(bw_rcpf(0.002f), 500.f, 0.000013f);
	TEST_REL(bw_rcpf(0.001f), 1000.f, 0.000013f);
	TEST_REL(bw_rcpf(1e-10f), 1e10f, 0.000013f);
	TEST_REL(bw_rcpf(1e-20f), 1e20f, 0.000013f);
	TEST_REL(bw_rcpf(8.1e-28f), 1.234567901e27f, 0.000013f);
	TEST_REL(bw_rcpf(-1.2e27f), -8.333333333e-28f, 0.000013f);
	TEST_REL(bw_rcpf(-1e20f), -1e-20f, 0.000013f);
	TEST_REL(bw_rcpf(-1e10f), -1e-10f, 0.000013f);
	TEST_REL(bw_rcpf(-1e3f), -0.001f, 0.000013f);
	TEST_REL(bw_rcpf(-500.f), -0.002f, 0.000013f);
	TEST_REL(bw_rcpf(-200.f), -0.005f, 0.000013f);
	TEST_REL(bw_rcpf(-100.f), -0.01f, 0.000013f);
	TEST_REL(bw_rcpf(-50.f), -0.02f, 0.000013f);
	TEST_REL(bw_rcpf(-20.f), -0.05f, 0.000013f);
	TEST_REL(bw_rcpf(-10.f), -0.1f, 0.000013f);
	TEST_REL(bw_rcpf(-5.f), -0.2f, 0.000013f);
	TEST_REL(bw_rcpf(-2.f), -0.5f, 0.000013f);
	TEST_REL(bw_rcpf(-1.f), -1.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.5f), -2.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.2f), -5.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.1f), -10.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.05f), -20.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.02f), -50.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.01f), -100.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.005f), -200.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.002f), -500.f, 0.000013f);
	TEST_REL(bw_rcpf(-0.001f), -1000.f, 0.000013f);
	TEST_REL(bw_rcpf(-1e-10f), -1e10f, 0.000013f);
	TEST_REL(bw_rcpf(-1e-20f), -1e20f, 0.000013f);
	TEST_REL(bw_rcpf(-8.1e-28f), -1.234567901e27f, 0.000013f);
	
	TEST_ABS_REL(bw_sin2pif(0.f), sinf(6.283185307f * 0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.1f), sinf(6.283185307f * 0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.2f), sinf(6.283185307f * 0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.3f), sinf(6.283185307f * 0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.4f), sinf(6.283185307f * 0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.5f), sinf(6.283185307f * 0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.6f), sinf(6.283185307f * 0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.7f), sinf(6.283185307f * 0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.8f), sinf(6.283185307f * 0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(0.9f), sinf(6.283185307f * 0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(1.f), sinf(6.283185307f * 1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(5.678f), sinf(6.283185307f * 5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(56.78f), sinf(6.283185307f * 56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(567.8f), sinf(6.283185307f * 567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(5678.f), sinf(6.283185307f * 5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.f), sinf(6.283185307f * -0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.1f), sinf(6.283185307f * -0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.2f), sinf(6.283185307f * -0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.3f), sinf(6.283185307f * -0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.4f), sinf(6.283185307f * -0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.5f), sinf(6.283185307f * -0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.6f), sinf(6.283185307f * -0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.7f), sinf(6.283185307f * -0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.8f), sinf(6.283185307f * -0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-0.9f), sinf(6.283185307f * -0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-1.f), sinf(6.283185307f * -1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-5.678f), sinf(6.283185307f * -5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-56.78f), sinf(6.283185307f * -56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-567.8f), sinf(6.283185307f * -567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif(-5678.f), sinf(6.283185307f * -5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_sinf(0.f), sinf(0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(0.5f), sinf(0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(1.0f), sinf(1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(1.5f), sinf(1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(2.0f), sinf(2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(2.5f), sinf(2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(3.0f), sinf(3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(3.5f), sinf(3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(4.0f), sinf(4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(4.5f), sinf(4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(5.0f), sinf(5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(5.5f), sinf(5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(6.0f), sinf(6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(5.678f), sinf(5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(56.78f), sinf(56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(567.8f), sinf(567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(5678.f), sinf(5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-0.f), sinf(-0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-0.5f), sinf(-0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-1.0f), sinf(-1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-1.5f), sinf(-1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-2.0f), sinf(-2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-2.5f), sinf(-2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-3.0f), sinf(-3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-3.5f), sinf(-3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-4.0f), sinf(-4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-4.5f), sinf(-4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-5.0f), sinf(-5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-5.5f), sinf(-5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-6.0f), sinf(-6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-5.678f), sinf(-5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-56.78f), sinf(-56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-567.8f), sinf(-567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf(-5678.f), sinf(-5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_cos2pif(0.f), cosf(6.283185307f * 0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.1f), cosf(6.283185307f * 0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.2f), cosf(6.283185307f * 0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.3f), cosf(6.283185307f * 0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.4f), cosf(6.283185307f * 0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.5f), cosf(6.283185307f * 0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.6f), cosf(6.283185307f * 0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.7f), cosf(6.283185307f * 0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.8f), cosf(6.283185307f * 0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(0.9f), cosf(6.283185307f * 0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(1.f), cosf(6.283185307f * 1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(5.678f), cosf(6.283185307f * 5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(56.78f), cosf(6.283185307f * 56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(567.8f), cosf(6.283185307f * 567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(5678.f), cosf(6.283185307f * 5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.f), cosf(6.283185307f * -0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.1f), cosf(6.283185307f * -0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.2f), cosf(6.283185307f * -0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.3f), cosf(6.283185307f * -0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.4f), cosf(6.283185307f * -0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.5f), cosf(6.283185307f * -0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.6f), cosf(6.283185307f * -0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.7f), cosf(6.283185307f * -0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.8f), cosf(6.283185307f * -0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-0.9f), cosf(6.283185307f * -0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-1.f), cosf(6.283185307f * -1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-5.678f), cosf(6.283185307f * -5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-56.78f), cosf(6.283185307f * -56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-567.8f), cosf(6.283185307f * -567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif(-5678.f), cosf(6.283185307f * -5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_cosf(0.f), cosf(0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(0.5f), cosf(0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(1.0f), cosf(1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(1.5f), cosf(1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(2.0f), cosf(2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(2.5f), cosf(2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(3.0f), cosf(3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(3.5f), cosf(3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(4.0f), cosf(4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(4.5f), cosf(4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(5.0f), cosf(5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(5.5f), cosf(5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(6.0f), cosf(6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(5.678f), cosf(5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(56.78f), cosf(56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(567.8f), cosf(567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(5678.f), cosf(5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-0.f), cosf(-0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-0.5f), cosf(-0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-1.0f), cosf(-1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-1.5f), cosf(-1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-2.0f), cosf(-2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-2.5f), cosf(-2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-3.0f), cosf(-3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-3.5f), cosf(-3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-4.0f), cosf(-4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-4.5f), cosf(-4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-5.0f), cosf(-5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-5.5f), cosf(-5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-6.0f), cosf(-6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-5.678f), cosf(-5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-56.78f), cosf(-56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-567.8f), cosf(-567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf(-5678.f), cosf(-5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_tan2pif(0.f), tanf(6.283185307f * 0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.02f), tanf(6.283185307f * 0.02f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.04f), tanf(6.283185307f * 0.04f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.06f), tanf(6.283185307f * 0.06f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.08f), tanf(6.283185307f * 0.08f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.10f), tanf(6.283185307f * 0.10f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.12f), tanf(6.283185307f * 0.12f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.14f), tanf(6.283185307f * 0.14f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.16f), tanf(6.283185307f * 0.16f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.18f), tanf(6.283185307f * 0.18f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.20f), tanf(6.283185307f * 0.20f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.22f), tanf(6.283185307f * 0.22f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.24f), tanf(6.283185307f * 0.24f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(0.249f), tanf(6.283185307f * 0.249f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(5.678f), tanf(6.283185307f * 5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(56.78f), tanf(6.283185307f * 56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(567.8f), tanf(6.283185307f * 567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(5678.f), tanf(6.283185307f * 5678.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.f), tanf(6.283185307f * -0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.02f), tanf(6.283185307f * -0.02f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.04f), tanf(6.283185307f * -0.04f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.06f), tanf(6.283185307f * -0.06f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.08f), tanf(6.283185307f * -0.08f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.10f), tanf(6.283185307f * -0.10f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.12f), tanf(6.283185307f * -0.12f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.14f), tanf(6.283185307f * -0.14f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.16f), tanf(6.283185307f * -0.16f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.18f), tanf(6.283185307f * -0.18f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.20f), tanf(6.283185307f * -0.20f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.22f), tanf(6.283185307f * -0.22f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.24f), tanf(6.283185307f * -0.24f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-0.249f), tanf(6.283185307f * -0.249f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-5.678f), tanf(6.283185307f * -5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-56.78f), tanf(6.283185307f * -56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-567.8f), tanf(6.283185307f * -567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif(-5678.f), tanf(6.283185307f * -5678.f), 0.06f, 0.008);
	
	TEST_ABS_REL(bw_tanf(0.f), tanf(0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.1f), tanf(0.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.2f), tanf(0.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.3f), tanf(0.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.4f), tanf(0.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.5f), tanf(0.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.6f), tanf(0.6f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.7f), tanf(0.7f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.8f), tanf(0.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(0.9f), tanf(0.9f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.0f), tanf(1.0f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.1f), tanf(1.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.2f), tanf(1.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.3f), tanf(1.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.4f), tanf(1.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.5f), tanf(1.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(1.56f), tanf(1.56f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(5.678f), tanf(5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(56.78f), tanf(56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(567.8f), tanf(567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(5678.f), tanf(5678.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.f), tanf(-0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.1f), tanf(-0.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.2f), tanf(-0.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.3f), tanf(-0.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.4f), tanf(-0.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.5f), tanf(-0.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.6f), tanf(-0.6f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.7f), tanf(-0.7f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.8f), tanf(-0.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-0.9f), tanf(-0.9f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.0f), tanf(-1.0f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.1f), tanf(-1.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.2f), tanf(-1.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.3f), tanf(-1.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.4f), tanf(-1.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.5f), tanf(-1.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-1.56f), tanf(-1.56f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-5.678f), tanf(-5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-56.78f), tanf(-56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-567.8f), tanf(-567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf(-5678.f), tanf(-5678.f), 0.06f, 0.008);
	
	TEST_ABS_REL(bw_log2f(1.175494350822287e-38f), -126.f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.e-30f), -9.965784284662087e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.438449888287666e-27f), -8.916754359960814e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(2.069138081114798e-24f), -7.867724435259542e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(2.976351441631313e-21f), -6.818694510558271e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(4.281332398719396e-18f), -5.769664585856998e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(6.158482110660280e-15f), -4.720634661155725e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(8.858667904100795e-12f), -3.671604736454454e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.274274985703132e-08f), -2.622574811753181e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.832980710832437e-05f), -1.573544887051908e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(2.636650898730366e-02f), -5.245149623506357e+00f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.f), 0.f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.3f), 0.378511623253730f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(3.792690190732238e+01f), 5.245149623506357e+00f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(5.455594781168515e+04f), 1.573544887051908e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(7.847599703514622e+07f), 2.622574811753181e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.128837891684693e+11f), 3.671604736454454e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1.623776739188718e+14f), 4.720634661155725e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(2.335721469090121e+17f), 5.769664585856998e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(3.359818286283788e+20f), 6.818694510558271e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(4.832930238571732e+23f), 7.867724435259542e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(6.951927961775592e+26f), 8.916754359960814e+01f, 0.0055f, 0.012f);
	TEST_ABS_REL(bw_log2f(1e+30f), 9.965784284662087e+01f, 0.0055f, 0.012f);
	
	TEST_ABS_REL(bw_logf(1.175494350822287e-38f), -87.33654475055312f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.e-30f), -6.907755278982137e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.438449888287666e-27f), -6.180623144352438e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(2.069138081114798e-24f), -5.453491009722740e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(2.976351441631313e-21f), -4.726358875093041e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(4.281332398719396e-18f), -3.999226740463342e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(6.158482110660280e-15f), -3.272094605833644e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(8.858667904100795e-12f), -2.544962471203945e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.274274985703132e-08f), -1.817830336574247e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.832980710832437e-05f), -1.090698201944548e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(2.636650898730366e-02f), -3.635660673148490e+00f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.f), 0.f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.3f), 0.262364264467491f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(3.792690190732238e+01f), 3.635660673148490e+00f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(5.455594781168515e+04f), 1.090698201944548e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(7.847599703514622e+07f), 1.817830336574247e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.128837891684693e+11f), 2.544962471203945e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1.623776739188718e+14f), 3.272094605833644e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(2.335721469090121e+17f), 3.999226740463342e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(3.359818286283788e+20f), 4.726358875093041e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(4.832930238571732e+23f), 5.453491009722740e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(6.951927961775592e+26f), 6.180623144352438e+01f, 0.0038f, 0.012f);
	TEST_ABS_REL(bw_logf(1e+30f), 6.907755278982137e+01f, 0.0038f, 0.012f);
	
	TEST_ABS_REL(bw_log10f(1.175494350822287e-38f), -37.92977945366163f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.e-30f), -3.000000000000000e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.438449888287666e-27f), -2.684210526315789e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(2.069138081114798e-24f), -2.368421052631579e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(2.976351441631313e-21f), -2.052631578947368e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(4.281332398719396e-18f), -1.736842105263158e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(6.158482110660280e-15f), -1.421052631578947e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(8.858667904100795e-12f), -1.105263157894737e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.274274985703132e-08f), -7.894736842105264e+00f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.832980710832437e-05f), -4.736842105263158e+00f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(2.636650898730366e-02f), -1.578947368421051e+00f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.f), 0.f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.3f), 0.113943352306837f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(3.792690190732238e+01f), 1.578947368421051e+00f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(5.455594781168515e+04f), 4.736842105263158e+00f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(7.847599703514622e+07f), 7.894736842105264e+00f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.128837891684693e+11f), 1.105263157894737e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1.623776739188718e+14f), 1.421052631578947e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(2.335721469090121e+17f), 1.736842105263158e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(3.359818286283788e+20f), 2.052631578947368e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(4.832930238571732e+23f), 2.368421052631579e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(6.951927961775592e+26f), 2.684210526315789e+01f, 0.0017f, 0.012f);
	TEST_ABS_REL(bw_log10f(1e+30f), 3.000000000000000e+01f, 0.0017f, 0.012f);
	
	TEST(bw_pow2f(-INFINITY), 0.f);
	TEST(bw_pow2f(-127.f), 0.f);
	TEST_REL(bw_pow2f(-120.5f), 5.319680170962675e-37f, 0.00062f);
	TEST_REL(bw_pow2f(-100.5f), 5.578088954947358e-31f, 0.00062f);
	TEST_REL(bw_pow2f(-80.5f), 5.849050204022881e-25f, 0.00062f);
	TEST_REL(bw_pow2f(-60.5f), 6.133173666733497e-19f, 0.00062f);
	TEST_REL(bw_pow2f(-40.5f), 6.431098710768743e-13f, 0.00062f);
	TEST_REL(bw_pow2f(-20.5f), 6.743495761743046e-07f, 0.00062f);
	TEST_REL(bw_pow2f(0.f), 1.f, 0.00062f);
	TEST_REL(bw_pow2f(0.1f), 1.071773462536293f, 0.00062f);
	TEST_REL(bw_pow2f(0.2f), 1.148698354997035f, 0.00062f);
	TEST_REL(bw_pow2f(0.3f), 1.231144413344916f, 0.00062f);
	TEST_REL(bw_pow2f(0.4f), 1.319507910772894f, 0.00062f);
	TEST_REL(bw_pow2f(0.5f), 1.414213562373095f, 0.00062f);
	TEST_REL(bw_pow2f(0.6f), 1.515716566510398f, 0.00062f);
	TEST_REL(bw_pow2f(0.7f), 1.624504792712471f, 0.00062f);
	TEST_REL(bw_pow2f(0.8f), 1.741101126592248f, 0.00062f);
	TEST_REL(bw_pow2f(0.9f), 1.866065983073615f, 0.00062f);
	TEST_REL(bw_pow2f(1.f), 2.f, 0.00062f);
	TEST_REL(bw_pow2f(20.5f), 1482910.400378931f, 0.00062f);
	TEST_REL(bw_pow2f(40.5f), 1554944255987.738f, 0.00062f);
	TEST_REL(bw_pow2f(60.5f), 1.630477228166598e+18f, 0.00062f);
	TEST_REL(bw_pow2f(80.5f), 1.709679290002019e+24f, 0.00062f);
	TEST_REL(bw_pow2f(100.5f), 1.792728671193157e+30f, 0.00062f);
	TEST_REL(bw_pow2f(120.5f), 1.879812259125035e+36f, 0.00062f);
	
	TEST(bw_expf(-INFINITY), 0.f);
	TEST(bw_expf(-88.f), 0.f);
	TEST_REL(bw_expf(-80.5f), 1.094697702953142e-35f, 0.00062f);
	TEST_REL(bw_expf(-60.5f), 5.311092249679095e-27f, 0.00062f);
	TEST_REL(bw_expf(-40.5f), 2.576757109154981e-18f, 0.00062f);
	TEST_REL(bw_expf(-20.5f), 1.250152866386743e-09f, 0.00062f);
	TEST_REL(bw_expf(0.f), 1.f, 0.00062f);
	TEST_REL(bw_expf(0.1f), 1.105170918075648f, 0.00062f);
	TEST_REL(bw_expf(0.2f), 1.221402758160170f, 0.00062f);
	TEST_REL(bw_expf(0.3f), 1.349858807576003f, 0.00062f);
	TEST_REL(bw_expf(0.4f), 1.491824697641270f, 0.00062f);
	TEST_REL(bw_expf(0.5f), 1.648721270700128f, 0.00062f);
	TEST_REL(bw_expf(0.6f), 1.822118800390509f, 0.00062f);
	TEST_REL(bw_expf(0.7f), 2.013752707470477f, 0.00062f);
	TEST_REL(bw_expf(0.8f), 2.225540928492468f, 0.00062f);
	TEST_REL(bw_expf(0.9f), 2.459603111156950f, 0.00062f);
	TEST_REL(bw_expf(1.f), 2.718281828459045f, 0.00062f);
	TEST_REL(bw_expf(20.5f), 799902177.4755054f, 0.00062f);
	TEST_REL(bw_expf(40.5f), 3.880846962436204e+17f, 0.00062f);
	TEST_REL(bw_expf(60.5f), 1.882851874885851e+26f, 0.00062f);
	TEST_REL(bw_expf(80.5f), 9.134941978066843e+34f, 0.00062f);
	
	TEST(bw_pow10f(-INFINITY), 0.f);
	TEST(bw_pow10f(-38.f), 0.f);
	TEST_REL(bw_pow10f(-30.5f), 3.162277660168379e-31f, 0.00062f);
	TEST_REL(bw_pow10f(-20.5f), 3.162277660168379e-21f, 0.00062f);
	TEST_REL(bw_pow10f(-10.5f), 3.162277660168379e-11f, 0.00062f);
	TEST_REL(bw_pow10f(0.f), 1.f, 0.00062f);
	TEST_REL(bw_pow10f(0.1f), 1.258925411794167e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.2f), 1.584893192461114e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.3f), 1.995262314968880e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.4f), 2.511886431509580e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.5f), 3.162277660168380e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.6f), 3.981071705534972e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.7f), 5.011872336272722e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.8f), 6.309573444801933e+00f, 0.00062f);
	TEST_REL(bw_pow10f(0.9f), 7.943282347242816e+00f, 0.00062f);
	TEST_REL(bw_pow10f(1.f), 10.f, 0.00062f);
	TEST_REL(bw_pow10f(10.5f), 31622776601.68379f, 0.00062f);
	TEST_REL(bw_pow10f(20.5f), 3.162277660168379e+20f, 0.00062f);
	TEST_REL(bw_pow10f(30.5f), 3.162277660168380e+30f, 0.00062f);

	printf("\nsuceeded: %d, failed: %d\n\n", n_ok, n_ko);
	
	return n_ko ? EXIT_FAILURE : EXIT_SUCCESS;
}
