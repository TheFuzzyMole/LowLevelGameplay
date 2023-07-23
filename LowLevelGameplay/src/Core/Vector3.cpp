#pragma once
#include <Core/Vector3.h>
#include <Core/Vector2.h>

using LLGP::Vector3;

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::zero((T)0, (T)0, (T)0);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::one((T)1, (T)1, (T)1);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::left((T)1, (T)0, (T)0);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::right(-(T)1, (T)0, (T)0);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::up((T)0, (T)1, (T)0);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::down((T)0, -(T)1, (T)0);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::forward((T)0, (T)0, (T)1);

template <typename T> requires arithmetic<T>
const Vector3<T> Vector3<T>::backward((T)0, (T)0, -(T)1);
