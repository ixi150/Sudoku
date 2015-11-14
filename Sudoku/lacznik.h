#include<iostream>
#include<string>
#include<Windows.h>
#include<fstream>
#include<ctime>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------------

struct sudoku{
	short wpisane[3][3][3][3];
	bool komentarz[3][3][3][3][10];
	sudoku* prev;
	sudoku* next;
	int nr;
};

struct kroki{
	sudoku* stan_zero;
	sudoku* stan_koncowy;
	sudoku* stan_obecny;
	short podswietlona_cyfra;
};

struct wspolrzedne{
	short int X, Y, x, y;
};

//----------------------------------------------------------------------------------------------------------------------------
////////////// Współrzędne rysowanych elementów //////////////
//----------------------------------------------------------------------------------------------------------------------------

#define WYSOKOSC_EKRANU 41
#define SZEROKOSC_EKRANU 96

#define POZYCJA_LOGO_X 15
#define POZYCJA_LOGO_Y 1
#define DOMYSLNY_KOLOR_LOGO LIGHTGREEN

#define POZYCJA_PLANSZY_X 37 //szerokosc ~27
#define POZYCJA_PLANSZY_Y 10 //wysokosc  ~25 (nie zmniejszać poniżej 2!)

#define POZYCJA_KROKOW_X POZYCJA_PLANSZY_X
#define POZYCJA_KROKOW_Y POZYCJA_PLANSZY_Y + 23
#define POZYCJA_ZEGARA_X POZYCJA_PLANSZY_X
#define POZYCJA_ZEGARA_Y POZYCJA_PLANSZY_Y - 2

#define POZYCJA_PODPOWIEDZI_X 66
#define POZYCJA_PODPOWIEDZI_Y 8
#define WIELKOSC_PODPOWIEDZI_X 28
#define WIELKOSC_PODPOWIEDZI_Y 27

#define POZYCJA_POMOCY_X 3
#define POZYCJA_POMOCY_Y 8
#define WIELKOSC_POMOCY_X 31
#define WIELKOSC_POMOCY_Y 15

#define POZYCJA_KOMUNIKATOW_X 3
#define POZYCJA_KOMUNIKATOW_Y POZYCJA_POMOCY_Y+WIELKOSC_POMOCY_Y+2  //pozycja zależna od pozycji i wielkości POMOCY
#define WIELKOSC_KOMUNIKATOW_X 31
#define WIELKOSC_KOMUNIKATOW_Y 10

#define POZYCJA_WYGRANEJ_X 19
#define POZYCJA_WYGRANEJ_Y 10

#define POZYCJA_PODPISU_X 67
#define POZYCJA_PODPISU_Y 39

#define WIELKOSC_LOADSAVE_X 50
#define WIELKOSC_LOADSAVE_Y 16
#define POZYCJA_LOADSAVE_LOGO_X 25
#define POZYCJA_LOADSAVE_LOGO_Y 8
#define POZYCJA_LOADSAVE_X POZYCJA_LOADSAVE_LOGO_X - 3
#define POZYCJA_LOADSAVE_Y POZYCJA_LOADSAVE_LOGO_Y + 7
#define MAX_DLUGOSC_PLIKU 37 

#define LOAD 1
#define SAVE 0


//----------------------------------------------------------------------------------------------------------------------------
////////////// deklaracje funkcji zewnętrznych: //////////////
//----------------------------------------------------------------------------------------------------------------------------

	/*Funkcje rysujące*/
char* ekran_powitalny();
void rysuj_wszystko();
void rysuj_logo(int);	// wpisz kolor logo
void rysuj_zegar(int);	// wpisz liczbe sekund
void przerysuj_plansze(sudoku* stan, sudoku* stan_zero);
void wypisz_komunikat(char* komunikat);
void wypisz_krok(kroki*, bool);
void wypisz_ruchy(long long);
void wypisz_komentarze(sudoku*, wspolrzedne, bool);
int  wypisz_pozostale(sudoku*);

	/*Podpowiedzi i logika*/
void podswietl_cyfry(kroki k);
bool sprawdz(short, sudoku*, wspolrzedne);
void dostepne_cyfry(sudoku* stan, wspolrzedne);
void czysc_dostepne_cyfry();
void jednoznaczna_podpowiedz(kroki*);
void sprawdz_komentarze(sudoku*, wspolrzedne);


void stworz_zerowy_krok(kroki*, int);
sudoku* kolejny_krok(kroki*);

	/*Obsługa plików .xml*/
void odczyt_xml(char* adres, kroki* k);
void zapis_xml(char* adres, kroki* k);
char* pisanie_nazwy_xml(bool);
void rysuj_loadsave(bool);

/*Funkcje kończące aplikację*/
void wygrana(int, long long);
void wyjscie_z_gry(kroki*);

	/*Obsługa listy*/
void dodaj_zerowy_element(kroki*); 
void dodaj_element(kroki*);
void usun_elementy_po_obecnym(kroki*);
void zeruj_stan(sudoku*);
sudoku* znajdz_element_nr(int, kroki*);

	/*Pomoc w programowaniu*/
void wypisz_ascii();


//----------------------------------------------------------------------------------------------------------------------------
//////////////////////////// Kody klawiszy sterujących /////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------

#define UP		0x48
#define DOWN	0x50
#define LEFT	0x4b
#define RIGHT	0x4d
#define DEL		0x53
#define INS		0x52
#define HOME	0x47
#define END		0x4f
#define PGUP	0x49
#define PGDN	0x51
#define ENTER	0x0D
#define BACKSPACE 0x08
#define TAB		0x09
#define ESCAPE	0x1B