#pragma once
#ifndef QVECTOR3_H
#define QVECTOR3_H

template<typename T>
class QVector3
{
public:
	QVector3(T x, T y, T z) { _x = x; _y = y; _z = z; }
	~QVector3() {}

	QVector3 operator+(T value)
	{
		_x = _x + value;
		_y = _y + value;
		_z = _z + value;
	}

	QVector3 operator+(QVector3* value)
	{
		_x = _x + value->_x;
		_y = _y + value->_y;
		_z = _z + value->_z;
	}

	QVector3 operator-(T value)
	{
		_x = _x - value;
		_y = _y - value;
		_z = _z - value;
	}

	QVector3 operator-(QVector3* value)
	{
		_x = _x - value->_x;
		_y = _y - value->_y;
		_z = _z - value->_z;
	}

	QVector3 operator*(T value)
	{
		_x = _x * value;
		_y = _y * value;
		_z = _z * value;
	}

	QVector3 operator*(QVector3* value)
	{
		_x = _x * value->_x;
		_y = _y * value->_y;
		_z = _z * value->_z;
	}

	QVector3 operator/(T value)
	{
		_x = _x / value;
		_y = _y / value;
		_z = _z / value;
	}

	QVector3 operator/(QVector3* value)
	{
		_x = _x / value->_x;
		_y = _y / value->_y;
		_z = _z / value->_z;
	}

	QVector3 reverse()
	{
		_x = _x * -1;
		_y = _y * -1;
		_z = _z * -1;
	}
protected:
	T _x;
	T _y;
	T _z;
};

#endif