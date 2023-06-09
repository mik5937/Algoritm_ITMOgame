# Algoritm_ITMOgame
Алгоритм
Игра начинается со следующими параметрами:
● В городе проживает 100 человек
● В закромах города 2800 бушелей пшеницы
● Город занимает 1000 акров земли
В начале каждого раунда игрок видит
● Номер раунда
● Количество людей, умерших от голода (если не было, то не
показывается)
● Количество людей, приехавших в город (если не было, то не
показывается)
● Была ли чума
● Текущее население города
● Сколько всего пшеницы было собрано и сколько пшеницы было
получено с акра
● Сколько пшеницы уничтожили крысы
● Сколько акров сейчас занимает город
● Какова цена одного акра земли в этом году
Цена акра определяется случайно в начале каждого раунда из диапазона
от 17 до 26
Затем игрок вводит:
● Количество акров земли, которые он хочет купить
● Количество акров земли, которые он хочет продать
● Количество бушелей пшеницы, которые будут использованы в
качестве еды
● Количество акров земли, которые необходимо засеять пшеницей
Игра контролирует вводимые значения, и, если значения оказываются
некорректными (например, количество земли, которое хочет купить игрок
не может быть оплачено имеющимся пшеницей, или введено
отрицательное количество акров земли), выводит соответствующие
предупреждения.
Для контроля используются следующие значения (некоторые из них
скрыты от игрока, он должен определить их сам):
● Продажа и покупка земли выполняются по текущей цене
● Каждый житель потребляет 20 бушелей пшеницы в год
● Каждый житель может обрабатывать максимум 10 акров земли
● Каждый акр требует .5 бушелей пшеницы на семена
Переход к следующему раунду определяется правилами
● Количество пшеницы, собранного с акра определяется случайно из
диапазона от 1 до 6
● Крысы съедают случайное количество пшеницы из диапазона от 0 до
0.07*имеющееся количество бушелей с учетом сборов
● Каждый житель потребляет 20 бушелей пшеницы в раунд, те, кому не
хватает еды, умирают от голода
● Если за раунд умерли от голода более 45% населения, игрок сразу же
проигрывает
● Количество вновь прибывших определяется по формуле:
умерших_от_голода /2 + (5 – сбор_с_акра) * бушелей_пшеницы / 600 +
1
затем , если это количество меньше 0, берется 0, если больше 50, то
50
● Чума возникает с вероятностью 15%, если это происходит, то
население уменьшается наполовину с округлением вниз
После 10 раунда игра вычисляет следующие статистики:
● Среднегодовой процент умерших от голода P
● Количество акров земли на одного жителя L
и выставляет игроку оценку по результатам правления:
● Если P > 33% и L < 7 – плохо («Из-за вашей некомпетентности в
управлении, народ устроил бунт, и изгнал вас их города. Теперь вы
вынуждены влачить жалкое существование в изгнании»)
● Если P > 10% и L < 9 – удовлетворительно («Вы правили железной
рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с
облегчением, и никто больше не желает видеть вас правителем»)
● Если P > 3% и L < 10 – хорошо («Вы справились вполне неплохо, у вас,
конечно, есть недоброжелатели, но многие хотели бы увидеть вас во
главе города снова»)
● В противном случае – отлично («Фантастика! Карл Великий,
Дизраэли и Джефферсон вместе не справились бы лучше»
