# HW-Operating-Systems
## Задание выполнено на 10 баллов.
1. Программа завершает работу, если позьзователь использовал сочетание клавиш `CTRL+C` (Windows/Linux) или `CONTROL+C` (macOS).
```c
//other code
volatile sig_atomic_t exit_flag = 0;
//other code
void handle_exit_signal(int signum)
{
    exit_flag = 1;
}
//other code
int main(int argc, char *argv[])
{
  //other code
  signal(SIGINT, handle_exit_signal);
  while (!exit_flag)
  {
    //other code
  }
}
```
2. Программа завершает работу, если клиент сгенерировал число `999999999`.
```c
if (message == 999999999)
{
  printf("LUCKY NUMBER\n");
  exit(0);
}
```
### `Условие на оценку 8`
#### Решение (с подробными комментариями) предоставлено в файле [messenger.c](https://github.com/AMGureev/HW6-Operating-System/blob/main/messenger.c). </br>
#### Инструкция по запуску:
+ Ввести в консоль команду `gcc file_name -o messenger`, где далее `file_name` - расположение файла.
+ Создать два файла fifa командой `mkfifo file_name`, где `file_name` - имя файла.
+ Ввести в консоль команду `./messenger first_argument second_argument`, где `first_argument` - первый fifa файл, а `second_argument` - второй fifa файл.   
## Работу выполнил студент 2 курса БПИ227 Гуреев Александр Михайлович
