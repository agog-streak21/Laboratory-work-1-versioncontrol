#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для зберігання даних про ремонт
typedef struct Repair {
    char address[100];      // Адреса ремонту
    char type[100];         // Вид ремонту
    float cost;             // Вартість
    int duration;           // Тривалість у днях
    struct Repair* next;    // Вказівник на наступний елемент
} Repair;

// Глобальні вказівники на початок і кінець черги
Repair* front = NULL;
Repair* rear = NULL;

// Прототипи функцій
void enqueue();
void dequeue();
void displayAll();
void editRepair();
void belowCost();
void byAddress();
void totalCostByAddress();
void longestWork();
void sortByCost();
void saveToFile();
void loadFromFile();
void deleteFile();
void menu();

int main() {
    menu(); // Виклик головного меню
    return 0;
}

// Додавання нового елемента до черги
void enqueue() {
    Repair* new_node = (Repair*)malloc(sizeof(Repair)); // Виділяємо пам'ять
    printf("\nВведіть адресу: ");
    scanf(" %[^\n]", new_node->address);
    printf("Введіть вид робіт: ");
    scanf(" %[^\n]", new_node->type);
    printf("Введіть вартість: ");
    scanf("%f", &new_node->cost);
    printf("Введіть тривалість (днів): ");
    scanf("%d", &new_node->duration);
    new_node->next = NULL; // Новий елемент - останній

    // Якщо черга порожня
    if (rear == NULL) {
        front = rear = new_node;
    } else {
        rear->next = new_node; // Додаємо в кінець
        rear = new_node;       // Оновлюємо rear
    }
    printf("Роботу додано до черги.\n\n");
}

// Видалення першого елемента з черги
void dequeue() {
    printf("\n");
    if (front == NULL) { // Черга порожня
        printf("Черга порожня.\n");
        return;
    }
    Repair* temp = front;  // Тимчасовий вказівник
    front = front->next;   // Переносимо front на наступний
    if (front == NULL) rear = NULL; // Якщо черга спорожніла
    free(temp); // Звільняємо пам'ять
    printf("Елемент видалено.\n\n");
}

// Виведення всіх записів у черзі
void displayAll() {
    Repair* temp = front;
    printf("\n");
    if (!temp) {
        printf("Черга порожня.\n");
        return;
    }
    // Переглядаємо кожен елемент
    while (temp) {
        printf("Адреса: %s | Вид: %s | Вартість: %.2f | Тривалість: %d\n",
               temp->address, temp->type, temp->cost, temp->duration);
        temp = temp->next;
    }
    printf("\n");
}

// Редагування даних за адресою
void editRepair() {
    char addr[100];
    printf("\nВведіть адресу для коригування: ");
    scanf(" %[^\n]", addr);
    Repair* temp = front;

    // Шукаємо елемент за адресою
    while (temp) {
        if (strcmp(temp->address, addr) == 0) {
            printf("\nНовий вид: ");
            scanf(" %[^\n]", temp->type);
            printf("Нова вартість: ");
            scanf("%f", &temp->cost);
            printf("Нова тривалість: ");
            scanf("%d", &temp->duration);
            printf("Дані оновлено.\n\n");
            return;
        }
        temp = temp->next;
    }
    printf("Роботу за такою адресою не знайдено.\n\n");
}

// Виведення робіт з вартістю меншою за вказану
void belowCost() {
    float limit;
    printf("\nВведіть граничну вартість: ");
    scanf("%f", &limit);
    Repair* temp = front;

    // Переглядаємо всі роботи
    while (temp) {
        if (temp->cost < limit) {
            printf("Адреса: %s | Вид: %s | Вартість: %.2f | Тривалість: %d\n",
                   temp->address, temp->type, temp->cost, temp->duration);
        }
        temp = temp->next;
    }
    printf("\n");
}

// Виведення робіт за заданою адресою
void byAddress() {
    char addr[100];
    printf("\nВведіть адресу: ");
    scanf(" %[^\n]", addr);
    Repair* temp = front;

    // Переглядаємо всі роботи
    while (temp) {
        if (strcmp(temp->address, addr) == 0) {
            printf("Вид: %s | Вартість: %.2f | Тривалість: %d\n",
                   temp->type, temp->cost, temp->duration);
        }
        temp = temp->next;
    }
    printf("\n");
}

