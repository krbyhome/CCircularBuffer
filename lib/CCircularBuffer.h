#pragma once

#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

const size_t kDefaultCapacity = 100;

template<typename T, class Allocator = std::allocator<T>>
class CCircularBuffer {
private:
    T* elements = nullptr;
    Allocator allocator;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t begin_ = 0;
    size_t end_ = 0;
public:
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    class Iterator {
        private:
            T* elements_it = nullptr;
            size_t size_it;
            size_t index_it;
            size_t begin_it;
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using iterator = pointer;
            using reference = T&;

            explicit Iterator(T* _elements_, size_t _size_, size_t _index_, size_t _begin_) {
                elements_it = _elements_;
                size_it = _size_;
                index_it = _index_;
                begin_it = _begin_;
            }

            Iterator(const Iterator& rhs) {
                elements_it = rhs.elements_it;
                size_it = rhs.size_it;
                index_it = rhs.index_it;
                begin_it = rhs.begin_it;
            }

            Iterator& operator+=(difference_type diff) {
                index_it += diff;

                return *this;
            }

            Iterator operator+(difference_type diff) const {
                Iterator res = *this;
                res += diff;

                return res;
            }

            Iterator& operator++() {
                index_it++;

                return *this;
            }

            Iterator& operator-=(difference_type diff) {
                index_it -= diff;

                return *this;
            }

            Iterator operator-(difference_type diff) const {
                Iterator res_ = *this;
                res_ -= diff;

                return res_;
            }

            difference_type operator-(const Iterator& diff) const {
                return index_it - diff.index_it;
            }

            Iterator& operator--() {
                index_it--;

                return *this;
            }

            reference operator*() {
                return elements_it[(begin_it + index_it) % size_it];
            }

            pointer operator->() {
                return elements_it + (begin_it + index_it) % size_it;
            }

            bool operator==(const Iterator& rhs) const {
                return index_it == rhs.index_it;
            }

            bool operator!=(const Iterator& rhs) const {
                return index_it != rhs.index_it;
            }

            bool operator<(const Iterator& rhs) const {
                return index_it < rhs.index_it;
            }

            bool operator>(const Iterator& rhs) const {
                return index_it > rhs.index_it;
            }
    };

    explicit CCircularBuffer(size_t _capacity_) {
        elements = allocator.allocate(_capacity_ + 1);
        capacity_ = _capacity_ + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
    }

    CCircularBuffer(size_t t, const_reference val_) {
        elements = allocator.allocate(t + 1);
        capacity_ = t + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        for (auto i = 0; i < t; ++i) {
            push_back(val_);
        }
    }

    CCircularBuffer(std::initializer_list<T> list) {
        elements = allocator.allocate(list.size() + 1);
        capacity_ = list.size() + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        for (auto c : list) {
            push_back(c);
        }
    }

    CCircularBuffer(const CCircularBuffer& rhs) {
        capacity_ = rhs.capacity_;
        elements = allocator.allocate(rhs.capacity_);
        size_ = rhs.size_;
        begin_ = rhs.begin_;
        end_ = rhs.end_;
        
        for (size_t i = 0; i < size_; ++i) {
            elements[i] = rhs.elements[i];
        }
    }
    
    CCircularBuffer() {
        capacity_ = kDefaultCapacity;
        elements = allocator.allocate(capacity_);
        size_ = 0;
        begin_ = 0;
        end_ = 0;
    }

    ~CCircularBuffer() {
        allocator.deallocate(elements, capacity_);
    }

    size_t capacity() {
        return capacity_;
    }

    void clear() {
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        allocator.deallocate(elements, capacity_);
    }

    bool empty() {
        return begin_ == end_;
    }

    size_t size() {
        return size_;
    }
    
    CCircularBuffer& operator=(const std::initializer_list<T>& list) {
        clear();
        elements = allocator.allocate(list.size() + 1);
        capacity_ = list.size() + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        for (auto c : list) {
            push_back(c);
        }
    }

    CCircularBuffer& operator=(const CCircularBuffer& rhs) {
        clear();
        capacity_ = rhs.capacity_;
        elements = allocator.allocate(rhs.size_);
        size_ = rhs.size_;
        begin_ = rhs.begin_;
        end_ = rhs.end_;

        for (size_t i = 0; i < size_; ++i) {
            *elements[i] = rhs.elements[i];
        }

        return *this;
    }

    T& operator[](size_t idx) const {
        if (size_ == 0)
            throw std::out_of_range("Empty buffer");
        if (idx >= size_)
            throw std::out_of_range("Error: index is out of range"); 
        
        return elements[((idx % size_) + begin_) % capacity_];
    }

    Iterator push_front(const_reference element_) {
        begin_ = (begin_ - 1 + capacity_) % capacity_;
        allocator.construct(&elements[begin_], element_);

        if (size_ + 1 == capacity_) {
            end_ = (end_ + capacity_ - 1) % capacity_;
        } else {
            size_++;
        }

        return begin();
    }

    Iterator push_back(const_reference element_) {
        allocator.construct(&elements[end_], element_);
        end_ = (end_ + 1) % capacity_;

        if (size_ + 1 == capacity_)
            begin_ = (begin_ + 1) % capacity_;
        else
            size_++;

        return end();
    }

    T pop_front() {
        if (size_ > 0) {
            size_--;
            begin_ = (begin_ + 1) % capacity_;

            return elements[(begin_ - 1) % capacity_];
        } else {
            throw std::out_of_range("Empty buffer");
        }
    }

    T pop_back() {
        if (size_ > 0) {
            size_--;
            end_ = (capacity_ + end_ - 1) % capacity_;

            return elements[(end_) % capacity_];
        } else {
            throw std::out_of_range("Empty buffer");
        }
    }

    void assign(const CCircularBuffer& val_) {
        for (auto i = val_.begin(); i < val_.end(); ++i) {
            pop_front();
        }

        for (auto i = val_.begin(); i < val_.end(); ++i) {
            push_front(val_[val_.end() - i - 1]);
        }
    }

    void assign(size_t n, const_reference val_) {
        for (size_t i = 0; i < n; ++i) {
            pop_front();
        }

        for (size_t i = 0; i < n; ++i) {
            push_front(val_);
        }
    }

    void erase(size_t idx_) {
        if (size_  + 1 == capacity_) {
            pop_back();
        }
        for (size_t i = idx_; i < end_; i++) {
            elements[i % capacity_] = elements[(i + 1) % capacity_];
        }

        end_ = (end_ - 1) % capacity_;
    }

    void erase(const Iterator& idx) {
        size_t index = idx - begin();
        erase(index);
    }

    Iterator insert(const_reference element_, Iterator index) {
        if (size_  + 1 == capacity_) {
            pop_back();
        }
        size_t idx_ = index - begin();

        for (size_t i = end_; i != idx_; --i) {
            elements[i % capacity_] = elements[(i - 1) % capacity_];
        }

        allocator.construct(elements[idx_], element_);
        size_++;
        end_ = (end_ + 1) % capacity_;

        return Iterator(elements, capacity_, idx_, begin_);
    }

    Iterator insert(Iterator pos, size_t times, const_reference element_) {
        for (size_t i = 0; i < times; ++i) {
            insert(element_, pos);
        }
        size_t idx_ = pos - begin();

        return Iterator(elements, capacity_, idx_, begin_);
    }

    Iterator insert(Iterator pos, Iterator from_, Iterator to_) {
        CCircularBuffer copy(from_ - to_ + 1);
        size_t idx_;
        for (auto i = from_; i < to_; ++i) {
            idx_ = i - begin();
            copy.push_back(elements[idx_]);
        }
        for (auto i = from_; i < to_; ++i) {
            insert(copy[i - from_], pos);
            pos += 1;
        }

        return pos;
    }

    Iterator insert(Iterator pos, const CCircularBuffer& val) {
        return insert(pos, val.begin(), val.end());
    }

    void reserve(size_t _capacity_) {
        capacity_ =  _capacity_ + 1;
        elements = allocator.allocate(_capacity_ + 1);
    }

    Iterator begin() const {
        return Iterator(elements, capacity_, 0, begin_);
    }

    Iterator end() const {
        return Iterator(elements, capacity_, size_, begin_);
    }

    T& front() const {
        return elements[begin_];
    }

    T& back() const {
        return elements[(size_ + begin_ - 1) % capacity_];
    }

    bool operator==(const CCircularBuffer& rhs) const {
        return std::equal(this->begin(), this->end(), rhs.begin(), rhs.end());
    }

    bool operator!=(const CCircularBuffer& rhs) const {
        return !(*this == rhs);
    }
};

template<typename T, class Allocator = std::allocator<T>>
class CCircularBufferExt {
private:
    T* elements = nullptr;
    Allocator allocator;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t begin_ = 0;
    size_t end_ = 0;
public:
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    class Iterator {
        private:
            T* elements_it = nullptr;
            size_t size_it;
            size_t index_it;
            size_t begin_it;
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;


            explicit Iterator(T* _elements_, size_t _size_, size_t _index_, size_t _begin_) {
                elements_it = _elements_;
                size_it = _size_;
                index_it = _index_;
                begin_it = _begin_;
            }

            Iterator(const Iterator& rhs) {
                elements_it = rhs.elements_it;
                size_it = rhs.size_it;
                index_it = rhs.index_it;
                begin_it = rhs.begin_it;
            }

            Iterator& operator+=(difference_type diff) {
                index_it += diff;

                return *this;
            }

            Iterator operator+(difference_type diff) const {
                Iterator res = *this;
                res += diff;

                return res;
            }

            Iterator& operator++() {
                index_it++;

                return *this;
            }

            Iterator& operator-=(difference_type diff) {
                index_it -= diff;

                return *this;
            }

            Iterator operator-(difference_type diff) const {
                Iterator res_ = *this;
                res_ -= diff;

                return res_;
            }

            difference_type operator-(const Iterator& diff) const {
                return index_it - diff.index_it;
            }

            Iterator& operator--() {
                index_it--;

                return *this;
            }

            reference operator*() {
                return elements_it[(begin_it + index_it) % size_it];
            }

            pointer operator->() {
                return elements_it + (begin_it + index_it) % size_it;
            }

            bool operator==(const Iterator& rhs) const {
                return index_it == rhs.index_it;
            }

            bool operator!=(const Iterator& rhs) const {
                return index_it != rhs.index_it;
            }

            bool operator<(const Iterator& rhs) const {
                return index_it < rhs.index_it;
            }

            bool operator>(const Iterator& rhs) const {
                return index_it > rhs.index_it;
            }
    };

    size_t capacity() {
        return capacity_ - 1;
    }

    size_t size() {
        return size_;
    }

    explicit CCircularBufferExt(size_t _capacity_) {
        elements = allocator.allocate(_capacity_ + 1);
        capacity_ = _capacity_ + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
    }

    CCircularBufferExt(const CCircularBufferExt& rhs) {
        capacity_ = rhs.capacity_;
        elements = allocator.allocate(rhs.size_);
        size_ = rhs.size_;
        begin_ = rhs.begin_;
        end_ = rhs.end_;
        
        for (size_t i = 0; i < size_; ++i) {
            *elements[i] = rhs.elements[i];
        }
    }

    CCircularBufferExt(size_t t, const_reference val_) {
        elements = allocator.allocate(t + 1);
        capacity_ = t + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        for (auto i = 0; i < t; ++i) {
            push_back(val_);
        }
    }

    CCircularBufferExt(std::initializer_list<T> list) {
        elements = allocator.allocate(list.size() + 1);
        capacity_ = list.size() + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        for (auto c : list) {
            push_back(c);
        }
    }

    void clear() {
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        allocator.deallocate(elements, capacity_);
    }
    
    CCircularBufferExt& operator=(const std::initializer_list<T>& list) {
        clear();
        elements = allocator.allocate(list.size() + 1);
        capacity_ = list.size() + 1;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        for (auto c : list) {
            push_back(c);
        }
    }

    CCircularBufferExt() {
        capacity_ = 0;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
        elements = nullptr;
    }

    ~CCircularBufferExt() {
        allocator.deallocate(elements, capacity_);
    }

    T& operator[](size_t idx) const {
        if (size_ == 0)
            throw std::out_of_range("Empty buffer");
        if (idx >= size_)
            throw std::out_of_range("Error: index is out of range"); 
        
        return elements[((idx % size_) + begin_) % capacity_];
    }

    Iterator push_front(const_reference element_) {

        if (size_ + 1 == capacity_) {
            capacity_ = 2 * capacity_ + 1;
            T* temp = allocator.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                allocator.construct(temp + i, elements[i]);
            }
            allocator.deallocate(elements, (capacity_ - 1) / 2 );
            elements = temp;
        } 
        
        begin_ = (begin_ - 1 + capacity_) % capacity_;
        allocator.construct(&elements[begin_], element_);
        size_++;

