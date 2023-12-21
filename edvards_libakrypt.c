#include <stdio.h>
#include <libakrypt.h>

int main() {
    int e = 0, d = 0;
    double x, y;

    // Инициализация библиотеки libakrypt
    if (ak_libakrypt_create(ak_function_log_stderr) != ak_true) {
        printf("Ошибка инициализации libakrypt\n");
        return 1;
    }

    // Ввод и проверка параметров e и d
    while (e * d * (1 - e * e * e * e * d) == 0) {
        printf("Помните, что существуют ограничения на e и d: e*d*(1-e^4*d)!=0\n");
        printf("Введите параметр e: ");
        scanf("%d", &e);
        printf("Введите параметр d: ");
        scanf("%d", &d);
        printf("Параметры кривой Эдвардса: e=%d, d=%d\n", e, d);
    }

    // Ввод координат точки
    printf("Введите координаты точки, которую вы хотите удвоить\n");
    printf("x=");
    scanf("%lf", &x);
    printf("y=");
    scanf("%lf", &y);

    // Вычисление и вывод результата
    printf("Искомая кратная точка на кривой Эдвардса с параметрами e=%d, d=%d:\n", e, d);
    printf("P2(%lf, %lf)\n", 2 * x * y / (e + e * d * x * x * y * y), (y * y - x * x) / (e - e * d * x * x * y * y));

    // Завершение работы с библиотекой
    ak_libakrypt_destroy();

    return 0;
}
