template <typename T>
struct A
{};

template <typename T, template<class> class C>
struct B
{};

/*template <typename T, typename C>
void foo(C<T>)  // error: 'C' is not a template
{}*/

template <typename T, template<class> class C>
void foo(C<T>)  // ok
{}

/*template <typename T, typename C>
void boo(B<T, C>)  // error, kompilátor musí vedieť, že ide o šablónu
{}*/

template <typename T, template<class> class C>
void boo(B<T, C>)  // ok
{}

int main()
{
	return 0;
}

