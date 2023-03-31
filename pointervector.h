#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#pragma push_macro("CONST")
#undef CONST
template<typename T>
using PointerVector = std::vector<std::unique_ptr<T>>;
template<typename T>
using SharedPointerVector = std::vector<std::shared_ptr<T>>;

template<typename T, bool CONST = false, bool SHARED = false>
class PointerVectorIterable {
public:
    using type = std::conditional_t<CONST, std::add_const_t<T>, T>;
    using rawvector = std::conditional_t<SHARED, SharedPointerVector<T>, PointerVector<T>>;
    using vectortype = std::conditional_t<CONST, std::add_const_t<rawvector>, rawvector>;
    class Iterator {
    public:
        Iterator(PointerVectorIterable& iterable, size_t pos): iterable(&iterable), pos(pos){}
        Iterator& operator++() {
            pos++;
            return *this;
        }
        type* operator*() const {
            return iterable->vector[pos].get();
        }
        type* operator->() const {
            return iterable->vector[pos].get();
        }
        bool operator==(const Iterator& other) const{
            return other.iterable == iterable && other.pos == pos;
        }
        bool operator!=(const Iterator& other) const{
            return !(*this == other);
        }
        Iterator operator+(long long offset) const {
            return Iterator(*iterable, pos + offset);
        }
        Iterator operator-(long long offset) const {
            return Iterator(*iterable, pos - offset);
        }
    private:
        PointerVectorIterable* iterable;
        size_t pos;
    };
    PointerVectorIterable(vectortype& vector): vector(vector){}
    Iterator begin() {
        return Iterator(*this, 0);
    }
    Iterator end() {
        return Iterator(*this, vector.size());
    }
    type* operator[](size_t index) {
        return *(begin() + index);
    }
    size_t size() const {
        return vector.size();
    }
private:
    inline static vectortype emptyvector{};
    vectortype& vector;
public:
    static PointerVectorIterable empty;
};

template<typename T, bool CONST, bool SHARED>
inline PointerVectorIterable<T, CONST, SHARED> PointerVectorIterable<T, CONST, SHARED>::empty{PointerVectorIterable<T, CONST, SHARED>::emptyvector};

template<typename T>
using PointerVectorConstIterable = PointerVectorIterable<T, true, false>;
template<typename T>
using SharedPointerVectorConstIterable = PointerVectorIterable<T, true, true>;
template<typename T>
using SharedPointerVectorIterable = PointerVectorIterable<T, false, true>;

namespace pointervector {
    template<typename T>
    PointerVectorIterable<T> iter(PointerVector<T>& v) {
        return PointerVectorIterable<T>(v);
    }
    template<typename T>
    SharedPointerVectorIterable<T> iter(SharedPointerVector<T>& v) {
        return SharedPointerVectorIterable<T>(v);
    }
    template<typename T>
    PointerVectorConstIterable<T> iter(const PointerVector<T>& v) {
        return PointerVectorConstIterable<T>(v);
    }
    template<typename T>
    SharedPointerVectorConstIterable<T> iter(const SharedPointerVector<T>& v) {
        return SharedPointerVectorIterable<T>(v);
    }
}

#pragma pop_macro("CONST")
