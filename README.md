# DNSCache  
Этот проект реализует кэширование DNS-записей с использованием LRU (Least Recently Used) алгоритма и предоставляет возможность тестирования производительности при использовании многопоточности.

## Описание  
Проект представляет реализацию класса DNSCache, предназначенного для хранения соответствий между доменными именами и их IP-адресами. Используется алгоритм LRU для удаления самых старых записей, когда кэш переполняется. Также реализованы тесты для проверки корректности и производительности класса DNSCache, включая сценарии многопоточной работы. Для измерения времени выполнения используется специальный класс LogDuration, который позволяет анализировать производительность операций в удобном формате. Добавлена пользовательская хэш-функция custom_hash. Её целью является уменьшение вероятности возникновения коллизий в хэш-таблицах.

## Структура проекта  
- **DNSCache.h** — Заголовочный файл с описанием класса `DNSCache`.
- **DNSCache.cpp** — Реализация класса `DNSCache`.
- **test.h** — Заголовочный файл для тестирования.
- **test.cpp** — Файл с тестами для проверки корректности и производительности класса DNSCache.
- **log_duration.h** — Заголовочный файл для измерения времени выполнения операций.
- **custom_hash.h** — Заголовочный файл с определением кастомного хеша.
- **main.cpp** — Главный файл для запуска программы и выполнения тестов.
  
## Зависимости  
- C++17 и выше.
- Для многопоточности используется стандартная библиотека C++ (`<thread>`, `<mutex>`).
- Для измерения времени используется `<chrono>`.

## Установка  
1. Клонируйте репозиторий:  
   `git clone https://github.com/kateklec/DNSCache.git`
   
2. Перейдите в каталог проекта:  
   `cd DNSCache`

3. Скомпилируйте проект с помощью g++:  
   `g++ -std=c++20 -o dns_cache main.cpp DNSCache.cpp test.cpp -pthread`

Флаг `-pthread` необходим для поддержки многопоточности.  

4. Запустите программу:  
`./dns_cache`
