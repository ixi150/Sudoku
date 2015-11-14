#include "lacznik.h"
#include "conio2.h"
#include<stdio.h>
using namespace std;

//----------------------------------------------------------------------------------------------------------------------------

char* pisanie_nazwy_xml(bool save_czy_load){
	rysuj_loadsave(save_czy_load);
	gotoxy(POZYCJA_LOADSAVE_X + 7, POZYCJA_LOADSAVE_Y + 12);
	char klawisz;
	static char napis[MAX_DLUGOSC_PLIKU+4];
	int ile_liter = 0;
	do
	{
		klawisz = getch();
		if ((klawisz >= 'a' && klawisz <= 'z' || klawisz == ' ') && ile_liter < MAX_DLUGOSC_PLIKU)
		{
			cout << klawisz;
			napis[ile_liter++] = klawisz;
		}
		else if (klawisz == BACKSPACE && ile_liter > 0)
		{
			cout << "\b \b";
			ile_liter--;
		}
		//else if (klawisz == '@') wyswietl_pliki(save_czy_load);
		else if (klawisz == ESCAPE) return "#ANULOWANO";
	} while (klawisz != ENTER);

	/*konwersja sciezki pliku na XML*/
	napis[ile_liter + 0] = '.';
	napis[ile_liter + 1] = 'x';
	napis[ile_liter + 2] = 'm';
	napis[ile_liter + 3] = 'l';
	napis[ile_liter + 4] = NULL;
	return napis;
}

//----------------------------------------------------------------------------------------------------------------------------

int dlugosc(char* wyraz){ //funkcja mierzy d³ugoœæ podanego wyrazu nie wliczaj¹c koñcowego NULL
	int ile = 0;
	while (wyraz[ile] != 0) ile++;
	return ile;
}

//----------------------------------------------------------------------------------------------------------------------------

