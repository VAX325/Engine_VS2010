#pragma once
#ifndef NVARRAY_H
#define NVARRAY_H

template< class T1, class T2 >
class VArray2
{
public:
	T1 first;
	T2 second;
};

template< class T1, class T2, class T3, class T4 >
class VArray4
{
public:
	T1 first;
	T2 second;
	T3 third;
	T4 four;
};

#endif