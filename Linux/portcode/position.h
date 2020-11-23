#pragma once
#ifndef POSITION_CUSTOM_H
#define POSITION_CUSTOM_H

#include <memory>

//! In the ATL/MFC framework the POSISION struct basically points to a node on the inner structure.
//! In our case we want to use the iterator structure provided by standard C++ libraries.
//! To do that let's define a base interface from which each different structure will derive and provide
//! the necessary method to:
//! - create an iterator
//! - copy an iterator
struct __POSITION
{
	virtual ~__POSITION() {}
	virtual __POSITION* copy() = 0;
};
struct POSITION
{
    //! Iterator reference is unique within a POSITION structure. We want POSITION to work as if it was a
    //! raw pointer on a Node of the inner structure.:
    //! - if POSITION is passed by reference the instance of the POSITION will be modified;
    //! - if POSITION is passed by copy a new instance of the POSITION pointing to the same element is created;
    //! - if POSITION is assigned to another POISITION a copy a new instance of the POSITION pointing to the same element is created.
	typedef std::unique_ptr<__POSITION> POSITION_PTR;
	POSITION_PTR m_position;

	POSITION();
	POSITION(const POSITION& other);
	POSITION(const POSITION_PTR& other);
	POSITION(const std::nullptr_t& other);

	POSITION& operator = (const POSITION& other);
	POSITION& operator = (const POSITION_PTR& other);
	POSITION& operator = (const std::nullptr_t& other);

	bool operator == (const POSITION& other) const;
	bool operator != (const POSITION& other) const;

	const POSITION_PTR& get() const { return m_position; }
	POSITION_PTR& get() { return m_position; }
};

//! Simple extension method to do a dynamic_cast on an object wrapped in a unique_ptr in a single line.
namespace ext
{
	template <typename _Tp, typename _Up>
	inline _Tp* dynamic_unique_cast(const std::unique_ptr<_Up> &__r) noexcept
	{
		using _Sp = std::unique_ptr<_Tp>;
		if (auto *__p = dynamic_cast<typename _Sp::element_type *>(__r.get()))
			return __p;
		return nullptr;
	}
} // namespace

#endif // POSITION_CUSTOM_H

