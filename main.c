#include <stdio.h>
#include <string.h>

#define MAX_IMPLANTOW 100

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
    scanf(" %[^\n]s", nowy.nazwa);
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

void usunImplant() {
    int id;
    printf("Podaj numer ID rekordu do usuniecia: ");
    scanf("%d", &id);

    if (id < 0 || id >= licznik) {
        printf("Nieprawidlowy numer rekordu.\n");
        return;
    }

    if (baza[id].status == NIELEGALNY) {
        printf("BLAD: Nie mozna usunac nielegalnego implantu! Jest dowodem w sprawie.\n");
        return;
    }

    for (int i = id; i < licznik - 1; i++) {
        baza[i] = baza[i+1];
    }
    licznik--;
    printf("Rekord usuniety.\n");
}

void edytujImplant() {
    int id;
    printf("Podaj numer ID rekordu do edycji: ");
    scanf("%d", &id);

    if (id < 0 || id >= licznik) {
        printf("Nieprawidlowy numer.\n");
        return;
    }

    printf("Edytujesz implant: %s (Nazwa zablokowana do edycji)\n", baza[id].nazwa);

    printf("Nowy producent (aktualny: %s): ", baza[id].producent);
    scanf(" %[^\n]s", baza[id].producent);

    printf("Nowe ryzyko (aktualne: %d): ", baza[id].poziomRyzyka);
    scanf("%d", &baza[id].poziomRyzyka);


    printf("Dane zaktualizowane.\n");
}

void zapiszDoPliku() {
    FILE *fp = fopen("rejestr.dat", "wb");
    if (fp == NULL) {
        printf("Blad zapisu pliku!\n");
        return;
    }
    fwrite(&licznik, sizeof(int), 1, fp);
    fwrite(baza, sizeof(Implant), licznik, fp);
    fclose(fp);
    printf("Baza danych zapisana.\n");
}

void wczytajZPliku() {
    FILE *fp = fopen("rejestr.dat", "rb");
    if (fp == NULL) {
        printf("Brak pliku bazy danych. Tworze nowa.\n");
        return;
    }
    fread(&licznik, sizeof(int), 1, fp);
    fread(baza, sizeof(Implant), licznik, fp);
    fclose(fp);
    printf("Wczytano %d rekordow.\n", licznik);
}

void szukajPoWlascicielu() {
    char szukany[50];
    printf("Podaj ID wlasciciela: ");
    scanf(" %[^\n]s", szukany);

    printf("\nWyniki wyszukiwania:\n");
    int znaleziono = 0;
    for(int i=0; i<licznik; i++) {
        if(strcmp(baza[i].idWlasciciela, szukany) == 0) {
            printf(" -> Implant: %s, Producent: %s, Ryzyko: %d\n",
                   baza[i].nazwa, baza[i].producent, baza[i].poziomRyzyka);
            znaleziono = 1;
        }
    }
    if(!znaleziono) printf("Brak implantow dla tego obywatela.\n");
}

int main() {
    wczytajZPliku();
    int wybor;
    do {
        printf("\n=== REJESTR ULEPSZEN CYBERNETYCZNYCH ===\n");
        printf("1. Dodaj implant\n");
        printf("2. Wyswietl wszystkie\n");
        printf("3. Usun implant\n");
        printf("4. Edytuj implant\n");
        printf("0. Wyjscie\n");
        printf("Wybor: ");
        scanf("%d", &wybor);

        switch(wybor) {
            case 1:
                dodajImplant();
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
            case 0:
                zapiszDoPliku();
                printf("Zamykanie systemu...\n");
                break;
            default:
                printf("Nieznana opcja.\n");
        }
    } while (wybor != 0);
    return 0;
}