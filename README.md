# CCircularBuffer

STL - совместимый контейнер "Циклический буфер", с поддержкой возможности расширения и фиксированным размером.

[Циклический буфер](https://en.wikipedia.org/wiki/Circular_buffer) - структура данных, которая часто применяется для буферизации в задачах по передачи и обработки потока данных.

## Библиотека

Два класса:
CCirсularBuffer и CCircularBufferExt - для циклического буфера и циклического буфера с возможностью расширения (см ниже).

## Требования

Контейнер удовлетворяет [следующим требованиям](https://en.cppreference.com/w/cpp/named_req/Container) для stl-контейнера.
А также [требованиям для последовательного контейнера](https://en.cppreference.com/w/cpp/named_req/SequenceContainer)
Исключая rvalue и move-семантику.

## Итератор

Класс предоставляет итератор произвольного доступа.

## Кольцевой буфер с расширением максимального размера.

В случае достижения размера кольцевого буфера максимального возможного своего размера, значение максимального размера удваивается.

## Тесты

Реализация покрыта тестами с помощью фреймворка Google Test.


