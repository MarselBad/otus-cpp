#pragma once

#include <memory>
#include <array>


template<typename T, size_t N>
class custom_allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;

    template<typename U>
    struct rebind {
        using other = custom_allocator<U, N>;
    };

    custom_allocator() = default;
    ~custom_allocator() {
        if (buffer_) {
            std::free(buffer_);
        }
    }

    bool operator == (const custom_allocator<T, N>&) { return false; }
    bool operator != (const custom_allocator<T, N>&) { return true; }

    pointer allocate(size_t n) {
        if (n == 0) {
            return nullptr;
        }
        if (size_ + n > N) {
            std::cerr << "custom_allocator ran out available memory! capacity: " << 
                          N << " current size: " << size_ << " requested memory: " << n << "\n";
            throw std::bad_alloc();
        }
        if (!buffer_) {
            capacity_ = N;
            buffer_ = reinterpret_cast<T*>(std::malloc(capacity_ * sizeof(T)));
            if (!buffer_) {
                std::cerr << "custom_allocator failed to allocate memory!\n";
                throw std::bad_alloc();
            }
        }
        pointer cur_ptr = &buffer_[size_];
        size_ += n;
        return cur_ptr;
    }

    void deallocate(pointer, size_t n) {
        size_ -= std::min(size_, n);
    }

    template<typename U, typename ...Args>
    void construct(U *ptr, Args &&...args) {
        new(ptr) U(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~T();
    }

private:
    T* buffer_{nullptr};
    size_t size_{0};
    size_t capacity_{0};
};
