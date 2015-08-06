#include <cstddef>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <cassert>

typedef bool BOOL32;
#define _ASSERT(cond) assert(cond)


namespace AnyDetail
{
	class Placeholder
	{
	public:
		virtual ~Placeholder() {}
		virtual Placeholder * Clone() const = 0;
	};

	template <typename T>
	struct Holder : public Placeholder
	{
	public:
		T held;

		Holder( T const & value ) : held(value) {}
		Placeholder * Clone() const {return new Holder<T>( held );}

	private:
		Placeholder & operator=( Holder const & );
	};
};


/*! Pre podrobné info pozri boost::any.
\note Neimplementuje Type() funkcionalitu. */
class Any
{
public:
	Any();
	
	template <typename T>
	Any( T const & value ) 
		: m_content(new AnyDetail::Holder<T>(value)) 
	{}

	Any( Any const & other )
		: m_content(other.m_content ? other.m_content->Clone() : NULL)
	{}

	~Any() {delete m_content; m_content = NULL;}

	Any & Swap( Any & rhs )
	{
		std::swap( m_content, rhs.m_content );
		return *this;
	}

	template <typename T>
	Any & operator=( T const & rhs )
	{
		Any(rhs).Swap(*this);
		return *this;
	}

	Any & operator=( Any rhs )
	{
		rhs.Swap(*this);
		return *this;
	}

	BOOL32 Empty() const {return !m_content;}

private:
	template <typename R>
	friend R * AnyCast(Any *);

private:
	AnyDetail::Placeholder * m_content;	
};

template <typename R>
R * AnyCast( Any * operand )
{
	/* tu dopln test operand->Type() == typeid(R) */
	return operand ?
		&static_cast<AnyDetail::Holder<R> *>(operand->m_content)->held : 0;
}

template <typename R>
inline R const * AnyCast( Any const * operand )
{
	return AnyCast<R>( const_cast<Any *>(operand) );
}

/*! \note Ak sa nepodarí pretypovať, tak dôjde k dereferencii null-pointra
(pretože nepouzívame vínimky, takze sa to nedá transparentne ošetriť). */
template <typename R>
R AnyCast( Any & operand )
{
	typedef typename std::remove_reference<R>::type nonref;
	nonref * result = AnyCast<nonref>( &operand );

	_ASSERT( result && "error: nepodarilo sa pretypovat" );

	return *result;
}

template <typename R>
inline R AnyCast( Any const & operand )
{
	typedef typename std::remove_reference<R>::type nonref;
	return AnyCast<nonref const &>( const_cast<Any &>(operand) );
}


// super jednoduchý test
int main(int argc, char * argv[])
{
	Any a(137);

	int * pa = AnyCast<int>(&a);
	assert(*pa == 137);
	std::cout << "*pa=" << *pa << "\n";

	int & ra = AnyCast<int &>(a);
	assert(ra == 137);
	std::cout << "ra=" << ra << "\n";

	++ra;
	int incremented = AnyCast<int>(a);  // incremented = 138
	assert(ra == 138);
	std::cout << "incremented=" << incremented << "\n";

	// const
	Any const & ca = a;

	int const & cra = AnyCast<int const &>(ca);
	assert(cra == 138);
	std::cout << "cra=" << cra << "\n";

	return 0;
};

