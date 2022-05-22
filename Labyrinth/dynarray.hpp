#pragma once
#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <initializer_list>
#include <stdexcept>

template<typename _Type>
class dynarray
{
public:
	using value_type = _Type;
	using pointer = _Type * ;
	using const_pointer = const _Type *;
	using reference = _Type & ;
	using const_reference = const _Type&;
	using size_type = unsigned int;

	explicit dynarray(size_type count) :
		_Value(new value_type[count]),
		_Size(count)
	{
	}

	explicit dynarray(size_type count, const value_type& v) :
		_Value(new value_type[count]),
		_Size(count)
	{
		fill(v);
	}

	dynarray(std::initializer_list<value_type> init) :
		_Value(new value_type[init.size()]),
		_Size(init.size())
	{
		int i = 0;
		for (auto & e : init)
		{
			_Value[i] = e;
			i++;
		}
	}

	~dynarray()
	{
		delete[] _Value;
	}

	dynarray(dynarray<value_type>&& arr) noexcept :
		_Value(std::exchange(arr._Value, nullptr)),
		_Size(std::exchange(arr._Size, 0U))
	{
	}

	dynarray(const dynarray<value_type>& arr) :
		_Value(new value_type[arr._Size]),
		_Size(arr._Size)
	{
		std::copy(arr._Value, arr._Value + _Size, _Value);
	}

	dynarray<value_type>& operator=(dynarray<value_type>&& arr) noexcept
	{
		if (this != &arr)
		{
			_Size = std::exchange(arr._Size, 0U);
			_Value = std::exchange(arr._Value, nullptr);
		}
		return *this;
	}

	dynarray<value_type>& operator=(const dynarray<value_type>& arr)
	{
		if (this != &arr)
		{
			_Value = new value_type[arr._Size];
			_Size = arr._Size;
			std::copy(arr._Value, arr._Value + _Size, _Value);
		}
		return *this;
	}

	inline reference at(size_type index)
	{
		if (!(index < _Size)) throw std::out_of_range("Index out of range.");
		return _Value[index];
	}

	constexpr const_reference at(size_type index) const
	{
		if (!(index < _Size)) throw std::out_of_range("Index out of range.");
		return _Value[index];
	}

	inline reference operator[](size_type index)
	{
		if (!(index < _Size)) throw std::out_of_range("Index out of range.");
		return _Value[index];
	}

	constexpr const_reference operator[](size_type index) const
	{
		if (!(index < _Size)) throw std::out_of_range("Index out of range.");
		return _Value[index];
	}

	inline reference front() noexcept
	{
		return _Value[0];
	}

	constexpr const_reference front() const noexcept
	{
		return _Value[0];
	}

	inline reference back() noexcept
	{
		return _Value[_Size - 1];
	}

	constexpr const_reference back() const noexcept
	{
		return _Value[_Size - 1];
	}

	inline pointer data() noexcept
	{
		return _Value;
	}

	constexpr const_pointer data() const noexcept
	{
		return _Value;
	}

	constexpr bool empty() const noexcept
	{
		return _Size == 0;
	}

	constexpr size_type size() const noexcept
	{
		return _Size;
	}

	void fill(const value_type& v)
	{
		std::fill(begin(), end(), v);
	}

	inline pointer begin() noexcept
	{
		return _Value;
	}

	inline pointer end() noexcept
	{
		return _Value + _Size;
	}

	constexpr const_pointer begin() const noexcept
	{
		return _Value;
	}

	constexpr const_pointer end() const noexcept
	{
		return _Value + _Size;
	}

	constexpr const_pointer cbegin() const noexcept
	{
		return _Value;
	}

	constexpr const_pointer cend() const noexcept
	{
		return _Value + _Size;
	}

	void resize(size_type size)
	{
		value_type * _Old = _Value;
		_Value = new value_type[size];
		for (size_type i = (_Size > size ? size : _Size); i > 0; i--)
			_Value[i - 1] = _Old[i - 1];
		_Size = size;
		delete[] _Old;
	}
private:
	value_type * _Value;

	size_type _Size;
};

template<typename T>
bool operator==(const dynarray<T>& lhs, const dynarray<T>& rhs)
{
	if (lhs.size() != rhs.size()) return false;
	for (size_t i = 0; i < lhs.size(); i++)
	{
		if (lhs[i] != rhs[i]) return false;
	}
	return true;
}

template<typename T>
bool operator!=(const dynarray<T>& lhs, const dynarray<T>& rhs)
{
	if (lhs.size() != rhs.size()) return true;
	for (size_t i = 0; i < lhs.size(); i++)
	{
		if (lhs[i] != rhs[i]) return true;
	}
	return false;
}

template<typename T1, typename T2>
bool operator==(const dynarray<T1>&, const dynarray<T2>&)
{
	return false;
}

template<typename T1, typename T2>
bool operator!=(const dynarray<T1>&, const dynarray<T2>&)
{
	return true;
}

#endif