        return begin();
    }

    Iterator push_back(const_reference element_) {

        if (size_ + 1 == capacity_) {
            capacity_ = 2 * capacity_ + 1;
            T* temp = allocator.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                allocator.construct(temp + i, elements[i]);
            }
            allocator.deallocate(elements, (capacity_ - 1) / 2 );
            elements = temp;
        }

        allocator.construct(&elements[end_], element_);
        end_ = (end_ + 1) % capacity_;
        size_++;

        return end();
    }

    T pop_front() {
        if (size_ > 0) {
            size_--;
            begin_ = (begin_ + 1) % capacity_;

            return elements[(begin_ - 1) % capacity_];
        } else {
            throw std::out_of_range("Empty buffer");
        }
    }

    T pop_back() {
        if (size_ > 0) {
            size_--;
            end_ = (capacity_ + end_ - 1) % capacity_;

            return elements[(end_) % capacity_];
        } else {
            throw std::out_of_range("Empty buffer");
        }
    }    

    Iterator insert(const_reference element_, Iterator index) {
        size_t idx_ = index - begin();

        if (size_  + 1 == capacity_) {
            capacity_ = 2 * capacity_ + 1;
            T* temp = allocator.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                allocator.construct(temp + i, elements[i]);
            }
            allocator.deallocate(elements, (capacity_ - 1) / 2 );
            elements = temp;
        }

        for (size_t i = end_; i != idx_; --i) {
            allocator.construct(&elements[i % capacity_], elements[(i - 1) % capacity_]);
        }

        allocator.construct(&elements[idx_], element_);
        size_++;
        end_ = (end_ + 1) % capacity_;

        return Iterator(elements, capacity_, idx_, begin_);
    }

    Iterator insert(Iterator pos, size_t times, const_reference element_) {
        for (size_t i = 0; i < times; ++i) {
            insert(element_, pos);
        }
        size_t idx_ = pos - begin();

        return Iterator(elements, capacity_, idx_, begin_);
    }

    Iterator insert(Iterator pos, Iterator from_, Iterator to_) {
        CCircularBufferExt copy(1);
        size_t idx_;
        for (auto i = from_; i < to_; ++i) {
            idx_ = i - begin();
            copy.push_back(elements[idx_]);
        }
        for (auto i = from_; i < to_; ++i) {
            insert(copy[i - from_], pos);
            pos += 1;
        }

        return pos;
    }

    Iterator insert(Iterator pos, const CCircularBufferExt& val) {
        return insert(pos, val.begin(), val.end());
    }

    void assign(const CCircularBufferExt& val_) {
        for (auto i = val_.begin(); i < val_.end(); ++i) {
            pop_front();
        }

        for (auto i = val_.begin(); i < val_.end(); ++i) {
            push_front(val_[val_.end() - i - 1]);
        }
    }

    void assign(size_t n, const_reference val_) {
        for (size_t i = 0; i < n; ++i) {
            pop_front();
        }

        for (size_t i = 0; i < n; ++i) {
            push_front(val_);
        }
    }

    Iterator erase(size_t idx_) {
        if (size_  + 1 == capacity_) {
            capacity_ = 2 * capacity_ + 1;
            T* temp = allocator.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                allocator.construct(temp + i, elements[i]);
            }
            allocator.deallocate(elements, (capacity_ - 1) / 2 );
            elements = temp;
        }

        for (size_t i = idx_; i < end_; ++i) {
            allocator.construct(&elements[i % capacity_], elements[(i + 1) % capacity_]);
        }

        end_ = (end_ - 1) % capacity_;

        return Iterator(elements, capacity_, idx_, begin_);
    }

    Iterator erase(Iterator& idx) {
        size_t index = idx - begin();
        erase(index);

        return Iterator(elements, capacity_, index, begin_);
    }

    void reserve(size_t _capacity_) {
        capacity_ =  _capacity_ + 1;
        elements = allocator.allocate(_capacity_ + 1);
    }

    Iterator begin() const {
        return Iterator(elements, capacity_, 0, begin_);
    }

    Iterator end() const {
        return Iterator(elements, capacity_, size_, begin_);
    }

    T& front() const {
        return elements[begin_];
    }

    T& back() const {
        return elements[(size_ + begin_ - 1) % capacity_];
    }

    bool operator==(const CCircularBufferExt& rhs) const {
        return std::equal(this->begin(), this->end(), rhs.begin(), rhs.end());
    }

    bool operator!=(const CCircularBufferExt& rhs) const {
        return !(*this == rhs);
    }
};