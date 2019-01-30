// Zabezpieczenia i ich dobór.cpp : Definicja bibliotek wykorzystanych w programie
//
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// ----------------------------- Definicja zmiennych -----------------------------

// Zmienne dla pliku - wezly.txt

int wezel_wej[50], wezel_wyj[50], nr_kabla[50];
float dlugosc[50], moc_p[50], moc_q[50], napiecie[50];

//	Przechowywanie parametrów przewodów
string typ_k[50];
int nr_k[50], un_k[50], przekroj_k[50];
float r_k[50], x_k[50], c_k[50], obc_prad_k[50];

// ----------------------------- Klasy i Funkcje -----------------------------

class cPliki
{
private:
	string linia;

public:

	int ilosc_linii;
	int nr_linii, nr_elementu;

// ----------------------------- Metody -----------------------------

	void pobierz_plik_wezly(string nazwa_pliku)
	{
		ilosc_linii = 1;

		fstream wezly;
		wezly.open(nazwa_pliku, ios::in);

		if(wezly.good() == false)
		{
			cout << "Plik " << nazwa_pliku << " nie istnieje! Niepoprawny odczyt!" << endl;
		}

// Zmienne definiowane w celu zapisu tekstu z wczytanego pliku

		nr_linii = 1;
		nr_elementu = 1;

// Zapis danych z pliku do tablic odpowiadaj¹cyh poszczególnym parametrom

		while (getline(wezly,linia))
		{
			switch (nr_linii)
			{
				case 1: wezel_wej[nr_elementu] = atoi(linia.c_str());											break;
				case 2: wezel_wyj[nr_elementu] = atoi(linia.c_str());											break;
				case 3: nr_kabla[nr_elementu] = atoi(linia.c_str());											break;
				case 4: dlugosc[nr_elementu] = atof(linia.c_str());												break;
				case 5: moc_p[nr_elementu] = atof(linia.c_str());												break;
				case 6: moc_q[nr_elementu] = atof(linia.c_str());												break;
				case 7: napiecie[nr_elementu] = atof(linia.c_str());					ilosc_linii++;			break;
			}

			if (nr_linii==7) {nr_linii=0; nr_elementu++;}
			nr_linii++;
		}
		ilosc_linii = ilosc_linii - 1;
		wezly.close();
	}

	void pobierz_plik_kable(string nazwa_pliku)
	{
		ilosc_linii = 1;

		fstream kable;
		kable.open(nazwa_pliku, ios::in);

		if(kable.good() == false)
		{
			cout << "Plik " << nazwa_pliku << " nie istnieje! Niepoprawny odczyt!" << endl;
		}

// Zmienne definiowane w celu zapisu tekstu z wczytanego pliku

		int nr_linii = 1;
		int nr_elementu = 1;

// Zapis danych z pliku do tablic odpowiadaj¹cyh poszczególnym parametrom

		while (getline(kable,linia))
		{
			switch (nr_linii)
			{
				case 1: nr_k[nr_elementu] = atoi(linia.c_str());												break;
				case 2: typ_k[nr_elementu] = linia;																break;
				case 3: un_k[nr_elementu] = atof(linia.c_str());												break;
				case 4: przekroj_k[nr_elementu] = atoi(linia.c_str());											break;
				case 5: r_k[nr_elementu] = atof(linia.c_str());													break;
				case 6: x_k[nr_elementu] = atof(linia.c_str());													break;
				case 7: c_k[nr_elementu] = atof(linia.c_str());													break;
				case 8: obc_prad_k[nr_elementu] = atof(linia.c_str());					ilosc_linii++;			break;
			}

			if (nr_linii==8) {nr_linii=0; nr_elementu++;}
			nr_linii++;
		}
		ilosc_linii = ilosc_linii - 1;
		kable.close();
	}
};

class cObliczenia
{
private:
	
	float moc_s, prad_lini, straty_p;
	int j;
	float rezys, reak, poj;
	int y;

public:

	void moce(int ilosc_wezlow)
	{
		//cout << "Ilosc linii wezly: " << ilosc_wezlow << endl;

		for (int i = 1; i <= ilosc_wezlow; i++)
		{
			y = nr_kabla[i];
			
			if (napiecie[i] != un_k[y])
			{
				do
				{
					y++;
				} while (napiecie[i] != un_k[y]);

				cout << "Zmiana kabla ze wzgledu na napiecie linii!!!" << endl << "Zwiekszrenie napiecia z: " << un_k[nr_kabla[i]] << "V, do " << un_k[y] << "V" << endl;
			}

			else
			{
				cout << "Brak koniecznosci doboru ze wzgledu na napiecie." << endl;
			}
			
			rezys = r_k[y] * dlugosc[i];
			reak = x_k[y] * dlugosc[i];
			poj = c_k[y] * dlugosc[i];

			moc_s = (sqrt((moc_p[i]*moc_p[i])+(moc_q[i]*moc_q[i])))*1000;
			prad_lini = moc_s/(sqrt(3)*napiecie[i]);

			cout << "Napiecie linii = " << napiecie[i] << endl;
			cout << "Moc S = "<< moc_s << endl;
			cout << "Prad linii = " << prad_lini << endl;

			if (prad_lini > obc_prad_k[y])
			{
				j = y;
				do
				{
					j ++;
				} while (prad_lini > obc_prad_k[j]);

				cout << "!!!---> Wykryto zagrozenie - przeciazenie. <---!!!" << endl << "Zalecam zmiane przeowdu z " << typ_k[nr_kabla[i]] << " " << przekroj_k[nr_kabla[i]] << " na pzewod " << typ_k[j] << " " << przekroj_k[j] << endl;
			}

			else
			{
				cout << "Dobrany przewod dla wezlow: wejsciowego:" << wezel_wej[i] << endl <<"oraz wyjsciowego:" << wezel_wyj[i] << " spelnia warunki przeciazeniowe." << endl;
			}

			straty_p = (prad_lini*prad_lini)*rezys;
			cout << "Straty mocy P = " << straty_p << endl << endl;

		}
	}
};

void wyswietl(int linia)
{

	cout << endl << "Wezly oraz wykorzystane przewody i ich dlugosci w sieci: " << endl << endl;

	for (int i = 1; i <= linia; i++)
	{
		cout << "Nr.: "<< i << endl;
		cout << "Numer wezla wejsciowego: " << wezel_wej[i] << ", Numer wezla wyjsciowego: " << wezel_wyj[i] << endl;
		cout << "Przewod: " << typ_k[nr_kabla[i]] << " " << przekroj_k[nr_kabla[i]] << endl;
		cout << "Dlugosc lini: " << dlugosc[i] << "km" << endl << endl;
	}

}

// ----------------------------- Funkcja main -----------------------------

int main()
{
// Wczytanie pliku wezly.txt

	cPliki wezly_i_kable;
	wezly_i_kable.pobierz_plik_wezly("wezly.txt");

	int ilosc_lini_wezly = wezly_i_kable.ilosc_linii;				

// Wczytanie pliku kable.txt

	wezly_i_kable.pobierz_plik_kable("kable.txt");

// Wyœwietlanie danych o wezlach i przewodach

	cObliczenia rozplyw;
	rozplyw.moce(ilosc_lini_wezly);

	getchar ();
	return 0;
}