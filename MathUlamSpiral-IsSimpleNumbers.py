DIRECTION_LEFT = (-1, 0)
DIRECTION_RIGHT = (1, 0)
DIRECTION_UP = (0, -1)
DIRECTION_BOTTOM = (0, 1)

size = spiral = x = y = direction = steps_in_one_direction = next_direction_change = next_steps_change = None


def is_prime(n):
    """
    Определение простоты числа.
    :param n: Число, прототу которого необходимо определить.
    :return:
    """
    if n < 2:
        return False
    for i in range(int(n ** 0.5) - 1):
        m = i + 2
        if not (n % m):
            return False
    return True


def get_next_direction(current_direction=None):
    """
    Получение следующего направления движения. Порядок направлений слудующий: направо - вверх - налево - вниз.
    :param current_direction: Текущее направление движения или None, если направление движения запрашивается в первый
    раз.
    :return:
    """
    if current_direction is None or current_direction == DIRECTION_BOTTOM:
        return DIRECTION_RIGHT
    if current_direction == DIRECTION_RIGHT:
        return DIRECTION_UP
    if current_direction == DIRECTION_LEFT:
        return DIRECTION_BOTTOM
    if current_direction == DIRECTION_UP:
        return DIRECTION_LEFT


def init():
    """
    Инициализация переменных.
    Функция считывает размер скатерти с клавиатуры, инициалиизиует двумерный массив скатерти нулями, направление
    заполнения массива (см. выше), три служебные переменные: количество шагов, которые необходимо сделать в одном
    направлении, следующее число, на котором поменяется направление движения, следующее число, на котором будет
    увеличено количество шагов в одном направлении.
    Центр скатерти определяется следующим образом: если размер скатерти нечетный, то центром скатерти станет
    центральная клетка, а если четный, то центром станет клетка с координатами (n/2-1, n/2), где n - размер скатерти, а
    отсчет ведется с 0, начиная с верхнего левого угла (как в двумерном массиве).
    :return:
    """
    global size, spiral, x, y, direction, steps_in_one_direction, next_direction_change, next_steps_change
    size = abs(int(input()))
    spiral = [[0 for i in range(size)] for j in range(size)]
    if size % 2:
        x = size // 2
    else:
        x = size // 2 - 1
    y = size // 2
    direction = get_next_direction()
    steps_in_one_direction = 1
    next_direction_change = 2
    next_steps_change = steps_in_one_direction * 2 + 1


def fill():
    """
    Заполнение массива чисел.
    Функция проходит в цикле все числа от 1 до n^2, где n - размер скатерти. В теле цикла текущее число записывается в
    клетку с определеннымми заранее координатами (для числа 1 это координаты центра (см. функцию init()), после чего
    определяется дальнейшее направдение движения, обновляется количество шагов, которые необходимо сделать в одном
    направлении. Вся эта информация используется для определения координат клетки, в которую будет записано следующее
    число.
    Новое число, на котором будет изменено направление движения, вычисляется суммированим предыдущего числа и текущего
    количества шагов в одном направлении. Первое такое число - 2, так как на числе 2 в первый раз изменится направление
    движения.
    Новое число, на котором будет изменено количество шагов в одном направлении, вычисляется суммированием предыдущего
    числа и удвоенного количества шагов в одном направлении. Первое такое число - 3, так как на числе 3 в первый раз
    изменится количество шагов в одном направлении.
    :return:
    """
    global size, spiral, x, y, direction, steps_in_one_direction, next_direction_change, next_steps_change
    for i in range(size * size):
        n = i + 1
        # Запись в клетку массива
        spiral[y][x] = n
        # Проверка, а не пора ли нам увеличивать количество шагов в одном направлении
        if next_steps_change == n:
            steps_in_one_direction += 1
            next_steps_change += 2 * steps_in_one_direction
        # Проверка, а не пора ли нам менять направление движения?
        if next_direction_change == n:
            direction = get_next_direction(direction)
            next_direction_change += steps_in_one_direction
        # Установка новых координат
        x += direction[0]
        y += direction[1]


def show():
    """
    Отображение скатерти. Если число простое, будет выведен закрашенный квадрат, если составное - незакрашенный.
    :return:
    """
    global spiral
    for row in spiral:
        for n in row:
            if is_prime(n):
                print("\u25a0", end="")
            else:
                print("\u25a1", end="")
        print("")


def main():
    """
    Основная функция.
    :return:
    """
    init()
    fill()
    show()


if __name__ == "__main__":
    main()