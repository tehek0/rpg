#pragma once
// "Паспорт", чтобы в векторе можно было явно распознать тип указателя и в зависимости от этого манипулировать выходное значение функции int run();
// Сюда можно ещё пристроить смену локации, когда локации будут реализованы
enum class interaction_return_value_type {
   tell_line,
   give_choice,
   start_battle,
   start_trade
};