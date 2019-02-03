// akwb4.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <algorithm> 
#include <vector>
#include <time.h>
#include <ctime>

using namespace std;

bool*alreadyInSolution;
int theBiggestElement = 0;
int amountOfElements = 0;
int amountElementsInAMap = 0;
int solutionSum = 0;
double howLongDidItTake = 0;
vector <int> solution;
vector <int> sortedInput;
clock_t start, stop;

void read(){
	ifstream test;
	string seq, halp;
	int pos = 0;
	//string helpLine;
	test.open("plik5.txt", ios::in);
	if (test.good() == false)
		cout << "Blad otwierania pliku" << endl;
	else {
		getline(test, seq);
		while (seq.find(' ', pos) != string::npos) {
			int found = seq.find(' ', pos);//błąd może być z '/"
			halp.insert(0, seq, pos, found - pos);//(nie wiem, skąd, pozycja startu, ile skopiować) 
			pos = found + 1;
			sortedInput.push_back(atoi(halp.c_str()));//kolejne wartośći wiarygodności w wektorze
			amountOfElements++;
			halp.clear();
		}
		int found = seq.find(' ', pos);//błąd może być z '/"
		halp.insert(0, seq, pos, found - pos);//(nie wiem, skąd, pozycja startu, ile skopiować) 
		sortedInput.push_back(atoi(halp.c_str()));//kolejne wartośći wiarygodności w wektorze
		amountOfElements++;
		halp.clear();
		sort(sortedInput.begin(), sortedInput.end());	//now really sorted
		/*for (int i = 0; i < amountOfElements; i++) {
			cout << sortedInput[i] << " ";
		}
		cout << endl;*/
	}
	/////////szybka inicjacja
	theBiggestElement = sortedInput[sortedInput.size() - 1];
	alreadyInSolution = new bool[amountOfElements];

	for (int i = 0; i < amountOfElements; i++) {
		alreadyInSolution[i] = false;
	}
}

bool isThereAnySense() {
	int i = 1;
	while (amountOfElements * 2 != i * (i + 1)) {
		if (amountOfElements * 2 < i*(i + 1))return false;
		i++;
	}
	amountElementsInAMap = i;
	//cout << i << endl;
	return true;		//ilość elementów w mapie
}

bool isSumOK(vector<int>&halp) {	//działanie na oryginalnym, żeby pamięć
	int sum = 0;
	for (int i = 0; i < halp.size(); i++) {
		sum += halp[i];
	}
	if (sum == theBiggestElement)return true;
	else return false;
}

bool isItPromisingSolution(vector<int>&halp) {
	bool*hasItBeenAlready = new bool [amountOfElements];	//już ta liczba była użyta do sprawdzenia
	bool reallyPromising = false;

	for (int i = 0; i < amountOfElements; i++) {
		hasItBeenAlready[i] = false;
	}

	for (int i = 0; i < halp.size(); i++) {		//sprawdzanie poszczególnych sum długości sąsiadów
		bool promising = false;
		int lilHalp, sum = 0;
		//hasItBeenAlready[halp[i]] = true;	//element sam w sobie i tak jest sprawdzany w pierwszej iteracji
		for (int j = i; j < halp.size(); j++) {		//dla każdej nowo obliczonej sumy
			//lilHalp = i;
			sum += halp[j];
			for (int k = 0; k < sortedInput.size(); k++) {
				if (sortedInput[k] == sum && hasItBeenAlready[k] == false) {	//sprawdź czy jest w multiziorze z inputu
					hasItBeenAlready[k] = true;
					promising = true;
					break;
				}
				else if (sortedInput[k] > sum) {	//jeśli nie to rozwiązanie błędne
					promising = false;
					break;
				}
			}

		}
		if (promising == true)reallyPromising = true;	//narazie jest OK, zobaczymy czy tak się utzryma
		else {
			reallyPromising = false;	//źle, abort
			break;
		}
	}
	return reallyPromising;		//like a pro
}

