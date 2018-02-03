#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <gmp.h>
using namespace std;
using namespace std::chrono;


// Ten algorytm jest częścią Miller-Rabin test
long long int power_modulo_fast(long long int a, long long int b, long long int m)
{
	 int i;
	 long long int result = 1;
	 long long int x = a%m;

	for (i=1; i<=b; i<<=1)
	{
		x %= m;
		if ((b&i) != 0)
		{
			result *= x;
			result %= m;
		}
		x *= x;
	}

	return result;
}


bool Miller_Rabin(long long int n)/*Miller-Rabin test - strona wwww. 
Test dla k powtorzen przerobilem dla uzyskania wiarygodnych czasow 
tylko na jednokrotne wykonanie, co zmniejsza jego skutecznosc */
{

	int s = 0;
	int s2 = 1;
	int a, d, i, r, prime;
	srand(time(NULL));

	if (n<4)
	{
		return 1;
	}
	if (n%2 == 0)
	{
		return 0;
	}
	while ((s2 & (n-1)) == 0)
	{
		s  += 1;
		s2 <<= 1;
	}
	d = n/s2;

    int k=1;
	for (i=0; i<k; i++)
	{
		a = 1+(int) ((n-1)*rand()/(RAND_MAX+1.0));
		if (power_modulo_fast(a, d, n) != 1)
		{
			prime = 0;
			for (r=0; r<=s-1; r++)
			{
				if (power_modulo_fast(a, n*d, n) == n - 1)
				{
					prime = 1;
					break;
				}
			}
			if (prime == 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool primare_test (long long int number)//strona www
{
    if(number<2)
        return false;
    if(number==2)
        return true;
    if(number%2==0)
        return false;
    for(int i=3;i<=sqrt(number);i += 2)
    {
        if(number%i==0)
            return false;
    }
    return true;
}

size_t nr_testu=0;// przelacznik testu 0-primare_test,1-GMP,2-Mi-Ra

auto print_and_test=[](long long int n) -> void //Zapis do zmiennej adresu funkcji lambda, ktora uruchamia poszczegolne testy
{

	long long int max=9223372036854775807;//
	if (nr_testu==0)
	{
		if ((n<max)&&(primare_test(n)))
		{
			cout<<n<<" || ";
		}
	}
	if(nr_testu==1)
	{
   
    	MP_INT mpzNumber;
    	mpz_init(&mpzNumber);
    	mpz_set_si(&mpzNumber, static_cast<long long int>(n));
    	if((n<max)&&(mpz_probab_prime_p (&mpzNumber,50)))
    	{
			cout<<n<<" || ";
		}
	}
	if(nr_testu==2)
	{
		if ((n<max)&&(Miller_Rabin(n)))
		 {
		 	cout<<n<<" || ";
		 }
	};
};



int main()
{
size_t w;
string plik,tekst;
cout<<"Wybierz z klawiatury numer pliku z zestawem liczb\n1:Małe liczby-liczby_small.txt\n2:Średnie liczby-liczby_medium.txt\n3:Duże liczby-liczby_big.txt\n";
cout<<"Twoj wybór :";
cin>>w;
switch(w)
{
	case 1: plik="liczby_small.txt";
		break;
	case 2: plik="liczby_medium.txt";
		break;
	case 3: plik="liczby_big.txt";
		break;
	default: 
		break;
}


	ifstream data(plik);
	if (data.good())
	{
		cout<<"ZNALEZIONO PLIK "<<plik<<" Z DANYMI"<<endl;
		istream_iterator<long long int> start(data), end;
		vector<long long int> allnum(start, end);//wczytanie danych z pliku do vektora
		cout << "Odczytalem z pliku wszystkich liczb: " << allnum.size()<<endl;

		 
		cout<<endl;
		cout<<"----------------------------------------------------"<<endl;
		cout<<"ROZPOCZYNAM TEST PRIMARE"<<endl;
		cout<<endl;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for_each(allnum.begin(), allnum.end(), print_and_test);//petla for_ech na wektorze i wywołanie funkcji lambda dla każdego elementu wektora
		cout<<endl;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto czas = duration_cast<microseconds>( t2 - t1 ).count();
        cout << "CZAS WYKONANIA ALGORYTMU PRIMARE: "<< czas <<" <microseconds>"<<endl;
		cout<<endl;
		cout<<"----------------------------------------------------"<<endl;
		cout<<"ROZPOCZYNAM TEST PROPABILISTYCZNY Z BIBLIOTEKI GMP"<<endl;
		cout<<endl;
		nr_testu=1;
		t1 = high_resolution_clock::now();
		for_each(allnum.begin(), allnum.end(), print_and_test);
        t2 = high_resolution_clock::now();
        czas = duration_cast<microseconds>( t2 - t1 ).count();
        cout<<endl;
        cout << "CZAS WYKONANIA ALGORYTMU mpz_probab_prime_p: " << czas <<" <microseconds>"<<endl;
        cout<<endl;
        cout<<"----------------------------------------------------"<<endl;
        cout<<"ROZPOCZYNAM TEST MILLERA RABINA"<<endl;
        nr_testu=2;
        cout<<endl;
		t1 = high_resolution_clock::now();
		for_each(allnum.begin(), allnum.end(), print_and_test);
		t2 = high_resolution_clock::now();
        czas = duration_cast<microseconds>( t2 - t1 ).count();
        cout<<endl;
        cout << "CZAS WYKONANIA ALGORYTMY MILLERA-RABINA: "<< czas <<" <microseconds>"<<endl;

	}
	else
	{
		 cout<<"PROBLEM Z ODCZYTEM PLIKU "<<plik;
	}

    return 0;

}

