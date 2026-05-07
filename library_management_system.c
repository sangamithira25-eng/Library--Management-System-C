#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TEXT 128

typedef struct {
    int id;
    char title[MAX_TEXT];
    char author[MAX_TEXT];
    int year;
    bool isBorrowed;
} Book;

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void printBook(const Book *book) {
    printf("ID: %d | Title: %s | Author: %s | Year: %d | Status: %s\n",
           book->id,
           book->title,
           book->author,
           book->year,
           book->isBorrowed ? "Borrowed" : "Available");
}

int findBookIndex(const Book *books, int count, int id) {
    for (int i = 0; i < count; ++i) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addBook(Book **books, int *count, int *capacity, int *nextId) {
    if (*count >= *capacity) {
        int newCapacity = (*capacity == 0) ? 4 : (*capacity * 2);
        Book *newArray = realloc(*books, sizeof(Book) * newCapacity);
        if (newArray == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }
        *books = newArray;
        *capacity = newCapacity;
    }

    Book *book = &(*books)[*count];
    book->id = *nextId;
    printf("Enter book title: ");
    readLine(book->title, sizeof(book->title));
    printf("Enter author name: ");
    readLine(book->author, sizeof(book->author));

    printf("Enter publication year: ");
    while (scanf("%d", &book->year) != 1 || book->year <= 0) {
        printf("Please enter a valid year: ");
        clearInput();
    }
    clearInput();

    book->isBorrowed = false;
    (*count)++;
    (*nextId)++;
    printf("Book added successfully.\n");
}

void listBooks(const Book *books, int count) {
    printf("\nLibrary Catalog\n");
    if (count == 0) {
        printf("No books in the catalog.\n");
        return;
    }
    for (int i = 0; i < count; ++i) {
        printBook(&books[i]);
    }
}

void searchBook(const Book *books, int count) {
    printf("\nSearch Book by ID\n");
    printf("Enter book ID: ");
    int id;
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        clearInput();
        return;
    }
    clearInput();

    int index = findBookIndex(books, count, id);
    if (index == -1) {
        printf("Book not found.\n");
        return;
    }
    printBook(&books[index]);
}

void borrowBook(Book *books, int count) {
    printf("\nBorrow Book\n");
    printf("Enter book ID: ");
    int id;
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        clearInput();
        return;
    }
    clearInput();

    int index = findBookIndex(books, count, id);
    if (index == -1) {
        printf("Book not found.\n");
        return;
    }
    if (books[index].isBorrowed) {
        printf("Book is already borrowed.\n");
        return;
    }
    books[index].isBorrowed = true;
    printf("Book borrowed successfully.\n");
}

void returnBook(Book *books, int count) {
    printf("\nReturn Book\n");
    printf("Enter book ID: ");
    int id;
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        clearInput();
        return;
    }
    clearInput();

    int index = findBookIndex(books, count, id);
    if (index == -1) {
        printf("Book not found.\n");
        return;
    }
    if (!books[index].isBorrowed) {
        printf("Book is already available.\n");
        return;
    }
    books[index].isBorrowed = false;
    printf("Book returned successfully.\n");
}

void removeBook(Book *books, int *count) {
    printf("\nRemove Book\n");
    printf("Enter book ID: ");
    int id;
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        clearInput();
        return;
    }
    clearInput();

    int index = findBookIndex(books, *count, id);
    if (index == -1) {
        printf("Book not found.\n");
        return;
    }
    for (int i = index; i < *count - 1; ++i) {
        books[i] = books[i + 1];
    }
    (*count)--;
    printf("Book removed successfully.\n");
}

void showMenu(void) {
    printf("\nLibrary Management System\n");
    printf("1. Add book\n");
    printf("2. List books\n");
    printf("3. Search book\n");
    printf("4. Borrow book\n");
    printf("5. Return book\n");
    printf("6. Remove book\n");
    printf("7. Exit\n");
    printf("Choose an option: ");
}

int main(void) {
    Book *books = NULL;
    int count = 0;
    int capacity = 0;
    int nextId = 1;
    bool running = true;

    while (running) {
        showMenu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInput();
            continue;
        }
        clearInput();

        switch (choice) {
            case 1:
                addBook(&books, &count, &capacity, &nextId);
                break;
            case 2:
                listBooks(books, count);
                break;
            case 3:
                searchBook(books, count);
                break;
            case 4:
                borrowBook(books, count);
                break;
            case 5:
                returnBook(books, count);
                break;
            case 6:
                removeBook(books, &count);
                break;
            case 7:
                running = false;
                printf("Exiting Library Management System.\n");
                break;
            default:
                printf("Please choose a valid option between 1 and 7.\n");
                break;
        }
    }

    free(books);
    return 0;
}