// Обчислення загальної вартості робіт за адресою
void totalCostByAddress() {
    char addr[100];
    float total = 0;
    printf("\nВведіть адресу: ");
    scanf(" %[^\n]", addr);
    Repair* temp = front;

    // Сумуємо вартість
    while (temp) {
        if (strcmp(temp->address, addr) == 0) {
            total += temp->cost;
        }
        temp = temp->next;
    }
    printf("Загальна вартість робіт за адресою %s: %.2f\n\n", addr, total);
}

// Пошук та виведення найдовшої роботи
void longestWork() {
    if (!front) {
        printf("Черга порожня.\n");
        return;
    }
    Repair* temp = front;
    Repair* max = front;

    // Знаходимо елемент з найбільшою тривалістю
    while (temp) {
        if (temp->duration > max->duration) max = temp;
        temp = temp->next;
    }
    printf("\nНайтриваліша робота: %s (%s), тривалість: %d днів\n\n",
           max->address, max->type, max->duration);
}

// Сортування черги за зростанням вартості
void sortByCost() {
    if (!front || !front->next) return;
    // Сортуємо обміном полів
    for (Repair* i = front; i != NULL; i = i->next) {
        for (Repair* j = i->next; j != NULL; j = j->next) {
            if (i->cost > j->cost) {
                // Обмін полів
                char tempAddr[100], tempType[100];
                float tempCost = i->cost;
                int tempDur = i->duration;

                strcpy(tempAddr, i->address);
                strcpy(tempType, i->type);

                strcpy(i->address, j->address);
                strcpy(i->type, j->type);
                i->cost = j->cost;
                i->duration = j->duration;

                strcpy(j->address, tempAddr);
                strcpy(j->type, tempType);
                j->cost = tempCost;
                j->duration = tempDur;
            }
        }
    }
    printf("\nСписок відсортовано за вартістю.\n\n");
}

// Запис черги у файл
void saveToFile() {
    FILE* f = fopen("repairs.txt", "w");
    if (!f) {
        printf("Не вдалося відкрити файл.\n");
        return;
    }
    Repair* temp = front;

    // Запис кожного елемента у файл
    while (temp) {
        fprintf(f, "%s;%s;%.2f;%d\n", temp->address, temp->type, temp->cost, temp->duration);
        temp = temp->next;
    }
    fclose(f);
    printf("Дані записано у файл.\n");
}

// Зчитування та виведення даних з файлу
void loadFromFile() {
    FILE* f = fopen("repairs.txt", "r");
    if (!f) {
        printf("Файл відсутній.\n");
        return;
    }
    char line[300];
    printf("\nДані з файлу:\n");

    // Зчитуємо рядки з файлу
    while (fgets(line, sizeof(line), f)) {
        char addr[100], type[100];
        float cost;
        int dur;
        sscanf(line, " %[^\n;];%[^;];%f;%d", addr, type, &cost, &dur);
        printf("Адреса: %s | Вид: %s | Вартість: %.2f | Тривалість: %d\n",
               addr, type, cost, dur);
    }
    fclose(f);
    printf("\n");
}

// Видалення файлу
void deleteFile() {
    if (remove("repairs.txt") == 0) {
        printf("Файл успішно видалено.\n");
    } else {
        printf("Не вдалося видалити файл.\n");
    }
}

// Головне меню
void menu() {
    int choice;
    printf("\nРоботу виконав Швець Дмитро Ярославович група АІ-241\n");
    do {
        printf("\n1. Додати елемент\n2. Видалити елемент\n3. Коригувати дані\n");
        printf("4. Вивести всі дані\n5. Роботи з вартістю нижче заданої\n");
        printf("6. Роботи за адресою\n7. Сумарна вартість за адресою\n");
        printf("8. Найдовша робота\n9. Сортувати за вартістю\n");
        printf("10. Записати у файл\n11. Вивести з файлу\n12. Видалити файл\n");
        printf("0. Вихід\nВаш вибір: ");
        scanf("%d", &choice);

        // Виклик функцій відповідно до вибору
        switch (choice) {
            case 1: enqueue(); break;
            case 2: dequeue(); break;
            case 3: editRepair(); break;
            case 4: displayAll(); break;
            case 5: belowCost(); break;
            case 6: byAddress(); break;
            case 7: totalCostByAddress(); break;
            case 8: longestWork(); break;
            case 9: sortByCost(); break;
            case 10: saveToFile(); break;
            case 11: loadFromFile(); break;
            case 12: deleteFile(); break;
            case 0: printf("Вихід.\n"); break;
            default: printf("Невірний вибір.\n");
        }
    } while (choice != 0);
}
