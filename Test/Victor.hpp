#include "Victor.h"

template<typename type>
Victor<type>::Victor() {
	this->sizeS = 0;
	this->capacityS = 0;
	this->dataS = nullptr;
	this->maxSize = std::numeric_limits<size_t>::max() / sizeof(type);
}


template<typename type>
inline Victor<type>::Victor(size_t size)
{
	this->sizeS = 0;     
	this->capacityS = 0;
	this->dataS = nullptr;
	this->maxSize = std::numeric_limits<size_t>::max() / sizeof(type);

	if (size > this->maxSize) throw std::length_error("Too large size");
	if (size > 0) {
		reserve(size);
		this->sizeS = size;
		for (int i = 0; i < this->sizeS; i++) {
			new (this->dataS + i) type();
		}
	}
}

template<typename type>
inline Victor<type>::Victor(const Victor& other)
{
	this->sizeS = other.sizeS;
	this->capacityS = other.capacityS;
	this->maxSize = other.maxSize;
	if (this->capacityS == 0) {
		this->dataS = nullptr;
		return;
	}

	type* ptr = reinterpret_cast<type*>(::operator new(this->capacityS * sizeof(type)));
	for (int i = 0; i < this->sizeS; i++) {
		new (ptr + i) type(other[i]);
	}
	this->dataS = ptr;
}

template<typename type>
Victor<type>& Victor<type>::operator=(const Victor<type>& other) {
	if (this == &other) return *this; 

	for (size_t i = 0; i < this->sizeS; ++i) {
		this->dataS[i].~type();
	}
	::operator delete(this->dataS); 

	this->sizeS = other.sizeS;
	this->capacityS = other.capacityS;
	this->maxSize = other.maxSize;

	if (this->capacityS == 0) {
		this->dataS = nullptr;
		return *this;
	}

	type* ptr = reinterpret_cast<type*>(::operator new(sizeof(type) * this->capacityS));
	for (int i = 0; i < this->sizeS; i++) {
		new (ptr + i) type(other[i]);
	}
	this->dataS = ptr;

	return *this;
}

template<typename type>
inline Victor<type>::Victor(Victor&& other)
{
	this->sizeS = other.sizeS;
	this->capacityS = other.capacityS;
	this->maxSize = other.maxSize;


	this->dataS = other.dataS;

	other.dataS = nullptr;
	other.sizeS = 0;
	other.capacityS = 0;
}

template<typename type>
Victor<type>& Victor<type>::operator=(Victor<type>&& other) {
	if (this == &other) return *this; 

	for (size_t i = 0; i < this->sizeS; ++i) {
		this->dataS[i].~type();
	}
	::operator delete(this->dataS);

	this->sizeS = other.sizeS;
	this->capacityS = other.capacityS;
	this->maxSize = other.maxSize;
	this->dataS = other.dataS;

	other.dataS = nullptr;
	other.sizeS = 0;
	other.capacityS = 0;

	return *this;
}

template<typename type>
Victor<type>::~Victor()
{
	for (size_t i = 0; i < this->sizeS; ++i) {
		this->dataS[i].~type();
	}

	::operator delete(this->dataS);
	this->dataS = nullptr;
	this->sizeS = 0;
	this->capacityS = 0;
	this->maxSize = 0;
}

template<typename type>
void Victor<type>::push_back(type&& element)
{
	if (this->sizeS >= this->capacityS){
		if (this->capacityS == 0) {
			reserve(4);
		}
		else reserve(this->capacityS * 2);
		
	}
	new (this->dataS + this->sizeS) type(std::move(element));
	this->sizeS++;
}

template<typename type>
void Victor<type>::push_back(const type& element)
{
	if (this->sizeS == this->capacityS) {
		if (this->capacityS == 0) {
			reserve(4);
		}
		else reserve(this->capacityS * 2);

	}
	new (this->dataS + this->sizeS) type(element);
	this->sizeS++;
}
template<typename type>
void Victor<type>::pop_back() {
	this->dataS[this->sizeS - 1].~type();
	this->sizeS--;
}

