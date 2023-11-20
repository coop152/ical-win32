/* Copyright (c) 1995 by Sanjay Ghemawat */
#ifndef _ARRAYH
#define _ARRAYH

/*
 * Generic resizable array.
 */

#include <assert.h>

template <typename T>
class Array {
public:

	/* Constructors */
	Array();                        /* Empty array */
	Array(int predict);             /* Empty array with size predict */
	Array(T const*, int);     /* Initialized with C array */
	Array(Array const&);        /* Initialized with another Array */
	Array(T, int);            /* Fill with n copies of element */

	/* Destructor */
	~Array();

	/* Assignment operator */
	Array& operator=(Array const&);

	/* Array indexing */
	T& operator[](int index) const;
	T& slot(int index) const;

	/* Other Array operations */
	int  size() const;                  /* Return size; */
	T& high() const;              /* Return last element */
	T* as_pointer() const;        /* Return as pointer to base */
	void append(T v);             /* append an element */
	void append(T, int n);        /* Append n copies of element */
	void concat(T const*, int);   /* Concatenate C array */
	void concat(Array const&);      /* Concatenate another Array */
	void fill(T, int start, int n); /* Fill specified region with Elem */
	T remove();                   /* Remove and return last element */
	void remove(int num);               /* Remove last num elements */
	void clear();                       /* Remove all elements */

	/* Storage stuff */
	void reclaim();                     /* Reclaim all unused space */
private:
	T* store_;                 /* Actual storage */
	int         alloc_;                 /* Size of allocated storage */
	int         size_;                  /* Size of used storage */

	/* Storage enlargers */
	void enlarge_allocation_to(int s);  /* Enlarge to s */
	void enlarge_to(int s);             /* Enlarge to s if necessary */

	/* Various checkers */
	void bound_check(int) const;
	void range_check(int) const;
	void size_check(int) const;
};

template <typename T>
inline void Array<T>::bound_check(int val) const {
	assert(val >= 0);
	assert(val < size_);
}

template <typename T>
inline void Array<T>::range_check(int val) const {
	assert(val >= 0);
}

template <typename T>
inline void Array<T>::size_check(int val) const {
	assert(val >= 0);
	assert(val <= size_);
}

template <typename T>
inline Array<T>::Array() {
	alloc_ = 0;
	size_ = 0;
	store_ = nullptr;
}

template <typename T>
inline int Array<T>::size() const {
	return size_;
}

template <typename T>
inline T& Array<T>::operator[](int index) const {
	bound_check(index);
	return store_[index];
}

template <typename T>
inline T& Array<T>::slot(int index) const {
	bound_check(index);
	return store_[index];
}

template <typename T>
inline T& Array<T>::high() const {
	bound_check(size_ - 1);
	return store_[size_ - 1];
}

template <typename T>
inline T* Array<T>::as_pointer() const {
	return store_;
}

template <typename T>
inline void Array<T>::append(T v) {
	if (size_ >= alloc_)
		enlarge_allocation_to(size_ + 1);
	store_[size_++] = v;
}

template <typename T>
inline T Array<T>::remove() {
	if (size_ > 0) size_--;
	return store_[size_];
}

template <typename T>
inline void Array<T>::remove(int num) {
	size_check(num);
	size_ -= num;
}

template <typename T>
inline void Array<T>::clear() {
	size_ = 0;
}


template <typename T>
Array<T>::Array(int predict) {
	range_check(predict);
	alloc_ = 0;
	size_ = 0;
	store_ = nullptr;
	enlarge_to(predict);
	size_ = 0;
}

template <typename T>
Array<T>::~Array() {
	if (alloc_ > 0) delete[] store_;
}

template <typename T>
Array<T>::Array(T const* src, int s) {
	range_check(s);
	alloc_ = 0;
	size_ = 0;
	store_ = 0;
	enlarge_to(s);
	for (int i = 0; i < s; i++)
		store_[i] = src[i];
}

template <typename T>
Array<T>::Array(Array const& d) {
	alloc_ = 0;
	size_ = 0;
	store_ = 0;
	enlarge_to(d.size_);
	for (int i = 0; i < size_; i++)
		store_[i] = d.store_[i];
}

template <typename T>
Array<T>::Array(T element, int numberOfCopies) {
	range_check(numberOfCopies);
	alloc_ = 0;
	size_ = 0;
	store_ = 0;
	enlarge_to(numberOfCopies);
	for (int i = 0; i < size_; i++)
		store_[i] = element;
}

template <typename T>
Array<T>& Array<T>::operator=(Array const& d) {
	size_ = 0;
	enlarge_to(d.size_);
	for (int i = 0; i < size_; i++)
		store_[i] = d.store_[i];
	return (*this);
}

template <typename T>
void Array<T>::append(T element, int n) {
	range_check(n);
	int oldsize = size_;
	enlarge_to(size_ + n);
	for (int i = 0; i < n; i++)
		store_[i + oldsize] = element;
}

template <typename T>
void Array<T>::concat(Array const& d) {
	int oldsize = size_;
	enlarge_to(size_ + d.size_);
	for (int i = 0; i < d.size_; i++)
		store_[i + oldsize] = d.store_[i];
}

template <typename T>
void Array<T>::concat(T const* src, int s) {
	range_check(s);
	int oldsize = size_;
	enlarge_to(s + size_);
	for (int i = 0; i < s; i++)
		store_[i + oldsize] = src[i];
}

template <typename T>
void Array<T>::fill(T element, int start, int count) {
	bound_check(start);
	range_check(count);
	bound_check(start + count - 1);
	for (int i = start; i < start + count; i++)
		store_[i] = element;
}

template <typename T>
void Array<T>::enlarge_to(int newsize) {
	if (newsize > alloc_)
		enlarge_allocation_to(newsize);
	size_ = newsize;
}

template <typename T>
void Array<T>::enlarge_allocation_to(int newsize) {
	int newalloc = alloc_ * 2;
	if (newsize > newalloc) newalloc = newsize;

	T* oldstore = store_;
	store_ = new T[newalloc];

	for (int i = 0; i < size_; i++)
		store_[i] = oldstore[i];

	if (alloc_ > 0) delete[] oldstore;
	alloc_ = newalloc;
}

template <typename T>
void Array<T>::reclaim() {
	if (alloc_ > size_) {
		/* Some free entries that can be reclaimed */
		if (size_ > 0) {
			/* Array not empty - create new store */
			T* newstore = new T[size_];
			for (int i = 0; i < size_; i++)
				newstore[i] = store_[i];
			delete[] store_;
			alloc_ = size_;
			store_ = newstore;
		}
		else {
			/* Array empty - delete old store */
			if (alloc_ > 0) {
				delete[] store_;
				alloc_ = 0;
			}
		}
	}
}


#endif /*_ARRAYH */
