#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

class Matrix {
private:
    double** data;  //указатель на указатель (для двумерного динамического массива)
    int rows, cols;

public:
    //конструктор
    Matrix(int r = 0, int c = 0) : rows(r), cols(c) {
        //выделяем память под массив указателей на строки
        data = new double* [rows];

        //для каждой строки выделяем память под столбцы
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            //заполняем нулями
            for (int j = 0; j < cols; j++) {
                data[i][j] = 0.0;
            }
        }
    }

    //конструктор копирования (копируем данные)
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        //выделяем новую память
        data = new double* [rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            //копируем данные из other
            for (int j = 0; j < cols; j++) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    //оператор присваивания
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {  //проверка на самоприсваивание
            //освобождаем старую память
            for (int i = 0; i < rows; i++) {
                delete[] data[i];
            }
            delete[] data;

            //обновляем размеры
            rows = other.rows;
            cols = other.cols;

            //выделяем новую память, копируем данные
            data = new double* [rows];
            for (int i = 0; i < rows; i++) {
                data[i] = new double[cols];
                for (int j = 0; j < cols; j++) {
                    data[i][j] = other.data[i][j];
                }
            }
        }
        return *this; //возвращаем ссылку на текущий объект
    }

    //деструктор (освобождаем память)
    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[] data[i];  //удаляем каждую строку
        }
        delete[] data;  //удаляем массив указателей
    }

    //геттеры (не меняют)
    int getRows() const {
        return rows; 
    }
    int getCols() const {
        return cols; 
    }

    //сложение
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            cout << "Ошибка: размеры матриц не совпадают!" << endl;
            return 1;
        }
        Matrix summaMatr(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                summaMatr.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return summaMatr;
    }

    //вычитание
    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            cout << "Ошибка: размеры матриц не совпадают!" << endl;
            return 1;
        }
        Matrix raznostMatr(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                raznostMatr.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return raznostMatr;
    }

    //умножение матриц
    Matrix operator*(const Matrix& other) const {
        if (cols != other.cols) {
            cout << "Ошибка: кол-во столбцов 1ой матрицы не совпадает с кол-вом строк 2ой!" << endl;
            return 1;
        }
        Matrix proizvMatr(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    proizvMatr.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return proizvMatr;
    }

    //умножение на скаляр
    Matrix operator*(double scalar) const {
        Matrix proizvSkal(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                proizvSkal.data[i][j] = data[i][j] * scalar;
            }
        }
        return proizvSkal;
    }

    //деление на скаляр
    Matrix operator/(double scalar) const {
        if (abs(scalar) == 0) {
            cout << "Ошибка: скаляр равен 0!" << endl;
            return 1;
        }
        Matrix delenScal(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                delenScal.data[i][j] = data[i][j] / scalar;
            }
        }
        return delenScal;
    }

    //Комбинированные операции присваивания
    Matrix& operator+=(const Matrix& other) {
        *this = *this + other;
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        *this = *this - other;
        return *this;
    }

    Matrix& operator*=(const Matrix& other) {
        *this = *this * other;
        return *this;
    }

    Matrix& operator*=(double scalar) {
        *this = *this * scalar;
        return *this;
    }

    Matrix& operator/=(double scalar) {
        *this = *this / scalar;
        return *this;
    }

    //Операции сравнения на равенство/неравенство
    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            return false;
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (abs(data[i][j] - other.data[i][j]) != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    //транспонирование
    Matrix transponirovanie() const {
        Matrix tMatr(cols, rows); //меняем размерность местами
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                tMatr.data[j][i] = data[i][j];
            }
        }
        return tMatr;
    }

    //определитель
    double determinant() const {
        if (rows != cols) { //если матрица не квадратная
            cout << "Ошибка: матрица не квадратная!" << endl;
            return 1;
        }
        return determinantDop(data, rows);
    }

