//_body.hpp
//at namespace elc::defs::exception_n
/*
未完成的elc解释器base_exception文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct exception{
	virtual ~exception()noexcept=0;
	virtual const char_t*what()noexcept{return U"unknow error.";}
};
inline constexpr exception::~exception()noexcept{}

//file_end

