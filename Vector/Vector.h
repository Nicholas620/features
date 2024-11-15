#ifndef LEETCODE_VECTOR_H
#define LEETCODE_VECTOR_H

#include <iterator>
#include <compare>
#include <initializer_list>

template<typename T>
concept MoveAssignable = requires(T a, T b) {
    { a = std::move(b) } -> std::same_as<T&>;
};

template<typename T>
concept MoveInsertable = requires(T a, T&& b) {
    a = std::move(b);
};

template<typename T>
concept EmplaceConstructible = requires(T&& t) {
    T(std::forward<T>(t));
};

template<typename T>
concept CopyAssignable = requires(T a, T b) {
    { a = b } -> std::same_as<T&>;
};

template<typename T>
concept CopyInsertable = requires(T a, const T& b) {
    a = b;
};

template<typename T>
concept Swappable = requires(T a, T b) {
    std::swap(a, b);
};

template<typename T>
concept Suitable = MoveAssignable<T> &&
                             MoveInsertable<T> &&
                             EmplaceConstructible<T> &&
                             CopyAssignable<T> &&
                             CopyInsertable<T> &&
                             Swappable<T>;

template<Suitable T, typename Alloc = std::allocator<T>>
class Vector;

template<Suitable T, bool is_const, bool is_reversed>
class Vector_iterator{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<is_const, const T, T>;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using pointer = value_type*;

    friend Vector_iterator<T, is_const, !is_reversed>;
    friend Vector_iterator<T, !is_const, !is_reversed>;
    friend Vector_iterator<T, !is_const, is_reversed>;
    friend Vector<T>;

    Vector_iterator() = default;
    Vector_iterator(std::size_t pos, std::size_t top, pointer data): _pos(pos), _top(top), _data(data) {}

    template<bool other_const, bool other_reversed>
    Vector_iterator(Vector_iterator<T, other_const, other_reversed>& other_iterator) noexcept
    requires(is_const >= other_const && is_reversed == other_reversed) : _pos(other_iterator._pos), _top(other_iterator._top), _data(other_iterator._data) {}

    template<bool other_const, bool other_reversed>
    Vector_iterator(Vector_iterator<T, other_const, other_reversed>&& other_iterator) noexcept
    requires(is_const >= other_const && is_reversed == other_reversed) : _pos(other_iterator._pos), _top(other_iterator._top), _data(other_iterator._data) {
        other_iterator._pos = 0;
        other_iterator._top = 0;
        other_iterator._data = nullptr;
    }

    template<bool other_const, bool other_reversed>
    Vector_iterator& operator=(Vector_iterator<T, other_const, other_reversed>& other_iterator) noexcept
    requires(is_const >= other_const && is_reversed == other_reversed){
        _pos = other_iterator._pos;
        _top = other_iterator._top;
        _data = other_iterator._data;

        return *this;
    }

    template<bool other_const, bool other_reversed>
    Vector_iterator& operator=(Vector_iterator<T, other_const, other_reversed>&& other_iterator) noexcept
    requires(is_const >= other_const && is_reversed == other_reversed){
        _pos = other_iterator._pos;
        _top = other_iterator._top;
        _data = other_iterator._data;

        other_iterator._pos = 0;
        other_iterator._top = 0;
        other_iterator._data = nullptr;

        return *this;
    }

    Vector_iterator& operator++() noexcept{
        if(is_reversed){
            if(_pos > -1) _pos--;
        }
        else{
            if(_pos < _top) _pos++;
        }

        return *this;
    }

    Vector_iterator operator++(int) noexcept{
        Vector_iterator copy(_pos, _top, _data);

        if(is_reversed){
            if(_pos > -1) _pos--;
        }
        else{
            if(_pos < _top) _pos++;
        }

        return *copy;
    }

    Vector_iterator& operator--() noexcept{
        if(!is_reversed){
            if(_pos > 0) _pos--;
        }
        else{
            if(_pos < _top - 1) _pos++;
        }

        return *this;
    }

    Vector_iterator operator--(int) noexcept{
        Vector_iterator copy(_pos, _top, _data);

        if(!is_reversed){
            if(_pos > 0) _pos--;
        }
        else{
            if(_pos < _top - 1) _pos++;
        }

        return *copy;
    }

