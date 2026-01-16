#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLIK_DANYCH "rejestr_cyber.dat"

typedef enum {
    LEGALNY = 0,
    SZARA_STREFA = 1,
    NIELEGALNY = 2
} StatusLegalnosci;

typedef struct {
    char nazwa[101];
    char producent[100];
    int poziomRyzyka;
    float energia;
    StatusLegalnosci status;
    char idWlasciciela[50];
} Implant;

struct Wezel {
    Implant dane;
    struct Wezel* nastepny;
};

struct Wezel* glowa = NULL;

const char* statusToString(StatusLegalnosci s) {
    switch(s) {
        case LEGALNY: return "Legalny";
        case SZARA_STREFA: return "Szara Strefa";
        case NIELEGALNY: return "Nielegalny";
        default: return "Nieznany";
    }
}

void dodajDoListy(Implant nowyImplant) {
    struct Wezel* nowyWezel = (struct Wezel*)malloc(sizeof(struct Wezel));
    if (nowyWezel == NULL) {
        return;
    }

    nowyWezel->dane = nowyImplant;
    nowyWezel->nastepny = NULL;

    if (glowa == NULL) {
        glowa = nowyWezel;
    } else {
        struct Wezel* temp = glowa;
        while (temp->nastepny != NULL) {
            temp = temp->nastepny;
        }
        temp->nastepny = nowyWezel;
    }
}

void zapiszDoPliku() {
    FILE *fp = fopen(PLIK_DANYCH, "wb");
    if (fp == NULL) {
        return;
    }

    struct Wezel* temp = glowa;
    while (temp != NULL) {
        fwrite(&(temp->dane), sizeof(Implant), 1, fp);
        temp = temp->nastepny;
    }
    fclose(fp);
}

void wczytajZPliku() {
    FILE *fp = fopen(PLIK_DANYCH, "rb");
    if (fp == NULL) {
        return;
    }

    Implant bufor;
    while (fread(&bufor, sizeof(Implant), 1, fp) == 1) {
        dodajDoListy(bufor);
    }
    fclose(fp);
}

void interfejsDodaj() {
    Implant nowy;
    printf("\n--- DODAWANIE ---\n");
    printf("Nazwa implantu: ");
    scanf(" %[^\n]s", nowy.nazwa);
    printf("Producent: ");
    scanf(" %[^\n]s", nowy.producent);
    printf("Poziom ryzyka (0-10): ");
    scanf("%d", &nowy.poziomRyzyka);
    printf("Energia: ");
    scanf("%f", &nowy.energia);

    int st;
    printf("Status (0-Legalny, 1-Szara Strefa, 2-Nielegalny): ");
    scanf("%d", &st);
    nowy.status = (StatusLegalnosci)st;

    printf("ID Wlasciciela: ");
    scanf(" %[^\n]s", nowy.idWlasciciela);

    dodajDoListy(nowy);
}

void wyswietlWszystkie() {
    printf("\n--- LISTA ---\n");
    if (glowa == NULL) {
        printf("Pusta lista.\n");
        return;
    }

    struct Wezel* temp = glowa;
    int i = 0;
    while (temp != NULL) {
        printf("[%d] Nazwa: %-15s | Wlasciciel: %-10s | Status: %-10s | Ryzyko: %d\n",
               i++, temp->dane.nazwa, temp->dane.idWlasciciela,
               statusToString(temp->dane.status), temp->dane.poziomRyzyka);
        temp = temp->nastepny;
    }
}

void usunImplant() {
    wyswietlWszystkie();
    if (glowa == NULL) return;

    int id;
    printf("Podaj indeks do usuniecia: ");
    scanf("%d", &id);

    struct Wezel* temp = glowa;
    struct Wezel* poprzedni = NULL;

    if (id == 0) {
        if (temp->dane.status == NIELEGALNY) {
            printf("BLAD: Nie mozna usunac implantu nielegalnego.\n");
            return;
        }
        glowa = temp->nastepny;
        free(temp);
        printf("Usunieto.\n");
        return;
    }

    int counter = 0;
    while (temp != NULL && counter != id) {
        poprzedni = temp;
        temp = temp->nastepny;
        counter++;
    }

    if (temp == NULL) {
        printf("Bledny indeks.\n");
        return;
    }

    if (temp->dane.status == NIELEGALNY) {
        printf("BLAD: Nie mozna usunac implantu nielegalnego.\n");
        return;
    }

    poprzedni->nastepny = temp->nastepny;
    free(temp);
    printf("Usunieto.\n");
}

void edytujImplant() {
    wyswietlWszystkie();
    int id;
    printf("Podaj indeks do edycji: ");
    scanf("%d", &id);

    struct Wezel* temp = glowa;
    int counter = 0;
    while (temp != NULL && counter != id) {
        temp = temp->nastepny;
        counter++;
    }

    if (temp == NULL) {
        printf("Bledny indeks.\n");
        return;
    }

    printf("Edycja implantu: %s\n", temp->dane.nazwa);
    printf("Nowy producent: ");
    scanf(" %[^\n]s", temp->dane.producent);
    printf("Nowe ryzyko: ");
    scanf("%d", &temp->dane.poziomRyzyka);
}

void szukajPoWlascicielu() {
    char szukany[50];
    printf("Podaj ID wlasciciela: ");
    scanf(" %[^\n]s", szukany);

    struct Wezel* temp = glowa;
    int znaleziono = 0;

    while (temp != NULL) {
        if (strcmp(temp->dane.idWlasciciela, szukany) == 0) {
            printf(" -> %s (Ryzyko: %d)\n", temp->dane.nazwa, temp->dane.poziomRyzyka);
            znaleziono = 1;
        }
        temp = temp->nastepny;
    }
    if (!znaleziono) printf("Brak wynikow.\n");
}

void sortujImplanty() {
    if (glowa == NULL) return;

    int zamiana;
    struct Wezel *ptr1;
    struct Wezel *lptr = NULL;

    do {
        zamiana = 0;
        ptr1 = glowa;

        while (ptr1->nastepny != lptr) {
            if (ptr1->dane.poziomRyzyka < ptr1->nastepny->dane.poziomRyzyka) {
                Implant temp = ptr1->dane;
                ptr1->dane = ptr1->nastepny->dane;
                ptr1->nastepny->dane = temp;
                zamiana = 1;
            }
            ptr1 = ptr1->nastepny;
        }
        lptr = ptr1;
    } while (zamiana);
    printf("Posortowano.\n");
}

void zwolnijPamiec() {
    struct Wezel* temp = glowa;
    while (temp != NULL) {
        struct Wezel* doUsuniecia = temp;
        temp = temp->nastepny;
        free(doUsuniecia);
    }
    glowa = NULL;
}

int main() {
    wczytajZPliku();
    int wybor;
    do {
        printf("\n1. Dodaj\n2. Wyswietl\n3. Usun\n4. Edytuj\n5. Szukaj\n6. Sortuj\n0. Wyjscie\nWybor: ");
        scanf("%d", &wybor);

        switch(wybor) {
            case 1:
                interfejsDodaj();
                break;
            case 2:
                wyswietlWszystkie();
                break;
            case 3:
                usunImplant();
                break;
            case 4:
                edytujImplant();
                break;
            case 5:
                szukajPoWlascicielu();
                break;
            case 6:
                sortujImplanty();
                break;
            case 0:
                zapiszDoPliku();
                zwolnijPamiec();
                break;
            default:
                break;
        }
    } while (wybor != 0);
    return 0;
}