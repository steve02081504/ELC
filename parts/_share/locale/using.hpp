//zh_cn.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
locale说明
elc使用的字符串.
调整这些,实现快速本地化.
*/
#if !defined(ELC_LOCALE)
	#define ELC_LOCALE
	namespace elc::locale::func{

	}
	namespace elc::locale::str{
		auto const gc_fail="gc失败";
		auto const empty_gc_method="gc方法为空时gc被调用";
	}
#endif

//file_end

