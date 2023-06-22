#ifndef RT_API_HPP
#define RT_API_HPP

#ifdef SHARED 

#ifdef RT_EXPORTS
#define RT_API __declspec(dllexport)
#else
#define RT_API __declspec(dllimport)
#endif

#else

#define RT_API 

#endif // SHARED 

#endif // RT_API_HPP
