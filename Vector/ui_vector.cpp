#include "ui_vector.h"
#include <cassert>

using namespace std;

const unsigned int* ui_vector::get_ptr()const {
	if (is_big) {
		return data.big_data.data_ptr.get();
	}
	else {
		return data.small_data;
	}
}
unsigned int* ui_vector::get_ptr() {
	if (is_big) {
		return data.big_data.data_ptr.get();
	}
	else {
		return data.small_data;
	}
}
void ui_vector::make_big(size_t cap, unsigned int* d = nullptr) {
	unsigned int* new_data = static_cast<unsigned int*>(operator new(cap * sizeof(unsigned int)));
	if (d != nullptr) {
		copy(d, d + length, new_data);
	}
	new (&data.big_data) big_vector(new_data, cap);
}
void ui_vector::make_unique() {
	if (is_big && !data.big_data.data_ptr.unique()) {
		unsigned int* new_data = static_cast<unsigned int*>(operator new(data.big_data.capacity * sizeof(unsigned int)));
		copy(data.big_data.data_ptr.get(), data.big_data.data_ptr.get() + length, new_data);
		data.big_data.data_ptr.reset(new_data);
	}
}
ui_vector::ui_vector() : length(0), is_big(false){}

ui_vector::~ui_vector() {
	if (is_big) {
		data.big_data.~big_vector();
	}

}

ui_vector::ui_vector(ui_vector const& other) : is_big(other.is_big), length(other.length){
	if (is_big) {
		new (&data.big_data) big_vector(other.data.big_data);
	} 
	else {
		copy(other.data.small_data, other.data.small_data + SMALL_DATA_SIZE, data.small_data);
	}
}

ui_vector& ui_vector::operator=(ui_vector const& other) {
	ui_vector temp(other);
	swap(temp);
	return *this;
}

ui_vector::ui_vector(size_t newsize) : length(newsize), is_big(false){
	resize(newsize);
	if (newsize <= SMALL_DATA_SIZE) {
		is_big = false;
	}
	else {
		is_big = true;
	}
}

size_t ui_vector::size() const{
	return length;
}
unsigned int ui_vector::back() {
	return get_ptr()[length - 1];
}
unsigned int& ui_vector::operator[](size_t ind) {
	make_unique();
	return get_ptr()[ind];
}
unsigned int const&  ui_vector::operator[](size_t ind) const {
	return get_ptr()[ind];
}
void ui_vector::pop_back() {
	length--;
}
void ui_vector::push_back(unsigned int value) {
	make_unique();
	if (!is_big && length < SMALL_DATA_SIZE) {
		data.small_data[length] = value;
	}
	else if (is_big && length < data.big_data.capacity) {
		data.big_data.data_ptr.get()[length] = value;
	}
	else if (!is_big) {
		make_big(CAPACITY_FACT * SMALL_DATA_SIZE, get_ptr());
		data.big_data.data_ptr.get()[SMALL_DATA_SIZE] = value;
		is_big = true;
	}
	else {
		make_big(CAPACITY_FACT * data.big_data.capacity, get_ptr());
		data.big_data.data_ptr.get()[length] = value;
	}
	length++;
}
void  ui_vector::resize(size_t newsize) {
	if (newsize > SMALL_DATA_SIZE) {
		if (is_big && data.big_data.capacity < newsize) {
			unsigned int* new_data = static_cast<unsigned int*>(operator new(newsize * sizeof(unsigned int)));
			copy(data.big_data.data_ptr.get(), data.big_data.data_ptr.get() + length, new_data);
			data.big_data.data_ptr.reset(new_data, [](unsigned int *p) { delete p; });
			data.big_data.capacity = newsize;
		}
		else if (!is_big) {
			is_big = true;
			length = SMALL_DATA_SIZE;
			make_big(newsize, data.small_data);
		}
	}
	length = newsize;
}
bool operator==(ui_vector const& a, ui_vector const& b) {
	if (a.length != b.length) {
		return false;
	}
	return (equal(a.get_ptr(), a.get_ptr() + a.length, b.get_ptr()));
}
void ui_vector::swap_big_small(ui_vector& a, ui_vector& b) {
	unsigned int temp[SMALL_DATA_SIZE];
	copy(b.data.small_data, b.data.small_data + SMALL_DATA_SIZE, temp);
	new (&b.data.big_data) big_vector(a.data.big_data);
	a.data.big_data.~big_vector();
	copy(temp, temp+ SMALL_DATA_SIZE, a.data.small_data);
}
void ui_vector::swap(ui_vector& other) {
	if (is_big && other.is_big) {
		std::swap(data.big_data, other.data.big_data);
	}
	else if (!is_big && !other.is_big) {
		for (size_t i = 0; i < SMALL_DATA_SIZE; i++) {
			std::swap(data.small_data[i], other.data.small_data[i]);
		}
	}
	else if (is_big && !other.is_big) {
		swap_big_small(*this, other);
	}
	else {
		swap_big_small(other, *this);
	}
	std::swap(length, other.length);
	std::swap(is_big, other.is_big);
}