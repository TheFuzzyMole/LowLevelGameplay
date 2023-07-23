#pragma once
#include <Core/Vector2.h>
#include <Core/Vector3.h>

using LLGP::Vector2;

template<typename T> requires arithmetic<T>
const Vector2<T> Vector2<T>::zero((T)0, (T)0);

template<typename T> requires arithmetic<T>
const Vector2<T> Vector2<T>::one((T)1, (T)1);

template<typename T> requires arithmetic<T>
const Vector2<T> Vector2<T>::right((T)1, (T)0);

template<typename T> requires arithmetic<T>
const Vector2<T> Vector2<T>::left(-(T)1, (T)0);

template<typename T> requires arithmetic<T>
const Vector2<T> Vector2<T>::up((T)0, (T)1);

template<typename T> requires arithmetic<T>
const Vector2<T> Vector2<T>::down((T)0, -(T)1);
