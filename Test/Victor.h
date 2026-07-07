#pragma once
#include <iostream>
#include <limits>
#include <stdexcept>

template<typename type>
class VictorIterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = type;
	using difference_type = std::ptrdiff_t;
	using pointer = type*;
	using reference = type&;

	VictorIterator(pointer data); // r

	reference operator*(); // r
	reference operator*() const; // r
	pointer operator->(); // r
	pointer operator->() const; // r

	VictorIterator& operator++(); // r
	VictorIterator operator++(int); // r
	VictorIterator& operator--(); // r
	VictorIterator operator--(int); // r

	VictorIterator& operator+=(difference_type); // r
	VictorIterator& operator-=(difference_type); // r

	friend VictorIterator operator+(VictorIterator f, difference_type n) {
		f += n;
		return f;
	}

	friend VictorIterator operator+(difference_type n, VictorIterator f) {
		f += n;
		return f;
	}

	friend VictorIterator operator-(VictorIterator f, difference_type n) {
		f -= n;
		return f;
	}

	difference_type operator-(const VictorIterator&) const; // r

	reference operator[](difference_type) const; // r

	bool operator==(const VictorIterator&) const; // r
	bool operator!=(const VictorIterator&) const; // r
	bool operator<(const VictorIterator&) const; // r
	bool operator>(const VictorIterator&) const; // r
	bool operator<=(const VictorIterator&) const; // r
	bool operator>=(const VictorIterator&) const; // r


private:
	pointer raw_ptr;

};


template<typename type>
class Victor {
public:
	using iterator = VictorIterator<type>;
	using const_iterator = const VictorIterator<type>;

	iterator begin();
	iterator end();
	const_iterator cbegin();
	const_iterator cend();


	Victor(); // r
	Victor(const Victor& other); // r
	Victor(size_t size); // r
	Victor(Victor&& other); // r


	~Victor(); // r

	void push_back(type&& element); // r
	void push_back(const type& element); // r
	void pop_back(); // r
	size_t capacity() const; // r
	size_t size() const; // r
	size_t max_size() const; // r

	void resize(size_t n, type value = type());
	bool empty(); // r

	void reserve(size_t n); // r
	void shrink_to_fit(); // r

	type &operator[](size_t idx); // r
	const type& operator[](size_t idx) const; // r
	type& at(size_t idx); // r

	type& front(); // r
	type& back(); // r
	type* data(); // r
	const type& front() const; // r
	const type& back() const; // r
	const type* data() const; // r

	void assign(size_t n, type);
	void insert(size_t pos, type); // r
	void erase(size_t pos); // r

	Victor& operator=(const Victor&); // r
	Victor& operator=(Victor&&); // r
private:
	size_t sizeS;
	size_t capacityS;
	size_t maxSize;
	type* dataS;

	void* operator new(size_t size);
	void operator delete(void* ptr) noexcept;

};

#include "Victor.hpp"