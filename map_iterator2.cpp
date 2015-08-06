template <class Container>
class MapIterator
{
public:
	typedef ... ContainerKeyType;
	typedef ... ContainerValueType;

	typedef if Container const ValueType is 
		ContainerKeyType, ContainerValueType const
	else
		ContainerKeyType, ContainerType

	


}


template <typename Key, typename Value>
struct MapPair
{
	Key first;
	Value second;
	MapPair(Key key, Value value);
}

