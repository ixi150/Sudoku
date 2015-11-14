#include "lacznik.h"
#include "conio2.h"
using namespace std;

//----------------------------------------------------------------------------------------------------------------------------
					/////////////////////// Kody ASCII znaków rysuj¹cych ////////////////////////// 
//----------------------------------------------------------------------------------------------------------------------------

const unsigned char naroznik_lewy_gorny		= 201;
const unsigned char naroznik_prawy_gorny	= 187;
const unsigned char naroznik_lewy_dolny		= 200;
const unsigned char naroznik_prawy_dolny	= 188;
const unsigned char belka_pozioma			= 205;
const unsigned char belka_pionowa			= 186;
const unsigned char trojnik_gorny			= 203; 
const unsigned char trojnik_dolny			= 202;
const unsigned char trojnik_lewy			= 204;
const unsigned char trojnik_prawy			= 185;
const unsigned char krzyz					= 206;

const unsigned char ramka_pozioma			= 219;
const unsigned char ramka_gorna				= 220;
const unsigned char ramka_dolna				= 223;


//----------------------------------------------------------------------------------------------------------------------------
					/////////////////////// Funkcje Wewnêtrzne //////////////////////////
//----------------------------------------------------------------------------------------------------------------------------

void czysc_ekran(){ //funkcja czyszcz¹ca ca³y ekran krótk¹ animacj¹
	gotoxy(1, 1);
	textcolor(LIGHTGREEN);
	textbackground(BLACK);

	/*Animacja uœmiechniêtych buziek*/
	for (int y = 0; y < WYSOKOSC_EKRANU; y++){
		for (int x = 0; x < SZEROKOSC_EKRANU; x++){
			printf("%c", (char)2); //Kod ASCII emotki
		}
		printf("%c", '\n');
	}

	/*Faktyczne czyszczenie ekranu poprzez wypisanie spacji*/
	gotoxy(1, 1);
	for (int y = 0; y < WYSOKOSC_EKRANU; y++){
		for (int x = 0; x < SZEROKOSC_EKRANU; x++){
			printf(" ");
		}
		printf("%c", '\n');
	}
	clrscr();
	gotoxy(1, 1);
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_kwadrat(short int x, short int y){ //rysowanie jednego z 9 ma³ych kwadratów sudoku
	for (int i = 1; i<= 7; i++){
		gotoxy( (POZYCJA_PLANSZY_X + 8*x +2), (POZYCJA_PLANSZY_Y + 7*y + i) );
		if (i == 1) cout << naroznik_lewy_gorny << belka_pozioma << trojnik_gorny << belka_pozioma << trojnik_gorny << belka_pozioma << naroznik_prawy_gorny;
		if (i == 3 || i == 5) cout << trojnik_lewy << belka_pozioma << krzyz << belka_pozioma << krzyz << belka_pozioma << trojnik_prawy;
		if (i == 7) cout << naroznik_lewy_dolny << belka_pozioma << trojnik_dolny << belka_pozioma << trojnik_dolny << belka_pozioma << naroznik_prawy_dolny;
		if (i % 2 == 0) cout << belka_pionowa << " " << belka_pionowa << " " << belka_pionowa << " " << belka_pionowa;
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_plansze(){
	textcolor(GREEN);
	textbackground(BLACK);
	
	/*Rysowanie ramki na planszê sudoku*/
	for (short int y = 0; y <= 22; y++){
		gotoxy(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y+y);
		for (short int x = 0; x <= 26; x++){
			if		(y == 0)	cout << ramka_gorna;
			else if (y == 22)	cout << ramka_dolna;
			else if (x == 0 || x == 26) cout << ramka_pozioma;
			else
				cout << " ";
		}
	}

	/*Przygotowanie wszystkich pól na wpisywanie cyfr*/
	for (short int y = 0; y <= 2; y++){
		for (short int x = 0; x <= 2; x++){
			rysuj_kwadrat(x, y);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_pomoc(){ //funkcja wypisuje w ramce legende klawiszy potrzebnych do obs³ugi gry
	textcolor(LIGHTMAGENTA);
	textbackground(BLACK);

	/*rysowanie ramki*/
	for (short int y = 0; y <= WIELKOSC_POMOCY_Y; y++){
		for (short int x = 0; x <= WIELKOSC_POMOCY_X; x++){
			gotoxy(POZYCJA_POMOCY_X + x, POZYCJA_POMOCY_Y + y);

			if (y == WIELKOSC_POMOCY_Y)			cout << ramka_dolna;
			else if (x == 0 || x == WIELKOSC_POMOCY_X) cout << ramka_pozioma;
			else if (y == 0)								cout << ramka_dolna;
			else if (y == 1 && x == (WIELKOSC_POMOCY_X / 2 - 4)) cout << "INSTRUKCJA";
			else if ((y == 2 || y == WIELKOSC_POMOCY_Y - 1) && x == 3)
			{
				for (short int i = 1; i < WIELKOSC_POMOCY_X - 4; i++)
				{
					cout << belka_pozioma;
				}
			}
		}
	}

	/*wypisywanie tekstu w ramce*/
	for (int i = 0; i < WIELKOSC_POMOCY_Y; i++){
		gotoxy(POZYCJA_POMOCY_X+2, POZYCJA_POMOCY_Y + i+1);
		switch (i)
		{
		case 0:  cout << ""; break;
		case 1:  cout << ""; break;
		case 2:  cout << "1-9 - wstawienie liczby"; break;
		case 3:  cout << "Del - usuniecie cyfry"; break;
		case 4:  cout << "U , - poprzedni krok (undo)"; break;
		case 5:  cout << "R . - nastepny krok (redo)"; break;
		case 6:  cout << "L   - lista mozliwych cyfr"; break;
		case 7:  cout << "P   - podpowiedz"; break;
		case 8:  cout << "S   - zapisanie do pliku"; break;
		case 9:  cout << "O   - odczytanie pliku"; break;
		case 10: cout << "K   - edycja komentarza"; break;
		case 11: cout << "J * - podswietlenie cyfr"; break;
		case 12: cout << "Q   - wyjscie z gry"; break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_komunikaty(){
	textcolor(YELLOW);
	textbackground(BLACK);

	/*Przygotowanie ramki podpisanej KOMUNIKATY*/
	for (short int y = 0; y <= WIELKOSC_KOMUNIKATOW_Y; y++){
		for (short int x = 0; x <= WIELKOSC_KOMUNIKATOW_X; x++){
			gotoxy(POZYCJA_KOMUNIKATOW_X+x, POZYCJA_KOMUNIKATOW_Y + y);

			if		(y == WIELKOSC_KOMUNIKATOW_Y)			cout << ramka_dolna;
			else if (x == 0 || x == WIELKOSC_KOMUNIKATOW_X) cout << ramka_pozioma;
			else if (y == 0)								cout << ramka_dolna;
			else if (y == 1 && x == (WIELKOSC_KOMUNIKATOW_X / 2 - 4)) cout << "KOMUNIKATY";
			else if ((y == 2 || y == WIELKOSC_KOMUNIKATOW_Y-1) && x == 3)
			{
				for (short int i = 1; i < WIELKOSC_KOMUNIKATOW_X-4; i++)
				{
					cout << belka_pozioma;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_logo(int kolor){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textcolor(kolor);
	textbackground(BLACK);

	/*Wypisanie grapiki ASCII*/
	for (int i = 0; i < 6; i++){
		gotoxy(POZYCJA_LOGO_X, POZYCJA_LOGO_Y + i);
		switch (i)
		{
		case 0:  cout << "  ________                   _________           .___        __           "; break;
		case 1:  cout << " /  _____/_______ _____     /   _____/ __ __   __| _/ ____  |  | __ __ __ "; break;
		case 2:  cout << "/   \\  ___\\_  __ \\\\__  \\    \\_____  \\ |  |  \\ / __ | /  _ \\ |  |/ /|  |  \\"; break;
		case 3:  cout << "\\    \\_\\  \\|  | \\/ / __ \\_  /        \\|  |  // /_/ |(  <_> )|    < |  |  /"; break;
		case 4:  cout << " \\______  /|__|   (____  / /_______  /|____/ \\____ | \\____/ |__|_ \\|____/ "; break;
		case 5:  cout << "        \\/             \\/          \\/             \\/             \\/       "; break;
		}
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_podpowiedzi(){
	textcolor(RED);
	textbackground(BLACK);

	/*Przygotowanie ramki na podpowiedzi i komentarze*/
	for (short int y = 0; y <= WIELKOSC_PODPOWIEDZI_Y; y++){
		for (short int x = 0; x <= WIELKOSC_PODPOWIEDZI_X; x++){
			gotoxy(POZYCJA_PODPOWIEDZI_X + x, POZYCJA_PODPOWIEDZI_Y + y);

			if (y == WIELKOSC_PODPOWIEDZI_Y)			cout << ramka_dolna;
			else if (x == 0 || x == WIELKOSC_PODPOWIEDZI_X) cout << ramka_pozioma;
			else if (y == 0)								cout << ramka_dolna;
			else if (y == 1 && x == (WIELKOSC_PODPOWIEDZI_X / 2 - 4)) cout << "PODPOWIEDZI";
			else if (y == 18 && x == (WIELKOSC_PODPOWIEDZI_X / 2 - 4)) cout << "KOMENTARZ ";
			else if ((y == 2 || y == 9 || y == 17 || y == WIELKOSC_PODPOWIEDZI_Y - 1) && x == 3)
			{
				for (short int i = 1; i < WIELKOSC_PODPOWIEDZI_X - 4; i++)
				{
					cout << belka_pozioma;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_podpis(){ //funkcja wypisuje imie, nazwisko, nr indeksu i grupy autora gry
	textcolor(LIGHTGREEN);
	textbackground(BLACK);
	gotoxy(POZYCJA_PODPISU_X, POZYCJA_PODPISU_Y);
	cout << "Michal Kobzdaj 127272, gr.7";
}

//----------------------------------------------------------------------------------------------------------------------------
					/////////////////////// Funkcje Zewnêtrzne //////////////////////////
//----------------------------------------------------------------------------------------------------------------------------

void rysuj_wszystko(){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	czysc_ekran();
	rysuj_logo(DOMYSLNY_KOLOR_LOGO); //wybrany domyœlny kolor Loga
	rysuj_plansze();
	rysuj_pomoc();
	rysuj_komunikaty();
	rysuj_podpowiedzi();
	rysuj_podpis();
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void przerysuj_plansze(sudoku* stan, sudoku* stan_zero){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textbackground(BLACK);

	for (short int X = 0; X <= 2; X++){
		for (short int Y = 0; Y <= 2; Y++){
			for (short int x = 0; x <= 2; x++){
				for (short int y = 0; y <= 2; y++)
				{
					/*Pozycjonowanie kursora*/
					gotoxy((POZYCJA_PLANSZY_X + 8 * X + 2 * x + 3), (POZYCJA_PLANSZY_Y + 7 * Y + 2 * y + 2));

					/*Cyfry w stanie ZERO, czyli z poczatku planszy*/
					if (stan_zero->wpisane[X][Y][x][y])
					{
						textcolor(GREEN);
						cout << stan_zero->wpisane[X][Y][x][y];
					}

					/*Cyfry wpisane przez gracza*/
					else if (stan->wpisane[X][Y][x][y])
					{
						textcolor(YELLOW);
						cout << stan->wpisane[X][Y][x][y];
					}

					/*Puste pola*/
					else cout << " ";
				}
			}
		}
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

int dlugosc_komunikatu(char* komunikat)
{
	/*Obliczanie d³ugoœci komunikatu na potrzeby jego wyœrodkowania*/
	int dlugosc = 0;
	while (*(komunikat++) != 0)
		dlugosc++;
	return dlugosc;
}

//----------------------------------------------------------------------------------------------------------------------------

void wypisz_komunikat(char* komunikat){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza

	/*Dbanie o zmiany koloru komunikatów*/
	static int kolor = 0;
	if (++kolor > 15) kolor = 1;
	textbackground(BLACK);

	/*Przechowywanie treœci komunikatów*/
	static char* komunikaty[WIELKOSC_KOMUNIKATOW_Y - 4] = { "" };

	/*Zerowanie zawartoœci wszystkich komórek na komunikaty przy pierwszym wywo³aniu funkcji*/
	static bool pierwszy_raz = 1;
	if (pierwszy_raz)
	{
		for (short i = 0; i < WIELKOSC_KOMUNIKATOW_Y - 4; i++) komunikaty[i] = "";
		pierwszy_raz = 0;
	}

	/*Przesuniêcie komunikatów w górê*/
	for (short i = 1; i < WIELKOSC_KOMUNIKATOW_Y - 4; i++) komunikaty[i - 1] = komunikaty[i];
	
	/*Przypisanie nowego komunikatu na dó³ listy*/
	komunikaty[WIELKOSC_KOMUNIKATOW_Y - 5] = komunikat;

	/*Wypisanie wszystkich komunikatów z pamiêci*/
	for (short i = 0; i<WIELKOSC_KOMUNIKATOW_Y - 4; i++)
	{
			gotoxy(POZYCJA_KOMUNIKATOW_X + 3, POZYCJA_KOMUNIKATOW_Y + 3 + i);
			textcolor((kolor+i) % 15 + 1);
			short centrowanie = (WIELKOSC_KOMUNIKATOW_X - 4 - dlugosc_komunikatu(komunikaty[i])) / 2;
			for (short j = 0; j < centrowanie; j++) cout << " ";
			cout << komunikaty[i];
			for (short j = 0; j < centrowanie; j++) cout << " ";
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void wypisz_krok(kroki* k, bool to_count_or_not_to_count){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textcolor(BLACK);
	textbackground(GREEN);
	gotoxy(POZYCJA_KROKOW_X, POZYCJA_KROKOW_Y);

	/*Wypisanie obecnego kroku w formie np. 13/35, gdzie pierwsza liczba to aktualny krok, a druga to maksymalny*/
	printf("  Krok: %5d/%5d        ", k->stan_obecny->nr, k->stan_koncowy->nr);

	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_zegar(int zegar){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textcolor(BLACK);	
	textbackground(GREEN);
	gotoxy(POZYCJA_ZEGARA_X, POZYCJA_ZEGARA_Y);

	/*Wyœwietlenie zegara jako 13:58 gdzie pierwsza liczba to minuty, a druga to sekundy*/
	printf("  Czas: %4d:%2d            ", zegar/60, zegar%60);
	
	/*dodanie zera w sekundniku jeœli jest mniej ni¿ 10 sekund*/
	if (zegar % 60 < 10){ 
		gotoxy(POZYCJA_ZEGARA_X+13, POZYCJA_ZEGARA_Y);
		printf("0");
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void wypisz_ruchy(long long ruchy){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textcolor(BLACK);
	textbackground(GREEN);
	gotoxy(POZYCJA_KROKOW_X, POZYCJA_KROKOW_Y+1);

	/*Wyœwietlenie podanej do funkcji liczby - s³u¿y do wyœwietlania liczby wykonanych ruchów od w³¹czenia programu*/
	printf(" Laczna ilosc ruchow: %5d", ruchy);

	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

int wypisz_pozostale(sudoku* stan){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textbackground(GREEN);
	textcolor(BLACK);

	/*Obliczenie ile pozosta³o jeszcze niewype³nionych pól w podanym stanie*/
	int pozostale = 0;
	for (short int X = 0; X <= 2; X++){
		for (short int Y = 0; Y <= 2; Y++){
			for (short int x = 0; x <= 2; x++){
				for (short int y = 0; y <= 2; y++)
				{
					if (stan->wpisane[X][Y][x][y] == 0) pozostale++;
				}
			}
		}
	}
	/*Pozycjonowanie linika pod zegarem*/
	gotoxy(POZYCJA_ZEGARA_X, POZYCJA_ZEGARA_Y + 1);

		/*zmiana koloru gdy liczba spadnie poni¿ej pewnych wartoœci*/
		if (pozostale < 40 && pozostale > 15) textcolor(YELLOW);
		if (pozostale <= 15) textcolor(RED);

	/*Wypisanie pozosta³ych pól */
	printf(" Pola do wypelnienia: %2d   ", pozostale);
	
	_setcursortype(_SOLIDCURSOR);
	return pozostale;
}

//----------------------------------------------------------------------------------------------------------------------------

void wypisz_komentarze(sudoku* stan, wspolrzedne kursor, bool edycja_komentarza_wlaczona){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textbackground(BLACK);

	/*Tekst wyœwietla siê tylko gdy edycja komentarza jest w³¹czona,
	gdy nie jest, tekst jest czarny - niewidoczny*/
	if (edycja_komentarza_wlaczona)
		textcolor(RED);
	else
		textcolor(BLACK);

	/*Wypisanie instrukcji postêpowania*/
	short linia = 20;
	for (short j = 0; j <= 4; j++){
		gotoxy(POZYCJA_PODPOWIEDZI_X + 3, POZYCJA_PODPOWIEDZI_Y + linia++);
		switch (j)
		{
		case 0: cout << " Wcisnij  klawisz cyfry"; break;
		case 1: cout << "  aby dodac lub usunac"; break;
		case 2: cout << "      komentarz."; break;

		/*Wypisanie komentarzy w obecnym polu*/
		case 4:
			textcolor(RED);
			cout << "   ";
			for (short i = 1; i <= 9; i++)
			{
				if (stan->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][i] == 1) cout << i << " ";
				else cout << "  ";
			}
			break;
		}
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void podswietl_cyfry(kroki k){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textcolor(LIGHTCYAN); //kolor podœwietlenia
	if (k.podswietlona_cyfra != 0)
	{
		/*Poszukiwanie na ca³ej obecnej planszy cyfr zgodnych z t¹ do podœwietlenia*/
		for (short int X = 0; X <= 2; X++){
			for (short int Y = 0; Y <= 2; Y++){
				for (short int x = 0; x <= 2; x++){
					for (short int y = 0; y <= 2; y++)
					{
						/*Podœwietlenie znalezionej cyfry*/
						if (k.stan_obecny->wpisane[X][Y][x][y] == k.podswietlona_cyfra)
						{
							gotoxy((POZYCJA_PLANSZY_X + 8 * X + 2 * x + 3), (POZYCJA_PLANSZY_Y + 7 * Y + 2 * y + 2));
							cout << k.stan_obecny->wpisane[X][Y][x][y];
						}
					}
				}
			}
		}
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void rysuj_loadsave(bool save_czy_load){
	czysc_ekran();

	/*Rysowanie logo*/
	if (save_czy_load == LOAD) textcolor(LIGHTBLUE);
	if (save_czy_load == SAVE) textcolor(CYAN);
	textbackground(BLACK);
	for (int i = 0; i < 6; i++){
		gotoxy(POZYCJA_LOADSAVE_LOGO_X, POZYCJA_LOADSAVE_LOGO_Y + i);
		switch (i)
		{
		case 0:  cout << "  ______             _         _            "; break;
		case 1:  cout << " / _____)           | |       | |           "; break;
		case 2:  cout << "( (____   _   _   __| |  ___  | |  _  _   _ "; break;
		case 3:  cout << " \\____ \\ | | | | / _  | / _ \\ | |_/ )| | | |"; break;
		case 4:  cout << " _____) )| |_| |( (_| || |_| ||  _ ( | |_| |"; break;
		case 5:  cout << "(______/ |____/  \\____| \\___/ |_| \\_)|____/ "; break;
		}
	}

	/*Rysowanie ramki*/
	for (short int y = 0; y <= WIELKOSC_LOADSAVE_Y; y++){
		for (short int x = 0; x <= WIELKOSC_LOADSAVE_X; x++){
			gotoxy(POZYCJA_LOADSAVE_X + x, POZYCJA_LOADSAVE_Y + y);

			if (y == WIELKOSC_LOADSAVE_Y)			cout << ramka_dolna;
			else if (x == 0 || x == WIELKOSC_LOADSAVE_X) cout << ramka_pozioma;
			else if (y == 0)								cout << ramka_dolna;
			else if (y == 1 && x == (WIELKOSC_LOADSAVE_X / 2 - 8))
			{
				if (save_czy_load == LOAD) cout << "=== Odczyt gry ===";
				if (save_czy_load == SAVE) cout << "=== Zapis gry ===";
			}
			else if ((y == 2 || y == WIELKOSC_LOADSAVE_Y - 1) && x == 3)
			{
				for (short int i = 1; i < WIELKOSC_LOADSAVE_X - 4; i++)
				{
					cout << belka_pozioma;
				}
			}
		}
	}
	
	/*Wypisanie instrukcji postepowania w obu przypadkach save/load*/
	for (int i = 0; i < WIELKOSC_LOADSAVE_Y; i++){
		gotoxy(POZYCJA_LOADSAVE_X + 5, POZYCJA_LOADSAVE_Y + i + 3);
		switch (i)
		{
		case 0:  cout << "   Wpisz nazwe swojego pliku do " << (save_czy_load == SAVE ? "zapisu." : "odczytu."); break;
		case 1:  cout << "     Nie wpisuj rozszerzenia \".xml\"."; break;
		case 2:  cout << "        Dostepne sa jedynie male"; break;
		case 3:  cout << "           litery oraz SPACJA. "; break;
		case 4:  cout << "     Nazwe potwierdz klawiszem ENTER."; break;
		case 5:  cout << "   Aby anulowac wcisnij klawisz ESCAPE."; break;
		case 6:  cout << "  Pomylki poprawiaj klawiszem BACKSPACE."; break;
		//case 7:  cout << " Wcisnij klawisz \"@\" aby wyswietlic pliki."; break;
		}
	}
	/*Rysowanie rameczki na adres pliku*/
	for (short i = 0; i < 3; i++){
		gotoxy(POZYCJA_LOADSAVE_X + 5, POZYCJA_LOADSAVE_Y + i + 11);
		if (i == 0)			cout << naroznik_lewy_gorny;
		if (i == 1)			cout << belka_pionowa;
		if (i == 2)			cout << naroznik_lewy_dolny;
		for (short j = 0; j < MAX_DLUGOSC_PLIKU +2; j++)
		{
			if (i == 1) cout << " ";
			else		cout << belka_pozioma;
		}
		if (i == 0)			cout << naroznik_prawy_gorny;
		if (i == 1)			cout << belka_pionowa;
		if (i == 2)			cout << naroznik_prawy_dolny;
	}
}

//----------------------------------------------------------------------------------------------------------------------------
char* ekran_powitalny(){
	czysc_ekran();
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	textcolor(LIGHTGREEN);
	textbackground(BLACK);

	for (int i = 0; i <= 20; i++){
		gotoxy(POZYCJA_WYGRANEJ_X, POZYCJA_WYGRANEJ_Y + i);
		switch (i)
		{
			/*Stare logo ekranu powitalnego (za du¿e)*/
		/*case 1: cout << "   SSSSSSSSSSSSSSS                               d::::::d                 kkkkkkkk                             "; break;
		case 2: cout << " SS:::::::::::::::S                              d::::::d                 k::::::k                             "; break;
		case 3: cout << "S:::::SSSSSS::::::S                              d::::::d                 k::::::k                             "; break;
		case 4: cout << "S:::::S     SSSSSSS                              d:::::d                  k::::::k                             "; break;
		case 5: cout << "S:::::S            uuuuuu    uuuuuu      ddddddddd:::::d    ooooooooooo    k:::::k    kkkkkkkuuuuuu    uuuuuu  "; break;
		case 6: cout << "S:::::S            u::::u    u::::u    dd::::::::::::::d  oo:::::::::::oo  k:::::k   k:::::k u::::u    u::::u  "; break;
		case 7: cout << " S::::SSSS         u::::u    u::::u   d::::::::::::::::d o:::::::::::::::o k:::::k  k:::::k  u::::u    u::::u  "; break;
		case 8: cout << "  SS::::::SSSSS    u::::u    u::::u  d:::::::ddddd:::::d o:::::ooooo:::::o k:::::k k:::::k   u::::u    u::::u  "; break;
		case 9: cout << "    SSS::::::::SS  u::::u    u::::u  d::::::d    d:::::d o::::o     o::::o k::::::k:::::k    u::::u    u::::u  "; break;
		case 10: cout << "       SSSSSS::::S u::::u    u::::u  d:::::d     d:::::d o::::o     o::::o k:::::::::::k     u::::u    u::::u  "; break;
		case 11: cout << "            S:::::Su::::u    u::::u  d:::::d     d:::::d o::::o     o::::o k:::::::::::k     u::::u    u::::u  "; break;
		case 12: cout << "            S:::::Su:::::uuuu:::::u  d:::::d     d:::::d o::::o     o::::o k::::::k:::::k    u:::::uuuu:::::u  "; break;
		case 13: cout << "SSSSSSS     S:::::Su:::::::::::::::uud::::::ddddd::::::ddo:::::ooooo:::::ok::::::k k:::::k   u:::::::::::::::uu"; break;
		case 14: cout << "S::::::SSSSSS:::::S u:::::::::::::::u d:::::::::::::::::do:::::::::::::::ok::::::k  k:::::k   u:::::::::::::::u"; break;
		case 15: cout << "S:::::::::::::::SS   uu::::::::uu:::u  d:::::::::ddd::::d oo:::::::::::oo k::::::k   k:::::k   uu::::::::uu:::u"; break;
		case 16: cout << " SSSSSSSSSSSSSSS       uuuuuuuu  uuuu   ddddddddd   ddddd   ooooooooooo   kkkkkkkk    kkkkkkk    uuuuuuuu  uuuu"; break;
		case 17: cout << ""; break;
		case 18: cout << "==============================================================================================================="; break;
		case 19: cout << "                                          Michal Kobzdaj, 127272, gr.7										 "; break;
		case 20: cout << "                               Wydzial Elektroniki, Telekomunikacji i Informatyki								 "; break;
		case 21: cout << "                                             Informatyka, semestr 1											 "; break;
		case 22: cout << "==============================================================================================================="; break;
		case 23: cout << "		1: Zaladuj podstawowa plansze do Sudoku			"; break;
		case 24: cout << "		2: Zaladuj trudna plansze do Sudoku				"; break;
		case 25: cout << "		3: Wczytaj gre z AutoSave.xml					"; break;*/

			/*Nowe logo i przedstawienie autora*/
		case 1: cout << "     _______           ______   _______  _                "; break;
		case 2: cout << "    (  ____ \\|\\     /|(  __  \\ (  ___  )| \\    /\\|\\     /|"; break;
		case 3: cout << "    | (    \\/| )   ( || (  \\  )| (   ) ||  \\  / /| )   ( |"; break;
		case 4: cout << "    | (_____ | |   | || |   ) || |   | ||  (_/ / | |   | |"; break;
		case 5: cout << "    (_____  )| |   | || |   | || |   | ||   _ (  | |   | |"; break;
		case 6: cout << "          ) || |   | || |   ) || |   | ||  ( \\ \\ | |   | |"; break;
		case 7: cout << "    /\\____) || (___) || (__/  )| (___) ||  /  \\ \\| (___) |"; break;
		case 8: cout << "    \\_______)(_______)(______/ (_______)|_/    \\/(_______)"; break;
		case 9: cout << ""; textcolor(YELLOW); break;
		case 10: cout << "============================================================"; break;
		case 11: cout << "                 Michal Kobzdaj, 127272, gr.7				 "; break;
		case 12: cout << "      Wydzial Elektroniki, Telekomunikacji i Informatyki	 "; break;
		case 13: cout << "                   Informatyka, semestr 1					 "; break;
		case 14: cout << "============================================================"; textcolor(LIGHTGREEN); break;
		case 15: cout << "     _____        ESC: Wylacz program					"; break;
		case 16: cout << "    || 1 ||       1: Zaladuj podstawowa plansze do Sudoku	"; break;
		case 17: cout << "    ||___||       2: Zaladuj trudna plansze do Sudoku		"; break;
		case 18: cout << "    |/___\\|       3: Wczytaj gre z AutoSave.xml			"; break;
		case 19: cout << "                  4: Odczytaj gre z wybranego pliku		"; break;
		case 20: cout << " "; break; //pozycjonowanie komunikatów systemowych
		}
	}

	/*Zczytywanie klawisza i na jego podstawie gra siê koñczy lub wczytuje wskazany plik*/
	char klawisz = 0;
	while (TRUE){
		klawisz = getch();
		if (klawisz == ESCAPE) exit(EXIT_SUCCESS);;
		if (klawisz == '1') return "!Start01.xml";
		if (klawisz == '2') return "!Start02.xml";
		if (klawisz == '3') return "_AutoSave.xml";
		if (klawisz == '4') return pisanie_nazwy_xml(LOAD);
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void wygrana(int zegar, long long ruchy){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	czysc_ekran();

	for (int i = 0; i <= 16; i++){
		gotoxy(POZYCJA_WYGRANEJ_X, POZYCJA_WYGRANEJ_Y + i);
		switch (i)
		{
			/*Komunikat o wygranej*/
		case 0:  cout << "    ____                 __            __                 "; break;
		case 1:  cout << "   /\\  _`\\              /\\ \\          /\\ \\                "; break;
		case 2:  cout << "   \\ \\,\\L\\_\\   __  __   \\_\\ \\     ___ \\ \\ \\/'\\    __  __  "; break;
		case 3:  cout << "    \\/_\\__ \\  /\\ \\/\\ \\  /'_` \\   / __`\\\\ \\ , <   /\\ \\/\\ \\ "; break;
		case 4:  cout << "      /\\ \\L\\ \\\\ \\ \\_\\ \\/\\ \\L\\ \\ /\\ \\L\\ \\\\ \\ \\\\`\\ \\ \\ \\_\\ \\"; break;
		case 5:  cout << "      \\ `\\____\\\\ \\____/\\ \\___,_\\\\ \\____/ \\ \\_\\ \\_\\\\ \\____/"; break;
		case 6:  cout << "       \\/_____/ \\/___/  \\/__,_ / \\/___/   \\/_/\\/_/ \\/___/ "; break;
		case 7:  cout << ""; break;
		case 8:  cout << "================================================================"; break;
		case 9:  cout << "===                                                          ==="; break;
		case 10: cout << "===                     !!! WYGRALES !!!                     ==="; break;
		case 11: cout << "===                                                          ==="; break;
		case 12: cout << "================================================================"; break;
		case 13: cout << ""; break;
		case 14: printf( "  Twoj czas: %3d : %2d                      Liczba ruchow: %4d", zegar / 60, zegar % 60, ruchy); 

			/*dodanie zera w sekundniku jeœli jest mniej ni¿ 10 sekund*/
			if (zegar % 60 < 10){ 
				gotoxy(POZYCJA_WYGRANEJ_X + 19, POZYCJA_WYGRANEJ_Y + i);
				printf("0");
			}
			break;

		case 16: cout << " "; //pozycjonowanie tekstu wypisywanego przez system
		}
	}
	
	/*Wstrzymanie gry na sekundê by siê nie wy³¹czy³a sama*/
	Sleep(1000);
	exit(EXIT_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------------

void wyjscie_z_gry(kroki* k){
	_setcursortype(_NOCURSOR); //zapobieganie pojawiania siê kursora w losowych miejscach poza plansza
	czysc_ekran();
	textcolor(RED);
	textbackground(BLACK);

	for (int i = 0; i <= 16; i++){
		gotoxy(POZYCJA_WYGRANEJ_X, POZYCJA_WYGRANEJ_Y + i);
		switch (i)
		{
			/*Komunikat koñca gry*/
		case 1: cout << "              _____             __        __						"; break;
		case 2: cout << "             / ___/ __  __ ____/ /____   / /__ __  __			"; break;
		case 3: cout << "             \\__ \\ / / / // __  // __ \\ / //_// / / /			"; break;
		case 4: cout << "            ___/ // /_/ // /_/ // /_/ //  <  / /_/ /				"; break;
		case 5: cout << "           /____/ \\____/ \\____/ \\____//_/\\_\\ \\____/		"; break;

		case 8:  cout << "================================================================"; break;
		case 9:  cout << "===                                                          ==="; break;
		case 10: cout << "===                        Koniec gry.                       ==="; break;
		case 11: cout << "===                                                          ==="; break;
		case 12: cout << "================================================================"; break;
		case 13: cout << ""; break;
		case 14: cout << " Twoja gra zostala automatycznie zapisana do pliku autosave.xml"; break;
		case 15:
		case 16: cout << " "; //pozycjonowanie tekstu wypisywanego przez system
		}
	}

	/*AutoSave*/
	zapis_xml("_AutoSave.xml", k);
}




//----------------------------------------------------------------------------------------------------------------------------
					/////////////////////// POMOC W PROGRAMOWANIU (nieu¿ywane w projekcie) //////////////////////////
//----------------------------------------------------------------------------------------------------------------------------

void wypisz_ascii(){
	clrscr();
	gotoxy(1,1);
	textcolor(GREEN);
	textbackground(BLACK);
	for (int i = 0; i < 256; i++){
		cout << i << " " << (char)i << "\t";
		if (i % 10 == 9) cout << endl;
	}
}

void wyswietl_pliki(bool save_czy_load){
	czysc_ekran();
	textcolor(WHITE);
	textbackground(BLACK);

	system("dir");
	system("pause");
	czysc_ekran();
	rysuj_loadsave(save_czy_load);
}