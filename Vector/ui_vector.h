#ifndef UI_VECTOR_H
#define UI_VECTOR_H

#include <memory>
struct ui_vector{
	ui_vector();
	~ui_vector();
	ui_vector(ui_vector const& other);
	ui_vector& operator=(ui_vector const& other);
	explicit ui_vector(size_t newsize);
	size_t size() const;
	unsigned int back();
	unsigned int& operator[](size_t ind);
	unsigned int const& operator[](size_t ind) const;
	void pop_back();
	void push_back(unsigned int value);
	void resize(size_t newsize);
	friend bool operator==(ui_vector const& a, ui_vector const& b);
	void swap(ui_vector& other);
	void make_unique();

private:
	const void* get_ptr()const ;
	void make_big(size_t cap, unsigned int* d = nullptr);
	void swap_big_small(ui_vector& a, ui_vector& b);
	static const size_t SMALL_DATA_SIZE = 4;
	static const size_t CAPACITY_FACT = 2;
	size_t length;
	struct big_vector {
		size_t capacity;
		big_vector(unsigned int* ptr, const size_t cap) : data_ptr(ptr, [](unsigned int *p) { delete (p); }), capacity(cap) {};
		big_vector() {};
		std::shared_ptr<unsigned int> data_ptr;
	};
	union data_type {
		unsigned int small_data[SMALL_DATA_SIZE];
		big_vector big_data;
		~data_type() {};
		data_type() {};
	} data;
	bool is_big;
	
};

#endif