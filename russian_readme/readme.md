# ptusa_main #

## Содержание ##

1. [Общая архитектура системы](#общая-архитектура-системы)
2. [Общее описание](#общее-описание)
3. [Как собрать проект](#как-собрать-проект)
4. [Использование набора инструментов PLCnext Technology C++](#использование-набора-инструментов-plcnext-technology-c)
5. [Как запустить проект](#как-запустить-проект)
6. [Используемые библиотеки](#используемые-библиотеки)

## Общая архитектура системы ##

Общая архитектура системы изображена на рисунке ниже:

![System architecture ](../readme_images/system_architecture_ru.png)

Так же возможна следующая архитектура системы, когда управляющая программа развёрнута на сервере:

![System architecture ](../readme_images/system_architecture_2_ru.png)

## Общее описание ##

Управляющая логика реализуется в виде скрипта Lua. В ходе выполнения программы управление передается в отдельные специальные функции Lua, которые отвечают за отдельные этапы.

+ Общая схема работы программы контроллера (в [Drakon](https://drakonhub.com) diagram language)

<p align="center"><img src="../docs/drakon_diagram/images/control_program.svg"></p>

+ Выполнение технологического процесса:

``` mermaid

flowchart TD

    A(["2 Выполнение <br/> технологического <br/> процесса"])
    ---
    B[/"Для каждого <br/> аппарата (unit)"\]
    B --- C[["3 Выполнение <br/> аппаратной <br/> процедуры"]]
    C --- D[\"Следующий <br/> аппарат (unit)"/]
    D --- E([Конец])

```

``` mermaid

flowchart TD

    A(["3 Выполнение <br/> аппаратной процедуры"]) --- B[/Для каждой операции\]
    B --- C[[4 Выполнение операции]]
    C --- D[\Следующая операция/]
    D --- E([Конец])

```

<p align="center"><img src="../docs/drakon_diagram/images/operation.svg"></p>

``` mermaid

flowchart TD

    A([5 Выполнение шагов]) --- A1[[6 Выполнение <br/> главного шага]]
    A1 --- B[/Для каждого <br/> дополнительного шага\]
    B --- C[[6 Выполнение шага]]
    C --- D[\"Следующий <br/> дополнительный шаг"/]
    D --- E([Конец])

```

``` mermaid

flowchart TD

    A([6 Выполнение шага]) --- B[/Для каждого действия\]
    B --- C[[7 Выполнение действия]]
    C --- D[\Следующее действие/]
    D --- E([Конец])

```

``` mermaid

flowchart TD

    A([7 Выполнение действия]) --- B[Логика действия]
    B --- E([Конец])

```

+ При включении/выключении операции происходит следующее:

![Clone repository](../readme_images/tech_object__set_mode.png)

+ При выполнении команды происходит следующее:

![Clone repository](../readme_images/tech_object__exec_cmd.png)

## Как собрать проект ##

Вы можете клонировать репозиторий при помощи следующей команды:
`git clone --recurse-submodules https://github.com/savushkin-r-d/ptusa_main.git`

Или, если вы клонировали проект без инициализированных и обновленных подмодулей, попробуйте следующее:
`git submodules update --init --recursive`

### win ###

Требования:

+ Microsoft Visual Studio Community 2019

Вы должны открыть (в Visual Studio) корневой каталог. Таким образом, вы получаете проект на основе CMake.

+ Qt Creator

В Qt вы можете открыть файл CMakeList.txt клонированного репозитория.

Или клонировать репозиторий используя Qt:
    Добавить -> Импортировать проект -> Клонирование Git
    указать ссылку на репозиторий и поставить галочку для рекурсивного клонирования.

+ Microsoft Visual Studio Community 2022

Перед запуском проекта необходимо убедиться в том, что у вас установлен C++ CMake tools for Windows.
Средства C++ CMake для Windows устанавливаются с помощью:

`Visual Studio Installer -> Разработка классических приложений на C++ -> Средства CMake C++ для Windows `

Подробную информацию по установке можно получить [здесь](https://docs.microsoft.com/ru-ru/cpp/build/cmake-projects-in-visual-studio?view=msvc-160). Версия CMake, необходимая для сборки: 3.13 и выше.

После этого вы должны открыть (в Visual Studio) корневой каталог. Таким образом, вы получаете проект на основе CMake. Если после запуска возникла ошибка, то следует очистить кэш CMake: `Проект -> Удалить кэш и выполнить перенастройку`.

### linux ###

Eclipse IDE с проектом на базе CMake в Linux.

## Использование набора инструментов PLCnext Technology C++ для ОС Windows ##

Загрузите последние версии с веб-сайта Phoenix Contact https://www.phoenixcontact.com/products (например, из области **AXC F 2152**) `PLCnext Technology C++ Toolchain для Windows` (текущая 2023.0 LTS) и `sdk для Windows 64 bit` (текущая 2022.0.4 LTS) , в разделе Загрузки->Программное обеспечение $-$ http://www.phoenixcontact.com/qr/2404267/softw.

Перейдите в папку, где находятся загрузки (типичный путь `%userprofile%/Downloads`), распакуйте архив (`SDK_2021.9_Windows_AXC_F_2152.tar.xz.zip`).

### Установка (обновление) SDK ###

Перейдите в папку, в которую ранее был распакован архив, вызовите CLI в консоли, используя следующую команду:

```ps
plcncli.exe install sdk –d [путь установки] –p [путь к архивному файлу]
```

>Если вы устанавливаете несколько SDK, Phoenix Contact рекомендует использовать структуру папок "целевое имя/версия встроенного ПО".

Также ваш путь установки должен совпадать с путем в CMake.

Например:

```ps
plcncli.exe install sdk -d C:\CLI\sdks\AXCF2152\2022_0 \ -p pxc-glibc-x86_64-mingw32-axcf2152-image-mingw-cortexa9t2hf-neon-axcf2152-toolchain-2022.0.tar.xz
```

>SDK указан для контроллера. Полный список контроллеров можно найти на сайте PHOENIX CONTACT ([Главная страница > Продукция > ПЛК и системы ввода-вывода > Управление PLCnext > Список продуктов Технология PLCnext components](https://www.phoenixcontact.com/online/portal/pi?1dmy&urile=wcm%3apath%3a/pien/web/main/products/list_pages/PLCnext_technology_components_P-21-14-01/f77f0eb0-2a70-40c3-8679-7df2450e26db))

## Как запустить проект ##

Для запуска демо проекта вы должны использовать эту команду:

### windows ###

+ После сборки и развертывания *`windows-default`* конфигурации cmake:

```cmd
    bin\install\windows-default\Release\ptusa_main.exe demo_projects/T1-PLCnext-Demo/main.plua path demo_projects/T1-PLCnext-Demo/ sys_path demo_projects/T1-PLCnext-Demo/sys/ debug
```

+ Запуск проекта в Microsoft Visual Studio Community 2022: `Выбрать элемент запуска -> ptusa_main.exe`.

## Используемые библиотеки ##

### lua ###

Lua $-$ это скриптовый язык программирования. Характерной особенностью Lua является реализация большого количества программных сущностей с минимумом синтаксических средств.

Так, все составные пользовательские типы данных (массивы, структуры, множества, очереди, списки) реализуются через механизм таблиц, а механизмы объектно-ориентированного программирования, включая множественное наследование — с использованием метатаблиц, которые также отвечают за перегрузку операций и ряд других возможностей.

Lua — это процедурный динамически типизированный модульный язык с автоматическим управлением памятью. Включает базовые элементы для поддержки функционального и объектного стилей программирования. Таким образом, Lua можно называть мультипарадигменным языком. Встроенные средства параллельного программирования позволяют писать многопоточные программы только средствами языка, не обращаясь к API операционной системы или внешним библиотекам. Так как основным назначением Lua является встраивание, он имеет эффективные средства межъязыкового взаимодействия, ориентированные, главным образом, на вызов библиотек Си и на работу в Си-окружении. Язык поддерживает небольшое количество встроенных типов данных: логические значения, числа, строки, функции, потоки. Типичные комбинированные структуры данных, такие как массивы, наборы, списки и записи, отсутствуют, вместо всех их используется одна базовая структура Lua — таблица.

Отдельный тип *userdata* предназначен специально для низкоуровневого программирования и обмена данными с внешним кодом на других языках. Функции в Lua являются объектами первого класса, могут присваиваться и передаваться в параметрах. Поддерживаются замыкания, есть возможность создания функций высших порядков. Объектная система прототипная, отсутствует явная поддержка наследования, однако оно легко реализуется с помощью метатаблиц.

Подробную информацию можно получить [здесь](https://www.lua.org).

### tolua++ ###

Tolua $-$ это инструмент, который значительно упрощает интеграцию кода C/C++ с Lua. На основе чистого файла заголовка tolua автоматически генерирует код привязки для доступа к функциям C/C++ из Lua. Используя Lua API и средства метода тегов, tolua сопоставляет константы C/C++, внешние переменные, функции, классы и методы с Lua.
Чтобы использовать tolua, создается файл пакета, файл заголовка C/C++, содержащий список констант, переменных, функций, классов и методов, которые мы хотим экспортировать в среду Lua. Затем tolua анализирует этот файл и создает файл C/C++, который автоматически привязывает код C/C++ к Lua. Если связать созданный файл с приложением, указанный код C/C++ может быть доступен из Lua.

Подробную информацию можно получить [здесь](https://web.tecgraf.puc-rio.br/~celes/tolua/tolua-3.2.html).

### zlib ###

Zlib $-$ свободная кроссплатформенная библиотека для сжатия данных и является обобщением алгоритма сжатия данных DEFLATE, используемого в их компрессоре данных gzip.

Подробную информацию можно получить [здесь](https://www.zlib.net).

### subhook ###

Subhook $-$ это простая библиотека подключений для C и C++, которая работает в Windows, Linux и macOS, поддерживает только x86 (32-разрядную и 64-разрядную версии).

Подробную информацию можно получить [здесь](https://github.com/Zeex/subhook).

### googletest ###

Google Test $-$ библиотека для модульного тестирования (англ. unit testing) на языке C++. Google Test построена на методологии тестирования xUnit, то есть когда отдельные части программы (классы, функции, модули) проверяются отдельно друг от друга, в изоляции. Библиотека сама по себе разработана с активным применением тестирования, когда при добавлении каких-либо частей в официальную версию, кроме кода самих изменений необходимо написать набор тестов, подтверждающих их корректность.

Подробную информацию можно получить [здесь](https://github.com/google/googletest).

### google benchmark ###

Google benchmark $-$ это библиотека поддержки микробенчмарков. Библиотеку можно использовать с C++03. Однако для сборки требуется C++11, включая поддержку компилятора и стандартной библиотеки.

Подробную информацию можно получить [здесь](https://github.com/google/benchmark).
