#include <iostream> 
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

/**********FUNKCJA ORGANIZUJĄCA ELEMENTY W KOPCU**********/
void heapify(int* tab, int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
  
    if (left < size && tab[left] > tab[largest])
      largest = left;
  
    if (right < size && tab[right] > tab[largest])
      largest = right;
  
    //Kontynuacja jeżeli wierzchołek kopca nie jest największy
    if (largest != i) {
      swap(tab[i], tab[largest]);
      heapify(tab, size, largest);
    }
}


/**********SORTOWANIE PRZEZ KOPCOWANIE**********/
void heapSort(int* tab, int size) {
	//Budowanie kopca
	for (int i = size / 2 - 1; i >= 0; i--)
      heapify(tab, size, i);
	//Wyciąganie pojedynczo elementów z kopca
	for (int i = size - 1; i >= 0; i--) {
      swap(tab[0], tab[i]);        
      heapify(tab, i, 0);
    }
}

/**********SORTOWANIE PRZEZ WSTAWIANIE**********/
void insertionSort(int* tab, int size) {
	int i, j;
	int temp;

	for(i = 1; i < size; ++i) {
		//Przypisanie elementu do zmiennej tymczasowej
		temp = tab[i];
		//Przeszukanie tablicy w celu znalezienia miejsca do wstawienia elementu z zmiennej tymczasowej
		for (j = i; j > 0 && temp < tab[j-1]; --j) {
			tab[j] = tab[j-1];
		}
		//Wstawienie wartości zmiennej tymczasowej w odpowiednie miejsce
		tab[j] = temp;
	}
}

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

void introSort(int* tab, int size, int M) {
	int div;

	// Jeżeli głębokość rekurencji jest mniejsza lub równa 0, następuje wywołanie sortowania przez kopcowanie
	if (M <= 0) {
		heapSort(tab, size);
		return;
	}
	//Jeżeli głębokość rekurencji jest dozwolona, algorytm działa jak quicksort
	div = partition(tab, 0, size);
	//Jeżeli prawa i lewa będą miały po 9 elementów, quicksort zakończy pracę, a dla tych małych tablic będzie wywołanie sortowanie przez wstawianie
	if (div > 9)
		introSort(tab, div, M - 1);
	if (size - 1 - div > 9)
		introSort(tab + div + 1, size - 1 - div, M - 1);
}

void introspectiveSort(int* tab, int size) {
	//Wywołanie sortowania introspektywnego
	introSort(tab, size, (int)floor(2 * log(size) / M_LN2));
	//Dokończenie sortowania małych tablic poprzez wstawianie
	insertionSort(tab, size);
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
	cout << endl << "---Introsort by SkibA (v2.13.7)---" << endl;
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
	cout << "Program sortuje 100 tablic składające się z losowych lub częściowo posortowanych elementów wykorzystując sortowanie introspektywne." << endl;
	cout << "Liczby losowane są z przedziału od zera do pięciokrotności rozmiaru tablicy podanej przez użytkownika." << endl;
}

/**********FUNKCJA GŁÓWNA**********/
int main() {
	//Rozmiar tablicy i procent posortowanych elementów
	int size = 10000;
	float per = 50;

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
                    introspectiveSort(tab, size - 1);
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
					introspectiveSort(tab, size - 1);

					//Jeżeli wystąpił błąd w sortowaniu rosnąco - przerywa akcję
					if(!check(tab, size)) {
						break;
					}

					//Odwracanie elementów tablicy
					reverseTab(tab, size - 1);

					//Sortowanie rosnąco elementów posortowanych malejąco i mierzenie czasu sortowania każdej tablicy
					clock_t timeStart = clock();
					introspectiveSort(tab, size - 1);
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
                    introspectiveSort(tab, ((size - 1) * per)/100);
					//introSort(tab, ((size) * per)/100);
					if(!check(tab, ((size - 1) * per)/100)) {
						break;
					}

					//Sortowanie rosnąco i mierzenie czasu sortowania każdej tablicy wstępnie posortowanej
					clock_t timeStart = clock();
					introspectiveSort(tab, size - 1);
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