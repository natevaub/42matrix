#include <vector>
#include <iostream>


// • A function to return the size/shape of a vector/matrix.

// • A function to tell if a matrix is square.

// • A function to print a vector/matrix on the standard output.

// • A function to reshape a vector into a matrix, and vice-versa.
template <typename T>
class Vector_v0 {

	public:
		std::vector<T> container;

		Vector_v0(std::initializer_list<T> values) : container(values) {
			std::cout << "Vector constructed" << std::endl;
		}

		// void: Rust prototype has no `-> Type`, the result goes into *this.
		void add(const Vector_v0 &arg) {
			// this->getSize(): `this` is a pointer -> use `->` (or just getSize()).
			if (this->getSize() != arg.getSize()) {
				return; // in a void function: bare `return;` (no value) exits early.
			}

			for (size_t i = 0; i < this->getSize(); i++) {
				this->container[i] = this->container[i] + arg.container[i]; 
			}

			// TODO: the addition loop (your turn)
		}

		void sub(const Vector_v0 &arg) {
			// this->getSize(): `this` is a pointer -> use `->` (or just getSize()).
			if (this->getSize() != arg.getSize()) {
				return; // in a void function: bare `return;` (no value) exits early.
			}

			for (size_t i = 0; i < this->getSize(); i++) {
				this->container[i] = this->container[i] - arg.container[i]; 
			}

			// TODO: the addition loop (your turn)
		}

		void scl(const T sc) {

			for (size_t i = 0; i < this->getSize(); i++) {
				this->container[i] = this->container[i] * sc; 
			}

			// TODO: the addition loop (your turn)
		}



		// Fix 1: `const` moved AFTER the () -> "this function doesn't modify the object".
		//        (before, it was on the return type, where it does nothing).
		// Fix 5: size_t instead of int -> same (unsigned) type as std::vector::size().
		// size_t getVector_v0Size() const {

		// 	// Fix 2: `typename` -> inside a template, tells the compiler that
		// 	//        vector<T>::const_iterator is a TYPE (dependent name).
		// 	//        const_iterator (not iterator) because the function is const.
		// 	typename vector<T>::const_iterator it;
		// 	size_t size = 0;

		// 	// Fix 3: this->container.begin() -> `this` is a pointer (so ->), and
		// 	//        begin()/end() belong to the member `container`, not to Vector_v0.
		// 	// Fix 4: removed the extra `;` after ++it (a for header has exactly 2).
		// 	for (it = this->container.begin(); it != this->container.end(); ++it) {
		// 		size++;
		// 	}

		// 	return size;
		// 	// Note: this is O(n). container.size() gives the same answer in O(1).
		// }

		void printVector_v0() const {

			for (size_t i = 0; i < this->getSize(); i++) {
				std::cout << this->container[i] << std::endl;
			}

		}

		// Vector_v0 sub(Vector_v0 this->container, Vector_v0 &arg) {

		// }

		// impl<K> Vector<K> {

		// 	fn add(&mut self, v: &Vector<K>);
			
		// 	fn sub(&mut self, v: &Vector<K>);
			
		// 	fn scl(&mut self, a: K);
			
		// 	}

		// --- C++98 way: iterator-range constructor ---
		// C++98 has no initializer_list, so the "classic" way to build from
		// a list of values is: put them in a plain array, then pass a
		// [first, last) range. This is exactly what std::vector does itself
		// (see cppreference std::vector::vector, overload "InputIt first, InputIt last").
		//
		// Usage (C++98):
		//     float arr[] = {1.f, 2.f, 3.f};
		//     Vector_v0<float> v(arr, arr + 3);   // arr + 3 = one past the end
		//
		// Why a template on the iterator type (InputIt) instead of `const T*`?
		//   -> it accepts ANY range: plain arrays (pointers are iterators),
		//      std::vector, std::list, ... e.g. Vector_v0<float> v(sv.begin(), sv.end());
		//
		// Why ": container(first, last)" in the init list instead of push_back in a loop?
		//   -> std::vector already has this constructor; we just forward the range.
		//      Member initializer list = the member is built directly, not
		//      default-built then filled.
		//
		// Gotcha: if you later add a (size, value) constructor like
		// Vector_v0(size_t n, const T& val), a call such as Vector_v0<int> v(3, 1)
		// will pick THIS template (InputIt = int) and break. std::vector solves
		// that with SFINAE / enable_if — a good thing to look up when you get there.

	size_t getSize() const {
		return container.size();
	}

};

template <typename T>
std::ostream &operator<<(std::ostream &out, const Vector_v0<T> &Vector) {
	for (size_t i = 0; i < Vector.getSize(); i++) {
		out << "[" << Vector.container[i] << "]" << std::endl;
	}
	return out;
}


int main()
{
	// Value initialization (uses default constructor)
	// std::vector<int> empty{}; // vector containing 0 int elements

	Vector_v0<float> a{1, 2, 3};
	Vector_v0<float> b= {1, 0, 0};
	a.add(b);
	a.printVector_v0();


	return 0;
}