private:
    //дополнительная функция для определителя (рекурсивное разложение по 1й строке)
    double determinantDop(double** matr, int n) const {
        double det = 0;

        //матрица 1х1
        if (n == 1) {
            return matr[0][0];
        }
        //матрица 2х2
        if (n == 2) {
            return matr[0][0] * matr[1][1] - matr[0][1] * matr[1][0];
        }

        //cоздаем пустую подматрицу (выделяем память (n-1)x(n-1)
        double** podmatr = new double* [n - 1];
        for (int i = 0; i < n - 1; i++)
            podmatr[i] = new double[n - 1];

        //заполняем подматрицу, удаляя первую строку и x-ый столбец.
        for (int x = 0; x < n; x++) { //по столбцам
            int i2 = 0; //строка в подматрице
            for (int i = 1; i < n; i++) { //строки исх матрицы, первую пропускаю
                int j2 = 0; //столбец в подматрицы
                for (int j = 0; j < n; j++) { //столбцы исходной
                    if (j == x) {
                        continue; //пропускаем x-ый столбец
                    }
                    podmatr[i2][j2] = matr[i][j]; //копируем данные
                    j2++;
                }
                i2++;
            }
            int znak;
            if (x % 2 == 0) {
                znak = 1;
            }
            else {
                znak = -1;
            }
            //matr[0][x] - эл-нт из 1ой строки, на котором раскладываем
            //и вызываем эту же функцию для матпицы размером на 1 меньше
            det += znak * matr[0][x] * determinantDop(podmatr, n - 1);
        }

        //освобождаем память подматрицы
        for (int i = 0; i < n - 1; i++) {
            delete[] podmatr[i];
        }
        delete[] podmatr;

        return det;
    }

