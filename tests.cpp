// Unit tests for Vector_v0 (ex00: add, sub, scl, getSize, operator<<).
//
// Build & run:
//     c++ -std=c++17 -Wall -Wextra -Werror tests.cpp -o tests && ./tests
//
// No external framework (42 usually forbids them): a tiny CHECK macro does the job.
// Each TEST is a function; main() runs them all and returns 1 if anything failed,
// so the exit code can be used by a script / Makefile later.

// --- Temporary trick to reuse main.cpp without its main() ---
// main.cpp has its own main(), and a program can only have one.
// We rename it while including the file. The clean fix, later: move the class
// into a header (e.g. Vector.hpp) and #include that from both main.cpp and tests.cpp.
#define main main_cpp_unused
#include "main.cpp"
#undef main

#include <sstream>
#include <string>
#include <cmath>

// ---------------------------------------------------------------------------
// Mini test framework
// ---------------------------------------------------------------------------
static int g_checks = 0;
static int g_failures = 0;

// CHECK(cond): if cond is false, print where and what, but keep going.
#define CHECK(cond)                                                          \
	do {                                                                     \
		++g_checks;                                                          \
		if (!(cond)) {                                                       \
			++g_failures;                                                    \
			std::cerr << "  FAIL " << __FILE__ << ":" << __LINE__            \
			          << "  " #cond << std::endl;                            \
		}                                                                    \
	} while (0)

// Floats: never compare with == after arithmetic (0.1f * 3 != 0.3f).
// Compare with a small tolerance instead.
static bool nearly_equal(double a, double b, double eps = 1e-5) {
	return std::fabs(a - b) <= eps;
}

// Compares a whole vector against expected values, element by element.
// `expected` is initializer_list<double> (not <T>) on purpose: with <T>, the
// compiler would try to deduce T from BOTH arguments, and {7, 10} (ints)
// would conflict with Vector_v0<float> -> "deduced conflicting types".
template <typename T>
static bool vec_equals(const Vector_v0<T> &v, std::initializer_list<double> expected) {
	if (v.getSize() != expected.size())
		return false;
	size_t i = 0;
	for (double e : expected) {
		if (!nearly_equal(static_cast<double>(v.container[i]), e))
			return false;
		++i;
	}
	return true;
}

#define RUN(test)                                                            \
	do {                                                                     \
		int before = g_failures;                                             \
		test();                                                              \
		std::cerr << (g_failures == before ? "[ OK ] " : "[FAIL] ")          \
		          << #test << std::endl;                                     \
	} while (0)

// ---------------------------------------------------------------------------
// getSize
// ---------------------------------------------------------------------------
static void test_getSize_returns_number_of_elements() {
	Vector_v0<float> v{1, 2, 3};
	CHECK(v.getSize() == 3);
}

static void test_getSize_of_empty_vector_is_zero() {
	Vector_v0<float> v{};
	CHECK(v.getSize() == 0);
}

// ---------------------------------------------------------------------------
// add  (subject example: [2, 3] + [5, 7] = [7, 10])
// ---------------------------------------------------------------------------
static void test_add_subject_example() {
	Vector_v0<float> u{2, 3};
	Vector_v0<float> v{5, 7};
	u.add(v);
	CHECK(vec_equals(u, {7, 10}));
}

static void test_add_does_not_modify_argument() {
	Vector_v0<float> u{2, 3};
	Vector_v0<float> v{5, 7};
	u.add(v);
	CHECK(vec_equals(v, {5, 7}));   // v was passed by const& -> must be unchanged
}

static void test_add_zero_vector_is_identity() {
	Vector_v0<float> u{2, 3};
	Vector_v0<float> zero{0, 0};
	u.add(zero);
	CHECK(vec_equals(u, {2, 3}));
}

static void test_add_negative_values() {
	Vector_v0<float> u{1, -2};
	Vector_v0<float> v{-1, -3};
	u.add(v);
	CHECK(vec_equals(u, {0, -5}));
}

static void test_add_to_itself() {
	// Aliasing: the argument IS the object. Each element is read then written,
	// so this should still give 2 * u.
	Vector_v0<float> u{2, 3};
	u.add(u);
	CHECK(vec_equals(u, {4, 6}));
}

static void test_add_different_sizes_leaves_vector_unchanged() {
	// Subject: "undefined". This test documents YOUR choice (silent return).
	// If you switch to assert/throw, change or remove this test.
	Vector_v0<float> u{1, 2, 3};
	Vector_v0<float> v{1, 2};
	u.add(v);
	CHECK(vec_equals(u, {1, 2, 3}));
}

