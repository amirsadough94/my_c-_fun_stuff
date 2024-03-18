#include<tuple>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>

template<std::size_t Size>
struct LiteralString
{
	constexpr LiteralString(const char (&str)[Size])
	{
		std::copy_n(str,Size,value);
	}

	char value[Size];
};

template<LiteralString Key, auto Value>
struct LiteralPair
{
	static constexpr auto key = Key;
	static constexpr auto value = Value;
};

template<LiteralString Key,typename Value>
struct LiteralKeyValue
{
	LiteralKeyValue(Value&& i_value):value(i_value){}
	
	static constexpr auto key = Key;
	Value value;
};


template<typename KeyType, typename... Pairs>
struct LiteralMap
{

	 LiteralMap():map(Pairs::value...){}

	 std::tuple<LiteralKeyValue<Pairs::key,KeyType>...> map;

	 template<LiteralString Key>
	 KeyType get(){
	 	return std::get<LiteralKeyValue<Key,KeyType>>(map).value;
	 }


	
};



int main()
{
	LiteralMap<std::size_t,LiteralPair<"a",1>,LiteralPair<"b",2>,LiteralPair<"c",3>> map;

	std::cout << "size of items stored is " <<  std::tuple_size<decltype(map.map)>::value << std::endl;

	std::cout << "the value of a is " << map.get<"a">() << std::endl;
	std::cout << "the value of b is " << map.get<"b">() << std::endl;
	std::cout << "the value of c is " << map.get<"c">() << std::endl;

	// you can also make more beautiful ;)
	std::apply([](auto&&... args) {((std::cout << args.key.value << " => " << args.value << '\n'), ...);}, map.map);

	return 0;
};