template<typename type>
void Victor<type>::reserve(size_t n) {
	
	if (n <= this->capacityS) return;

	type* ptr = reinterpret_cast<type*>(::operator new(n*sizeof(type)));
	

	for (int i = 0; i < this->sizeS; i++) {
		new (ptr + i) type(this->dataS[i]);
	}
	for (size_t i = 0; i < this->sizeS; ++i) {
		this->dataS[i].~type();
	}
	::operator delete(this->dataS);
	this->dataS = ptr;
	this->capacityS = n;
}


template<typename type>
type &Victor<type>::operator[](size_t idx) {
	
	return this->dataS[idx];

	
}

template<typename type>
const type& Victor<type>::operator[](size_t idx) const {
	
	return this->dataS[idx];
}


template<typename type>
type& Victor<type>::at(size_t idx)  {
	if (idx >= 0 && idx < this->sizeS) {
		return this->dataS[idx];
	}
	else throw std::out_of_range("out of range");
}


template<typename type>
void* Victor<type>::operator new(size_t size) {
	void* ptr = malloc(size);
	if (!ptr) {
		throw std::bad_alloc();
	}

	return ptr;

}

template<typename type>
void Victor<type>::operator delete(void* ptr) noexcept{
	free(ptr);
}

template<typename type>
type& Victor<type>::front() {
	return this->at(0);
}

template<typename type>
type& Victor<type>::back() {
	return this->at(this->sizeS - 1);
}

template<typename type>
type* Victor<type>::data() {
	return this->dataS;
}

template<typename type>
const type& Victor<type>::front() const {
	return this->at(0);
}

template<typename type>
const type& Victor<type>::back() const {
	return this->at(this->sizeS - 1);
}

template<typename type>
const type* Victor<type>::data() const {
	return this->dataS;
}

template<typename type>
inline void Victor<type>::assign(size_t n, type value)
{
	type* new_data = nullptr;
	if (n > 0) {
		new_data = static_cast<type*>(::operator new(sizeof(type) * n));

		for (size_t i = 0; i < n; i++) {
			new (new_data + i) type(value);
		}
	}

	for (size_t i = 0; i < this->sizeS; i++) {
		this->dataS[i].~type();
	}
	::operator delete(this->dataS);

	this->dataS = new_data;
	this->sizeS = n;
	this->capacityS = n;

}

template<typename type>
inline void Victor<type>::insert(size_t pos, type value)
{
	if (this->sizeS == this->capacityS) {
		if (this->capacityS == 0) {
			reserve(4);
		}
		else reserve(this->capacityS * 2);

	}
	if (pos == this->sizeS) {
		new (this->dataS + this->sizeS) type(value);
		this->sizeS++;
		return;
	}

	new (this->dataS + this->sizeS) type(this->dataS[this->sizeS - 1]);

	for (size_t i = this->sizeS - 1; i > pos; i--) {
		this->dataS[i] = this->dataS[i - 1];
	}

	this->dataS[pos] = value;

	this->sizeS++;
}

template<typename type>
inline void Victor<type>::erase(size_t pos)
{
	if (this->sizeS == 0) return;

	for (int i = pos; i < this->sizeS - 1; i++) {
		this->dataS[i] = this->dataS[i + 1];
	}
	this->dataS[this->sizeS - 1].~type();
	this->sizeS--;
}

template<typename type>
bool Victor<type>::empty() {
	return this->sizeS == 0;
}

