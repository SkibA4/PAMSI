#include <iostream> 
#include <random>
#include <ctime>

using namespace std;

int partition(int* tab, int l, int r) {
	//Określenie pivota w środkowym miejscu tabeli
	int piwo = tab[(l + r)/2];

	while(true) {
		//Pętle ustawiające pozycję elementów tablicy, które będą zamieniane
		while(tab[r] > piwo) 
			r--;
		while(tab[l] < piwo) 
			l++;

		// Przerzucenie mniejszego elemenu na lewo od pivota i większego na prawo
		if(l < r) {
			swap(tab[l], tab[r]);
			l++;
			r--;
		}
		// Jeżeli markery doszły do pivota, zwraca pozycję ostatniego elementu podtablicy dla której będzie rekurencyjnie wywołana funkcja
		else return r;
	}
}

void quicksort(int* tab, int l, int r) {
	int div;
	if(l < r) {
		//Przypisanie do zmiennej pozycji podziału tablicy
		div = partition(tab, l, r);
		//Rekurencyjne wywołanie dla lewej podtabliy
		quicksort(tab, l, div);
		//Rekurencyjne wywołanie dla prawej podtablicy
		quicksort(tab, div + 1, r);
	}
}

/**********FUNKCJA REALIZUJĄCA SPRAWDZENIE POPRAWNOŚCI SORTOWANIA**********/
bool check(int* tab, int size) {
	for(int i = 0; i < size - 2; i++) {
		if(!(tab[i] < tab[i+1] || tab[i] == tab[i+1])){
			return false;
		}
	}
	return true;
}

/**********FUNKCJA REALIZUJĄCA ZAMIANĘ MIEJSC W TABLICY**********/
void reverseTab(int* tab, int size) {
	int tmp;
	for(int i = 0; i <= size / 2; i++) {
		tmp = tab[i];
		tab[i] = tab[size - i];
		tab[size - i] = tmp;
	}

}

/**********FUNKCJE REALIZUJĄCE WYŚWIETLANIE MENU ORAZ INFORMACJI O PROGRAMIE**********/
void menu() {
	cout << endl << "---Quicksort by SkibA (v2.13.7)---" << endl;
	cout << "Podaj opcję: " << endl;
	cout << "1 - Sortowanie rosnąco" << endl;
	cout << "2 - Sortowanie rosnąco liczb posortowanych malejąco" << endl;
	cout << "3 - Sortowanie liczb częściowo posortowanych" << endl;
	cout << "i - informacje o działaniu programu" << endl;
	cout << "k - Koniec działania programu" << endl;
	cout << "Twój wybór: ";
}

void info() 
{
	cout << "Program sortuje 100 tablic składające się z losowych lub częściowo posortowanych elementów wykorzystując sortowanie szybkie." << endl;
	cout << "Liczby losowane są z przedziału od zera do pięciokrotności rozmiaru tablicy podanej przez użytkownika." << endl;
}

/**********FUNKCJA GŁÓWNA**********/
int main() {
	//Rozmiar tablicy i procent posortowanych elementów
	int size = 10000;
	float per = 99.7;

	char type;

	//Zmienne z biblioteki random wykorzystane w generowanu losowych liczb do sortowania
	random_device rand;
	mt19937 gen(rand());
	uniform_int_distribution<> distr(0, size * 5);

	while(type != 'k') {
		menu();
		cin >> type;
		switch(type) {
			case '1':
			{
				float time = 0;
				for(int i = 0; i < 100; i++) {
					//Utworzenie tablicy dynamicznej
					int *tab = new int[size];

					//Wypełnienie tablicy losowymi elementami
					for(int j = 0; j < size; j++) {
						tab[j] = distr(gen);
					}

					//Sortowanie rosnąco i mierzenie czasu sortowania każdej tablicy
					clock_t timeStart = clock();
					quicksort(tab, 0, size - 1);
					clock_t timeEnd = clock();

					//Dodanie czasu sortowania pojedynczej tablicy do ogólnego czasu działania algorytmu
					time += timeEnd - timeStart;

					//Sprawdzanie pomyślności sortowania
					if(!check(tab, size)) {
						cout << "Błąd sortowania" << endl;
					}
					else {
						cout << endl << "Tablica nr " << i + 1 << " posortowana poprawnie!" << endl;
					}

					//Usunięcie dynamicznej tablicy
					delete[] tab;
				}
				cout << "Czas sortowania: " << time/CLOCKS_PER_SEC << "s" << endl;
				break;
			}
			case '2':
			{
				float time = 0;
				for(int i = 0; i < 100; i++) {
					int *tab = new int[size];

					for(int j = 0; j < size; j++) {
						tab[j] = distr(gen);
					}

					//Sortowanie rosnąco
					quicksort(tab, 0, size - 1);

					//Jeżeli wystąpił błąd w sortowaniu rosnąco - przerywa akcję
					if(!check(tab, size)) {
						break;
					}

					//Odwracanie elementów tablicy
					reverseTab(tab, size - 1);

					//Sortowanie rosnąco elementów posortowanych malejąco i mierzenie czasu sortowania każdej tablicy
					clock_t timeStart = clock();
					quicksort(tab, 0, size - 1);
					clock_t timeEnd = clock();
					time += timeEnd - timeStart;

					if(!check(tab, size)) {
						cout << "Błąd sortowania" << endl;
					}
					else {
						cout << endl << "Tablica nr " << i + 1 << " posortowana poprawnie!" << endl;
					}
					delete[] tab;
				}
				cout << "Czas sortowania: " << time/CLOCKS_PER_SEC << "s" << endl;
				break;
			}
			case '3':
			{
				float time = 0;
				for(int i = 0; i < 100; i++) {
					int *tab = new int[size];

					for(int j = 0; j < size; j++) {
						tab[j] = distr(gen);
					}

					//Sortowanie wstępne części tablic
					quicksort(tab, 0, ((size - 1) * per)/100);
					if(!check(tab, ((size - 1) * per)/100)) {
						break;
					}

					//Sortowanie rosnąco i mierzenie czasu sortowania każdej tablicy wstępnie posortowanej
					clock_t timeStart = clock();
					quicksort(tab, 0, size - 1);
					clock_t timeEnd = clock();
					time += timeEnd - timeStart;

					if(!check(tab, size)) {
						cout << "Błąd sortowania" << endl;
					}
					else {
						cout << endl << "Tablica nr " << i + 1 << " posortowana poprawnie!" << endl;
					}
					delete[] tab;
				}
				cout << "Czas sortowania: " << time/CLOCKS_PER_SEC << "s" << endl;
				break;
			}
			case 'i':
			{
				info();
				break;
			}
			case 'k':
			{
				return 0;
			}
			default:
			{
				cout << endl << "Błędny wybór, spróbuj jeszcze raz..." << endl;
				break;
			}
		}
	}
}