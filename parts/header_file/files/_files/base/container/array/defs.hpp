//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace array_n{
	template<typename T>
	class array_t{
		typedef array_t<T>this_t;
		T*_m;

		/*return {ptr};*/
		constexpr array_t(T*a):_m(a){}
		/*返回一个自身的副本*/
		template<enable_if(copy_get.able<T>)>
		[[nodiscard]]this_t&& copy()const noexcept(copy_get.nothrow<T>){
			return{copy_get(_m)};
		}
	public:
		/*默认构造*/
		constexpr array_t():_m(null_ptr){}
		/*
		默认构造size个T
		*/
		explicit array_t(note::size_t<size_t>size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size.value]();
		}
		/*
		此重载适用于T[N]，std::init_list<T>以及range_t<const T*>
		*/
		template<class T_,enable_if(get<T>.as_array.template able<T_>)>
		array_t(T_&&a)noexcept(get<T>.as_array.template nothrow<T_>){
			_m=get<T>.as_array(forward<T_>(a));
		}

		~array_t()noexcept(unget.nothrow<T>){
			unget(_m);
		}
		[[nodiscard]]size_t size()const noexcept{
			return get_size_of_get(_m);
		}
		void resize(size_t size)noexcept(get_resize.nothrow<T>){
			get_resize(_m,size);
		}
		[[nodiscard]]T&operator[](size_t size)noexcept{return _m[size];}
		[[nodiscard]]const T&operator[](size_t size)const noexcept{return _m[size];}
		[[nodiscard]]explicit operator hash_t()noexcept{return hash(_m,size());}

		typedef iterator_t<T>iterator;
		typedef iterator_t<const T>const_iterator;

		[[nodiscard]]constexpr iterator get_iterator_at(size_t a)noexcept{
			return _m+a;
		}
		[[nodiscard]]constexpr iterator begin()noexcept{
			return get_iterator_at(zero);
		}
		[[nodiscard]]iterator end()noexcept{
			return get_iterator_at(size());
		}
		[[nodiscard]]const_iterator cbegin()const noexcept{
			return const_cast<this_t*>(this)->begin();
		}
		[[nodiscard]]const_iterator cend()const noexcept{
			return const_cast<this_t*>(this)->end();
		}

		[[nodiscard]]bool empty()const{
			return _m==null_ptr;
		}

		void swap_with(this_t&a)noexcept{
			swap(_m,a._m);
		}

		#define expr declvalue(this_t).copy()
		array_t(const this_t&a)noexcept_as(expr):array_t(a.copy()){}
		this_t&operator=(const this_t&a)&noexcept_as(expr){
			return operator=(a.copy());
		}
		#undef expr
		array_t(this_t&&a)noexcept:array_t(){swap_with(a);}
		this_t&operator=(this_t&&a)&noexcept{
			swap_with(a);
			return*this;
		}

		void clear()noexcept(re_construct.nothrow<this_t>){
			re_construct(this);
		}

		#define expr declvalue(func_t)(declvalue(T&))
		template<typename func_t,enable_if_not_ill_from(expr)>
		void for_each(func_t&&func)noexcept_as(expr){
			auto asize=size();
			while(asize--)
				func((*this)[asize]);
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(const T&))
		template<typename func_t,enable_if_not_ill_from(expr)>
		void for_each(func_t&&func)const noexcept_as(expr){
			auto asize=size();
			while(asize--)
				func((*this)[asize]);
		}
		#undef expr
	
		void add(T a){
			get<T>.apply_end(note::to(_m),a);
		}
		void remove(T a){
			get<T>.remove(a,note::form(_m));
		}
	};
	template<typename T>
	inline void swap(array_t<T>&a,array_t<T>&b)noexcept{a.swap_with(b);}
}

//file_end

