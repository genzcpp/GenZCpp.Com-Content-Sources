#include <cstddef>

template <size_t N> class Arena {
    static constexpr size_t alignment = alignof(std::max_align_t);

  public:
    Arena() noexcept : ptr_(buffer_) {}
    Arena(const Arena &) = delete;
    Arena &operator=(const Arena &) = delete;
    auto reset() noexcept { ptr_ = buffer_; }
    static constexpr auto size() noexcept { return N; }
    auto used() const noexcept { return static_cast<size_t>(ptr_ - buffer_); }
    auto allocate(size_t n) -> std::byte *;
    auto deallocate(std::byte *p, size_t n) noexcept -> void;

  private:
    static auto align_up(size_t n) noexcept -> size_t {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }
    auto pointer_in_buffer(const std::byte *p) const noexcept -> bool {
        return std::uintptr_t(p) >= std::uintptr_t(buffer_) &&
               std::uintptr_t(p) < std::uintptr_t(buffer_) + N;
    }
    alignas(alignment) std::byte buffer_[N];
    std::byte *ptr_{};
};

template <size_t N>
auto Arena<N>::allocate(size_t n) -> std::byte * {
    const auto aligned_n = align_up(n);
    const auto available_bytes =
        static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
    if (available_bytes >= aligned_n) {
        auto *r = ptr_;
        ptr_ += aligned_n;
        return r;
    }
    return static_cast<std::byte *>(::operator new(n));
}

template <size_t N>
auto Arena<N>::deallocate(std::byte *p, size_t n) noexcept -> void {
    if (pointer_in_buffer(p)) {
        n = align_up(n);
        if (p + n == ptr_) {
            ptr_ = p;
        }
    } else {
        ::operator delete(p);
    }
}

template <class T, size_t N> struct ShortAlloc {
    using value_type = T;
    using arena_type = Arena<N>;
    ShortAlloc(const ShortAlloc &) = default;
    ShortAlloc &operator=(const ShortAlloc &) = default;
    ShortAlloc(arena_type &arena) noexcept : arena_{&arena} {}
    template <class U>
    ShortAlloc(const ShortAlloc<U, N> &other) noexcept : arena_{other.arena_} {}
    template <class U> struct rebind {
        using other = ShortAlloc<U, N>;
    };
    auto allocate(size_t n) -> T * {
        return reinterpret_cast<T *>(arena_->allocate(n * sizeof(T)));
    }
    auto deallocate(T *p, size_t n) noexcept -> void {
        arena_->deallocate(reinterpret_cast<std::byte *>(p), n * sizeof(T));
    }
    template <class U, size_t M>
    auto operator==(const ShortAlloc<U, M> &other) const noexcept {
        return N == M && arena_ == other.arena_;
    }
    template <class U, size_t M>
    auto operator!=(const ShortAlloc<U, M> &other) const noexcept {
        return !(*this == other);
    }
    template <class U, size_t M> friend struct ShortAlloc;

  private:
    arena_type *arena_;
};