    reference operator*() const noexcept {
        return _data[_pos];
    }

    pointer operator->() const noexcept {
        return &(_data[_pos]);
    }

    template<bool other_const, bool other_reversed>
    bool operator==(const Vector_iterator<T, other_const, other_reversed>& other_iterator) const noexcept {
        return _pos == other_iterator._pos && _top == other_iterator._top && _data == other_iterator._data;
    }

    template<bool other_const, bool other_reversed>
    std::strong_ordering operator<=>(const Vector_iterator<T, other_const, other_reversed>& other_iterator) const noexcept
    requires (other_reversed == is_reversed) {
        if(!is_reversed) {
            if (_pos < other_iterator._pos) return std::strong_ordering::less;
            if (_pos > other_iterator._pos) return std::strong_ordering::greater;
            return std::strong_ordering::equivalent;
        } else {
            if (_pos < other_iterator._pos) return std::strong_ordering::greater;
            if (_pos > other_iterator._pos) return std::strong_ordering::less;
            return std::strong_ordering::equivalent;
        }
    }

    Vector_iterator operator+(difference_type n) const noexcept {
        Vector_iterator copy(_pos, _top, _data);
        if(!is_reversed){
            if(_pos + n < _top) copy._pos += n;
            else copy._pos = _top;
        } else {
            if(_pos - n > -1) copy._pos -= n;
            else copy._pos = -1;
        }

        return copy;
    }

    friend Vector_iterator operator+(difference_type n, const Vector_iterator<T, is_const, is_reversed>& other_iterator) noexcept {
        return other_iterator + n;
    }

    Vector_iterator operator-(difference_type n) const noexcept {
        Vector_iterator copy(_pos, _top, _data);
        if(is_reversed){
            if(_pos + n < _top) copy._pos += n;
            else copy._pos = _top;
        } else {
            if(_pos - n > -1) copy._pos -= n;
            else copy._pos = -1;
        }

        return copy;
    }

    template<bool other_const, bool other_reversed>
    difference_type operator-(const Vector_iterator<T, other_const, other_reversed>& other_iterator) const noexcept requires(other_const == is_const){
        if(is_reversed) return _pos - other_iterator._pos;
        else return other_iterator._pos - _pos;
    }

    Vector_iterator& operator+=(difference_type n) noexcept {
        if(!is_reversed){
            if(_pos + n < _top) _pos += n;
            else _pos = _top;
        } else {
            if(_pos - n > -1) _pos -= n;
            else _pos = -1;
        }

        return *this;
    }

    Vector_iterator& operator-=(difference_type n) noexcept {
        if(is_reversed){
            if(_pos + n < _top) _pos += n;
            else _pos = _top;
        } else {
            if(_pos - n > -1) _pos -= n;
            else _pos = -1;
        }

        return *this;
    }

    reference operator[](difference_type n) const noexcept {
        if(!is_reversed){
            if(_pos + n < _top - 1) return _data[_pos + n];
            else return T();
        } else {
            if(_pos - n >= 0) return _data[_pos - n];
            else return T();
        }
    }


private:
    std::size_t _pos = 0;
    std::size_t _top = 0;
    pointer _data = nullptr;
};

template<Suitable T, typename Alloc>
class Vector{
public:
    using value_type = T;
    using reference = value_type&;
    using allocator_type = Alloc;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using iterator = Vector_iterator<T, false, false>;
    using const_iterator = Vector_iterator<T, true, false>;
    using reverse_iterator = Vector_iterator<T, false, true>;
    using const_reverse_iterator = Vector_iterator<T, true, true>;

    Vector() = default;

    explicit Vector(size_type size, const allocator_type& alloc = allocator_type()): _size(size), _top(size), _alloc(alloc) {
        _data = _alloc.allocate(size);
    }

    Vector(value_type def, size_type size): _size(size), _top(size), _data(_alloc.allocate(_size)){
        std::fill(_data, _data + _size, def);
    }

    Vector(std::initializer_list<value_type> list): _size(list.size()), _top(_size), _data(_alloc.allocate(_size)){
        std::copy(list.begin(), list.end(), _data);
    }