public:
    //oбратная матрица
    Matrix inverse() const {
        if (rows != cols) {
            cout << "Ошибка: матрица не квадратная!" << endl;
            return 1;
        }
            
        double det = determinant();
        if (abs(det) < 1e-10) {
            cout << "Обратной матрицы не существует (определитель почти 0)." << endl;
        }

        int n = rows;
        Matrix obratMatr(n, n);

        //матрица 1х1
        if (n == 1) {
            obratMatr.data[0][0] = 1.0 / data[0][0];
            return obratMatr;
        }

        //к нашей матрице справа добавляем единичную, итговый размер n х 2n
        double** newMatr = new double* [n];//массив указателей на строки
        for (int i = 0; i < n; i++) {
            newMatr[i] = new double[2 * n];
            for (int j = 0; j < n; j++) {
                newMatr[i][j] = data[i][j]; //копируем нашу матрицу
            }
            //добавляем единичную
            for (int j = n; j < 2 * n; j++) {
                if (j - n == i) { //совпадает ли индекс с номером строки
                    newMatr[i][j] = 1.0; //диагональные элементы единич матрицы
                }
                else {
                    newMatr[i][j] = 0.0;
                }
            }
        }

        //Метод Гаусса-Жордана
        for (int i = 0; i < n; i++) { //ищем строку с макс по модулю эл-том в текущем столбце (чтобы избежать деления на 0)
            int maxRow = i;
            for (int k = i + 1; k < n; k++) {
                if (abs(newMatr[k][i]) > abs(newMatr[maxRow][i])) {
                    maxRow = k;
                }
            }
            //ставим строку с макс эл-том на текущую позицию
            swap(newMatr[i], newMatr[maxRow]);

            double diagEl = newMatr[i][i]; //диагональный элемент (самый большой в столбце)
            //столбцы от 0 до i-1 обнулены, их не трогаем
            for (int j = i; j < 2 * n; j++) {
                newMatr[i][j] /= diagEl;
            }
            //обнуляем остальные строки в этом столбце
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    double factor = newMatr[k][i]; //сколько раз вычетаем строку i из строки k
                    for (int j = i; j < 2 * n; j++) {
                        newMatr[k][j] -= factor * newMatr[i][j];
                    }
                }
            }
        }

        //извлекаем обратную матрицу
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                obratMatr.data[i][j] = newMatr[i][n + j]; //эл-нт из правой половины расширенной матрицы
            }
        }

        //ocвобождаем память
        for (int i = 0; i < n; i++) {
            delete[] newMatr[i];
        }
        delete[] newMatr;

        return obratMatr;
    }

    //возведение в степень
    Matrix stepen(int n) const {
        if (n < 0) {
            //обратную матицу возводим уже в положительную степеь
            return inverse().stepen(-n);
        }
        //матрица в 0ой степени - единичная 
        if (n == 0) { 
            Matrix stepenMatr(rows, cols);
            for (int i = 0; i < rows; i++) {
                stepenMatr.data[i][i] = 1.0; //главную диаг заполняем 1
            }
            return stepenMatr;
        }

        Matrix stepenMatr = *this;
        for (int i = 1; i < n; i++) {
            stepenMatr = stepenMatr * (*this);
        }
        return stepenMatr;
    }

    //норма (макс сумма эл-нт по столбцам)
    double norma() const {
        double maxsum = 0;
        for (int j = 0; j < cols; j++) {
            double sum = 0;
            for (int i = 0; i < rows; i++) {
                sum += abs(data[i][j]);
            }
            if (sum > maxsum) {
                maxsum = sum;
            }
        }
        return maxsum;
    }

    //проверка типа матрицы
    //квадратная
    bool Square() const { 
        return rows == cols; 
    }
    //диагональная
    bool Diagonal() const {
        if (!Square()) {
            return false;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i != j && data[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    //нулевая (все 0)
    bool Zero() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (data[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    //единичная (диагональ 1, остальное 0)
    bool Identity() const {
        if (!Square()) {
            return false;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == j) {
                    if (data[i][j] != 1) {
                        return false;
                    }
                }
                else {
                    if (data[i][j] != 0) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    //симметрическая (отн главн диаг)
    bool Symmetric() const {
        if (!Square()) {
            return false;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (data[i][j] != data[j][i]) {
                    return false;
                }
            }
        }
        return true;
    }
    //верхняя треугольная (ниже главн диаг все 0)
    bool UpperTriangular() const {
        if (!Square()) {
            return false;
        }
        for (int i = 1; i < rows; i++) {
            for (int j = 0; j < i; j++) {
                if (data[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    //нижняя треугольная
    bool LowerTriangular() const {
        if (!Square()) {
            return false;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = i + 1; j < cols; j++) {
                if (data[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    //вывод
    friend ostream& operator<<(ostream& os, const Matrix& matr) {
        for (int i = 0; i < matr.rows; i++) {
            for (int j = 0; j < matr.cols; j++) {
                //ширина поля 5 символов, фиксированный формат, 2 знака полсе запятой
                os << setw(5) << fixed << setprecision(2) << matr.data[i][j] << " ";
            }
            os << endl;
        }
        return os;
    }

    //ввод
    friend istream& operator>>(istream& is, Matrix& matr) {
        int newRows, newCols;

        cout << "Введите количество строк в матрице: ";
        is >> newRows;
        cout << "Введите количество столбцов в матрице: ";
        is >> newCols;

        //освобождаем старую память
        for (int i = 0; i < matr.rows; i++) {
            delete[] matr.data[i];
        }
        delete[] matr.data;

        //устанавливаем новые размеры
        matr.rows = newRows;
        matr.cols = newCols;

        //выделяем новую память
        matr.data = new double* [matr.rows];
        for (int i = 0; i < matr.rows; i++) {
            matr.data[i] = new double[matr.cols];
        }

        cout << "Введите элементы матрицы (по строкам):" << endl;
        for (int i = 0; i < matr.rows; i++) {
            for (int j = 0; j < matr.cols; j++) {
                is >> matr.data[i][j];
            }
        }
        return is;
    }
};

void Menu(int proverka) {
    if (proverka == 0) {
        cout << endl;
        cout << "----  МЕНЮ  ----" << endl;
        cout << "0. Выход;" << endl;
        cout << "1. Создать новую матрицу." << endl;
        cout << endl;
    }
    else {
        cout << endl;
        cout << "----  МЕНЮ  ----" << endl;
        cout << "0. Выход;" << endl;
        cout << "1. Создать новую матрицу;" << endl;
        cout << "2. Вывод текущей матрицы;" << endl;
        cout << "3. Сложение матриц;" << endl;
        cout << "4. Вычитание матриц;" << endl;
        cout << "5. Умножение матриц;" << endl;
        cout << "6. Умножение матрицы на скаляр;" << endl;
        cout << "7. Деление матрицы на скаляр;" << endl;
        cout << "8. Транспонирование матрицы;" << endl;
        cout << "9. Обратная матрицы;" << endl;
        cout << "10. Возвести матрицу в степень;" << endl;
        cout << "11. Определитель матрицы;" << endl;
        cout << "12. Норма матрицы;" << endl;
        cout << "13. Определить тип матрицы;" << endl;
        cout << "14. Сравнение матриц;" << endl;
        cout << "15. Комбинированные операции присваивания (+=, -=, *=, /=)." << endl;
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Matrix m1, m2, result;
    int nomer, step; //номер из меню, степень
    double scalar;

    int proverka = 0;

    while (true) {
        Menu(proverka);
        proverka = 1;
        cout << "Введите номер: ";
        cin >> nomer;

            switch (nomer) {
            case 0: {
                cout << "Вы вышли из программы!" << endl;
                return 0;
            }
            case 1: {
                cout << "\nСоздание матрицы:" << endl;
                cin >> m1;
                cout << endl;
                cout << m1;
                break;
            }
            case 2: {
                cout << "\nТекущая матрица:" << endl;
                cout << m1;
                break;
            }
            case 3: {
                cout << "\nСоздание второй матрицы для сложения:" << endl;
                cin >> m2;
                cout << "Исходная матрица:" << endl;
                cout << m1;
                cout << "Новая матрица:" << endl;
                cout << m2;
                cout << "Сумма:";
                result = m1 + m2;
                cout << endl;
                cout << result;
                break;
            }
            case 4: {
                cout << "\nСоздание второй матрицы для вычитания:" << endl;
                cin >> m2;
                cout << "Исходная матрица:" << endl;
                cout << m1;
                cout << "Новая матрица:" << endl;
                cout << m2;
                cout << "Разность:\n";
                result = m1 - m2;
                cout << result;
                break;
            }
            case 5: {
                cout << "\nСоздание второй матрицы для умножения:" << endl;
                cin >> m2;
                cout << "Исходная матрица:" << endl;
                cout << m1;
                cout << "Новая матрица:" << endl;
                cout << m2;
                cout << "Произведение:\n";
                result = m1 * m2;
                cout << result;
                break;
            }
            case 6: {
                cout << "Введите скаляр для умножения: ";
                cin >> scalar;
                cout << "Исходная матрица:" << endl;
                cout << m1;
                cout << "Умножение на скаляр:\n";
                result = m1 * scalar;
                cout << result;
                break;
            }
            case 7: {
                cout << "Введите скаляр для деления: ";
                cin >> scalar;
                cout << "Исходная матрица:" << endl;
                cout << m1;
                cout << "Деление на скаляр:\n";
                result = m1 / scalar;
                cout << result;
                break;
            }
            case 8: {
                cout << "Исходная матрица: " << endl;
                cout << m1;
                cout << "Транспонированная:\n";
                result = m1.transponirovanie();
                cout << result;
                break;
            }
            case 9: {
                cout << "Исходная матрица: " << endl;
                cout << m1;
                cout << "Обратная:\n";
                result = m1.inverse();
                cout << result;
                break;
            }
            case 10: {
                cout << "Введите степень: ";
                cin >> step;
                cout << "Исходная матрица:\n ";
                cout << m1;
                cout << "Матрица в степени:\n";
                result = m1.stepen(step);
                cout << result;
                break;
            }
            case 11: {
                cout << "Матрица: " << endl;
                cout << m1;
                cout << "Определитель:\n";
                cout << m1.determinant() << endl;
                break;
            }
            case 12: {
                cout << "Матрица: " << endl;
                cout << m1;
                cout << "\nНорма: " << m1.norma() << endl;
                break;
            }
            case 13: {
                cout << "Матрица: " << endl;
                cout << m1;
                cout << "Квадратная: " << (m1.Square() ? "Да" : "Нет") << endl;
                if (m1.Square()) {
                    cout << "Диагональная: " << (m1.Diagonal() ? "Да" : "Нет") << endl;
                    cout << "Eдиничная: " << (m1.Identity() ? "Да" : "Нет") << endl;
                    cout << "Cимметрическая: " << (m1.Symmetric() ? "Да" : "Нет") << endl;
                    cout << "Верхняя треугольная: " << (m1.UpperTriangular() ? "Да" : "Нет") << endl;
                    cout << "Нижняя треугольная: " << (m1.LowerTriangular() ? "Да" : "Нет") << endl;
                }
                cout << "Нулевая: " << (m1.Zero() ? "Да" : "Нет") << endl;
                break;
            }
            case 14: {
                cout << "\nСоздание второй матрицы для сравнения:" << endl;
                cin >> m2;
                cout << "Исходная матрица:" << endl;
                cout << m1;
                cout << "Новая матрица:" << endl;
                cout << m2;
                if (m1 == m2) {
                    cout << "Матрицы равны." << endl;
                }
                else {
                    cout << "Матрицы не равны." << endl;
                }
                break;
            }
            case 15: {
                cout << "Создание второй матрицы m2:" << endl;
                cin >> m2;
                cout << "\nПервая матрица m1:" << endl;
                cout << m1;
                cout << "Вторая матрица m2:" << endl;
                cout << m2;

                Matrix temp = m1;
                temp += m2;
                cout << "m1 += m2 результат:" << endl;
                cout << temp;

                temp = m1;
                temp -= m2;
                cout << "m1 -= m2 результат:" << endl;
                cout << temp;

                temp = m1;
                temp *= m2;
                cout << "m1 *= m2 результат:" << endl;
                cout << temp;

                cout << "Введите скаляр для *= и /= : ";
                cin >> scalar;
                temp = m1;
                temp *= scalar;
                cout << "m1 *= scalar результат:" << endl;
                cout << temp;

                temp = m1;
                temp /= scalar;
                cout << "m1 /= scalar результат:" << endl;
                cout << temp;
                break;
            }
            default: {
                cout << "Неверный ввод!" << endl;
            }
            }
    }

    return 0;
}