void searchingForSolution(int toAdd, int level, bool*done) {
	//if (level == 2)cout << toAdd << endl;
	vector<int>alreadyChecked;
	solution.push_back(toAdd);
	solutionSum += toAdd;
	for (int i = 0; i < amountOfElements; i++) {	//dodaliśmy do rozwiązania więc trzeba to odnotować
		if (toAdd == sortedInput[i] && alreadyInSolution[i] == false) {
			alreadyInSolution[i] = true;	//Checked jest aktualizowana PRZED dodaniem do rozwiązania
			break;
		}
	}
	if (isItPromisingSolution(solution)) {	//jeśli narazie się wszystko zgadza
		if (level < amountElementsInAMap - 1) {	//i można dodać kolejny element do rozwiązania
			for (int i = 0; i < amountOfElements; i++) {
				if (alreadyInSolution[i] == false) {
					alreadyChecked.push_back(sortedInput[i]);	//inSolution jest aktualizowana PO dodaniu do rozwiązania
					////////////////W GŁĄB/////////////////
					if (solutionSum + sortedInput[i] > sortedInput[sortedInput.size() - 1])	//żeby nei wchodzić w pętle które widać gołym oke=iem że nei mają sensu
						continue;
					searchingForSolution(sortedInput[i], (level + 1), done);
					break;
				}
			}
		}
		else if(isSumOK(solution)){
			*done = true;
		}
		else {
			int c = solution[solution.size() - 1];
			solution.pop_back();
			solutionSum -= c;
			for (int i = 0; i < amountOfElements; i++) {	//skoro wyrzucon coś z rozwiązania to oznacz w tabeli
				if (sortedInput[i] == c && alreadyInSolution[i] == true) {
					alreadyInSolution[i] = false;
					break;
				}
			}
			return;
		}
	}
	else {
		int c = solution[solution.size() - 1];
		solution.pop_back();
		solutionSum -= c;
		for (int i = 0; i < amountOfElements; i++) {	//skoro wyrzucon coś z rozwiązania to oznacz w tabeli
			if (sortedInput[i] == c && alreadyInSolution[i] == true) {
				alreadyInSolution[i] = false;
				break;
			}
		}
		return;
	}
	if (*done == true)
		return;
	else {///////////WSZERZ/////////////
		for (int i = 0; i < amountOfElements; i++) {
			vector<int>::iterator it;
			//it = find(solution.begin(), solution.end(), sortedInput[i]);
			//if (it == solution.end()) {	//jeśli nie ma jej już w rozwiązniu
				it = find(alreadyChecked.begin(), alreadyChecked.end(), sortedInput[i]);
				if (it == alreadyChecked.end() && alreadyInSolution[i] == false) {	//i nie była jeszcze sprawdzana
					alreadyChecked.push_back(sortedInput[i]);
					if (solutionSum + sortedInput[i] > sortedInput[sortedInput.size() - 1])	//żeby nei wchodzić w pętle które widać gołym oke=iem że nei mają sensu
						continue;
					searchingForSolution(sortedInput[i], level + 1, done);
					if (*done == true)return;
					//break;
				}
			//}
		}//jak sprawdzi wszystkie wszerz i żadna sie nie nadaje to trzeba sie cofnąć o jeden level
		int c = solution[solution.size() - 1];
		solution.pop_back();
		solutionSum -= c;
		for (int i = 0; i < amountOfElements; i++) {	//skoro wyrzucon coś z rozwiązania to oznacz w tabeli
			if (sortedInput[i] == c && alreadyInSolution[i] == true) {
				alreadyInSolution[i] = false;
				break;
			}
		}
	}
}



int main()
{
	vector<int>didWeTry;
	bool done = false;
	int i = 0;
	read();
	/////////////czas start
	start = clock();

	if (isThereAnySense()) {
		while (1) {
			if (i == amountOfElements - 1)break;
			cout << "petla " << i << endl;
			vector<int>::iterator it;
			it = find(didWeTry.begin(), didWeTry.end(), sortedInput[i]);
			if (it == didWeTry.end()) {	//jeśli nie ma jej już w rozwiązniu
				solution.clear();
				searchingForSolution(sortedInput[i], 0, &done);
			}
			if (done) {
				stop = clock();
				break;
			}
			i++;
		}
		if (!done) cout << "BRAK ROZWIAZANIA DLA WPROWADZONYCH LICZB" << endl;
	}
	else cout << "NIEPRAWIDLOWY ZBIOR WEJSCIOWY" << endl;//rozróżniaj czy liczba cięć zła czy elementy do siebie nie pasują
	/////////////czas stop
	if (done) {	//wypisanie rozwiązania
		cout << "CZAS SZUKANIA: " << ((double)(stop - start) / CLOCKS_PER_SEC) << "s" << endl;
		cout << "ROZWIAZANIE:\n";
		for (int i = 0; i < solution.size(); i++) {
			cout << solution[i] << " ";
		}
		cout << endl;
	}

	system("Pause");
	return 0;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//	 5. Lubie placki.
//   6. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   7. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln