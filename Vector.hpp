#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>

template<typename T>
class Vector {
  private:
    T* data = nullptr;
    size_t size = 0;
    size_t cap = 0;

    void reallocate(size_t new_cap) {
        T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        size_t new_size = 0;
        try {
            for (; new_size < size; ++new_size) {
                new (new_data + new_size) T(std::move(data[new_size]));
            }
        } catch (...) {
            for (size_t i = 0; i < new_size; ++i) {
                new_data[i].~T();
            }
            ::operator delete(new_data);
            throw;
        }
        for (size_t i = 0; i < size; ++i) {
            data[i].~T();
        }
        ::operator delete(data);
        data = new_data;
        cap = new_cap;
    }

  public:
    Vector() = default;

    explicit Vector(size_t cnt) : cap(cnt) {
        data = static_cast<T*>(::operator new(cap * sizeof(T)));
        try {
            for (; size < cnt; ++size) {
                new (data + size) T();
            }
        } catch (...) {
            for (size_t i = 0; i < size; ++i) {
                data[i].~T();
            }
            ::operator delete(data);
            throw;
        }
    }

    Vector(size_t cnt, const T& val) : cap(cnt) {
        data = static_cast<T*>(::operator new(cap * sizeof(T)));
        try {
            for (; size < cnt; ++size) {
                new (data + size) T(val);
            }
        } catch (...) {
            for (size_t i = 0; i < size; ++i) {
                data[i].~T();
            }
            ::operator delete(data);
            throw;
        }
    }

    Vector(std::initializer_list<T> init) : cap(init.size()) {
        data = static_cast<T*>(::operator new(cap * sizeof(T)));
        try {
            for (const auto& item : init) {
                new (data + size) T(item);
                ++size;
            }
        } catch (...) {
            for (size_t i = 0; i < size; ++i) {
                data[i].~T();
            }
            ::operator delete(data);
            throw;
        }
    }

    Vector(const Vector& other) : cap(other.cap) {
        data = static_cast<T*>(::operator new(cap * sizeof(T)));
        try {
            for (; size < other.size; ++size) {
                new (data + size) T(other.data[size]);
            }
        } catch (...) {
            for (size_t i = 0; i < size; ++i) {
                data[i].~T();
            }
            ::operator delete(data);
            throw;
        }
    }

    ~Vector() {
        clear();
        ::operator delete(data);
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    T& operator[](size_t index) {
        return data[index];
    } 

    const T& operator[](size_t index) const {
        return data[index];
    }

    T& at(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    T& front() {
        return data[0];
    }

    const T& front() const {
        return data[0];
    }

    T& back() {
        return data[size - 1];
    }

    const T& back() const {
        return data[size - 1];
    }

    T* data_() noexcept {
        return data;
    }

    const T* data_() const noexcept {
        return data;
    }

    bool empty() const noexcept {
        return size == 0;
    }

    size_t size_() const noexcept {
        return size;
    }

    size_t cap_() const noexcept {
        return cap;
    }

    void push_back(const T& val) {
        if (size >= cap) reserve(cap ? cap * 2 : 1);
        new (data + size) T(val);
        ++size;
    }

    void push_back(T&& val) {
        if (size >= cap) reserve(cap ? cap * 2 : 1);
        new (data + size) T(std::move(val));
        ++size;
    }

    template<typename... Args>
    T& emplace_back(Args&&... args) {
        if (size >= cap) reserve(cap ? cap * 2 : 1);
        new (data + size) T(std::forward<Args>(args)...);
        return data[size++];
    }

    void pop_back() {
        if (size > 0) {
            data[size - 1].~T();
            --size;
        }
    }

    void resize(size_t new_size) {
        if (new_size > cap) reserve(new_size);
        if (new_size > size) {
            for (size_t i = size; i < new_size; ++i) {
                new (data + i) T();
            }
        } else {
            for (size_t i = new_size; i < size; ++i) {
                data[i].~T();
            }
        }
        size = new_size;
    }

    void resize(size_t new_size, const T& val) {
        if (new_size > cap) reserve(new_size);
        if (new_size > size) {
            for (size_t i = size; i < new_size; ++i) {
                new (data + i) T(val);
            }
        } else {
            for (size_t i = new_size; i < size; ++i) {
                data[i].~T();
            }
        }
        size = new_size;
    }

    void reserve(size_t new_cap) {
        if (new_cap > cap) reallocate(new_cap);
    }

    void clear() noexcept {
        for (size_t i = 0; i < size; ++i) {
            data[i].~T();
        }
        size = 0;
    }

    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
        std::swap(cap, other.cap);
    }

    T* begin() noexcept {
        return data;
    }

    const T* begin() const noexcept {
        return data;
    }

    T* end() noexcept {
        return data + size;
    }

    const T* end() const noexcept {
        return data + size;
    }
};
