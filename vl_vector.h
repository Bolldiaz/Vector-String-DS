#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#include <algorithm>
#include <iterator>

#define DEFAULT_DYN_CAP 0
#define DEFAULT_STAT_CAP 16
#define DEFAULT_VEC_SIZE 0
#define GROWTH_FACTOR 3/2

template<typename T, size_t CAP = DEFAULT_STAT_CAP>
class vl_vector {
 protected:
  T _stat[CAP];
  T* _cur;
  T* _dyn;
  size_t _size, _DynamicCapacity, _StaticCapacity;

  /**
   * check if the vector is full, if does resize it
   */
  void increase_by_k (size_t k = 1)
  {
    if (_size + k > _StaticCapacity && _size <= _StaticCapacity)
      {
        _DynamicCapacity = (_size + k) * GROWTH_FACTOR;
        _dyn = new T[_DynamicCapacity];
        std::copy (begin (), begin () + _size, _dyn);
        _cur = _dyn;
      }

    else if (_size + k > _DynamicCapacity &&
    _DynamicCapacity != DEFAULT_DYN_CAP)
      {
        _DynamicCapacity = (_size + k) * GROWTH_FACTOR;
        _dyn = new T[_DynamicCapacity];
        std::copy (begin (), begin () + _size, _dyn);
        delete[] _cur;
        _cur = _dyn;
      }
    _size += k;
  }

  /**
   * check if the vector should move from dynamic to static, if does resize it
   */
  void back_to_static (size_t k = 1)
  {
    _size -= k;
    if (_size <= _StaticCapacity && _size + k > _StaticCapacity)
      {
        _DynamicCapacity = 0;
        std::copy (begin (), begin () + _size, _stat);
        delete[] _dyn;
        _dyn = nullptr;
        _cur = _stat;
      }
  }

 public:

  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator <iterator>;
  using const_reverse_iterator = std::reverse_iterator <const_iterator>;

  /**
   * Default Ctor
   */
  vl_vector () :  _cur(_stat), _dyn(nullptr), _size (DEFAULT_VEC_SIZE),
                  _DynamicCapacity (DEFAULT_DYN_CAP), _StaticCapacity (CAP) {}

  /**
   * Copy Ctor
   * @param oth another vl_vector object
   */
  vl_vector (const vl_vector<T,CAP> &oth) : _cur(nullptr),_dyn(nullptr),
  _size (oth._size), _DynamicCapacity (oth._DynamicCapacity),
  _StaticCapacity (CAP)
  {
    if (_size > _StaticCapacity)
      {
        _dyn = new T[_DynamicCapacity];
        _cur = _dyn;
      }
    else { _cur = _stat; }
    std::copy (oth.begin (), oth.begin () + oth._size, this->begin());
  }

  /**
   * Construct the vector by inserting all the items n the range firs-last
   * @tparam InputIterator InputIterator of elements type T
   * @param first the InputIterator to the first element of the range
   * @param last the InputIterator to the last element of the range
   */
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last) : vl_vector ()
  {
    size_t k = std::distance (first, last);
    increase_by_k (k);
    std::copy (first, last, this->begin ());
  }

  /**
   * Construct the vector by inserting the element v, count times
   * @param count number of elements to insert
   * @param v T element to insert
   */
  vl_vector (size_t count, const T &v) : vl_vector ()
  {
    increase_by_k (count);
    std::fill (begin (), begin() + size(), v);
  }

  /**
   * Dtor
   */
  virtual ~vl_vector ()
  { delete[] _dyn; }


  // Methods:
  /**
   * @return the size of the vl_vector
   */
  virtual size_t size () const
  { return _size; }

  /**
   * @return the Capacity of the vl_vector
   */
  size_t capacity () const
  {
    return _size > _StaticCapacity ? _DynamicCapacity : _StaticCapacity;
  }

  /**
   * check if the vector is empty
   * @return True if is, else False
   */
  bool empty () const
  { return size() == 0; }

