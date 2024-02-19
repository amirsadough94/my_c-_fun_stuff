#include <iostream>
#include <cstddef>

template<typename T, typename IndexType>
struct IndexHolder
{
	IndexHolder(IndexType i_index):index(i_index){}
	
	IndexType get()
	{
		return index;
	}
private:
	IndexType index;
};

template<typename T, auto Index>
class Pair
{
public:
	template<typename IndexType>
	using IndexHolderType = IndexHolder<T,IndexType>;

	static constexpr auto IndexValue = Index;
};

template<typename IndexType,typename... Pair>
class TypedMap: private Pair::template IndexHolderType<IndexType>...
{
public:
	TypedMap(): Pair::template IndexHolderType<IndexType>(Pair::IndexValue)...{}
 
	template<typename T>
	IndexType get_index()
	{
		return IndexHolder<T,IndexType>::get();
	}
};

int main()
{
	TypedMap<std::size_t,Pair<int,1>,Pair<char,2>,Pair<double,10>> mp;
    
	std::cout << "index of int is " <<  mp.get_index<int>() << std::endl;
	std::cout << "index of double is " <<  mp.get_index<double>() << std::endl;
	
  return 0;
}
