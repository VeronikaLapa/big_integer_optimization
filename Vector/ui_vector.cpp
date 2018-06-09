#include "ui_vector.h"
#include <cassert>

using namespace std;

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
		//data.big_data.data_ptr.reset(); //check if nessecery
		data.big_data.~big_vector();
	}

}

ui_vector::ui_vector(ui_vector const& other) : is_big(other.is_big), length(other.length){
	if (other.is_big) {
		/*for (size_t i = 0; i < SMALL_DATA_SIZE; i++) {
			data.small_data[i] = 0;
		}*/
		new (&data.big_data) big_vector(other.data.big_data);
		//data.big_data = big_vector(other.data.big_data.data_ptr.get(), other.data.big_data.capacity);

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
/*
const unsigned int ui_vector::back() {
	return  operator[] (length - 1);
}
*/
unsigned int ui_vector::back() {
	if (is_big) {
		return data.big_data.data_ptr.get()[length - 1];
	}
	else {
		return data.small_data[length - 1];
	}
}
unsigned int& ui_vector::operator[](size_t ind) {
	//assert(!(is_big && !data.big_data.data_ptr.unique()));
	make_unique();
	if (is_big) {
		return data.big_data.data_ptr.get()[ind];
	}
	else {
		return data.small_data[ind];
	}
}
unsigned int const&  ui_vector::operator[](size_t ind) const {
	if (is_big) {
		return data.big_data.data_ptr.get()[ind];
	}
	else {
		return data.small_data[ind];
	}
}
void ui_vector::pop_back() {
	length--;
}
void ui_vector::push_back(unsigned int value) {
	//assert(!(is_big && !data.big_data.data_ptr.unique()));
	make_unique();
	if (!is_big && length < SMALL_DATA_SIZE) {
		data.small_data[length] = value;
	}
	else if (is_big && length < data.big_data.capacity) {
		data.big_data.data_ptr.get()[length] = value;
	}
	else if (!is_big) {
		is_big = true;
		//unsigned int* new_data = new unsigned int[CAPACITY_FACT * SMALL_DATA_SIZE];
		unsigned int* new_data = static_cast<unsigned int*>(operator new(CAPACITY_FACT * SMALL_DATA_SIZE * sizeof(unsigned int)));
		copy(data.small_data, data.small_data + SMALL_DATA_SIZE, new_data);
		*(new_data + SMALL_DATA_SIZE) = value;
		new (&data.big_data) big_vector(new_data, CAPACITY_FACT * SMALL_DATA_SIZE);
		//data.big_data = big_vector(new_data, CAPACITY_FACT * SMALL_DATA_SIZE);
	}
	else {
		//unsigned int* new_data = new unsigned int[CAPACITY_FACT * data.big_data.capacity];
		unsigned int* new_data = static_cast<unsigned int*>(operator new(CAPACITY_FACT * data.big_data.capacity * sizeof(unsigned int)));
		copy(data.big_data.data_ptr.get(), data.big_data.data_ptr.get() + length, new_data);
		*(new_data + length) = value;
		//data.big_data.data_ptr.reset(new_data, [](unsigned int *p) { delete[] p; });
		//data.big_data.capacity = CAPACITY_FACT * data.big_data.capacity;
		new (&data.big_data) big_vector(new_data, CAPACITY_FACT * data.big_data.capacity);
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
			data.small_data;
			is_big = true;
			unsigned int* new_data = new unsigned int[newsize];
			copy(data.small_data, data.small_data + SMALL_DATA_SIZE, new_data);
			new (&data.big_data) big_vector(new_data, newsize);
			//data.big_data = big_vector(new_data, newsize);
			//big_data = big_vector();
			//data.big_data.data_ptr = shared_ptr<unsigned int>(new_data, [](unsigned int *p) { delete p; });
			//data.big_data.capacity = newsize;
		}
	}
	length = newsize;
}
bool operator==(ui_vector const& a, ui_vector const& b) {
	if (a.length != b.length) {
		return false;
	}
	const unsigned int * aptr, *bptr;
	if (a.is_big) {
		aptr = a.data.big_data.data_ptr.get();
	}
	else {
		aptr = a.data.small_data;
	}
	if (b.is_big) {
		bptr = b.data.big_data.data_ptr.get();
	}
	else {
		bptr = b.data.small_data;
	}
	return (memcmp(aptr, bptr, a.length * sizeof(unsigned int)) == 0);
}
void ui_vector::swap_big_small(ui_vector& a, ui_vector& b) {
	unsigned int temp[SMALL_DATA_SIZE];
	copy(b.data.small_data, b.data.small_data + SMALL_DATA_SIZE, temp);
	new (&b.data.big_data) big_vector(a.data.big_data);
	copy(temp, temp+ SMALL_DATA_SIZE, a.data.small_data);
}
void ui_vector::swap(ui_vector& other) {
	if (is_big && other.is_big) {
		std::swap(data.big_data, other.data.big_data);
		std::swap(length, other.length);
	}
	else if (!is_big && !other.is_big) {
		for (size_t i = 0; i < SMALL_DATA_SIZE; i++) {
			std::swap(data.small_data[i], other.data.small_data[i]);
		}
		std::swap(length, other.length);
	}
	else if (is_big && !other.is_big) {
		swap_big_small(*this, other);
		std::swap(length, other.length);
		std::swap(is_big, other.is_big);
	}
	else {
		swap_big_small(other, *this);
		std::swap(length, other.length);
		std::swap(is_big, other.is_big);
	}
}