// ---------------------------------------------------------------------------
// sub  (subject example: [2, 3] - [5, 7] = [-3, -4])
// ---------------------------------------------------------------------------
static void test_sub_subject_example() {
	Vector_v0<float> u{2, 3};
	Vector_v0<float> v{5, 7};
	u.sub(v);
	CHECK(vec_equals(u, {-3, -4}));
}

static void test_sub_itself_gives_zero() {
	Vector_v0<float> u{2, 3};
	u.sub(u);
	CHECK(vec_equals(u, {0, 0}));
}

static void test_sub_is_inverse_of_add() {
	Vector_v0<float> u{2, 3};
	Vector_v0<float> v{5, 7};
	u.add(v);
	u.sub(v);
	CHECK(vec_equals(u, {2, 3}));
}

// ---------------------------------------------------------------------------
// scl  (subject example: [2, 3] * 2 = [4, 6])
// ---------------------------------------------------------------------------
static void test_scl_subject_example() {
	Vector_v0<float> u{2, 3};
	u.scl(2.f);
	CHECK(vec_equals(u, {4, 6}));
}

static void test_scl_by_one_is_identity() {
	Vector_v0<float> u{2, 3};
	u.scl(1.f);
	CHECK(vec_equals(u, {2, 3}));
}

static void test_scl_by_zero_gives_zero_vector() {
	Vector_v0<float> u{2, 3};
	u.scl(0.f);
	CHECK(vec_equals(u, {0, 0}));
}

static void test_scl_by_fraction() {
	Vector_v0<float> u{1, 3};
	u.scl(0.5f);
	CHECK(vec_equals(u, {0.5f, 1.5f}));
}

static void test_scl_accepts_int_argument() {
	// int -> float implicit conversion (see our discussion on scl's type).
	Vector_v0<float> u{2, 3};
	u.scl(3);
	CHECK(vec_equals(u, {6, 9}));
}

static void test_scl_on_int_vector_truncates() {
	// Not a bug in scl: a Vector_v0<int> cannot hold 0.5.
	// 0.5 is converted to int (0) before the call.
	Vector_v0<int> u{1, 3};
	u.scl(static_cast<int>(0.5));
	CHECK(vec_equals(u, {0, 0}));
}

// ---------------------------------------------------------------------------
// operator<<  (format: one "[x]" per line)
// ---------------------------------------------------------------------------
static void test_print_format() {
	Vector_v0<float> u{7, 10};
	std::ostringstream out;    // a stream that writes into a string -> testable
	out << u;
	// Note: the subject shows [7.0]. cout prints a float 7 as "7".
	// If you add std::fixed / setprecision, update this expected string.
	CHECK(out.str() == "[7]\n[10]\n");
}

static void test_print_empty_vector_prints_nothing() {
	Vector_v0<float> u{};
	std::ostringstream out;
	out << u;
	CHECK(out.str().empty());
}

static void test_print_returns_stream_for_chaining() {
	Vector_v0<float> u{1};
	std::ostringstream out;
	out << u << "end";
	CHECK(out.str() == "[1]\nend");
}

// ---------------------------------------------------------------------------
int main() {
	RUN(test_getSize_returns_number_of_elements);
	RUN(test_getSize_of_empty_vector_is_zero);

	RUN(test_add_subject_example);
	RUN(test_add_does_not_modify_argument);
	RUN(test_add_zero_vector_is_identity);
	RUN(test_add_negative_values);
	RUN(test_add_to_itself);
	RUN(test_add_different_sizes_leaves_vector_unchanged);

	RUN(test_sub_subject_example);
	RUN(test_sub_itself_gives_zero);
	RUN(test_sub_is_inverse_of_add);

	RUN(test_scl_subject_example);
	RUN(test_scl_by_one_is_identity);
	RUN(test_scl_by_zero_gives_zero_vector);
	RUN(test_scl_by_fraction);
	RUN(test_scl_accepts_int_argument);
	RUN(test_scl_on_int_vector_truncates);

	RUN(test_print_format);
	RUN(test_print_empty_vector_prints_nothing);
	RUN(test_print_returns_stream_for_chaining);

	std::cerr << std::endl << (g_checks - g_failures) << "/" << g_checks
	          << " checks passed" << std::endl;
	return g_failures == 0 ? 0 : 1;
}
