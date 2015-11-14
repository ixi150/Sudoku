#include "lacznik.h"
#include "conio2.h"
using namespace std;

//----------------------------------------------------------------------------------------------------------------------------

bool sprawdz(short porownywane, sudoku* stan, wspolrzedne kursor){

	for (short i = 0; i <= 2; i++){
		for (short j = 0; j <= 2; j++){
			if (stan->wpisane[i][kursor.Y][j][kursor.y] == porownywane					// sprawdzanie w poziomie
				|| stan->wpisane[kursor.X][i][kursor.x][j] == porownywane				// sprawdzanie w pionie
				|| stan->wpisane[kursor.X][kursor.Y][i][j] == porownywane) return 0;	// sprawdzanie w kwadracie
		}
	}
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------------

void zeruj_stan(sudoku* stan){
	for (short int i = 0; i <= 2; i++){
		for (short int j = 0; j <= 2; j++){
			for (short int k = 0; k <= 2; k++){
				for (short int l = 0; l <= 2; l++){
					stan->wpisane[i][j][k][l] = 0; //zerowanie cyfr
					for (short int b = 0; b < 10; b++)
						stan->komentarz[i][j][k][l][b] = FALSE; //zerowanie komentarzy
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void dostepne_cyfry(sudoku* stan, wspolrzedne kursor){
	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	textcolor(RED);
	short linia = 3;
	for (short j = 0; j <= 4; j++){
		gotoxy(POZYCJA_PODPOWIEDZI_X + 3, POZYCJA_PODPOWIEDZI_Y + linia++);
		switch (j)
		{
			/*wypisanie instrukcji*/
		case 0: cout << "   Cyfry mozliwe do   "; break;
		case 1: cout << " wpisania we wskazanej"; break;
		case 2: cout << "      komorce to :    "; break;

		case 4:
			/*Wypisanie wszystkich pasuj¹cych cyfr*/
			cout << "   ";
			for (short i = 1; i <= 9; i++)
			{
				if (sprawdz(i, stan, kursor)) cout << i << " ";
				else cout << "  ";
			}
			break;
		}
	}

	/*Je¿eli w oznaczonym polu jest jakaœ liczba to jest wyœwietlana na czerwonym tle*/
	textbackground(RED);
	textcolor(YELLOW);
	gotoxy((POZYCJA_PLANSZY_X + 8 * kursor.X + 2 * kursor.x + 3), (POZYCJA_PLANSZY_Y + 7 * kursor.Y + 2 * kursor.y + 2));
	if (stan->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] != 0)
		cout << stan->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y];
	else cout << " ";
	
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void czysc_dostepne_cyfry() {
	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	textcolor(BLACK);

	short linia = 3;
	for (short j = 0; j <= 4; j++){
		gotoxy(POZYCJA_PODPOWIEDZI_X + 3, POZYCJA_PODPOWIEDZI_Y + linia++);
		cout << "                        "; //czyszczenie lini poprzez wpisanie ci¹gu spacji.
	}
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

int szukaj_jednoznacznej(sudoku* stan, wspolrzedne kursor){ //funkcja szuka jedynej cyfry która by pasowa³a do pola w którym jest kursor
	int cyfra = 0;
	
	if (stan->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] == 0) //funkcja nie szuka w polach gdzie znajduje siê ju¿ cyfra
	{
		for (short i = 1; i <= 9; i++)
		{
			if (sprawdz(i, stan, kursor))
			{
				if (cyfra == 0) cyfra = i;
				else return 0; //znaleziono conajmniej 2 pasuj¹ce cyfry
			}
		}
		if (cyfra != 0) return cyfra;
	}
	return 0; //nie znaleziono jednoznacznej
}
//----------------------------------------------------------------------------------------------------------------------------
void jednoznaczna_podpowiedz(kroki* wszystkie){
	_setcursortype(_NOCURSOR);
	
	int cyfra;
	wspolrzedne kursor; //w³asny roboczy kursor

	/*Przejœcie po wszystkich mo¿liwych polach*/
	for (kursor.X = 0; kursor.X <= 2; kursor.X++){
		for (kursor.Y = 0; kursor.Y <= 2; kursor.Y++){
			for (kursor.x = 0; kursor.x <= 2; kursor.x++){
				for (kursor.y = 0; kursor.y <= 2; kursor.y++){

					cyfra = szukaj_jednoznacznej(wszystkie->stan_obecny, kursor);
					if (cyfra != 0) //gdy funkcja zwróci zero, znaczy to ¿e nie znaleziono jednoznacznej liczby i nale¿y szukaæ dalej
					{
						/*Ustawienie kursora w aktualnie sprawdzanym polu*/
						gotoxy((POZYCJA_PLANSZY_X + 8 * kursor.X + 2 + 2 * kursor.x + 1), (POZYCJA_PLANSZY_Y + 7 * kursor.Y + 1 + 2 * kursor.y + 1));
						textbackground(LIGHTBLUE);
						textcolor(LIGHTRED);
						/*wpisanie tymczasowo znalezionej cyfry w miejscu gdzie ona pasuje*/
						cout << cyfra;

						/*Instrukcja postepowania*/
						textbackground(BLACK);
						textcolor(LIGHTBLUE);
						short linia = 10;
						short ile_wierszy = 7;
						for (short j = 0; j < ile_wierszy; j++){
							gotoxy(POZYCJA_PODPOWIEDZI_X + 3, POZYCJA_PODPOWIEDZI_Y + linia++);
							switch (j)
							{
							case 0: cout << " Wskazana   podpowiedz "; break;
							case 1: cout << "  jest  jednoznaczna. "; break;
							case 2: cout << "   Jezeli chcesz ja "; break;
							case 3: cout << "   wpisac, kliknij "; break;
							case 4: cout << "  ponownie klawisz 'p'."; break;
							case 5: cout << " Aby anulowac, kliknij"; break;
							case 6: cout << "dowolny inny klawisz."; break;
							case 7: cout << ""; break;
							}
						}

						/*Klikniêcie przez gracza klawisza 'p' potwirdza wpisanie cyfry*/
						char klawisz = getchar();
						if (klawisz == 'p')
						{
							wypisz_krok(wszystkie, TRUE);
							dodaj_element(wszystkie);
							//wszystkie->stan_obecny = kolejny_krok(wszystkie);
							wszystkie->stan_obecny->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] = cyfra;
						}
						else
							wypisz_komunikat("Anulowano podpowiedz.");

						/*czyszczenie okna podpowiedzi*/
						linia -= ile_wierszy;
						textbackground(BLACK);
						for (short j = 0; j < ile_wierszy; j++){
							gotoxy(POZYCJA_PODPOWIEDZI_X + 3, POZYCJA_PODPOWIEDZI_Y + linia++);
							for (int i = 0; i < WIELKOSC_PODPOWIEDZI_X - 5; i++)
								cout << " ";
						}

						/*Pomyœlne zakoñczenie funkcji*/
						_setcursortype(_SOLIDCURSOR);
						return;
					}
				}
			}
		}
	}
	/*Niepomyœlne zakoñczenie funkcji*/
	wypisz_komunikat("Sorry, nie znalazlem :(");
	_setcursortype(_SOLIDCURSOR);
}

//----------------------------------------------------------------------------------------------------------------------------

void sprawdz_komentarze(sudoku* stan, wspolrzedne kursor){
	for (short i=0; i<10; i++)
	{
		if (stan->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][i] == TRUE)
		{
			stan->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][0] = TRUE; //ustalenie ze w danym punkcie istnieje komentarz
			return;
		}
	}
	stan->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][0] = FALSE; //ustalenie ze w punkcie NIE ma komentarza
}

//----------------------------------------------------------------------------------------------------------------------------
						//======================== LISTA ===============================
//----------------------------------------------------------------------------------------------------------------------------

void dodaj_zerowy_element(kroki* k){
	sudoku* temp = (sudoku*)malloc(sizeof(sudoku));
	temp->next = temp->prev = temp;
	zeruj_stan(temp);		//wpisz wszedzie zera (komentarze i wpisane)
	temp->nr = 0;
	k->stan_obecny = k->stan_zero = k->stan_koncowy = temp;
}

//----------------------------------------------------------------------------------------------------------------------------

void usun_elementy_po_obecnym(kroki* k){
	if (k->stan_obecny != k->stan_obecny->next)
	{
		sudoku* temp = k->stan_obecny->next;
		k->stan_koncowy = k->stan_obecny;

		//while (temp->nr >= 0)  //taki dziala idealnie
		while (temp != k->stan_obecny) 
		{
			k->stan_obecny = temp;
			temp = temp->next;
			free ( k->stan_obecny);
		}
		k->stan_obecny = k->stan_obecny->next = k->stan_koncowy;
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void dodaj_element(kroki* k){
	if (k->stan_obecny->next != k->stan_obecny) //sprawdzenie czy jest to ostatni element listy
		usun_elementy_po_obecnym(k);

	sudoku* temp = (sudoku*)malloc(sizeof(sudoku));
	*temp = *k->stan_obecny;	//przepisz wartosci komentarzy i wpisanych
	temp->next = temp;
	temp->prev = k->stan_obecny;
	k->stan_koncowy = k->stan_obecny = k->stan_obecny->next = temp;
	(temp->nr)++;
}

//----------------------------------------------------------------------------------------------------------------------------

sudoku* znajdz_element_nr(int i, kroki* k){
	sudoku* temp = k->stan_zero;
	while (temp->nr < i)
	{
		if (temp == temp->next) //sprawdzenie czy jest to ostatni element listy
			dodaj_element(k);
		else
			temp = temp->next;
	}
	return temp;
}