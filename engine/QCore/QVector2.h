#pragma once
#ifndef QVECTOR2_H
#define QVECTOR2_H

template<typename T>
class QVector2
{
public:
	QVector2(T x, T y) { _x = x; _y = y; }
	~QVector2() {}

	QVector2 operator+(T value) 
	{
		_x = _x + value;
		_y = _y + value;
	}

	QVector2 operator+(QVector2* value)
	{
		_x = _x + value->_x;
		_y = _y + value->_y;
	}

	QVector2 operator-(T value)
	{
		_x = _x - value;
		_y = _y - value;
	}

	QVector2 operator-(QVector2* value)
	{
		_x = _x - value->_x;
		_y = _y - value->_y;
	}

	QVector2 operator*(T value)
	{
		_x = _x * value;
		_y = _y * value;
	}

	QVector2 operator*(QVector2* value)
	{
		_x = _x * value->_x;
		_y = _y * value->_y;
	}

	QVector2 operator/(T value)
	{
		_x = _x / value;
		_y = _y / value;
	}

	QVector2 operator/(QVector2* value)
	{
		_x = _x / value->_x;
		_y = _y / value->_y;
	}

	QVector2 reverse()
	{
		_x = _x * -1;
		_y = _y * -1;
	}
protected:
	T _x;
	T _y;
};

#endif