/**
 * at method
 * @param index of the desired element in the vector
 * @return the T element if index in range, else throw exception
 */
  T& at (size_t index)
  {
    if (index < 0 || index >= _size)
      throw std::out_of_range ("Index not in range of [0,_size).");
    return _cur[index];
  }

  /**
   * const version of at method
   * @param index of the desired element in the vector
   * @return the T element if index in range, else throw exception
   */
  const T& at (size_t index) const
  {
    if (index < 0 || index >= _size)
      throw std::out_of_range ("Index not in range of [0,_size).");
    return _cur[index];
  }

  /**
   * push the new element at the end of the vector. if got full resize it
   * @param new_elem new element of type T
   */
  void push_back (const T &new_elem)
  {
    // insert the value on the last index, after the size got updated
    insert(begin() + size(), new_elem);
  }

  /**
   * insert new element before the element -it- is pointing on
   * @param it iterator of vl_vector
   * @param new_elem new element of type T
   * @return iterator which points the new value in the vector
   */
  iterator insert (const_iterator it, const T &new_elem)
  {
    size_t dist = it - _cur;
    // check if the insertion will fill-up the vector, and resize if necessary
    increase_by_k ();

    std::move_backward (_cur + dist, _cur + _size - 1, _cur + _size);
    *(_cur + dist) = new_elem;
    return _cur + dist;
  }

  /**
   * insert k new elements before the element -it- is pointing on
   * @tparam InputIterator
   * @param it iterator of vl_vector
   * @param first an InputIterator points the first value to insert
   * @param last an InputIterator points the end of values to insert
   * @return iterator which points the first new value inserted of the all k
   */
  template<class InputIterator>
  iterator insert (const_iterator it, InputIterator first, InputIterator last)
  {
    size_t dist = it - _cur;

    // check if the insertion will fill-up the vector, and resize if necessary
    size_t k = std::distance (first, last);
    increase_by_k (k);

    // hist right all the elements from -it-, by k indexes:
    std::move_backward (_cur + dist, _cur + _size - k, _cur + _size);

    // the insertion of the k elements:
    std::copy (first, last, _cur + dist);

    return _cur + dist;
  }

  /**
   * pop the last element. if empty - do nothing
   */
  void pop_back ()
  {
    if (!size())
      return;
    // check if the pop will fill-up the vector, and resize if necessary
    erase(begin() + size() -1);
  }

  /**
   * erase the element -it- is pointing on
   * @param it iterator of vl_vector
   * @param new_elem new element of type T
   * @return iterator which points the element on the right of the deleted elem
   */
  iterator erase (const_iterator it)
  {
    size_t dist = it - _cur;

    // hist left all the elements from -it- + 1, by 1 index:
    std::move (_cur + dist + 1,  _cur + _size, _cur + dist);

    // check if the erasing make the vector change its array location
    back_to_static ();

    return _cur + dist;
  }

  /**
   * erase all the elements in range: first->last on the vector
   * @param first points the first element in the range to delete
   * @param last points after the last element in the range to delete
   * @return iterator which point the element on the right of the deleted range
   */
  iterator erase (const_iterator first, const_iterator last)
  {
    size_t dist = first - _cur, k = last - first;

    // hist left all the elements from last to end, by k = last-end indexes:
    std::move (_cur + dist + k , _cur + _size, _cur + dist);

    // check if the erasing make the vector change its array location
    back_to_static (k);

    return _cur + dist;
  }

  /**
   * clear the vector from all it's elements
   */
  virtual void clear ()
  { back_to_static (size()); }

  /**
   * regular version
   * @return pointer to the begining of the vector array
   */
  T *data ()
  { return _cur; }

  /**
   * const version
   * @return pointer to the begining of the vector array
   */
  T *data () const
  { return _cur; }

  /**
   * check if T element in the vector
   * @param elem T element to look for
   * @return True if in vector, else False
   */
  bool contains (const T &elem) const
  { return end () != std::find (begin (), end (), elem); }

  // iterators:
  /**
   * iterator of a vl_vector
   * @return iterator point the start of the container
   */
  iterator begin ()
  { return _cur; }

  /**
   * iterator of a vl_vector
   * @return iterator point the end of the container
   */
  iterator end ()
  { return _cur + _size; }

  /**
   * const-iterator of a const vl_vector
   * @return iterator point the start of the container
   */
  const_iterator begin () const
  { return _cur; }

  /**
   * const-iterator of a const vl_vector
   * @return iterator point the end of the container
   */
  const_iterator end () const
  { return _cur + _size; }

  /**
   * const-iterator of a non-const vl_vector
   * @return iterator point the start of the container
   */
  const_iterator cbegin () const
  { return _cur; }

  /**
   * const-iterator of a non-const vl_vector
   * @return iterator point the end of the container
   */
  const_iterator cend () const
  { return _cur + _size; }

  // reverse-iterators:
  /**
   * reverse-iterator of the vl_vector
   * @return reverse-iterator point the end of the container
   */
  reverse_iterator rbegin ()
  { return reverse_iterator (this->end ()); }

  /**
   * reverse-iterator of the vl_vector
   * @return reverse-iterator point the start of the container
   */
  reverse_iterator rend ()
  { return reverse_iterator (this->begin ()); }

  /**
   * const-reverse-iterator of a const vl_vector
   * @return const-reverse-iterator point the end of the container
   */
  const_reverse_iterator rbegin () const
  { return const_reverse_iterator (this->end ()); }

  /**
   * const-reverse-iterator of a const vl_vector
   * @return const-reverse-iterator point the start of the container
   */
  const_reverse_iterator rend () const
  { return const_reverse_iterator (this->begin ()); }

  /**
   * const-reverse-iterator of a non-const vl_vector
   * @return const-reverse-iterator point the end of the container
   */
  const_reverse_iterator crbegin () const
  { return const_reverse_iterator (this->end ()); }

  /**
   * const-reverse-iterator of a non-const vl_vector
   * @return const-reverse-iterator point the start of the container
   */
  const_reverse_iterator crend () const
  { return const_reverse_iterator (this->begin ()); }


  // operators:
  /**
   * @param oth another vl_vector
   * @return this vl_vector by reference
   */
  virtual vl_vector<T, CAP> &operator= (const vl_vector<T, CAP>& oth)
  {
    if (this == &oth)
      return *this;
    _size = DEFAULT_VEC_SIZE;
    increase_by_k (oth._size);
    std::copy (oth.begin (), oth.end (), this->begin ());
    return *this;
  }

/**
 * subscript operator
 * @param index of the desired element in the vector
 * @return the #index T element in the vector
 */
  T &operator[] (size_t index)
  {
    return _cur[index];
  }

  /**
   * const version - subscript operator
   * @param index of the desired element in the vector
   * @return the #index T element in the vector
   */
  const T &operator[] (size_t index) const
  {
    return _cur[index];
  }

  /**
   * check if two vectors, has same size, and same elements by same order
   * @param oth another vl_vector
   * @return True if equal, else False
   */
  bool operator== (const vl_vector<T, CAP> &oth) const
  {
    if (this->_size != oth._size)
      return false;
    for (size_t i=0; i < _size; i++)
      if (_cur[i] != oth[i])
        return false;
    return true;
  }

  /**
   * check if two vectors, hasn't same size, or same elements by same order
   * @param oth another vl_vector
   * @return True if different, else False
   */
  bool operator!= (const vl_vector<T, CAP> &oth) const
  { return !(*this == oth); }
};

#endif //_VL_VECTOR_H_
