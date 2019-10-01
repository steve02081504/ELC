//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	template<class T>
	class base_func_data_t;
	template<class Ret_t,class...Args_t>
    struct base_func_data_t<Ret_t(Args_t...)>:ref_able<base_func_data_t<Ret_t(Args_t...)>>,build_by_get_only{
		typedef base_func_data_t<Ret_t(Args_t...)>this_t;

		virtual ~base_func_data_t()=0;
		virtual Ret_t call(Args_t...)=0;
		//for equal:
		[[nodiscard]]virtual base_type_info_t&get_type_info()noexcept=0;
		[[nodiscard]]virtual void*get_data_begin()noexcept=0;
		[[nodiscard]]virtual bool equal_with(void*)=0;
		[[nodiscard]]bool operator==(const this_t&a){
			return this->get_type_info()==a.get_type_info()&&this->equal_with(a.get_data_begin());
		}
    };
	template<class Ret_t,class...Args_t>
	base_func_data_t<Ret_t(Args_t...)>::~base_func_data_t(){nothing};

	template<class T,class Func_t>
	class func_data_t;
	template<class T,class Ret_t,class...Args_t>
    struct func_data_t<T,Ret_t(Args_t...)>:base_func_data_t<Ret_t(Args_t...)>{
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;

		T _value;

		func_data_t(T&)noexcept(construct<T>.nothrow<T>):_value(a){}
		virtual ~func_data_t()noexcept(destruct.nothrow<T>)override=default;
		virtual Ret_t call(Args_t...args)override{
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<args...>)
				template_error("this T can\'t becall as args.");
			if constexpr(type_info<decltype(declvalue(T)(declvalue(Args_t)...))> != type_info<Ret_t>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			return _value(forward<Args_t>(args)...);
		}
		[[nodiscard]]virtual base_type_info_t&get_type_info()noexcept override{return type_info<T>;}
		[[nodiscard]]virtual void*get_data_begin()noexcept override{return&_value;}
		[[nodiscard]]virtual bool equal_with(void*a)noexcept override{
			if constexpr(not noexcept(declvalue(T)==declvalue(T)))
				template_warning("the compare of T was not noexcept,this may cause terminate.");
			return _value==*reinterpret_cast<T*>(a);
		}
    };

	template<class T>
	class default_func_data_t;
	template<class Ret_t,class...Args_t>
    struct default_func_data_t<Ret_t(Args_t...)>:base_func_data_t<Ret_t(Args_t...)>{
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;

		virtual ~default_func_data_t()noexcept override{}
		virtual Ret_t call(Args_t...)override{return Ret_t();}
    };
	template<class Ret_t,class...Args_t>
	constexpr default_func_data_t<Ret_t(Args_t...)>default_func_data{};
	template<class Ret_t,class...Args_t>
	[[nodiscard]]constexpr base_func_data_t<Ret_t(Args_t...)>*get_null_ptr(type_info_t<base_func_data_t<Ret_t(Args_t...)>>)noexcept{
		return&default_func_data<Ret_t(Args_t...)>;
	}

	template<class T,bool nothrow,bool promise_nothrow_destruct>
	class base_function_t;
	template<class Ret_t,class...Args_t,bool nothrow,bool promise_nothrow_destruct>
	class base_function_t<Ret_t(Args_t...),nothrow,promise_nothrow_destruct>{
		typedef base_function_t<Ret_t(Args_t...),nothrow,promise_nothrow_destruct>this_t;

		template<class T_>
		using func_data_t=function_n::func_data_t<T_,Ret_t(Args_t...)>;

		typedef comn_ptr_t<base_func_data_t<Ret_t(Args_t...)>>ptr_t;
		typedef Ret_t(*func_ptr_t)(Args_t...)noexcept(nothrow);

		ptr_t _m;
	public:
		base_function_t()noexcept=default;
		~base_function_t()noexcept(promise_nothrow_destruct)=default;
		void swap(this_t&a)noexcept{
			swap(_m,a._m);
		}
		Ret_t operator()(Args_t...args)const noexcept(nothrow){
			return _m->call(forward<Args_t>(args)...);
		}
		template<class func_t>
		this_t&operator=(func_t&&a)&noexcept(
		promise_nothrow_destruct&&(
		type_info<func_t>.can_convert_to<func_ptr_t>?
		type_info<func_t>.can_nothrow_convert_to<func_ptr_t>:
		get<func_data_t<::std::remove_cvref_t<func_t>>>.nothrow<func_t>;
		)){
			//BLOCK:constexpr checks
			if constexpr(promise_nothrow_destruct&&destruct.nothrow<func_t>)
				template_error("unexpected assign.");
			if constexpr(nothrow)
				if constexpr(!invoke<T>.nothrow<args...>)
					template_warning("the call of T was not noexcept,this may cause terminate.");
			//BLOCK_END
			if constexpr(type_info<func_t>.can_convert_to<func_ptr_t>){
				auto tmp=(func_ptr_t)(a);
				if(tmp)
					_m=get<func_ptr_t>(tmp);
				else
					_m=null_ptr;
			}else
				_m=get<func_data_t<::std::remove_cvref_t<func_t>>>(a);
			return*this;
		}
		this_t&operator=(const this_t&a)&noexcept(promise_nothrow_destruct){
			_m=a._m;
			return*this;
		}
		this_t&operator=(this_t&&a)&noexcept{
			swap(_m,a._m);
			return*this;
		}
		this_t&operator=(nullptr_t)noexcept(promise_nothrow_destruct){
			_m=null_ptr;
			return*this;
		}

		template<class func_t>
		base_function_t(func_t&&a)noexcept_as(declvalue(this_t)=declvalue(func_t)):base_function_t(){
			*this=(forward<func_t>(a));
		}

		template<bool nothrow_,bool promise_nothrow_destruct_>
		this_t&operator=(const base_function_t<Ret_t(Args_t...),nothrow_,promise_nothrow_destruct_>&a)&noexcept(promise_nothrow_destruct){
			if constexpr((nothrow&&!nothrow_)||
						(promise_nothrow_destruct&&!promise_nothrow_destruct_))
				template_error("unexpected assign.");
			_m=a._m;
			return*this;
		}

		template<bool nothrow_,bool promise_nothrow_destruct_>
		[[nodiscard]]bool operator==(const base_function_t<Ret_t(Args_t...),nothrow_,promise_nothrow_destruct_>&a)noexcept{
			return *_m==*(a._m);
		}
	private:
		//以下是突然想加的功能(没什么用<迷惑行为大赏>).
		static ptr_t _func_ptr_data;
		static Ret_t _func_ptr_value(Args_t...args)noexcept(nothrow){
			return _func_ptr_data->call(forward<Args_t>(args)...);
		}
	public:
		[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_destruct){
			_func_ptr_data=_m;
			return _func_ptr_value;
		}
	};

	template<class T>
	class function_t;
	template<class Ret_t,class...Args_t>
	struct function_t<Ret_t(Args_t...)noexcept>:base_function_t<Ret_t(Args_t...),true,true>{
		typedef function_t<Ret_t(Args_t...)noexcept>this_t;
		typedef base_function_t<Ret_t(Args_t...),true,true>base_t;
		using base_t::base_t;
		template<class assign_t>
		this_t&operator=(assign_t&&a)noexcept_as(declval(base_t)=declval(assign_t)){
			base_t::operator=(a);
			return*this;
		}
	};
	template<class Ret_t,class...Args_t>
	struct function_t<Ret_t(Args_t...)>:base_function_t<Ret_t(Args_t...),false,true>{
		typedef function_t<Ret_t(Args_t...)>this_t;
		typedef base_function_t<Ret_t(Args_t...),false,true>base_t;
		using base_t::base_t;
		template<class assign_t>
		this_t&operator=(assign_t&&a)noexcept_as(declval(base_t)=declval(assign_t)){
			base_t::operator=(a);
			return*this;
		}
	};
	template<class T>
	void swap(function_t<T>&a,function_t<T>&b)noexcept{
		a.swap(b);
	}

	/*
	template<class T>
	class may_throw_in_destruct_function_t;
	template<class Ret_t,class...Args_t>
	struct may_throw_in_destruct_function_t<Ret_t(Args_t...)noexcept>:base_function_t<Ret_t(Args_t...),true,false>{
		typedef may_throw_in_destruct_function_t<Ret_t(Args_t...)noexcept>this_t;
		typedef base_function_t<Ret_t(Args_t...),true,false>base_t;
		using base_t::base_t;
		template<class assign_t>
		this_t&operator=(assign_t&&a)noexcept_as(declval(base_t)=declval(assign_t)){
			base_t::operator=(a);
			return*this;
		}
	};
	template<class Ret_t,class...Args_t>
	struct may_throw_in_destruct_function_t<Ret_t(Args_t...)>:base_function_t<Ret_t(Args_t...),false,false>{
		typedef may_throw_in_destruct_function_t<Ret_t(Args_t...)noexcept>this_t;
		typedef base_function_t<Ret_t(Args_t...),false,false>base_t;
		using base_t::base_t;
		template<class assign_t>
		this_t&operator=(assign_t&&a)noexcept_as(declval(base_t)=declval(assign_t)){
			base_t::operator=(a);
			return*this;
		}
	};
	template<class T>
	void swap(may_throw_in_destruct_function_t<T>&a,may_throw_in_destruct_function_t<T>&b)noexcept{
		a.swap(b);
	}
	*/
}