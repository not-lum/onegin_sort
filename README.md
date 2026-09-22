# onegin_sort

> _Quick sort был самых честных правил,_<br>
> _Но вдруг он в шутку занемог,_<br>
> _Себя дебажить он заставил -_<br>
> _И лучше выдумать не мог._<br>
> <br>
> _Он n log n - твердит наука;_<br>
> _Но, боже мой, какая скука_<br>
> _Дебажить код и день и ночь,_<br>
> _Не отходя ни шагу прочь..._
>
> — А.С. Пушкин-Дебажкин

---

## Описание
Данная программа сортирует [быстрой сортировкой](https://ru.wikipedia.org/wiki/%D0%91%D1%8B%D1%81%D1%82%D1%80%D0%B0%D1%8F_%D1%81%D0%BE%D1%80%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%BA%D0%B0) роман в стихах [А.С. Пушкина](https://ru.wikipedia.org/wiki/%D0%9F%D1%83%D1%88%D0%BA%D0%B8%D0%BD,_%D0%90%D0%BB%D0%B5%D0%BA%D1%81%D0%B0%D0%BD%D0%B4%D1%80_%D0%A1%D0%B5%D1%80%D0%B3%D0%B5%D0%B5%D0%B2%D0%B8%D1%87) "Евгений Онегин" (в английском переводе).

Она печатает произведение в выходной файл в 3-х экземплярах:
1. Евгений Онегин, строки которого отсортированы слева направо стандартной функцией `qsort` из `stdlib.h`
2. Евгений Онегин, строки которого отсортированы **справа налево** самописной быстрой сортировкой
3. И оригинальный Евгений Онегин

> Примечание: сортировки пропускают символы, не являющиеся буквами английского алфавита

### Пример:
Входной файл:
```
Thus mused a madcap young, who drove
Through clouds of dust at postal pace,
By the decree of Mighty Jove,
Inheritor of all his race.
```
Выходной файл:
```
Onegin sorted via default qsort:
================================

By the decree of Mighty Jove,
Inheritor of all his race.
Through clouds of dust at postal pace,
Thus mused a madcap young, who drove

================================
Onegin sorted via myqsort and strings compared from right to left:
================================

Through clouds of dust at postal pace,
Inheritor of all his race.
By the decree of Mighty Jove,
Thus mused a madcap young, who drove

================================
Original Onegin:
================================

Thus mused a madcap young, who drove
Through clouds of dust at postal pace,
By the decree of Mighty Jove,
Inheritor of all his race.

================================
```


## Компиляция и запуск
### Компиляция:
```bash
gcc src/main.c src/sort_utils.c src/file_utils.c -o onegin_sort
```

В папке `data` лежит файл `onegin.txt` c текстом английского перевода "Евгения Онегина". Текст взят [отсюда](https://www.gutenberg.org/cache/epub/23997/pg23997.txt), перевод Henry Spalding'а. Французские буквы заменены на наиболее похожие английские, убраны знаки ударений, отступы, двойные переносы строки, примечания, наименования глав и т.п. - оставлен только сплошной текст произведения.

### Запуск:
```bash
./onegin_sort data/onegin.txt <output_file>
```