bool porownaj(char* wyraz, char* slowo){ //funkcja zwraca TRUE jeœli oba podane wyrazy s¹ identyczne
	for (int i = 0; i < dlugosc(wyraz); i++)
	{
		if (slowo[i] != wyraz[i])
			return 0;
	}
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------------

void przesun_wyraz_w_lewo(char* wyraz, int dl){ //funkcja przesuwa w lewo litery wewn¹trz podanej tablicy
	for (int i = 1; i < dl; i++)
		wyraz[i - 1] = wyraz[i];
}

//----------------------------------------------------------------------------------------------------------------------------

bool szukaj(FILE* plik, char* wyraz, char* koniec){ //funkcja zwraca TRUE jeœli znajdzie w pliku WYRAZ, zanim pojawi siê w tekœcie KONIEC.
	/*Stworzenie roboczych zmiennych na wyrazy podane w parametrach*/
	int dl_wyraz = dlugosc(wyraz);
	int dl_koniec = dlugosc(koniec);
	char* roboczy = new char[dl_wyraz];
	char* koncowy = new char[dl_koniec];

	do
	{
		przesun_wyraz_w_lewo(roboczy, dl_wyraz);
		przesun_wyraz_w_lewo(koncowy, dl_koniec);

		/*Wpisanie kolejnego znaku z pliku na koniec obu roboczych tablic*/
		roboczy[dl_wyraz - 1] =	koncowy[dl_koniec - 1] = fgetc(plik);

		if (porownaj(koniec, koncowy))
		{
			delete roboczy;
			delete koncowy;
			return FALSE;
		}
	} while (porownaj(wyraz, roboczy) == 0);

	delete roboczy;
	delete koncowy;
	return TRUE;
}



//----------------------------------------------------------------------------------------------------------------------------

int szukaj_tagow(FILE* plik){ //funkcja znajduje który znacznik pojawia siê w pliku jako pierwszy z tych trzech {<board>,<comments>,</state>}

	char* b = "<board";					char* c = "<comments";						char* k = "</state";
	int dl_board = dlugosc(b);			int dl_comments = dlugosc(c);				int dl_koniec = dlugosc(k);
	char* board = new char[dl_board+1];	char* comments = new char[dl_comments+1];	char* koniec = new char[dl_koniec+1];
	board[dl_board] = 0;				comments[dl_comments] = 0;					koniec[dl_koniec] = 0;

	int odpowiedz = 0;
	do
	{
		przesun_wyraz_w_lewo(board, dl_board);
		przesun_wyraz_w_lewo(comments, dl_comments);
		przesun_wyraz_w_lewo(koniec, dl_koniec);

		board[dl_board-1] = comments[dl_comments - 1] = koniec[dl_koniec - 1] = fgetc(plik);

		if (porownaj(b, board)) { odpowiedz = 1; break; }		//znalaz³ <board>
		if (porownaj(c, comments)) { odpowiedz = 2; break; }	//znalaz³ <comments>
		
	} while (porownaj(k, koniec) == 0);

	delete board;
	delete comments;
	delete koniec;

	return odpowiedz;
}

//----------------------------------------------------------------------------------------------------------------------------

void przepisz_board(char* plansza, FILE* plik){ //funkcja spisuje z pliku pierwsze 81 cyfr (i znaków '-') znajduj¹cych siê w znaczniku <board>
	for (int i = 0; i < 81; i++){
		char znak = 0;
		while ((znak < '0' || znak > '9') && znak != '-'){
			znak = fgetc(plik);

		}
		if (znak == '-')	plansza[i] = '0';
		else				plansza[i] = znak;
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void przepisz_comment(sudoku* stan, FILE* plik){ //funkcja spisuje pojedynczy znacznik <comment> do podanego stanu
	char znak = 0;
	int row=0;
	int col=0;

	/*Szukanie pierwszej cyfry od kursora*/
	while (znak < '0' || znak > '9')
		znak = fgetc(plik);

	/*Wpisanie tej cyfry do row/col w zale¿noœci od tego, która z nich wystêpuje jako druga*/
	if (szukaj(plik, "col", "row")) row = znak - '0';
	else							col = znak - '0';
	znak = 0;

	/*Szukanie pierwszej cyfry od kursora*/
	while (znak < '0' || znak > '9')
		znak = fgetc(plik);

	/*Wpisanie znalezionej cyfry do zmiennej, która jeszcze jest pusta*/
	if (row == 0)	row = znak - '0';
	else			col = znak - '0';

	/*Obliczenie wspó³rzêdnych po danych row/col*/
	short X = (col - 1) / 3;
	short Y = (row - 1) / 3;
	short x = (col - 1) % 3;
	short y = (row - 1) % 3;

	/*Czyszczenie zawartoœci komentarza*/
	for (short int i = 0; i < 10; i++)
		stan->komentarz[X][Y][x][y][i] = FALSE;

	znak = 0;
	while (znak != '<'){
		znak = fgetc(plik);
		/*Szukanie cyfr w pliku*/
		if (znak >= '1' && znak <= '9'){

			/*Wpisywanie komentarza w danym polu*/
			stan->komentarz[X][Y][x][y][znak - '0'] = TRUE;

			/*Ustalenie, ¿e komentarz w tym polu istnieje*/
			if (stan->komentarz[X][Y][x][y][0] == FALSE)
				stan->komentarz[X][Y][x][y][0] = TRUE;
		}
	}
	
}

//----------------------------------------------------------------------------------------------------------------------------

int czytaj_state(FILE* plik){
	int state = 0;
	char znak = 0;

	do{
		znak = fgetc(plik);
		if (znak >= '0' && znak <= '9'){
			state *= 10;
			state = state + znak - '0';
		}
	} while (znak != '\"');
	return state;
}

//----------------------------------------------------------------------------------------------------------------------------

void odczyt_xml(char* adres, kroki* k){
	rysuj_wszystko();

	if (adres[0] == '#') //sprawdzenie czy wpisywanie nazwy zostalo anulowane
		wypisz_komunikat("Anulowano odczyt");
	else
	{

	FILE *plik;
	fopen_s(&plik, adres, "r");

	if (plik == NULL) //sprawdzenie czy istnieje wskazany plik
	{
		wypisz_komunikat("Nie ma takiego pliku! :[");
		return;
	}
	if (adres[0] != '!' && adres[0] != '_') //sprawdzenie czy nie jest to plik !Start01.xml czy _AutoSave
		wypisz_komunikat("Odczytano gre!");
	
		k->stan_obecny = k->stan_zero;
		usun_elementy_po_obecnym(k);
		delete k->stan_obecny;
		dodaj_zerowy_element(k);

		szukaj(plik, "<sudoku", "<state");
		szukaj(plik, "active-state", ">");
		szukaj(plik, "\"", ">");
		int active_state = czytaj_state(plik);
		int state;
		//////////////////////////////////////////////////////

		while (szukaj(plik, "<state", "</sudoku")){
			szukaj(plik, "\"", ">");
			state = czytaj_state(plik);
			sudoku* temp = znajdz_element_nr(state, k);
			int tag = 0;
			do
			{
				tag = szukaj_tagow(plik);
				switch (tag)
				{
				case 1:	//board
					char plansza[81];
					przepisz_board(plansza, plik);
					for (int i = 0; i < 81; i++){
						temp->wpisane[((i % 27) % 9) / 3][i / 27][i % 3][(i % 27) / 9] = plansza[i] - '0';
					}
					break;

				case 2:	//comments

					while (szukaj(plik, "<comment", "</comments")){
						przepisz_comment(temp, plik);
					}
					break;
				}
				
			} while (tag != 0);
		}
		//////////////////////////////////////////////////////
		k->stan_obecny = znajdz_element_nr(active_state, k);
		fclose(plik);
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void zapis_xml(char* adres, kroki* k){
	if (adres[0] != '_') //sprawdzenie czy nie jest to plik _AutoSave.xml
	{
		rysuj_wszystko();
		wypisz_komunikat("Zapisano gre!");
	}

	if (adres[0] == '#') //sprawdzenie czy wpisywanie nazwy zostalo anulowane
		wypisz_komunikat("Anulowano zapis");
	else
	{
		FILE *plik;
		fopen_s(&plik, adres, "w");

		fprintf(plik, "<?xml version=\"1.0\"?>	\n");
		fprintf(plik, "<sudoku active-state = \"%d\">\n\n", k->stan_obecny->nr);
		///////////////////////////////////////////////////////////////////////////////////////////////
		sudoku* stan_zapisywany = k->stan_zero;


		do
		{
			fprintf(plik, "<state nr=\"%d\">", stan_zapisywany->nr);
			fprintf(plik, "\n\t<board>");
			for (int j = 0; j < 81; j++)
			{
				if (j % 9 == 0) fprintf(plik, "\n\t\t");
				int cyfra = stan_zapisywany->wpisane[((j % 27) % 9) / 3][j / 27][j % 3][(j % 27) / 9];
				if (cyfra == 0) fprintf(plik, "- ");
				else			fprintf(plik, "%1d ", cyfra);
			}
			fprintf(plik, "\n\t</board>\n");
			////////////////////////////////////////////////
			fprintf(plik, "\t<comments>\n");
			for (short int X = 0; X <= 2; X++){
				for (short int Y = 0; Y <= 2; Y++){
					for (short int x = 0; x <= 2; x++){
						for (short int y = 0; y <= 2; y++){
							if (stan_zapisywany->komentarz[X][Y][x][y][0] == TRUE)
							{
								fprintf(plik, "\t\t<comment row=\"%d\" col=\"%d\">", Y * 3 + y + 1, X * 3 + x + 1);
								for (short cyfra = 1; cyfra <= 9; cyfra++)
								{
									if (stan_zapisywany->komentarz[X][Y][x][y][cyfra] == TRUE)
										fprintf(plik, "%d ", cyfra);
								}
								fprintf(plik, "</comment>\n");
							}
						}
					}
				}
			}
			fprintf(plik, "\t</comments>\n");
			////////////////////////////////////////////////
			fprintf(plik, "</state>\n\n");

			if (stan_zapisywany == stan_zapisywany->next)
				break;
			else
				stan_zapisywany = stan_zapisywany->next;
		} while (TRUE);
		///////////////////////////////////////////////////////////////////////////////////////////////
		fprintf(plik, "\n</sudoku>");
		fclose(plik);
	}
}
