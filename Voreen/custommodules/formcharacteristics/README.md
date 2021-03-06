### Модули характеристики формы

Данный раздел содержит инструменты по выравниванию и оценке выравнивания

        CalculateScore
        FormFinder
        MolMolAlign
        VolVolAlign


CalculateScore 
-------------------------
Данный модуль осуществляет точное вычисление разности двух карт электронных плонтостей (КЭП).
На вход модуль получает две КЭП и выводит в  поле "Current Score" численное значение посчитанной разности:
        - в норме пространства L1 (интеграл модуля   разности по всему объёму)
        - в норме пространства L2 (интеграл квадрата разности по всему объёму)
Мера выбирается пользователем с помощью аргумента "Measure".

FormFinder
-------------------------
Данный модуль осуществляет поиск белковых доменов из базы данных (БД), похожих, в некоторым смысле,
на заданную карту электронной плотности. Степень сходства определяется с помощью метода моментов,
а именно

1) для поданной на вход КЭП вычисляются 239 тригонометрических моментов 

2) выбирается домен, и из БД и берутся значения его моментов

3) соответствующие моменты КЭП и домена вычитаются и для полученных чисел считается среднее квадратическое

4) это среднее квадратическое и есть критерий сходства: чем оно меньше, тем лучше

5) берётся следующий домен и осуществляется переход к п.2

6) если домены кончились, выберем из них и загрузим структуры стольких наименьших, какое число указано в 
   качестве значения параметра "Max domains to load" (если, конечно, столько есть в БД)

По смыслу, подсчёт такого расстояния приближает расстояние в обратном (в смысле Фурье) пространстве,
а потому, непосредственно связано с расстоянием в прямом пространстве.

Данный модуль имеет три параметра:
* Max domains to load   -  о котором уже упоминалось выше;
 
* Bottom value cutoff   -  число, соответствующее нижней границе значения ЭП, которое будет учитываться;

* Weight factor         -  число, на которое определяет, насколько интеграл от электоронной 
                           плотности превышает значение электронной плотности единичной молекулы.
			
MolMolAlign и VolVolAlign
-------------------------
Данные модули осуществляют выравнивание двух молекул (molmol) и двух КЭП (volvol) соотвественно, используя
при этом оси инерции в качестве базиса и геометрические моменты для однозначного определения ориентации.