    Vector(const Vector<value_type>& other_vector) noexcept: _size(other_vector._size), _top(other_vector._top), _alloc(other_vector._alloc){
        _data = _alloc.allocate(_size);
        for(size_type i = 0; i < _top; i++) new(&_data[i]) value_type(other_vector._data[i]);
    }

    Vector(Vector<value_type>&& other_vector) noexcept : _size(other_vector._size), _top(other_vector._top), _data(other_vector._data) {
        other_vector._data = nullptr;
        other_vector._top = 0;
        other_vector._size = 0;
    }

    ~Vector(){
        clear();
        _alloc.deallocate(_data, _size);
    }

    Vector& operator=(const Vector<value_type>& other_vector) noexcept {
        if(this == &other_vector) return *this;

        _size = other_vector._size;
        _top = other_vector._top;
        _data = _alloc.allocate(_size);

        for(size_type i = 0; i < _top; i++) new(&_data[i]) value_type(other_vector._data[i]);

        return *this;
    }

    Vector& operator=(Vector<value_type>&& other_vector) noexcept {
        if(this == &other_vector) return *this;
        _size = other_vector._size;
        _top = other_vector._top;
        _data = other_vector._data;

        other_vector._data = nullptr;
        other_vector._top = 0;
        other_vector._size = 0;

        return *this;
    }

    iterator begin() noexcept {return Vector_iterator<value_type, false, false>(0, _top, _data); }
    const_iterator cbegin() const noexcept {return Vector_iterator<value_type, true, false>(0, _top, _data); }
    reverse_iterator rbegin() noexcept {return Vector_iterator<value_type, false, true>(_top - 1, _top, _data); }
    const_reverse_iterator crbegin() const noexcept {return Vector_iterator<value_type, true, true>(_top - 1, _top, _data); }

    iterator end() noexcept { return Vector_iterator<value_type, false, false>(_top, _top, _data); }
    const_iterator cend() const noexcept { return Vector_iterator<value_type, true, false>(_top, _top, _data); }
    reverse_iterator rend() noexcept { return Vector_iterator<value_type, false, true>(-1, _top, _data); }
    const_reverse_iterator crend() const noexcept { return Vector_iterator<value_type, true, true>(-1, _top, _data); }

    [[nodiscard]] size_type size() const noexcept { return _top; }
    [[nodiscard]] bool empty() const noexcept { return _top == 0; }

    void clear() noexcept {
        for(size_type i = 0; i < _top; i++) _data[i].~value_type();
        _top = 0;
    }
    void resize(size_type new_size){
        _resize(new_size);
        _top = new_size;
    }

    void assign(size_type n, const_reference elem) noexcept {
        _resize(n);
        for(size_type i = 0; i < n; i++) _data[i] = elem;
    }

    void assign(std::initializer_list<value_type> list) noexcept {
        _resize(list.size());

        for(size_type i = 0; i < list.size(); i++) _data[i] = std::move(list[i]);
    }

    reference at(size_type pos) {
        if(pos >= _top) throw std::out_of_range("index must be less then size of a vector");
        return *(begin() + pos);
    }
    const_reference at(size_type pos) const {
        if(pos >= _top) throw std::out_of_range("index must be less then size of a vector");
        return *(cbegin() + pos);
    }

    reference operator[](size_type pos) noexcept { return *(begin() + pos); }
    const_reference operator[](size_type pos) const noexcept { return *(cbegin() + pos); }

    reference front() { return *begin(); }
    const_reference front() const { return *cbegin(); }

    reference back() noexcept { return *(rbegin()); }
    const_reference back() const noexcept { return *(crbegin()); }

    iterator insert(const_iterator pos, const value_type& val) noexcept {
        if(_top == _size) realloc();

        for(size_type i = _top; i > pos._pos; i--) _data[i] = _data[i - 1];

        _data[pos._pos] = val;
        _top++;

        return iterator(pos._pos, _top, _data);
    }

    iterator insert(const_iterator pos, value_type&& val) noexcept {
        if(_top == _size) realloc();

        for(size_type i = _top; i > pos._pos; i--) _data[i] = std::move(_data[i - 1]);

        _data[pos._pos] = val;
        _top++;

        return iterator(pos._pos, _top, _data);
    }

