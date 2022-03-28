#include <iostream> 
#include <random>
#include <ctime>

using namespace std;

/**********FUNKCJA REALIZUJĄCA SCALANIE PODTABLIC**********/ 
void merge(int* tab, int l, int m, int r)
{
	//Utworzenie dynamicznej tablicy tymczasowej
	int *tempTab = new int[r + 1];
	int left = l, middle = m + 1;

	//Wypełnienie tablicy pomocnicej
	for(int i = l; i <= r; i++){
		tempTab[i] = tab[i];
	}
	
	//Scalanie tablic pomicniczych i zapisywanie ich elementów w głównej tablicy
	for(int j = l; j <= r; j++) {
		if(left <= m)
			if(middle <= r)
				if(tempTab[middle] < tempTab[left])
					tab[j] = tempTab[middle++];
				else
					tab[j] = tempTab[left++];
			else
				tab[j] = tempTab[left++];
		else
			tab[j] = tempTab[middle++];
	} 
	delete[] tempTab;
}
 
/**********FUNKCJA REALIZUJĄCA SORTOWANIE PRZEZ SCALANIE**********/ 
void mergeSort(int* tab, int l, int r) {
	if (r > l) {
		//Dzielenie tablicy na dwie części	
		int m = (l + r) / 2;
		//Rekurencyjne wywołanie funkcji dla lewej części tablicy
		mergeSort(tab, l, m);
		//Rekurencyjne wywołanie funkcji dla prawej części tablicy
		mergeSort(tab, m + 1, r);
		//Scalanie posortowanych tablic
		merge(tab, l, m, r);
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
	cout << endl << "---Merge sort by SkibA (v2.13.7)---" << endl;
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
	cout << "Program sortuje 100 tablic składające się z losowych lub częściowo posortowanych elementów wykorzystując sortowanie przez scalanie." << endl;
	cout << "Liczby losowane są z przedziału od zera do pięciokrotności rozmiaru tablicy podanej przez użytkownika." << endl;
}

/**********FUNKCJA GŁÓWNA**********/
int main() {
	//Rozmiar tablicy i procent posortowanych elementów
	int size = 50000;
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
					mergeSort(tab, 0, size - 1);
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
					mergeSort(tab, 0, size - 1);

					//Jeżeli wystąpił błąd w sortowaniu rosnąco - przerywa akcję
					if(!check(tab, size)) {
						break;
					}

					//Odwracanie elementów tablicy
					reverseTab(tab, size - 1);

					//Sortowanie rosnąco elementów posortowanych malejąco i mierzenie czasu sortowania każdej tablicy
					clock_t timeStart = clock();
					mergeSort(tab, 0, size - 1);
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
					mergeSort(tab, 0, ((size - 1) * per)/100);
					if(!check(tab, ((size - 1) * per)/100)) {
						break;
					}

					//Sortowanie rosnąco i mierzenie czasu sortowania każdej tablicy wstępnie posortowanej
					clock_t timeStart = clock();
					mergeSort(tab, 0, size - 1);
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