template<typename type>
void Victor<type>::shrink_to_fit() {
	if (this->sizeS == this->capacityS) return;

	if (this->sizeS == 0) {
		::operator delete(this->dataS);
		this->dataS = nullptr;
		this->capacityS = 0;
		return;
	}

	type* new_data = static_cast<type*>(::operator new(this->sizeS * sizeof(type)));

	for (size_t i = 0; i < this->sizeS; ++i) {
		new (new_data + i) type(std::move(this->dataS[i]));

		this->dataS[i].~type();
	}

	::operator delete(this->dataS);

	this->dataS = new_data;
	this->capacityS = this->sizeS;
}
template<typename type>
size_t Victor<type>::max_size() const {
	return this->maxSize;
}

template<typename type>
size_t Victor<type>::size() const {
	return this->sizeS;
}

template<typename type>
size_t Victor<type>::capacity() const {
	return this->capacityS;
}


template<typename type>
VictorIterator<type>::VictorIterator(pointer data): raw_ptr{data} {}

template<typename type>
typename VictorIterator<type>::reference VictorIterator<type>::operator*() {
	return *raw_ptr;
}

template<typename type>
typename VictorIterator<type>::pointer VictorIterator<type>::operator->() {
	return raw_ptr;
}

template<typename type>
VictorIterator<type>& VictorIterator<type>::operator++() {
	this->raw_ptr++;
	return *this;
}
template<typename type>
VictorIterator<type>& VictorIterator<type>::operator--() {
	this->raw_ptr--;
	return *this;
}
template<typename type>
VictorIterator<type> VictorIterator<type>::operator++(int) {
	VictorIterator<type> tmp = *this;
	this->raw_ptr++;
	return tmp;
}
template<typename type>
VictorIterator<type> VictorIterator<type>::operator--(int) {
	VictorIterator<type> tmp = *this;
	this->raw_ptr--;
	return tmp;
}

template<typename type>
VictorIterator<type>& VictorIterator<type>::operator-=(difference_type n) {
	this->raw_ptr -= n;
	return *this;
}

template<typename type>
VictorIterator<type>& VictorIterator<type>::operator+=(difference_type n) {
	this->raw_ptr += n;
	return *this;
}



template<typename type>
typename VictorIterator<type>::difference_type VictorIterator<type>::operator-(const VictorIterator<type>& other) const {
	return this->raw_ptr - other.raw_ptr;
}


template<typename type>
typename VictorIterator<type>::reference VictorIterator<type>::operator[](difference_type n) const {
	return *(raw_ptr + n);
}


template<typename type>
bool VictorIterator<type>::operator==(const VictorIterator<type>& other) const {
	return raw_ptr == other.raw_ptr;
}

template<typename type>
bool VictorIterator<type>::operator!=(const VictorIterator<type>& other) const {
	return raw_ptr != other.raw_ptr;
}

template<typename type>
bool VictorIterator<type>::operator<(const VictorIterator<type>&other) const {
	return raw_ptr < other.raw_ptr;
}

template<typename type>
bool VictorIterator<type>::operator>(const VictorIterator<type>& other) const {
	return raw_ptr > other.raw_ptr;
}

template<typename type>
bool VictorIterator<type>::operator<=(const VictorIterator<type>& other) const {
	return raw_ptr <= other.raw_ptr;
}

template<typename type>
bool VictorIterator<type>::operator>=(const VictorIterator<type>&other) const {
	return raw_ptr >= other.raw_ptr;
}

template<typename type>
typename Victor<type>::iterator Victor<type>::begin() {
	return iterator(dataS);
}

template<typename type>
typename Victor<type>::iterator Victor<type>::end() {
	return iterator(dataS + sizeS);
}

template<typename type>
typename Victor<type>::const_iterator Victor<type>::cbegin() {
	return const_iterator(dataS);
}

template<typename type>
typename Victor<type>::const_iterator Victor<type>::cend() {
	return const_iterator(dataS + sizeS);
}

template<typename type>
typename VictorIterator<type>::reference VictorIterator<type>::operator*() const {
	return *raw_ptr;
}

template<typename type>
typename VictorIterator<type>::pointer VictorIterator<type>::operator->() const { 
	return raw_ptr;
}