    template<typename ...Args>
    iterator emplace(const_iterator pos, Args&&... args) noexcept {
        if(_size == _top) realloc();

        for(size_type i = _top; i > pos._pos; i--) _data[i] = std::move(_data[i-1]);

        new (&_data[pos._pos]) value_type(std::forward<Args>(args)...);
        _top++;

        return iterator(pos._pos, _top, _data);
    }

    iterator erase(iterator pos) noexcept {
        if (pos._pos >= _top) return pos;

        _data[pos._pos].~value_type();

        for(size_type i = pos._pos; i < _top - 1; i++) _data[i] = std::move(_data[i + 1]);
        _top--;

        return iterator(pos._pos, _top, _data);
    }

    iterator erase(const_iterator pos){
        if (pos._pos >= _top) return pos;

        _data[pos._pos].~value_type();

        for(size_type i = pos._pos; i < _top - 1; i++) _data[i] = std::move(_data[i + 1]);
        _top--;

        return iterator(pos._pos, _top, _data);
    }

    iterator erase(iterator first, iterator last) noexcept {
        if (first._pos >= _top) return first;

        if(last._pos >= _top) _top = first._pos;
        else {
            for (size_type i = first._pos; i < last._pos; i++) {
                _data[i].~value_type();
                _data[i] = std::move(_data[i + last._pos - first._pos]);
            }
            _top -= last._pos - first._pos;
        }

        return iterator(first._pos, _top, _data);
    }

    iterator erase(const_iterator first, const_iterator last) noexcept {
        if (first._pos >= _top) return first;

        if(last._pos >= _top) _top = first._pos;
        else {
            for (size_type i = first._pos; i < _top - (last._pos - first._pos); i++) {
                _data[i].~value_type();
                _data[i] = std::move(_data[i + last._pos - first._pos]);
            }
            _top -= last._pos - first._pos;
        }

        return iterator(first._pos, _top, _data);
    }

    void push_back(const_reference val) noexcept {
        if(_top == _size) realloc();

        _data[_top] = val;
        _top++;
    }

    void push_back(value_type&& val) noexcept {
        if(_top == _size) realloc();

        _data[_top] = std::move(val);
        _top++;
    }

    template<typename ...Args>
    reference emplace_back(Args&&... args) noexcept {
        if(_top == _size) realloc();

        new (&_data[_top]) value_type(std::forward<Args>(args)...);
        _top++;

        return _data[_top - 1];
    }

    void pop_back() noexcept {
        if(_top == 0) return;

        _data[--_top].~value_type();
    }

    constexpr void swap(Vector<value_type>& other_vector){
        if(this == &other_vector) return;

        pointer temp_data = _data;
        size_type temp_size = _size;
        size_type temp_top = _top;

        _data = other_vector._data;
        _top = other_vector._top;
        _size = other_vector._size;

        other_vector._data = temp_data;
        other_vector._top = temp_top;
        other_vector._size = temp_size;
    }

    friend bool operator==(const Vector<value_type>& vec1, const Vector<value_type>& vec2){
        if(vec1._top != vec2._top) return false;

        for(size_type i = 0; i < vec1._top; i++) if(vec1[i] != vec2[i]) return false;

        return true;
    }

    friend auto operator<=>(const Vector<value_type>& vec1, const Vector<value_type>& vec2) {
        if (vec1.size() != vec2.size()) {
            return vec1._data.size() <=> vec2._data.size();
        }

        for (size_t i = 0; i < vec1.size(); ++i) {
            if (auto res = vec1._data[i] <=> vec2._data[i]; res != 0) {
                return res;
            }
        }

        return std::weak_ordering::equivalent;
    }


private:
    size_type _size = 0;
    size_type _top = 0;
    pointer _data = nullptr;
    allocator_type _alloc;

    void realloc(){
        auto new_size = _size == 0 ? 1 : _size * 2;
        _resize(new_size);
    }

    void _resize(size_type new_size){
        if(new_size < _size){
            _top = new_size;
            return;
        }

        pointer new_data = _alloc.allocate(new_size);
        for(size_type i = 0; i < _top; i++){
            new (&new_data[i]) value_type(_data[i]);
            _data[i].~value_type();
        }

        _alloc.deallocate(_data, _size);
        _size = new_size;
        _data = new_data;
    }
};

#endif //LEETCODE_VECTOR_H
