## Интерпритатор подмножества Python

#### [Грамматика](https://miro.com/welcomeonboard/VXFzT2JlZXE3cFBkSDNLYzVLREhqRHpoa1hpN250aWEyNWFTV0pCY3BlRWFsbHhVZ3ZNUUE0REh3dHNCTE01QXwzNDU4NzY0NTMyNTE0ODkyNzUzfDI=?share_link_id=385564918464)

# Описание языка:

## Типы данных:
> int   - целые числа от -2^32 ... 2^32-1
> str   - строка. Есть символ перехода на новую строку \n.
>> "Привет!\nКак дела?"
>> 'Ты читал "Война и мир"?'
>bool   - логический тип
>>false
>>true
### Присутсвует динамическая типизация данных.
a = 123/5
a = "123"

## Математические выражения:
> (...) - скобки
> **    - возведение в степень
> *, /, % - умножение, деление, остаток
> +, -, - сложение, вычитание, унарный плюс, унарный минус

## Логические выражения:
> `>`, `<`, `>=`, `<=`, `==`, `!=` сравнение строк, математических и логических выражений.
> `not` - логическое отрицание
> str1 `in` str2    - входит ли строка str1 в str2


## Операции со строками:
> `+` - конкатенация строк -> str
> `>`, `<`, `>=`, `<=`, `==`, `!=` - сравнение строк -> bool
> str1 `in` str2 - содержится ли str1 в str2 -> bool

## Ввод и вывод данных:
### Ввод
> a = int(input())   - ввод целого числа
> a = input() / a = str(input()) - ввод строки
### Вывод
> print(...)
>> Каждый print автоматически переводит на новую строку
>> print(1+2, "=", 9/2) -> 3 = 3
>>print("Привет, " + name + "!")

## Циклы
1. for *P* in range(a1[,a2[,a3]])
> Если указан только a1, то *p* будет менять [0; a1-1]
> Если указаны a1, a2, то *p* будет меняться от [a1, a2-1]
> Если указаны все три параметра, то *p* будет меняться от [a1, a2-1] с шагом a3
2. while *EXP*:
> Цикл будет выолняться, пока *EXP* == `true`
##### Внутри циклов так же допустимо использование `break` и `continue` и только там.
> `break` - выходит из самого внтуреннего цикла.
> `continue` - переходит к следующей итерации цикла.

## Условный оператор
### В языке допустимы условные опреатор if. Если текущее *exp* == false, то переходит к следующему elif. Если ни один не подошел, то к else, если он имеется. 
    if *exp*:
        ...
    {elif *exp2*:
        ...}
    [else:
        ...]

#### Зарезервированные идентификаторы: "def", "true", "false", "and", "or", "not", "if", "elif", "else", "break", "continue","return", "for", "while", "in", "print", "input", "range", "int", "str".

#### ":", ",", "==", "!=", "<", ">", "<=", ">=", "=", "+", "-", "*", "/", "%", "**", "(", ")"

# Запуск
1. `make task` - чтобы скомпилировать
2. `./task [path]` - чтобы запустить программу

# Как работает?
1. Лексический анализ - программа разбивается на токены, константы строки и числа записываются в таблицу.
2. Синтаксический анализ - проверяется синтаксис всех выражений и строится ПОЛИЗ
3. Выполнение - полученный ПОЛИЗ выполняется, типы вырожений проверяются




