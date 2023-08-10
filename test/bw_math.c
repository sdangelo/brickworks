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
	float err_rel = fabsf(v_expected.f) < tol_abs ? 0.f : fabsf((v_expr.f - v_expected.f) / v_expected.f); \
	if (err_abs <= tol_abs && err_rel <= tol_rel) { \
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
	
	TEST_REL(bw_rcpf_2(1.2e27f), 8.333333333e-28f, 0.000013f);
	TEST_REL(bw_rcpf_2(1e20f), 1e-20f, 0.000013f);
	TEST_REL(bw_rcpf_2(1e10f), 1e-10f, 0.000013f);
	TEST_REL(bw_rcpf_2(1e3f), 0.001f, 0.000013f);
	TEST_REL(bw_rcpf_2(500.f), 0.002f, 0.000013f);
	TEST_REL(bw_rcpf_2(200.f), 0.005f, 0.000013f);
	TEST_REL(bw_rcpf_2(100.f), 0.01f, 0.000013f);
	TEST_REL(bw_rcpf_2(50.f), 0.02f, 0.000013f);
	TEST_REL(bw_rcpf_2(20.f), 0.05f, 0.000013f);
	TEST_REL(bw_rcpf_2(10.f), 0.1f, 0.000013f);
	TEST_REL(bw_rcpf_2(5.f), 0.2f, 0.000013f);
	TEST_REL(bw_rcpf_2(2.f), 0.5f, 0.000013f);
	TEST_REL(bw_rcpf_2(1.f), 1.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.5f), 2.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.2f), 5.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.1f), 10.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.05f), 20.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.02f), 50.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.01f), 100.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.005f), 200.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.002f), 500.f, 0.000013f);
	TEST_REL(bw_rcpf_2(0.001f), 1000.f, 0.000013f);
	TEST_REL(bw_rcpf_2(1e-10f), 1e10f, 0.000013f);
	TEST_REL(bw_rcpf_2(1e-20f), 1e20f, 0.000013f);
	TEST_REL(bw_rcpf_2(8.1e-28f), 1.234567901e27f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1.2e27f), -8.333333333e-28f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1e20f), -1e-20f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1e10f), -1e-10f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1e3f), -0.001f, 0.000013f);
	TEST_REL(bw_rcpf_2(-500.f), -0.002f, 0.000013f);
	TEST_REL(bw_rcpf_2(-200.f), -0.005f, 0.000013f);
	TEST_REL(bw_rcpf_2(-100.f), -0.01f, 0.000013f);
	TEST_REL(bw_rcpf_2(-50.f), -0.02f, 0.000013f);
	TEST_REL(bw_rcpf_2(-20.f), -0.05f, 0.000013f);
	TEST_REL(bw_rcpf_2(-10.f), -0.1f, 0.000013f);
	TEST_REL(bw_rcpf_2(-5.f), -0.2f, 0.000013f);
	TEST_REL(bw_rcpf_2(-2.f), -0.5f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1.f), -1.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.5f), -2.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.2f), -5.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.1f), -10.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.05f), -20.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.02f), -50.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.01f), -100.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.005f), -200.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.002f), -500.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-0.001f), -1000.f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1e-10f), -1e10f, 0.000013f);
	TEST_REL(bw_rcpf_2(-1e-20f), -1e20f, 0.000013f);
	TEST_REL(bw_rcpf_2(-8.1e-28f), -1.234567901e27f, 0.000013f);
	
	TEST_ABS_REL(bw_sin2pif_3(0.f), sinf(6.283185307f * 0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.1f), sinf(6.283185307f * 0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.2f), sinf(6.283185307f * 0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.3f), sinf(6.283185307f * 0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.4f), sinf(6.283185307f * 0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.5f), sinf(6.283185307f * 0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.6f), sinf(6.283185307f * 0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.7f), sinf(6.283185307f * 0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.8f), sinf(6.283185307f * 0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(0.9f), sinf(6.283185307f * 0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(1.f), sinf(6.283185307f * 1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(5.678f), sinf(6.283185307f * 5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(56.78f), sinf(6.283185307f * 56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(567.8f), sinf(6.283185307f * 567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(5678.f), sinf(6.283185307f * 5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.f), sinf(6.283185307f * -0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.1f), sinf(6.283185307f * -0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.2f), sinf(6.283185307f * -0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.3f), sinf(6.283185307f * -0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.4f), sinf(6.283185307f * -0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.5f), sinf(6.283185307f * -0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.6f), sinf(6.283185307f * -0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.7f), sinf(6.283185307f * -0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.8f), sinf(6.283185307f * -0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-0.9f), sinf(6.283185307f * -0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-1.f), sinf(6.283185307f * -1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-5.678f), sinf(6.283185307f * -5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-56.78f), sinf(6.283185307f * -56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-567.8f), sinf(6.283185307f * -567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sin2pif_3(-5678.f), sinf(6.283185307f * -5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_sinf_3(0.f), sinf(0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(0.5f), sinf(0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(1.0f), sinf(1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(1.5f), sinf(1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(2.0f), sinf(2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(2.5f), sinf(2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(3.0f), sinf(3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(3.5f), sinf(3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(4.0f), sinf(4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(4.5f), sinf(4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(5.0f), sinf(5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(5.5f), sinf(5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(6.0f), sinf(6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(5.678f), sinf(5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(56.78f), sinf(56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(567.8f), sinf(567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(5678.f), sinf(5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-0.f), sinf(-0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-0.5f), sinf(-0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-1.0f), sinf(-1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-1.5f), sinf(-1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-2.0f), sinf(-2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-2.5f), sinf(-2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-3.0f), sinf(-3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-3.5f), sinf(-3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-4.0f), sinf(-4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-4.5f), sinf(-4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-5.0f), sinf(-5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-5.5f), sinf(-5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-6.0f), sinf(-6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-5.678f), sinf(-5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-56.78f), sinf(-56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-567.8f), sinf(-567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_sinf_3(-5678.f), sinf(-5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_cos2pif_3(0.f), cosf(6.283185307f * 0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.1f), cosf(6.283185307f * 0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.2f), cosf(6.283185307f * 0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.3f), cosf(6.283185307f * 0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.4f), cosf(6.283185307f * 0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.5f), cosf(6.283185307f * 0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.6f), cosf(6.283185307f * 0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.7f), cosf(6.283185307f * 0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.8f), cosf(6.283185307f * 0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(0.9f), cosf(6.283185307f * 0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(1.f), cosf(6.283185307f * 1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(5.678f), cosf(6.283185307f * 5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(56.78f), cosf(6.283185307f * 56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(567.8f), cosf(6.283185307f * 567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(5678.f), cosf(6.283185307f * 5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.f), cosf(6.283185307f * -0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.1f), cosf(6.283185307f * -0.1f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.2f), cosf(6.283185307f * -0.2f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.3f), cosf(6.283185307f * -0.3f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.4f), cosf(6.283185307f * -0.4f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.5f), cosf(6.283185307f * -0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.6f), cosf(6.283185307f * -0.6f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.7f), cosf(6.283185307f * -0.7f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.8f), cosf(6.283185307f * -0.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-0.9f), cosf(6.283185307f * -0.9f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-1.f), cosf(6.283185307f * -1.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-5.678f), cosf(6.283185307f * -5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-56.78f), cosf(6.283185307f * -56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-567.8f), cosf(6.283185307f * -567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cos2pif_3(-5678.f), cosf(6.283185307f * -5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_cosf_3(0.f), cosf(0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(0.5f), cosf(0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(1.0f), cosf(1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(1.5f), cosf(1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(2.0f), cosf(2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(2.5f), cosf(2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(3.0f), cosf(3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(3.5f), cosf(3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(4.0f), cosf(4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(4.5f), cosf(4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(5.0f), cosf(5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(5.5f), cosf(5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(6.0f), cosf(6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(5.678f), cosf(5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(56.78f), cosf(56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(567.8f), cosf(567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(5678.f), cosf(5678.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-0.f), cosf(-0.f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-0.5f), cosf(-0.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-1.0f), cosf(-1.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-1.5f), cosf(-1.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-2.0f), cosf(-2.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-2.5f), cosf(-2.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-3.0f), cosf(-3.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-3.5f), cosf(-3.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-4.0f), cosf(-4.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-4.5f), cosf(-4.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-5.0f), cosf(-5.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-5.5f), cosf(-5.5f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-6.0f), cosf(-6.0f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-5.678f), cosf(-5.678f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-56.78f), cosf(-56.78f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-567.8f), cosf(-567.8f), 0.011f, 0.017);
	TEST_ABS_REL(bw_cosf_3(-5678.f), cosf(-5678.f), 0.011f, 0.017);
	
	TEST_ABS_REL(bw_tan2pif_3(0.f), tanf(6.283185307f * 0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.02f), tanf(6.283185307f * 0.02f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.04f), tanf(6.283185307f * 0.04f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.06f), tanf(6.283185307f * 0.06f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.08f), tanf(6.283185307f * 0.08f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.10f), tanf(6.283185307f * 0.10f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.12f), tanf(6.283185307f * 0.12f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.14f), tanf(6.283185307f * 0.14f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.16f), tanf(6.283185307f * 0.16f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.18f), tanf(6.283185307f * 0.18f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.20f), tanf(6.283185307f * 0.20f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.22f), tanf(6.283185307f * 0.22f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.24f), tanf(6.283185307f * 0.24f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(0.249f), tanf(6.283185307f * 0.249f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(5.678f), tanf(6.283185307f * 5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(56.78f), tanf(6.283185307f * 56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(567.8f), tanf(6.283185307f * 567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(5678.f), tanf(6.283185307f * 5678.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.f), tanf(6.283185307f * -0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.02f), tanf(6.283185307f * -0.02f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.04f), tanf(6.283185307f * -0.04f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.06f), tanf(6.283185307f * -0.06f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.08f), tanf(6.283185307f * -0.08f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.10f), tanf(6.283185307f * -0.10f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.12f), tanf(6.283185307f * -0.12f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.14f), tanf(6.283185307f * -0.14f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.16f), tanf(6.283185307f * -0.16f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.18f), tanf(6.283185307f * -0.18f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.20f), tanf(6.283185307f * -0.20f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.22f), tanf(6.283185307f * -0.22f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.24f), tanf(6.283185307f * -0.24f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-0.249f), tanf(6.283185307f * -0.249f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-5.678f), tanf(6.283185307f * -5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-56.78f), tanf(6.283185307f * -56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-567.8f), tanf(6.283185307f * -567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tan2pif_3(-5678.f), tanf(6.283185307f * -5678.f), 0.06f, 0.008);
	
	TEST_ABS_REL(bw_tanf_3(0.f), tanf(0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.1f), tanf(0.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.2f), tanf(0.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.3f), tanf(0.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.4f), tanf(0.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.5f), tanf(0.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.6f), tanf(0.6f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.7f), tanf(0.7f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.8f), tanf(0.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(0.9f), tanf(0.9f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.0f), tanf(1.0f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.1f), tanf(1.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.2f), tanf(1.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.3f), tanf(1.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.4f), tanf(1.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.5f), tanf(1.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(1.56f), tanf(1.56f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(5.678f), tanf(5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(56.78f), tanf(56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(567.8f), tanf(567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(5678.f), tanf(5678.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.f), tanf(-0.f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.1f), tanf(-0.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.2f), tanf(-0.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.3f), tanf(-0.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.4f), tanf(-0.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.5f), tanf(-0.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.6f), tanf(-0.6f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.7f), tanf(-0.7f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.8f), tanf(-0.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-0.9f), tanf(-0.9f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.0f), tanf(-1.0f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.1f), tanf(-1.1f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.2f), tanf(-1.2f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.3f), tanf(-1.3f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.4f), tanf(-1.4f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.5f), tanf(-1.5f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-1.56f), tanf(-1.56f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-5.678f), tanf(-5.678f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-56.78f), tanf(-56.78f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-567.8f), tanf(-567.8f), 0.06f, 0.008);
	TEST_ABS_REL(bw_tanf_3(-5678.f), tanf(-5678.f), 0.06f, 0.008);

	printf("\nsuceeded: %d, failed: %d\n\n", n_ok, n_ko);
	
	return n_ko ? EXIT_FAILURE : EXIT_SUCCESS;
}
