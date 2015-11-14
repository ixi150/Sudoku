#include "lacznik.h" 
#include "conio2.h"
using namespace std;


int main()
{
	/*Podstawowe zmienne i wielkosc ekranu*/
	settitle("Michal Kobzdaj 127272");
	ResizeConsole(GetStdHandle(STD_OUTPUT_HANDLE), SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, 1);
	short klawisz = 0;
	wspolrzedne kursor = { 0, 0, 0, 0 };
	sudoku* wskaznik_sprawdzajacy; //przyda sie do kontroli odczytu gry
	bool edycja_komentarza_wlaczona = FALSE;
	bool podswietlenie_cyfry_wlaczone = FALSE;
	long long ruchy = 0;
	kroki wszystkie; //ta zmienna przechowuje wskaüniki na kluczowe elementy listy
	dodaj_zerowy_element(&wszystkie);

	/*Ekran powitalny, a w nim wybor pliku do wczytania*/
	odczyt_xml(ekran_powitalny(), &wszystkie);

	/*Zmienne czasowe*/
	time_t start_zegara = GetTickCount();
	int sekundy = 0;

	/*Przygotowanie planszy i innych grafik*/
	wypisz_krok(&wszystkie, FALSE);
	wypisz_pozostale(wszystkie.stan_obecny);
	wypisz_ruchy(ruchy);
	rysuj_zegar(sekundy);
	przerysuj_plansze(wszystkie.stan_obecny, wszystkie.stan_zero);
	wypisz_komentarze(wszystkie.stan_obecny, kursor, edycja_komentarza_wlaczona);

//----------------------------------------------------------------------------------------------------------------------------
/////////////////////// G£”WNA P TLA GRY SUDOKU //////////////////////////
//----------------------------------------------------------------------------------------------------------------------------
do {
	/*Tykanie zegara*/
	if (sekundy * CLOCKS_PER_SEC < GetTickCount() - start_zegara)
	{
		rysuj_zegar(++sekundy);
		if (sekundy % 60 == 0) wypisz_komunikat("Minela minuta");
		gotoxy((POZYCJA_PLANSZY_X + 8 * kursor.X + 2 + 2 * kursor.x + 1), (POZYCJA_PLANSZY_Y + 7 * kursor.Y + 1 + 2 * kursor.y + 1));
	}

	/* === Sterowanie === */
	/* ================== */
	if (kbhit()){
		klawisz = getch();
		switch (klawisz){
			case UP:	//  strza≥ka w gÛrÍ
				if (kursor.y > 0 || kursor.Y > 0){
					kursor.y--;
					if (kursor.y < 0){
						kursor.y = 2;
						kursor.Y--;
					}
				}
				break;

			case DOWN:	//  strza≥ka w dÛ≥
				if (kursor.y < 2 || kursor.Y < 2){
					kursor.y++;
					if (kursor.y > 2){
						kursor.y = 0;
						kursor.Y++;
					}
				}
				break;

			case LEFT:	//  strza≥ka w lewo
				if (kursor.x > 0 || kursor.X > 0){
					kursor.x--;
					if (kursor.x < 0){
						kursor.x = 2;
						kursor.X--;
					}
				}
				break;

			case RIGHT:	//  strza≥ka w prawo
				if (kursor.x < 2 || kursor.X < 2){
					kursor.x++;
					if (kursor.x > 2){
						kursor.x = 0;
						kursor.X++;
					}
				}
				break;
			
			case DEL: case BACKSPACE:	// delete - czyszczenie komorki
				if (wszystkie.stan_zero->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] == 0)
				{
					if (wszystkie.stan_obecny->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] != 0)
					{
						wypisz_krok(&wszystkie, TRUE);
						dodaj_element(&wszystkie);
						wszystkie.stan_obecny->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] = 0;
						wypisz_ruchy(++ruchy);
					}
				}
				else wypisz_komunikat("Nie mozesz tego usunac!");
				break;

			case 'u': case 'U': case ',':	//  poprzedni krok - PREV (UNDO)
				if (wszystkie.stan_obecny->nr > 0)
				{
					wszystkie.stan_obecny = wszystkie.stan_obecny->prev;
					wypisz_komunikat("Cofnieto");
					wypisz_ruchy(++ruchy);
				}
				break;

			case 'r': case 'R': case '.':	//  nastÍpny krok  - NEXT (REDO)
				if (wszystkie.stan_obecny->nr < wszystkie.stan_koncowy->nr)
				{
					wszystkie.stan_obecny = wszystkie.stan_obecny->next;
					wypisz_komunikat("Przywrocono");
					wypisz_ruchy(++ruchy);
				}
				break;

			case 'l': case 'L':	// lista dostepnych do wpisania cyfr
				if (wszystkie.stan_zero->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] == 0)
				{
					przerysuj_plansze(wszystkie.stan_obecny, wszystkie.stan_zero);
					dostepne_cyfry(wszystkie.stan_obecny, kursor);
				}
				break;

			case 'p':	// podpowiedz jednoznacznie pasujπcej cyfry
				wypisz_ruchy(++ruchy);
				jednoznaczna_podpowiedz(&wszystkie);
				break;

			case '*': case 'j': case 'J':	// tryb podúwietlania cyfr
				if (podswietlenie_cyfry_wlaczone)
				{
					podswietlenie_cyfry_wlaczone = FALSE;
					rysuj_logo(DOMYSLNY_KOLOR_LOGO);
					wypisz_komunikat("Oznaczenie oznaczonej... ");
					wypisz_komunikat(" ...wylacza oznaczenie.");
				}
				else
				{
					podswietlenie_cyfry_wlaczone = TRUE;
					wypisz_komunikat("Jaka oznaczyc cyfre?");
				}
				break;

			case 'k':	//edycja komentarza
				if (podswietlenie_cyfry_wlaczone == FALSE)
				{
					if (edycja_komentarza_wlaczona)
					{
						edycja_komentarza_wlaczona = FALSE;
						rysuj_logo(DOMYSLNY_KOLOR_LOGO);
						wypisz_komunikat("Wpisujesz cyfry");
					}
					else
					{
						edycja_komentarza_wlaczona = TRUE;
						wypisz_komunikat("Edytujesz komentarz");
					}
				}
				break;

			case 's':	//ZAPIS PLIKU XML
				zapis_xml(pisanie_nazwy_xml(SAVE), &wszystkie);
				wypisz_ruchy(ruchy);
				break;

			case 'o': case 'O':	//ODCZYT PLIKU XML
				wskaznik_sprawdzajacy = wszystkie.stan_obecny;

				odczyt_xml(pisanie_nazwy_xml(LOAD), &wszystkie);

				/*jeøeli wskaüniki siÍ rÛøniπ znaczy øe nastπpi≥ odczyt gry, w przeciwnym razie - by≥ on anulowany*/
				if (wskaznik_sprawdzajacy != wszystkie.stan_obecny)
				{
					start_zegara = GetTickCount();
					sekundy = 0;
					ruchy = 0;
				}

				wypisz_ruchy(ruchy);
				break;

			default:	// wpisywanie cyfr / komentarzy / wybÛr podúwietlanej cyfry
			{
				if (klawisz >= '1' && klawisz <= '9' &&
					(wszystkie.stan_zero->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] == 0 || podswietlenie_cyfry_wlaczone))
				{
					klawisz -= '0'; // konwersja z ASCII do int
					if (podswietlenie_cyfry_wlaczone)
					{
						if (wszystkie.podswietlona_cyfra == klawisz)
							wszystkie.podswietlona_cyfra = 0;
						else 
							wszystkie.podswietlona_cyfra = klawisz;
						podswietlenie_cyfry_wlaczone = FALSE; 
						rysuj_logo(DOMYSLNY_KOLOR_LOGO);
						break;
					}
					if (sprawdz(klawisz, wszystkie.stan_obecny, kursor) || edycja_komentarza_wlaczona)
					{
						dodaj_element(&wszystkie);
						wypisz_krok(&wszystkie, TRUE);

						if (edycja_komentarza_wlaczona) //WLACZANIE lub WY£•CZENIE WYBRANEJ CYFRY W KOMENTARZU
						{
							if (wszystkie.stan_obecny->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][klawisz] == TRUE)
								wszystkie.stan_obecny->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][klawisz] = FALSE;
							else
								wszystkie.stan_obecny->komentarz[kursor.X][kursor.Y][kursor.x][kursor.y][klawisz] = TRUE;
							sprawdz_komentarze(wszystkie.stan_obecny, kursor);
						}

						else wszystkie.stan_obecny->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] = klawisz; //WPISANIE CYFRY DO NOWEGO STANU
					}
					else //b≥Ídy wpisywania liczb
					{
						if (wszystkie.stan_obecny->wpisane[kursor.X][kursor.Y][kursor.x][kursor.y] == klawisz)
							wypisz_komunikat("Ta liczba jest juz wpisana");
						else
							wypisz_komunikat("Nieprawidlowa liczba");
					}
					wypisz_ruchy(++ruchy);
				}
			}
		}
		/*...=== koniec sterowania ===*/


		/*Odswiezenie oraz czyszczenie podpowiedzi dostepnych cyfr po wybranych klawiszach*/
		bool czy_odswiezyc = FALSE;
		switch (klawisz){
		case DEL: /*case BACKSPACE:*/		// delete - czyszczenie komorki
		case 'u': case 'U': case ',':	// poprzedni krok - PREV (UNDO)
		case 'r': case 'R': case '.':	// nastÍpny krok  - NEXT (REDO)
		case 'p':						// podpowiedz jednoznacznie pasujπcej cyfry
		case 's':						// ZAPIS PLIKU XML
		case 'o': case 'O':				// ODCZYT PLIKU XML
			czy_odswiezyc = TRUE;	//ustawienie odswiezania
		default:						// wpisywanie cyfr
			if (czy_odswiezyc == TRUE || (/*edycja_komentarza_wlaczona == FALSE &&*/ (klawisz >= 1 && klawisz <= 9))){
				przerysuj_plansze(wszystkie.stan_obecny, wszystkie.stan_zero);
				czysc_dostepne_cyfry();
			}
		}
		/*Wypisanie komentarzy obecnego stanu*/
		wypisz_komentarze(wszystkie.stan_obecny, kursor, edycja_komentarza_wlaczona);
		if (edycja_komentarza_wlaczona) rysuj_logo(LIGHTRED); //kolor logo przy edycji komentarzy

		/*Oznaczanie cyfr kolorem*/
		podswietl_cyfry(wszystkie);
		if (podswietlenie_cyfry_wlaczone) rysuj_logo(LIGHTCYAN); //kolor logo przy oznaczaniu podúwietlenia (wiÍkszy priorytet)

		/*Odúwieøenie liczby pozosta≥ych do uzupelnienia pÛl oraz zrobionych krokÛw*/
		if (wypisz_pozostale(wszystkie.stan_obecny) == 0) wygrana(sekundy, ruchy); // <-- SPRAWDZENIE WYGRANEJ
		wypisz_krok(&wszystkie, FALSE);

		/*Ustawienie kursora we w≥aúciwej pozycji*/
		gotoxy((POZYCJA_PLANSZY_X + 8 * kursor.X + 2 + 2 * kursor.x + 1), (POZYCJA_PLANSZY_Y + 7 * kursor.Y + 1 + 2 * kursor.y + 1));
	}

} while (klawisz != 'q' && klawisz != 'Q');


/*Koniec gry*/
wyjscie_z_gry(&wszystkie);
return 0;
}


