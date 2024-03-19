#include<tuple>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <optional>

struct NoneValue {
    constexpr NoneValue(){}
};

template<std::size_t Size>
struct LiteralString
{
	constexpr LiteralString(const char (&str)[Size])
	{
		std::copy_n(str,Size,value);
	}

	char value[Size];
};

template<LiteralString Key, auto Value = NoneValue{} >
struct LiteralPair
{
	static constexpr auto key = Key;
	static constexpr decltype(Value) value = Value;
};

template<typename KeyType, typename... Pairs>
struct LiteralMap
{
private:
    template<LiteralString Key>
    struct LiteralKeyValue
    {
        LiteralKeyValue(KeyType&& i_value):value(i_value){}
        LiteralKeyValue(NoneValue i_value){}

        static constexpr auto key = Key;
        std::optional<KeyType> value;
    };
public:

	 LiteralMap():map(Pairs::value...){}

	 std::tuple<LiteralKeyValue<Pairs::key>...> map;

	 template<LiteralString Key>
	 KeyType get(KeyType&& default_value = KeyType()){
	 	return std::get<LiteralKeyValue<Key>>(map).value.value_or(default_value);
	 }
    
    template<LiteralString Key>
    void set(KeyType&& value ){
        return std::get<LiteralKeyValue<Key>>(map).value = value;
    }
    
    template<LiteralString Key>
    bool contains(){
        return std::get<LiteralKeyValue<Key>>(map).value.has_value();
    }
    
    template<LiteralString Key>
    void erase(){
        return std::get<LiteralKeyValue<Key>>(map).value.reset();
    }
};

int main()
{
	LiteralMap<std::size_t,LiteralPair<"a",1>,LiteralPair<"b",2>,LiteralPair<"c",3>,LiteralPair<"d"> > map;

	std::cout << "size of items stored is " <<  std::tuple_size<decltype(map.map)>::value << std::endl;

	std::cout << "the value of a is " << map.get<"a">() << std::endl;
	std::cout << "the value of b is " << map.get<"b">() << std::endl;
	std::cout << "the value of c is " << map.get<"c">() << std::endl;
    	std::cout << "the value of d is " << map.get<"d">(100) << std::endl;
    	map.erase<"a">();
    	std::cout << "has value of a after erasing " << map.contains<"a">() << std::endl;
    
    	std::cout << "itterating over all compiled keys... \n";
	// you can also make more beautiful ;)
	std::apply([](auto&&... args) {((std::cout << args.key.value << " => " << args.value.value_or(100) << '\n'), ...);}, map.map);

	return 0;
};
