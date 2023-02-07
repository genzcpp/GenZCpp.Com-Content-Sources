#include <memory_resource>
#include <vector>
#include <iostream>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#ifndef MAX_STACK_SIZE
#define MAX_STACK_SIZE 1024
#endif
#pragma message "MAX_STACK_SIZE is: " STR(MAX_STACK_SIZE)

static constexpr size_t StackMaxBytes = MAX_STACK_SIZE;
class MyStackResource : public std::pmr::memory_resource {
  static constexpr size_t N = StackMaxBytes;
  public:
    MyStackResource() noexcept : ptr_(buffer_) {}
    MyStackResource(const MyStackResource&) = delete; 

    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void *p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const memory_resource &other) const noexcept override;

  private:
    static auto align_up(size_t n, size_t alignment) noexcept -> size_t {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }
    auto pointer_in_buffer(const std::byte *p) const noexcept -> bool {
        return std::uintptr_t(p) >= std::uintptr_t(buffer_) &&
               std::uintptr_t(p) < std::uintptr_t(buffer_) + N;
    }

    std::byte buffer_[N];
    std::byte *ptr_;
};

void* MyStackResource::do_allocate(std::size_t n, std::size_t alignment) {
    const auto aligned_n = align_up(n, alignment);
    const auto available_bytes =
        static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
    if (available_bytes >= aligned_n) {
        auto* r = ptr_;
        ptr_ += aligned_n;
        return (void*) r;
    }
    std::cout << "MyStackResource runs out of space!" << std::endl;
    return static_cast<std::byte *>(::operator new(n)); 
}

void MyStackResource::do_deallocate(void *void_p, size_t n, size_t alignment) {
    auto* p = (std::byte*)void_p;
    if (pointer_in_buffer(p)) {
        n = align_up(n, alignment);
        if (p + n == ptr_) {
            ptr_ = p;
        }
    } else {
        ::operator delete(p);
    }
}

bool MyStackResource::do_is_equal(const memory_resource &other) const noexcept {
    return false;
}