
template <typename T>
struct base {
	//using value = typename T::value;
	typedef typename T::value value;
};

struct derived /*: public base<derived>*/ {
	//using value = int;
	typedef int value;
};

int main(int argc, char * argv[])
{
	derived d;
	return 0;
}