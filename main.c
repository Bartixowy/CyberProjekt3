#include <stdio.h>
#include <string.h>

#define MAX_IMPLANTOW 100

// Definicje struktur
typedef enum { LEGALNY, SZARA_STREFA, NIELEGALNY } StatusLegalnosci;

typedef struct {
    char nazwa[101];
    char producent[100];
    int poziomRyzyka;
    float energia;
    StatusLegalnosci status;
    char idWlasciciela[50];
} Implant;

Implant baza[MAX_IMPLANTOW];
int licznik = 0;

// Pomocnicza funkcja do wyświetlania statusu jako tekst
const char* statusToString(StatusLegalnosci s) {
    switch(s) {
        case LEGALNY:
            return "Legalny";
        case SZARA_STREFA:
            return "Szara Strefa";
        case NIELEGALNY:
            return "Nielegalny";
        default:
            return "Nieznany";
    }
}

void dodajImplant() {
    if (licznik >= MAX_IMPLANTOW) {
        printf("Baza przepelniona!\n");
        return;
    }
    Implant nowy;
    printf("\n--- REJESTRACJA NOWEGO IMPLANTU ---\n");
    printf("Podaj nazwe implantu: ");
    scanf(" %[^\n]s", nowy.nazwa); // %[^\n]s pozwala wczytac spacje
    printf("Podaj producenta: ");
    scanf(" %[^\n]s", nowy.producent);
    printf("Poziom ryzyka (0-10): ");
    scanf("%d", &nowy.poziomRyzyka);
    printf("Zuzycie energii: ");
    scanf("%f", &nowy.energia);

    int st;
    printf("Status (0-Legalny, 1-Szara Strefa, 2-Nielegalny): ");
    scanf("%d", &st);
    nowy.status = (StatusLegalnosci)st;

    printf("ID Wlasciciela: ");
    scanf(" %[^\n]s", nowy.idWlasciciela);

    baza[licznik] = nowy;
    licznik++;
    printf("Implant zarejestrowany pomyślnie.\n");
}

void wyswietlWszystkie() {
    printf("\n--- LISTA OBYWATELI ULEPSZONYCH ---\n");
    for (int i = 0; i < licznik; i++) {
        printf("ID: %d | Nazwa: %s | Wlasciciel: %s | Status: %s | Ryzyko: %d\n",
               i, baza[i].nazwa, baza[i].idWlasciciela,
               statusToString(baza[i].status), baza[i].poziomRyzyka);
    }
}

int main() {
    int wybor;
    do {
        printf("\n=== REJESTR ULEPSZEN CYBERNETYCZNYCH ===\n");
        printf("1. Dodaj implant\n");
        printf("2. Wyswietl wszystkie\n");
        printf("0. Wyjscie\n");
        printf("Wybor: ");
        scanf("%d", &wybor);

        switch(wybor) {
            case 1: dodajImplant(); break;
            case 2: wyswietlWszystkie(); break;
            case 0: printf("Zamykanie systemu...\n"); break;
            default: printf("Nieznana opcja.\n");
        }
    } while (wybor != 0);
    return 0;
}