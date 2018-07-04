#ifndef _ISTL_ALLOCATOR_H_
#define _ISTL_ALLOCATOR_H_ 1

#include<new>

namespace istd {

using std::size_t;
using std::ptrdiff_t;
using std::align_val_t;
using std::true_type;
using std::bad_alloc;

template<typename T>
class allocator {
public:
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef T			value_type;

	template<typename T1>
	struct rebind {
		typedef allocator<T1> other;
	};

	typedef true_type propagate_on_container_move_assignment;
	typedef true_type is_always_equal;

	allocator() throw() {}
	allocator(const allocator&) throw() {}
	template<typename T1>
	allocator(const allocator<T1>&) throw() {}
	~allocator() throw() {}

	pointer allocate(size_type n, const void* = static_cast<const void*>(0)) {
		if (n > this->max_size())
			throw bad_alloc();
		if (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
			align_val_t al = align_val_t(alignof(T));
			return static_cast<T*>(::operator new(n * sizeof(T), al));
		}
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	void deallocate(pointer p, size_type) {
		if (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
			::operator delete(p, align_val_t(alignof(T)));
		}
		::operator delete(p);
	}

	size_type max_size() const noexcept {
		return size_t(-1) / sizeof(T);
	}

	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
	}

	template<typename U>
	void destroy(U* p) {
		p->~U();
	}
}; // end class allocator

template<typename T1, typename T2>
inline bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
	return true;
}

template<typename T>
inline bool operator==(const allocator<T>&, const allocator<T>&) noexcept {
	return true;
}

template<typename T1, typename T2>
inline bool operator!=(const allocator<T1>&, const allocator<T2>&) noexcept {
	return false;
}

template<typename T>
inline bool operator!=(const allocator<T>&, const allocator<T>&) noexcept {
	return false;
}

template<>
class allocator<void> {
public:
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef void*			pointer;
	typedef const void*	const_pointer;
	typedef void			value_type;

	template<typename T1>
	struct rebind {
		typedef allocator<T1> other;
	};

	typedef true_type propagate_on_container_move_assignment;
	typedef true_type is_always_equal;

	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
	}

	template<typename U>
	void destroy(U* p) {
		p->~U();
	}
}; // end class allocator<void>

} // end namespace

#endif