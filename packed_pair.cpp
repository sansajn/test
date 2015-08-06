template <typename T1, typename T2, bool = is_empty<T2>::value>
class packed_pair
{
public:
	packed_pair() {}
	packed_pair(T1 t1, T2 t2);

	T1 & first() {return _t1;}
	T1 const & first() const {return _t1;}
	T2 & second() {return _t2;}
	T2 const & second() const {return _t2;}

private:
	T1 _t1;
	T2 _t2;
};


template <typename T1, typename T2, bool = is_empty<T2>::value>
class packed_pair<T1, T2, true>
	: private T2
{
public:
	
private:
	T1 _t1;
};
