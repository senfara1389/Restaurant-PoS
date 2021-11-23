//Luka Paulic RT-6/19
//Projekat - Softver za asistenciju u rukovodjenju restoranom (kontrola i azuriranje skladista, jelovnika, rezervacija)

using namespace std;
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<cstdlib>
#include<cstdio>
#include<sstream>
#include<ctype.h>

struct sastojak{
	string naziv;
	int kolicina;
};

//Struktura koja definise sirove namirnice koje su potrebne za jela
struct namirnica:sastojak{
	int cena;
};

//Struktura koja definise jelo u jelovniku koje se sastoji od unetih namirnica
struct jelo:namirnica{
	string vrsta;
	sastojak *sas;
};

//Struktura koja definise rezervaciju stola sa svojim vremenom, brojem stola i nizom sturktura jela koja su porucena
struct rezervacija{
	string datum;
	int vreme;
	int sto;
	jelo *jel;
};

void glavniMeni();
void skladiste();
void jelovnik();
void rezervacije();
void sortiranjeSkladista(namirnica x[], int n);
void sortiranjeJelovnika(jelo jel[], int n);
void upisSkladiste(namirnica x[],int n);
void upisJelovnik(jelo jel[],int n,int brojSastojaka[]);
void upisRezervacije(rezervacija *rez, int n, int brojJela[]);
void azuriranjeSkladista(namirnica x[], int n);
void azuriranjeJela(jelo jel[], int n, int brojSastojaka[], string nazivJel);
void ispisSkladista(namirnica nam[],int n);
void ispisJelovnika(jelo *jel, int n, int brojSastojaka[]);
void azuriranjeJela(jelo *jel, int n, int brojSastojaka[]);
void azuriranjeRezervacije(rezervacija *rez, int n, int brojJela[]);
void azuriranjeRezervacije(rezervacija *rez, int n, int brojSastojaka[], string datum, int bVrem, int bSto);
void citanjeBrojaSastojaka(int brojSastojaka[]);
void citanjeBrojaJela(int brojJela[]);
void azuriranjeSastojka(jelo jel, int n, int brojSastojaka);
void azuriranjeJela(rezervacija rez, int n, int brojJela);
void ispisRezervacija(rezervacija *rez, int n, int brojJela[]);
int brisanjeSkladiste(namirnica x[], int n);
int brisanjeJela(jelo jel[], int n, int brojSastojaka[]);
int brisanjeRezervacije(rezervacija *rez, int n, int *brojJela);
int * citanjeJelovnika();
int * citanjeRezervacija();
int unosSkladiste(namirnica x[], int n);
int unosJela(jelo jel[], int n, int brojSastojaka[]);
int unosJela(jelo jel[], int n, int brojSastojaka[], string nazivJela);
int unosRezervacije(rezervacija *rez, int n, int brojJela[]);
namirnica * citanjeSkladista(int *n);
jelo * citanjeJelovnika(int n[]);
jelo * citanjeJelovnika(jelo jel[]);
rezervacija * citanjeRezervacija(int *n);
rezervacija * citanjeRezervacija(rezervacija rez[]);

int main()
{
	glavniMeni();
}

//Funkcija za "Glavni meni" programa, u kome se vrsi navigacija ka svim delovima programa
void glavniMeni(){
	system("cls");
	string x;
	int opcija;
	cout<<"Dobrodosli u program za asistenciju rada restorana!\nUkucajte jedan od rednih brojeva i pritisnite enter kako bi ste pristupili tom delu programa.\n\n"<<endl;
	cout<<"1. Skladiste"<<endl;
	cout<<"2. Jelovnik"<<endl;
	cout<<"3. Rezervacije"<<endl;
	cout<<"4. Izlaz\n"<<endl;
	cout<<"Vas unos: "<<endl;
	getline(cin,x);
	opcija=atoi(x.c_str());
	switch(opcija)
	{
		case 1: skladiste();break;
		case 2: jelovnik();break;
		case 3: rezervacije();break;
		case 4: exit(1);break;
		default: 
		{
			cout<<"Nije unet odgovarajuc broj"<<endl;
			system("pause");
			system("cls");
			glavniMeni();
		}
	}
}


//Funkcija koja otvara meni za upravljanje skladistem
void skladiste()
{	
	namirnica *nam;
	string x;
	int opcija;
	int brojNamirnice=0;
	
    //Ispis interfejsa
	system("cls");
	cout<<"_______________________________________S K L A D I S T E_______________________________________\n"<<endl;
	cout<<"Trenutno stanje u skladistu:"<<endl;
	
	nam=citanjeSkladista(&brojNamirnice);
	ispisSkladista(nam,brojNamirnice);
	
	//Ispis daljih opcija u ovom meniju
	cout<<"________________________________________\n"<<endl;
	cout<<"\n Izaberite sta zelite da radite:\n"<<endl;
	cout<<"1. Azuriraj postojecu namirnicu\n"<<endl;
	cout<<"2. Obrisi postojecu namirnicu\n"<<endl;
	cout<<"3. Dodaj novu namirnicu\n"<<endl;
	cout<<"4. Nazad u glavni meni\n"<<endl;
	
	cout<<"Vas unos: "<<endl;
	getline(cin,x);
	opcija=atoi(x.c_str());

	switch(opcija)
	{
		case 1: azuriranjeSkladista(nam, brojNamirnice);break;
		case 2: brojNamirnice=brisanjeSkladiste(nam, brojNamirnice);
				upisSkladiste(nam, brojNamirnice);											//Upis skladista sa obrisanom namirnicom u datoteku
				delete[] nam;																//Nakon cega se brise i ponovo alocira memorija za novu duzinu niza struktura
				nam = new namirnica[brojNamirnice]; 	
				if(nam==NULL)
				{
					cerr<<"Nije uspela dinamicka dodela memorije za namirnice\n"<<endl;
				}
				nam=citanjeSkladista(&brojNamirnice);
				skladiste();
				break;
		case 3: brojNamirnice=unosSkladiste(nam, brojNamirnice);
				skladiste();
				break;
		case 4: delete[] nam; glavniMeni();break;
		default: 
		{
			cout<<"Nije unet odgovarajuc broj"<<endl;
			system("pause");
			skladiste();
		}
	}
}

//Funkcija koja cita namirnice iz datoteke skladiste.txt i smesta ih u niz sturktura, a pokazivacem vraca duzinu niza
namirnica * citanjeSkladista(int *n)
{
	namirnica *nam;
	string red;
	int brojNamirnice=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("skladiste.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{
		brojNamirnice++;
	}
	
	//Dinamicka dodela memorije za namirnice i njena provera
    nam = new namirnica[brojNamirnice]; 
	if(nam==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za namirnice\n"<<endl;
	}
	
	//Priprema za ucitavanje txt datoteke u strukture
	int pos=0;
	int flag=0;
	brojNamirnice=0;
	myfile.clear();
	myfile.seekg(0);
	
	
	//Petlja cita datoteku red po red, izvlaci podatke iz nje i smesta u strukturu
	while(getline(myfile, red)) 
	{
		while ((pos = red.find(delimiter)) != std::string::npos) 	//U datoteci su podaci formatirani na sledeci nacin: namirnica | kolicina | cena | 
		{															//Ovaj deo programa cita red do prvog karaktera | koji je imenovan kao delimiter
    		string token = red.substr(0, pos-1);					//Od pocetka reda do prvog delimitera odvaja string i upisuje ga kao naziv namirnice unutar strukture
    		if(flag==0)												//Posto program broji poziciju svakog delimitera, nakon uspesnog upisivanja u strukturu, taj deo stringa se brise 
			{														//Kako bi mogao da se nadje novi delimiter(koji nakon brisanja postaje prvi u redu kako bi mogla da ga nadje funkcija find())
				nam[brojNamirnice].naziv=token;						//u redu koji ce predstavljati kolicinu, odnosno cenu namirnice
				flag=1;												//Ovo se obavlja dok funkcija find() ne vrati vrednost npos, sto znaci da se doslo do kraja reda
			}
			else if(flag==1)
			{
				stringstream konvertor(token);
				konvertor>>nam[brojNamirnice].kolicina;
				flag=2;
			}
			else if(flag==2)
			{
				stringstream konvertor(token);
				konvertor>>nam[brojNamirnice].cena;
				flag=0;
				brojNamirnice++;
			}		
    		red.erase(0, pos + delimiter.length());															
		}													
    }
   
  	//Funkcija koja sortira sve namirnice po abecednom poretku
    sortiranjeSkladista(nam, brojNamirnice);
	 	
	 //Zatvaranje fajla za citanje
  	myfile.close();
  	
  	*n=brojNamirnice;
  	return nam;
}

void ispisSkladista(namirnica nam[],int n)
{
	//Ispis trenutnog stanja skladista
    	for(int i=0;i<n;i++)
	{
		cout<<"________________________________________\n"<<endl;	//Posto su u prethodnoj funkciji sve namirnice ucitane iz datoteke u strukture
		cout<<"Naziv namirnice: ";									//U ovoj funkciji se samo cita niz struktura svih namirnica i izlistava
		cout<<nam[i].naziv<<endl;
		cout<<"Kolicina(broj ambalaza/broj grama): ";
		cout<<nam[i].kolicina<<endl;
		cout<<"Cena po komadu/Cena po 100g: ";
		cout<<nam[i].cena<<endl;
	}			
}

//Funkcija koja cita namirnice iz datoteke skladiste.txt i smesta u novi niz struktura jer je dodat novi element
namirnica * citanjeSkladista(namirnica nam[])
{
	string red;
	int brojNamirnice=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("skladiste.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }

	//Priprema za ucitavanje txt datoteke u strukture
	int pos=0;
	int flag=0;
	myfile.clear();
	myfile.seekg(0);
	
	
	//Petlja cita datoteku red po red, izvlaci podatke iz nje i smesta u strukturu
	while(getline(myfile, red)) 
	{
		while ((pos = red.find(delimiter)) != std::string::npos) 	//U datoteci su podaci formatirani na sledeci nacin: namirnica | kolicina | cena | 
		{															//Ovaj deo programa cita red do prvog karaktera | koji je imenovan kao delimiter
    		string token = red.substr(0, pos-1);					//Od pocetka reda do prvog delimitera odvaja string i upisuje ga kao naziv namirnice unutar strukture
    		if(flag==0)												//Posto program broji poziciju svakog delimitera, nakon uspesnog upisivanja u strukturu, taj deo stringa se brise 
			{														//Kako bi mogao da se nadje novi delimiter(koji nakon brisanja postaje prvi u redu kako bi mogla da ga nadje funkcija find())
				nam[brojNamirnice].naziv=token;						//u redu koji ce predstavljati kolicinu, odnosno cenu namirnice
				flag=1;												//Ovo se obavlja dok funkcija find() ne vrati vrednost npos, sto znaci da se doslo do kraja reda
			}
			else if(flag==1)
			{
				stringstream konvertor(token);
				konvertor>>nam[brojNamirnice].kolicina;
				flag=2;
			}
			else if(flag==2)
			{
				stringstream konvertor(token);
				konvertor>>nam[brojNamirnice].cena;
				flag=0;
				brojNamirnice++;
			}		
    		red.erase(0, pos + delimiter.length());															
		}													
    }
   
  	//Funkcija koja sortira sve namirnice po abecednom poretku
    sortiranjeSkladista(nam, brojNamirnice);
	 	
	 //Zatvaranje fajla za citanje
  	myfile.close();
  	
  	return nam;
}

//Funkcija za meni jelovnika
void jelovnik()
{
	jelo *jel;
	string opcija;
	int x;
	int brojJela=0;
	int *brojSastojaka;
	
	system("cls");
	
	//Prikupljanje svih struktura jela, ukupnog broja struktura, kao i brojeva sastojaka svake strukture
	jel=citanjeJelovnika(&brojJela);
	brojSastojaka = citanjeJelovnika();
	
	system("cls");
	cout<<"________________________________________J E L O V N I K________________________________________\n"<<endl;
	cout<<"Dostupna jela: "<<endl;
	
	ispisJelovnika(jel, brojJela, brojSastojaka);
	
	//Ispis daljih opcija u ovom meniju
	cout<<"________________________________________\n"<<endl;
	cout<<"\n Izaberite sta zelite da radite:\n"<<endl;
	cout<<"1. Azuriraj postojece jelo\n"<<endl;
	cout<<"2. Obrisi jelo\n"<<endl;
	cout<<"3. Dodaj novo jelo\n"<<endl;
	cout<<"4. Nazad u glavni meni\n"<<endl;
	
	cout<<"Vas unos: "<<endl;
	getline(cin,opcija);
	x=atoi(opcija.c_str());

	switch(x)
	{
		case 1: azuriranjeJela(jel, brojJela, brojSastojaka);break;
		case 2: brojJela=brisanjeJela(jel, brojJela, brojSastojaka);
				delete[] brojSastojaka;
				brojSastojaka = new int[brojJela];
				if(brojSastojaka==NULL)
				{
					cerr<<"Nije uspela dinamicka dodela memorije za niz koji sadrzi broj sastojaka\n"<<endl;
				}
				brojSastojaka = citanjeJelovnika();	
				upisJelovnik(jel, brojJela, brojSastojaka);										
				delete[] jel;																
				jel = new jelo[brojJela]; 	
				if(jel==NULL)
				{
					cerr<<"Nije uspela dinamicka dodela memorije za jelo\n"<<endl;
				}
				brojSastojaka = citanjeJelovnika();	
				jelovnik();
				break;
		case 3: brojJela=unosJela(jel, brojJela, brojSastojaka);
				jelovnik();
				break;
		case 4: delete[] jel->sas; delete[] jel;  glavniMeni();break;
		default: 
		{
			cout<<"Nije unet odgovarajuc broj"<<endl;
			system("pause");
			jelovnik();
		}
	}
	
}

void rezervacije()
{
	rezervacija *rez;
	string opcija;
	int x;
	int brojRezervacija=0;
	int *brojJela;
	
	//Prikupljanje svih struktura rezervacija, ukupnog broja struktura, kao i brojeva jela svake strukture
	rez=citanjeRezervacija(&brojRezervacija);
	brojJela = citanjeRezervacija();
	
	system("cls");
	cout<<"_____________________________________R E Z E R V A C I J E_____________________________________\n"<<endl;
	
	ispisRezervacija(rez, brojRezervacija, brojJela);
	
	//Ispis daljih opcija u ovom meniju
	cout<<"________________________________________\n"<<endl;
	cout<<"\n Izaberite sta zelite da radite:\n"<<endl;
	cout<<"1. Azuriraj postojecu rezervaciju\n"<<endl;
	cout<<"2. Obrisi rezervaciju\n"<<endl;
	cout<<"3. Dodaj novu rezervaciju\n"<<endl;
	cout<<"4. Nazad u glavni meni\n"<<endl;
	
	cout<<"Vas unos: "<<endl;
	getline(cin,opcija);
	x=atoi(opcija.c_str());

	switch(x)
	{
		case 1: azuriranjeRezervacije(rez, brojRezervacija, brojJela);break;
		case 2: brojRezervacija=brisanjeRezervacije(rez, brojRezervacija, brojJela);
				delete[] brojJela;
				brojJela = new int[brojRezervacija];
				if(brojJela==NULL)
				{
					cerr<<"Nije uspela dinamicka dodela memorije za niz koji sadrzi broj sastojaka\n"<<endl;
				}
				brojJela = citanjeRezervacija();	
				upisRezervacije(rez, brojRezervacija, brojJela);										
				delete[] rez;																
				rez = new rezervacija[brojRezervacija]; 	
				if(rez==NULL)
				{
					cerr<<"Nije uspela dinamicka dodela memorije za jelo\n"<<endl;
				}
				brojJela = citanjeRezervacija();	
				rezervacije();
				break;
		case 3: brojRezervacija=unosRezervacije(rez, brojRezervacija, brojJela);
				rezervacije();
				break;
		case 4: delete[] rez->jel; delete[] rez; glavniMeni();break;
		default: 
		{
			cout<<"Nije unet odgovarajuc broj"<<endl;
			system("pause");
			rezervacije();
		}
	}
	
}

//Funkcija za azuriranje vec postojecih namirnica u skladistu
void azuriranjeSkladista(namirnica nam[], int n)
{
	int indicator=0;
	string nazivNam;
	cout<<"Unesite naziv namirnice koju zelite da azurirate: "<< endl;
	getline(cin,nazivNam);
	for(int i=0;i<n;i++)
	{
		if(nam[i].naziv==nazivNam)
		{
			string noviNaziv;
			string opcija, cena, kol;																			//U ovoj funkciji se koriste atoi funkcije zajedno sa c_str() funkcijama
			int konv;																							//Za konverziju stringova koji predstavljaju brojeve u intove 
			int flag=0;																							//Jer sam odlucio da do kraja programa koristim getline() umesto cin 
			cout<<"\nPronadjena namirnica!"<< endl;																//Zbog rasprostranjenog koriscenja tipa podataka String i ciscenja ulaznog bafera
			indicator=1;
			while(flag==0)																						//While petlja je ovde da bi u slucaju unosa pogresne opcije moglo da se 
			{																									//vrati na izbor opcije
				cout<<"Unesite sta zelite da azurirate: (1 za naziv, 2 za kolicinu, 3 za cenu)"<<endl;
				getline(cin, opcija);
				konv=atoi(opcija.c_str());
				switch(konv)
				{
					case 1: cout<<"Unesite novi naziv na namirnicu: " <<endl;
							getline(cin,noviNaziv);
							nam[i].naziv=noviNaziv;
							flag=1;
							break;
					case 2: cout<<"Unesite novu kolicinu za namirnicu: "<<endl;
							getline(cin,cena);
							konv=atoi(cena.c_str());
							nam[i].kolicina=konv;
							flag=1;
							break;
					case 3: cout<<"Unesite novu cenu za namirnicu: "<<endl;
							getline(cin,kol);
							konv=atoi(kol.c_str());
							nam[i].cena=konv;
							flag=1;
							break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;	
							//system("pause");
				}		
			}
			break;	
		}	
	}
	if(indicator==0) 
	{
		string nazad;
		int konv;
		while(konv<1 || konv>2)
		{
			cout<<"\nNije pronadjena namirnica"<<endl;
			cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u skladiste: "<< endl;		       //Deo programa koji hvata neodgovarajuce upise i daje opciju da korisnik pokusa 
			getline(cin,nazad);																	   //pokusa ponovo ili da se vrati na prvobitni meni
			konv=atoi(nazad.c_str());
			if(konv==1)
			{
				azuriranjeSkladista(nam, n);
			}
			else if(konv==2)
	    	{
	    		skladiste();
			}
			else
			{
				cout<<"Pogresan unos\n"<<endl;
			}
		}	
	}
	//upis u datoteku
	else{
		upisSkladiste(nam,n);
		skladiste();
	}				
}

//Funkcija za azuriranje vec postojecih namirnica u skladistu kada se unese vec postojeca namirnica
void azuriranjeSkladista(namirnica nam[], int n, string naziv)
{
	int flag=0;
	string nazivNam=naziv;
	for(int i=0;i<n;i++)
	{
		if(nam[i].naziv==nazivNam)
		{
			string noviNaziv;
			string opcija, cena, kol;																			
			int konv;																																													
			cout<<"\nPronadjena namirnica!"<< endl;															
			while(flag==0)																						
			{																									
				cout<<"Unesite sta zelite da azurirate: (1 za naziv, 2 za kolicinu, 3 za cenu)"<<endl;
				getline(cin, opcija);
				konv=atoi(opcija.c_str());
				switch(konv)
				{
					case 1: cout<<"Unesite novi naziv na namirnicu: " <<endl;
							getline(cin,noviNaziv);
							nam[i].naziv=noviNaziv;
							flag=1;
							break;
					case 2: cout<<"Unesite novu kolicinu za namirnicu: "<<endl;
							getline(cin,cena);
							konv=atoi(cena.c_str());
							nam[i].kolicina=konv;
							flag=1;
							break;
					case 3: cout<<"Unesite novu cenu za namirnicu: "<<endl;
							getline(cin,kol);
							konv=atoi(kol.c_str());
							nam[i].cena=konv;
							flag=1;
							break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;	
							
				}		
			}
			break;	
		}	
	}
		
	//upis u datoteku
		upisSkladiste(nam,n);
		skladiste();	
}

//Funkcija za upis niza sturktura namirnica u datoteku skladiste.txt
void upisSkladiste(namirnica nam[],int n)
{
	std::ofstream ofs;
	ofs.open("skladiste.txt", ofstream::out);
	for(int i=0;i<n;i++)
	{
		ofs<<nam[i].naziv<<" | "<<nam[i].kolicina<<" | "<<nam[i].cena<<" | "<<endl;
	}
	ofs.close();
}

//Funkcija za brisanje trazenog elementa iz strukture i datoteke skladiste.txt
int brisanjeSkladiste(namirnica nam[], int n)
{
	int indicator = 0;
	string nazivNam;
	namirnica *ptr1, *ptr2;
	ptr1=ptr2=nam;
	cout<<"\nUnesite naziv namirnice koju zelite da obrisete: "<< endl;
	getline(cin,nazivNam);
	for(int i=0;i<n;i++)                            							 //Sazimanje niza struktura namirnica
	{	
																				 //uz pomoc dva pokazivaca. Svaki put kada se naidje na ime koje ne odgovara ukucanom
		if(nam[i].naziv!=nazivNam)					 						   	 //inkrementiraju se adrese oba pokazivaca, a kada se naidje na ime koje odgovara ukucanom,
		{																		 //inkrementira se samo 1(i indikator se postavlja na 1 kako ne bi doslo do ponavljanja procesa)															
			*ptr2=*ptr1;														 //Inkrementiranjem samo jednog pokazivaca kada naidjemo na trazeno ime preskacemo mesto na kom se nalazi trazeni element
			ptr2++;																 //I time skracujemo duzinu niza za jedan (posto funkcija vraca novu duzinu niza).
		}											 							 //Pokazivac ptr2 dobija vrednost pokazivaca ptr1 kako bi se, nakon nailaska na trazen element, u sledecem ciklusu 
		else if(nam[i].naziv==nazivNam)             							 //na njegovo mesto smestio element koji se nalazi posle njega
		{											 							 //Kada se ova funkcija zavrsi, u funkciji skladiste() se poziva funkcija koja upisuje novi niz struktura u datoteku
		    indicator=1;														 //Brise se stara memorija koju zauzima stara struktura i poziva se ponovo funkcija skladiste() koja na pocetku alocira memoriju za
		    								 									 //Novu strukturu
		}	
		ptr1++;	
	}
			
	if(indicator==0) 
	{
		string nazad;
		int konv;
		while(konv<1 || konv>2)
		{
			cout<<"\nNije pronadjena namirnica"<<endl;
			cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u skladiste: "<< endl;		
			getline(cin,nazad);
			konv=atoi(nazad.c_str());
			if(konv==1)
			{
				brisanjeSkladiste(nam, n);
			}
			else if(konv==2)
	    	{
	    		skladiste();
			}
			else
			{
				cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;
			}
		}
	}
	return (ptr2-nam);
}

//Funkcija za unos nove namirnice u skladiste unosom sa tastature, primaju se niz struktura namirnica, i ukupan broj namirnica
int unosSkladiste(namirnica nam[],int n)
{
	
	string nazivNam;
	int cena, kolicina;
	string opcija;
	int konv;
	int indicator=0;
	cout<<"\nUnesite naziv namirnice koju dodajete: "<<endl;
	getline(cin,nazivNam);
	for(int i=0;i<n;i++)															//Petlja za proveru da li vec postoji uneta namirnica
	{
		if(nazivNam==nam[i].naziv)
		{
			cout<<"\nUneta namirnica vec postoji u skladistu!"<<endl;
			while(indicator==0)
			{
				cout<<"\nUnesite 1 za azuriranje unete namirnice, 2 za ponovnu pretragu, 3 za povratak u skladiste: "<<endl;
				getline(cin,opcija);
				konv=atoi(opcija.c_str());
			
				switch(konv)
				{
					case 1: azuriranjeSkladista(nam,n,nazivNam);break;
					case 2: unosSkladiste(nam,n);break;
					case 3: skladiste();break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;
				}		
			}	
		}
	}
	cout<<"\nUnesite kolicinu namirnice koju dodajete: "<<endl;
	cin>>kolicina;
	cout<<"\nUnesite cenu namirnice koju dodajete: "<<endl;
	cin.ignore();
	cin>>cena;
	cin.ignore();
	
	//Povecanje duzine niza struktura za jedan i brisanje starog niza struktura
	n++;
	delete[] nam;
	
	//Nova alokacija memorije
	nam = new namirnica[n]; 
	if(nam==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za namirnice\n"<<endl;
	}
	
	nam=citanjeSkladista(nam);
	nam[n-1].naziv=nazivNam;
	nam[n-1].kolicina=kolicina;
	nam[n-1].cena=cena;
	
	upisSkladiste(nam,n);
		
	return n;	
}

//Funkcija za unos nove namirnice u skladiste kada program u citanju jelovnika ne moze da pronadje namirnicu iz jelovnika u skladistu
int unosSkladiste(namirnica nam[], int n, string nazivNam)
{
	
	int cena, kolicina;
	string opcija;
	int konv;
	cout<<"\nUnesite kolicinu namirnice koju dodajete: "<<endl;
	cin>>kolicina;
	cout<<"\nUnesite cenu namirnice koju dodajete: "<<endl;
	cin.ignore();
	cin>>cena;
	cin.ignore();
	
	//Povecanje duzine niza struktura za jedan i brisanje starog niza struktura
	n++;
	delete[] nam;
	
	//Nova alokacija memorije
	nam = new namirnica[n]; 
	if(nam==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za namirnice\n"<<endl;
	}
	
	nam=citanjeSkladista(nam);
	nam[n-1].naziv=nazivNam;
	nam[n-1].kolicina=kolicina;
	nam[n-1].cena=cena;
	
	upisSkladiste(nam,n);
		
	return n;	
}

//Funkcija za sortiranje skladista po abecednom redu
void sortiranjeSkladista(namirnica nam[], int n)
{	
	for(int i=0; i<n; i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if(nam[j].naziv<nam[i].naziv)							//Klasican bubble sort
			{
				namirnica pom=nam[j];
				nam[j]=nam[i];
				nam[i]=pom;	
			}
		}
	}
}

//Funkcija koja vraca niz svih jela sa njihovim vrstama, nazivima, sastojcima i cenom, kao i ukupan broj jela
jelo * citanjeJelovnika(int *n)
{
	jelo *jel;
	string red;
	int x;
	int brojJela=0;
	namirnica *nam;
	int brojNam;
	nam=citanjeSkladista(&brojNam);
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("jelovnik.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{	
		brojJela++;
	}
	
	//Dinamicka dodela memorije za jela i njena provera
    jel = new jelo[brojJela]; 
	if(jel==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za jela\n"<<endl;
	}
	
	//dinamicka alokacija memorije za niz koji sadrzi broj sastojaka za svako jelo
	int *brojSastojaka;
	brojSastojaka = new int[brojJela];
	if(brojSastojaka==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za broj sastojaka\n"<<endl;
	}
	
	//Priprema za brojanje sastojaka
	brojJela=0;
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje sastojaka za svako jelo i dinamicka alokacija niza sastojaka za svako jelo
	while(getline(myfile, red)) 
	{	
		brojSastojaka[brojJela]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojSastojaka[brojJela]++;
			red.erase(0, pos + delimiter.length());	
		}	
		
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze vrsta, naziv, cena i kolicine sastojaka (broji se samo polje gde je naziv sastojka)
		brojSastojaka[brojJela]=brojSastojaka[brojJela]-2;
		brojSastojaka[brojJela]=brojSastojaka[brojJela]/2;
		
		//Dinamicka dodela memorije za niz sastojaka ovog jela i provera dodele
		jel[brojJela].sas = new sastojak[brojSastojaka[brojJela]];
		if(jel[brojJela].sas==NULL)
		{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
		}
		brojJela++;
	}
	
	//Priprema za ucitavanje txt datoteke u strukture
	pos=0;
	brojJela=0;
	myfile.clear();
	myfile.seekg(0);

	
	//Petlja cita datoteku red po red, izvlaci podatke iz nje i smesta u strukturu
	while(getline(myfile, red)) 
	{
		int flag=0;
		int i=0;
		brojSastojaka[brojJela]=0;
		int skip=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	//U datoteci su podaci formatirani na sledeci nacin: vrsta | jelo | namirnica1 | namirnica2 | ... | cena 
		{			
			int exists=0;											//BrojSastojaka je niz brojeva koji sadrzi broj sastojaka za svako jelo i prethodno je dinamicki alociran
    		string token = red.substr(0, pos-1);										//Sto znaci da ako je brojSastojaka[0]=3, to znaci da prvo jelo u nizu ima 3 sastojka
    		if(flag==0)												
			{	
				jel[brojJela].vrsta=token;													
				flag=1;												
			}
			else if(flag==1)
			{
				jel[brojJela].naziv=token;								
				flag=2;
			}
	 		else if((flag%2)==0)
			{	
				for(int k=0;k<brojNam;k++)
				{
					if(token==nam[k].naziv)														//Prolaz kroz petlju namirnica u datoteci skladiste da se vidi da li sastojak 	
					{																			//Koji se trenutno cita postoji u skladistu
						jel[brojJela].sas[brojSastojaka[brojJela]].naziv=token;					//Ako postoji, smesta se u listu sastojaka, ako ne, daje se korisniku mogucnost
						flag++;																	//Na mestu da upise odmah u datoteku skladiste novi sastojak
						exists=1;
					}		
				}
				if(exists==0)
					{
						string odluka="b";
						cout<<"Sledeca namirnica ne postoji u skladistu: "<<token<<endl;
						cout<<"Potrebna je za jelo: "<<jel[brojJela].naziv<<endl;
						while(odluka!="y" && odluka!="n")
						{
							cout<<"Da li zelite da je dodate u skladiste? (y/n) ";
							getline(cin,odluka);
							if(odluka=="y")
							{
								brojNam = unosSkladiste(nam, brojNam, token);
								nam=citanjeSkladista(&brojNam);
								jel[brojJela].sas[brojSastojaka[brojJela]].naziv=token;
								flag++;
								break;
							}
							else if(odluka=="n")
							{
								cout<<"\nMoraju postojati sve namirnice za sva jela u skladistu. Povratak u glavni meni."<<endl;
								cout<<"Pritisnite enter da nastavite"<<endl;
								system("pause");
								glavniMeni();
								break;
							}
							else
							{
								cout<<"Pogresan unos opcije!"<<endl;
							}
						}
						cout<<endl;
					}
			}		
			else if((flag%2)!=0)
			{
				int konv=atoi(token.c_str());
				jel[brojJela].sas[brojSastojaka[brojJela]].kolicina=konv;	
				brojSastojaka[brojJela]++;
				flag++;		
			}
			red.erase(0, pos + delimiter.length()+1);			
		}
		int konv=atoi(red.c_str());
		jel[brojJela].cena=konv;
		brojJela++;														
	}																
	 	
	//Zatvaranje fajla za citanje
  	myfile.close();

  	*n=brojJela;
  	return jel;
}

//Funkcija koja funkciji jelovnik vraca niz koji sadrzi brojeve sastojaka za svako jelo
int * citanjeJelovnika()
{
	jelo *jel;
	string red;
	int x;
	int brojJela=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("jelovnik.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{	
		brojJela++;
	}
	
	//Dinamicka dodela memorije za jela i njena provera
    jel = new jelo[brojJela]; 
	if(jel==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za jela\n"<<endl;
	}
	
	//dinamicka alokacija memorije za niz koji sadrzi broj sastojaka za svako jelo
	int *brojSastojaka;
	brojSastojaka = new int[brojJela];
	if(brojSastojaka==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za broj sastojaka\n"<<endl;
	}
	
	//Priprema za brojanje sastojaka
	brojJela=0;
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje sastojaka za svako jelo i dinamicka alokacija niza sastojaka za svako jelo
	while(getline(myfile, red)) 
	{	
		brojSastojaka[brojJela]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojSastojaka[brojJela]++;
			red.erase(0, pos + delimiter.length()+1);	
		}	
		
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze vrsta, naziv, cena i kolicine sastojaka (broji se samo polje gde je naziv sastojka)
		brojSastojaka[brojJela]=brojSastojaka[brojJela]-2;
		brojSastojaka[brojJela]=brojSastojaka[brojJela]/2;
		
		//Dinamicka dodela memorije za niz sastojaka ovog jela i provera dodele
		jel[brojJela].sas = new sastojak[brojSastojaka[brojJela]];
		if(jel[brojJela].sas==NULL)
		{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
		}
		brojJela++;
	}
	
	//Zatvaranje fajla za citanje
  	myfile.close();
  	
	return brojSastojaka;
}

//Funkcija koja funkciji jelovnik vraca niz koji sadrzi brojeve sastojaka za svako jelo
void citanjeBrojaSastojaka(int brojSastojaka[])
{
	string red;
	int brojJela=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("jelovnik.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Priprema za brojanje sastojaka
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje sastojaka za svako jelo i dinamicka alokacija niza sastojaka za svako jelo
	while(getline(myfile, red)) 
	{	
		brojSastojaka[brojJela]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojSastojaka[brojJela]++;
			red.erase(0, pos + delimiter.length()+1);	
		}	
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze vrsta, naziv, cena i kolicine sastojaka (broji se samo polje gde je naziv sastojka)
		brojSastojaka[brojJela]=brojSastojaka[brojJela]-2;
		brojSastojaka[brojJela]=brojSastojaka[brojJela]/2;
		brojJela++;
	}
	
	//Zatvaranje fajla za citanje
  	myfile.close();
  	
}

//Funkcija koja vraca samo niz struktura jela (dobija novoalocirani niz pri unosu novog jela u niz struktura)
jelo * citanjeJelovnika(jelo jel[])
{
	string red;
	int x;
	int brojJela=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("jelovnik.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{	
		brojJela++;
	}
	
	//dinamicka alokacija memorije za niz koji sadrzi broj sastojaka za svako jelo
	int *brojSastojaka;
	brojSastojaka = new int[brojJela];
	if(brojSastojaka==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za broj sastojaka\n"<<endl;
	}
	
	//Priprema za brojanje sastojaka
	brojJela=0;
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje sastojaka za svako jelo i dinamicka alokacija niza sastojaka za svako jelo
	while(getline(myfile, red)) 
	{	
		brojSastojaka[brojJela]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojSastojaka[brojJela]++;
			red.erase(0, pos + delimiter.length());	
		}	
		
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze vrsta, naziv, cena i kolicine sastojaka (broji se samo polje gde je naziv sastojka)
		brojSastojaka[brojJela]=brojSastojaka[brojJela]-2;
		brojSastojaka[brojJela]=brojSastojaka[brojJela]/2;
		
		//Dinamicka dodela memorije za niz sastojaka ovog jela i provera dodele
		jel[brojJela].sas = new sastojak[brojSastojaka[brojJela]];
		if(jel[brojJela].sas==NULL)
		{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
		}
		brojJela++;
	}
	
	//Priprema za ucitavanje txt datoteke u strukture
	pos=0;
	brojJela=0;
	myfile.clear();
	myfile.seekg(0);

	
	//Petlja cita datoteku red po red, izvlaci podatke iz nje i smesta u strukturu
	while(getline(myfile, red)) 
	{
		int flag=0;
		int i=0;
		brojSastojaka[brojJela]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	//U datoteci su podaci formatirani na sledeci nacin: vrsta | jelo | namirnica1 | namirnica2 | ... | cena 
		{															//BrojSastojaka je niz brojeva koji sadrzi broj sastojaka za svako jelo i prethodno je dinamicki alociran
    		string token = red.substr(0, pos-1);					//Sto znaci da ako je brojSastojaka[0]=3, to znaci da prvo jelo u nizu ima 3 sastojka
    		if(flag==0)												
			{	
				jel[brojJela].vrsta=token;													
				flag=1;												
			}
			else if(flag==1)
			{
				jel[brojJela].naziv=token;								
				flag=2;
			}
	 		else if((flag%2)==0)
			{	
				jel[brojJela].sas[brojSastojaka[brojJela]].naziv=token;
				flag++;
			}		
			else if((flag%2)!=0)
			{
				int konv=atoi(token.c_str());
				jel[brojJela].sas[brojSastojaka[brojJela]].kolicina=konv;	
				brojSastojaka[brojJela]++;
				flag++;		
			}
			red.erase(0, pos + delimiter.length()+1);			
		}
		int konv=atoi(red.c_str());
		jel[brojJela].cena=konv;
		brojJela++;														
	}																
	 	
	//Zatvaranje fajla za citanje
  	myfile.close();
  	
  	return jel;
}

//Funkcija za pisanje svih jela koja su u nizu struktura po vrstama 
void ispisJelovnika(jelo *jel, int n, int brojSastojaka[])
{
	cout<<"________________________________________"<<endl;
	cout<<"_____________D O R U C A K______________"<<endl;
	
	for(int i=0;i<n;i++)
	{
		if(jel[i].vrsta=="dorucak")
		{
			cout<<"________________________________________\n"<<endl;	//Posto su u prethodnoj funkciji sve namirnice ucitane iz datoteke u strukture									
			cout<<"Naziv jela: ";										//U ovoj funkciji se samo cita niz struktura svih namirnica i izlistava
			cout<<jel[i].naziv<<endl;
			cout<<"Sastojci jela: "<<endl;
			for(int j=0;j<brojSastojaka[i];j++)
			{
				cout<<j+1<<". Sastojak: "<<jel[i].sas[j].naziv<<endl;
				cout<<"	Kolicina: "<<jel[i].sas[j].kolicina<<" kom/g"<<endl;
			}
			cout<<"Cena jela: "<<endl;
			cout<<jel[i].cena<<endl;
		}
	}	
	
	cout<<"________________________________________"<<endl;
	cout<<"_____________R U C A K______________"<<endl;
	
	for(int i=0;i<n;i++)
	{
		if(jel[i].vrsta=="rucak")
		{
			cout<<"________________________________________\n"<<endl;	
			cout<<"Naziv jela: ";
			cout<<jel[i].naziv<<endl;
			cout<<"Sastojci jela: "<<endl;
			for(int j=0;j<brojSastojaka[i];j++)
			{
				cout<<j+1<<". Sastojak: "<<jel[i].sas[j].naziv<<endl;
				cout<<"	Kolicina: "<<jel[i].sas[j].kolicina<<" kom/g"<<endl;
			}
			cout<<"Cena jela: "<<endl;
			cout<<jel[i].cena<<endl;
		}
	}
	
	cout<<"________________________________________"<<endl;
	cout<<"______________V E C E R A_______________"<<endl;
	
	for(int i=0;i<n;i++)
	{
		if(jel[i].vrsta=="vecera")
		{
			cout<<"________________________________________\n"<<endl;	
			cout<<"Naziv jela: ";
			cout<<jel[i].naziv<<endl;
			cout<<"Sastojci jela: "<<endl;
			for(int j=0;j<brojSastojaka[i];j++)
			{
				cout<<j+1<<". Sastojak: "<<jel[i].sas[j].naziv<<endl;
				cout<<"	Kolicina: "<<jel[i].sas[j].kolicina<<" kom/g"<<endl;
			}
			cout<<"Cena jela: "<<endl;
			cout<<jel[i].cena<<endl;
		}
	}
	
	cout<<"________________________________________"<<endl;
	cout<<"______________S A L A T E_______________"<<endl;
	
	for(int i=0;i<n;i++)
	{
		if(jel[i].vrsta=="salata")
		{
			cout<<"________________________________________\n"<<endl;	
			cout<<"Naziv jela: ";
			cout<<jel[i].naziv<<endl;
			cout<<"Sastojci jela: "<<endl;
			for(int j=0;j<brojSastojaka[i];j++)
			{
				cout<<j+1<<". Sastojak: "<<jel[i].sas[j].naziv<<endl;
				cout<<"	Kolicina: "<<jel[i].sas[j].kolicina<<" kom/g"<<endl;
			}
			cout<<"Cena jela: "<<endl;
			cout<<jel[i].cena<<endl;
		}
	}	
	
	cout<<"________________________________________"<<endl;
	cout<<"______________D E S E R T_______________"<<endl;
	
	for(int i=0;i<n;i++)
	{
		if(jel[i].vrsta=="desert")
		{
			cout<<"________________________________________\n"<<endl;	
			cout<<"Naziv jela: ";
			cout<<jel[i].naziv<<endl;
			cout<<"Sastojci jela: "<<endl;
			for(int j=0;j<brojSastojaka[i];j++)
			{
				cout<<j+1<<". Sastojak: "<<jel[i].sas[j].naziv<<endl;
				cout<<"	Kolicina: "<<jel[i].sas[j].kolicina<<" kom/g"<<endl;
			}
			cout<<"Cena jela: "<<endl;
			cout<<jel[i].cena<<endl;
		}
	}
	
	cout<<"________________________________________"<<endl;
	cout<<"______________PICA_______________"<<endl;
	
	for(int i=0;i<n;i++)
	{
		if(jel[i].vrsta=="pice")
		{
			cout<<"________________________________________\n"<<endl;	
			cout<<"Naziv jela: ";
			cout<<jel[i].naziv<<endl;
			cout<<"Sastojci jela: "<<endl;
			for(int j=0;j<brojSastojaka[i];j++)
			{
				cout<<j+1<<". Sastojak: "<<jel[i].sas[j].naziv<<endl;
				cout<<"	Kolicina: "<<jel[i].sas[j].kolicina<<" kom/g"<<endl;
			}
			cout<<"Cena jela: "<<endl;
			cout<<jel[i].cena<<endl;
		}
	}
	
	cout<<"________________________________________"<<endl;
}

//Funkcija za azuriranje neke od stavki jela koja se izabere unosom, radi po istom principu kao i azuriranje skladista, samo ima vise opcija i poziv funkcije za azuriranje sastojaka 
//nekog jela koji takodje radi po istom principu, ali je odvojen u posebnu funkciju i prosledjuje mu se samo jedna sturktura jer je preglednije i manje komplikovano
void azuriranjeJela(jelo *jel, int n, int brojSastojaka[])
{
	int indicator=0;
	string nazivJel;
	cout<<"Unesite naziv jela koju zelite da azurirate: "<< endl;
	getline(cin,nazivJel);
	for(int i=0;i<n;i++)
	{
		if(jel[i].naziv==nazivJel)
		{
			string noviNaziv;
			string opcija, cena, kol;																		
			int konv;																						
			int flag=0;																							
			cout<<"\nPronadjeno jelo!"<< endl;																
			indicator=1;
			while(flag==0)																					
			{																								
				cout<<"Unesite sta zelite da azurirate: (1 za vrstu, 2 za naziv, 3 za sastojke, 4 za cenu: )"<<endl;
				getline(cin, opcija);
				konv=atoi(opcija.c_str());
				switch(konv)
				{
					case 1: cout<<"Unesite novu vrstu jela: " <<endl;
							getline(cin,noviNaziv);
							jel[i].vrsta=noviNaziv;
							flag=1;
							break;
					case 2: cout<<"Unesite novi naziv jela: " <<endl;
							getline(cin,noviNaziv);
							jel[i].naziv=noviNaziv;
							flag=1;
							break;
					case 3: azuriranjeSastojka(jel[i], n, brojSastojaka[i]);
							flag=1;
							break;
					case 4: cout<<"Unesite novu cenu jela: " <<endl;
							getline(cin,cena);
							konv=atoi(cena.c_str());
							jel[i].cena=konv;
							flag=1;
							break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;	
							 system("pause");
				}		
			}
			break;	
		}	
	}
	if(indicator==0) 
	{
		string nazad;
		int konv;
		while(konv<1 || konv>2)
		{
			cout<<"\nNije pronadjena namirnica"<<endl;
			cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u skladiste: "<< endl;		       //Deo programa koji hvata neodgovarajuce upise i daje opciju da korisnik pokusa 
			getline(cin,nazad);																	   //pokusa ponovo ili da se vrati na prvobitni meni
			konv=atoi(nazad.c_str());
			if(konv==1)
			{
				azuriranjeJela(jel, n, brojSastojaka);
			}
			else if(konv==2)
	    	{
	    		jelovnik();
			}
			else
			{
				cout<<"Pogresan unos\n"<<endl;
			}
		}	
	}
	//upis u datoteku
	else{
		upisJelovnik(jel, n, brojSastojaka);
		jelovnik();
	}				
}

//Funkcija koja se poziva iz funkcije da bi se azurirao jedan ili vise sastojaka nekog jela, funkcionise po istom principu kao i azuriranje nekog jela, samo je ovo jos jedan
//ugnjezdeni prolaz kroz niz, ovoga puta niz sastojaka nekog jela
void azuriranjeSastojka(jelo jel, int n, int brojSastojaka)
{
	namirnica *nam;
	int brojNam;
	
	nam=citanjeSkladista(&brojNam);
	
	string nazivSastojka;
	string jos="y";
	while(jos=="y")																		//Ova while petlja je tu da bi moglo da se azurira vise sastojaka
	{
		cout<<"Unesite naziv sastojka koji zelite da azurirate: "<<endl;
		getline(cin,nazivSastojka);
		int sasIndicator=0;
		int konv;
		for(int j=0;j<brojSastojaka;j++)
		{
			if(jel.sas[j].naziv==nazivSastojka)														//Provera da li postoji sastojak sa unetim nazivom
			{
				int exists=0;
				sasIndicator=1;                                                 						//Ako postoji, indikator se stavlja na 1, cime se izbegava deo na kraju programa
				int sasFlag=0;																			//koji se pusta ako se ne pronadje sastojak, Flag se stavlja na 0 dok se uspesno
				string sasOpcija, noviNaziv, kol;														//ne promeni naziv, kolicina ili oba
				cout<<"\nPronadjen sastojak!"<< endl;
				while(sasFlag==0)
				{
					cout<<"Unesite sta zelite da azurirate: (1 za naziv sastojka, 2 za kolicinu): ";
					getline(cin, sasOpcija);
					konv=atoi(sasOpcija.c_str());
					switch(konv)
						{
							case 1: cout<<"\nUnesite novi naziv sastojka: " <<endl;
									getline(cin,noviNaziv);
									for(int k=0;k<brojNam;k++)											//Petlja koja prolazi kroz skladiste i trazi da li novouneti naziv sastojka
									{																	//Postoji kao namirnica u skladistu
										if(noviNaziv==nam[k].naziv)										//Ako postoji, samo se dodeljuje novi naziv, ako ne postoji, pita se korisnik
										{																//Da li zeli da unese sastojak u skladiste
											jel.sas[j].naziv=noviNaziv;
											sasFlag=1;
											exists=1;
										}		
									}
									if(exists==0)
									{
										string odluka="b";
										cout<<"Sledeca namirnica ne postoji u skladistu: "<<noviNaziv<<endl;
										cout<<"Potrebna je za jelo: "<<jel.naziv<<endl;
										while(odluka!="y" && odluka!="n")
										{
											cout<<"Da li zelite da je dodate u skladiste? (y/n) ";												//Proces koji unosi novouneti sastojak
											getline(cin,odluka);																				//kao namirnicu u skladiste
											if(odluka=="y")
											{
												brojNam = unosSkladiste(nam, brojNam, noviNaziv);
												nam=citanjeSkladista(&brojNam);
												jel.sas[j].naziv=noviNaziv;
												sasFlag=1;
												break;
											}
											else if(odluka=="n")
											{
												cout<<"\nMoraju postojati sve namirnice za sva jela u skladistu. Povratak u jelovnik."<<endl;
												cout<<"Pritisnite enter da nastavite"<<endl;
												system("pause");
												jelovnik();
												break;
											}
											else
											{
												cout<<"Pogresan unos opcije!"<<endl;
											}
										}
									while(sasOpcija!="n" || sasOpcija!="y")												//While petlja koja je tu u slucaju da se unese pogresna opcija
									{
										cout<<"Zelite li da azurirate i kolicinu(y/n): ";
										getline(cin,sasOpcija);
										if(sasOpcija=="y" || sasOpcija=="n")												//Dok ovaj if postoji samo da bi u slucaju da se nije unela
										{																					//odgovarajuca opcija moze izbaciti poruka greske
											break;
										}
										else cout<<"Nije uneta odgovarajuca opcija!\n"<<endl;
									}
									if(sasOpcija=="n")																	//U slucaju da se unese n, izlazi se iz switcha, u suprotnom se 
									{																					//Nastavlja na case 2 sto znaci da korisnik zeli da menja i kolicinu sastojka
										break;
									}
								}
							case 2: cout<<"\nUnesite novu kolicinu sastojka: "<<endl;
									getline(cin,kol);
									konv = atoi(kol.c_str());
									jel.sas[j].kolicina=konv;
									sasFlag=1;
									break;
							default: cout<<"\nNije uneta odgovarajuca opcija!\n"<<endl;
							system("pause");
							}
						cout<<"\nZelite li da azurirate jos neki sastojak ovog jela(y/n): ";
						getline(cin,jos);
						if(jos!="y" && jos!="n")
						{
							cout<<"\nPogresan unos opcije!\n"<<endl;
						}
						else if(jos=="y")
						{
							azuriranjeSastojka(jel, n, brojSastojaka);
						}
						else if(jos=="n")
						{
							break;
						}
				}
			}
		}
		if(sasIndicator==0) 
		{
			string nazad;
			int konv;
			while(konv<1 || konv>2)
			{
				cout<<"\nNije pronadjen sastojak\n"<<endl;
				cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u jelovnik: "<< endl;		       //Deo programa koji hvata neodgovarajuce upise i daje opciju da korisnik pokusa 
				getline(cin,nazad);																	   //pokusa ponovo ili da se vrati na prvobitni meni
				konv=atoi(nazad.c_str());
				if(konv==1)
				{
					azuriranjeSastojka(jel, n, brojSastojaka);
				}
				else if(konv==2)
	    		{
	    			cout<<"Sastojci koje ste do sad azurirali se nece sacuvati!"<<endl;
	    			system("pause");
	    			jelovnik();
				}
				else
				{
					cout<<"Pogresan unos\n"<<endl;
				}
			}	
		}
	}	
}

//Funkcija koja upisuje azurirani niz stuktura jela u txt datoteku
void upisJelovnik(jelo jel[],int n,int brojSastojaka[])
{
	std::ofstream ofs;
	ofs.open("jelovnik.txt", ofstream::out);
	for(int i=0;i<n;i++)
	{
		ofs<<jel[i].vrsta<<" | "<<jel[i].naziv<<" | ";
		for(int j=0;j<brojSastojaka[i];j++)												//Ova petlja prolazi kroz sve sastojke jela na kom se trenutno nalazi prva petlja
		{
			ofs<<jel[i].sas[j].naziv<<" | "<<jel[i].sas[j].kolicina<<" | ";
		}
		ofs<<jel[i].cena<<endl;
	}
	ofs.close();
}

//Funkcija za sortiranje jelovnika po abecednom redu 
//NE FUNKCIONISE TRENUTNO KAKO TREBA, POTREBNA DORADA
void sortiranjeJelovnika(jelo jel[], int n)
{	
	for(int i=0; i<n; i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if(jel[j].naziv<jel[i].naziv)							//Klasican bubble sort
			{
				jelo pom=jel[j];
				jel[j]=jel[i];
				jel[i]=pom;	
			}
		}
	}
}

//Funkcija za brisanje vec postojeceg jela u strukturama
int brisanjeJela(jelo jel[], int n, int brojSastojaka[])
{
	int indicator = 0;
	string nazivJel;
	jelo *ptr1, *ptr2;
	int *ptr3, *ptr4;
	ptr1=ptr2=jel;
	ptr3=ptr4=brojSastojaka;
	cout<<"\nUnesite naziv jela koje zelite da obrisete: "<< endl;
	getline(cin,nazivJel);
	for(int i=0;i<n;i++)                            
	{		 										 //Sazimanje niza struktura jela i niza broja sastojaka svakog jela
													 //ptr1 i ptr 2 su inicijalizovani na niz struktura jela i vrednost ptr2 se izjednacava sa vrednoscu ptr1 ukoliko
		if(jel[i].naziv!=nazivJel)					 //jos uvek nismo pronasli trazeno ime, isto se desava sa pokazivacima ptr4 i ptr3
		{										     //Ovo znaci da jos nismo pronasli ime jela koje treba da se brise i samim tim pokazivaci ptr2 i ptr4 treba da prate
			*ptr2=*ptr1;							 //svoje respektivne ekvivalente i primaju njihove vrednosti. Kada se naidje na trazeni element, ptr2 i ptr4 ne dobijaju  
			ptr2++;									 //vrednosti svojih ekvivalenata i ostaju na istoj adresi, a ptr1 i ptr3 se inkrementiraju (prebacuje im se adresa na 
			*ptr4=*ptr3;							 //element jedno mesto posle trazenog). U sledecim ciklusu, ptr2 i ptr4 ce primiti vrednosti od ptr1 i ptr3 koji se nalaze
			ptr4++;									 //jedno mesto posle trazenog elementa, i tako ce u sustini adresa trazenog mesta dobiti vrednost mesta ispred sebe
		}											 //i ovo se nastavlja do kraja niza
		else if(jel[i].naziv==nazivJel)            
		{					 						 
		    indicator=1;														 	
		}	
		ptr1++;	
		ptr3++;	
	}
			
	if(indicator==0) 
	{
		string nazad;
		int konv;
		while(konv<1 || konv>2)
		{
			cout<<"\nNije pronadjena namirnica"<<endl;
			cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u skladiste: "<< endl;		
			getline(cin,nazad);
			konv=atoi(nazad.c_str());
			if(konv==1)
			{
				brisanjeJela(jel, n, brojSastojaka);
			}
			else if(konv==2)
	    	{
	    		jelovnik();
			}
			else
			{
				cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;
			}
		}
	}
	return (ptr2-jel);
}

//Funkcija za unos novih jela u jelovnik
int unosJela(jelo jel[], int n, int brojSastojaka[])
{
	string nazivJela;
	int kolicina, cena;
	string opcija;
	string vrsta;
	int konv;
	int indicator=0;
	namirnica *nam;
	int brojNam;
	nam=citanjeSkladista(&brojNam);
	cout<<"\nUnesite naziv jela koje dodajete: "<<endl;
	getline(cin,nazivJela);
	for(int i=0;i<n;i++)															//Petlja za proveru da li vec postoji uneto jelo
	{
		if(nazivJela==jel[i].naziv)
		{
			cout<<"\nUneto jelo vec postoji na jelovniku!"<<endl;
			while(indicator==0)
			{
				cout<<"\nUnesite 1 za azuriranje unetog jela, 2 za ponovnu pretragu, 3 za povratak u jelovnik: "<<endl;
				getline(cin,opcija);
				konv=atoi(opcija.c_str());
			
				switch(konv)
				{
					case 1: azuriranjeJela(jel, n, brojSastojaka, nazivJela);break;
					case 2: unosJela(jel, n, brojSastojaka);break;
					case 3: jelovnik();break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;
				}		
			}	
		}
	}
	cout<<"\nUnesite vrstu jela koje dodajete: ";
	getline(cin,vrsta);
	cout<<"\nUnesite koliko sastojaka ima novo jelo: ";
	cin>>kolicina;
	cin.ignore();
	
	//Privremeni niz sastojaka za novo jelo u kom se unose sastojci novog jela
	sastojak *privSas;
	privSas=new sastojak[kolicina];
	if(privSas==NULL)
	{
		cerr<<"\nNeuspesna dodela memorije za privremeni niz sastojaka"<<endl;
	}
	
	for(int i=0;i<kolicina;i++)
	{
		int exists=0;
		cout<<"\nUnesite naziv "<<i+1<<". sastojka: "<<endl;
		getline(cin,privSas[i].naziv);
		for(int k=0;k<brojNam;k++)											//Petlja koja prolazi kroz skladiste i trazi da li novouneti naziv sastojka
		{																	//Postoji kao namirnica u skladistu
			if(privSas[i].naziv==nam[k].naziv)								//Ako postoji, samo se dodeljuje novi naziv, ako ne postoji, pita se korisnik
			{																//Da li zeli da unese sastojak u skladiste
				exists=1;
			}		
		}
		if(exists==0)
		{
			string odluka="b";
			cout<<"Sledeca namirnica ne postoji u skladistu: "<<privSas[i].naziv<<endl;
			while(odluka!="y" && odluka!="n")
			{
				cout<<"Da li zelite da je dodate u skladiste? (y/n) ";												//Proces koji unosi novouneti sastojak
				getline(cin,odluka);																				//kao namirnicu u skladiste
				if(odluka=="y")
				{
					brojNam = unosSkladiste(nam, brojNam, privSas[i].naziv);
					nam=citanjeSkladista(&brojNam);
					break;
				}
				else if(odluka=="n")
				{
					cout<<"\nMoraju postojati sve namirnice za sva jela u skladistu. Povratak u jelovnik."<<endl;
					cout<<"Pritisnite enter da nastavite"<<endl;
					system("pause");
					jelovnik();
					break;
				}
				else
				{
					cout<<"Pogresan unos opcije!"<<endl;
				}
			}
		}
		cout<<"\nUnesite kolicinu "<<i+1<<". sastojka: "<<endl;
		cin>>privSas[i].kolicina;
		cin.ignore();
	}
	cout<<"\nUnesite cenu jela koje dodajete: "<<endl;
	cin>>cena;
	cin.ignore();
	
	
	//Povecanje duzine niza struktura za jedan i brisanje starog niza struktura
	n++;
	delete[] jel;																
	jel = new jelo[n]; 	
	if(jel==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za jelo\n"<<endl;
	}
	
	jel=citanjeJelovnika(jel);
	
	//Nova alokacija memorije
	brojSastojaka = new int[n]; 
	if(brojSastojaka==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za namirnice\n"<<endl;
	}
	//Funkcija za dobavljanje niza brojeva koji sadrzi koliko svako jelo ima sastojaka (popunjavanje novo alociranog niza brojSastojaka)
	citanjeBrojaSastojaka(brojSastojaka);
	for(int i=0;i<n;i++)
	{
		cout<<brojSastojaka[i]<<endl;
	}
	brojSastojaka[n-1]=kolicina;

	//Dinamicka dodela memorije za sastojke novog jela
	jel[n-1].sas = new sastojak[brojSastojaka[n-1]];
	if(jel[n-1].sas==NULL)
	{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
	}
	
	jel[n-1].vrsta=vrsta;
	jel[n-1].naziv=nazivJela;
	for(int i=0;i<brojSastojaka[n-1];i++)
	{
		jel[n-1].sas[i]=privSas[i];
	}
	jel[n-1].cena=cena;
	upisJelovnik(jel, n, brojSastojaka);
	delete[] privSas;
	delete[] jel[n-1].sas;

	return n;
}

//Funkcija za azuriranje postojeceg jela kada se unese jelo koje vec postoji pri unosu
void azuriranjeJela(jelo jel[], int n, int brojSastojaka[], string nazivJel)
{
	int indicator=0;
	for(int i=0;i<n;i++)
	{
		if(jel[i].naziv==nazivJel)
		{
			string noviNaziv;
			string opcija, cena, kol;																		
			int konv;																						
			int flag=0;																							
			cout<<"\nPronadjeno jelo!"<< endl;																
			indicator=1;
			while(flag==0)																					
			{																								
				cout<<"Unesite sta zelite da azurirate: (1 za vrstu, 2 za naziv, 3 za sastojke, 4 za cenu: )"<<endl;
				getline(cin, opcija);
				konv=atoi(opcija.c_str());
				switch(konv)
				{
					case 1: cout<<"Unesite novu vrstu jela: " <<endl;
							getline(cin,noviNaziv);
							jel[i].vrsta=noviNaziv;
							flag=1;
							break;
					case 2: cout<<"Unesite novi naziv jela: " <<endl;
							getline(cin,noviNaziv);
							jel[i].naziv=noviNaziv;
							flag=1;
							break;
					case 3: azuriranjeSastojka(jel[i], n, brojSastojaka[i]);
							flag=1;
							break;
					case 4: cout<<"Unesite novu cenu jela: " <<endl;
							getline(cin,cena);
							konv=atoi(cena.c_str());
							jel[i].cena=konv;
							flag=1;
							break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;	
							 system("pause");
				}		
			}
			break;	
		}	
	}
	upisJelovnik(jel, n, brojSastojaka);
	jelovnik();				
}

//Funkcija koja iz datoteke cita sve rezervacije i smesta ih u niz struktura, takodje vraca ukupan broj struktura u nizu
rezervacija * citanjeRezervacija(int n[])
{
	rezervacija *rez;
	string red;
	int x;
	int brojRezervacija=0;
	jelo *jel;
	int brojJel;
	jel=citanjeJelovnika(&brojJel);
	int *brojSastojaka;
	brojSastojaka = citanjeJelovnika();
	
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("rezervacije.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{	
		brojRezervacija++;
	}
	
	//Dinamicka dodela memorije za jela i njena provera
    rez = new rezervacija[brojRezervacija]; 
	if(rez==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za jela\n"<<endl;
	}
	
	//dinamicka alokacija memorije za niz koji sadrzi broj jela za svaku rezervaciju, niz je koncipiran tako da ako je brojJela[0]=3, to znaci da prva rezervacija ima 3 narucena jela
	int *brojJela;
	brojJela = new int[brojRezervacija];
	if(brojJela==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za broj sastojaka\n"<<endl;
	}
	
	//Priprema za brojanje sastojaka
	brojRezervacija=0;
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje jela za svaku rezervaciju i dinamicka alokacija niza struktura Jelo za svaku rezervaciju
	while(getline(myfile, red)) 
	{	
		brojJela[brojRezervacija]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojJela[brojRezervacija]++;
			red.erase(0, pos + delimiter.length());	
		}	
		
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze datum, vreme, sto i kolicine jela (broji se samo polje gde je naziv jela)
		brojJela[brojRezervacija]=brojJela[brojRezervacija]-2;
		brojJela[brojRezervacija]=brojJela[brojRezervacija]/2;
		
		//Dinamicka dodela memorije za niz jela ove rezervacije i provera dodele
		rez[brojRezervacija].jel = new jelo[brojJela[brojRezervacija]];
		if(rez[brojRezervacija].jel==NULL)
		{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
		}
		brojRezervacija++;
	}
	
	//Priprema za ucitavanje txt datoteke u strukture
	pos=0;
	brojRezervacija=0;
	myfile.clear();
	myfile.seekg(0);

	
	//Petlja cita datoteku red po red, izvlaci podatke iz nje i smesta u strukturu
	while(getline(myfile, red)) 
	{
		int flag=0;
		int i=0;
		brojJela[brojRezervacija]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	//U datoteci su podaci formatirani na sledeci nacin: datum | vreme | jelo1 | jelo2 | ... | sto 
		{			
			int exists=0;											//BrojJela je niz brojeva koji sadrzi broj jela za svaku rezervaciju i prethodno je dinamicki alociran
    		string token = red.substr(0, pos-1);					//Sto znaci da ako je brojJela[0]=3, to znaci da prva rezervacija u nizu ima 3 narucena jela
    		if(flag==0)												
			{	
				rez[brojRezervacija].datum=token;													
				flag=1;												
			}
			else if(flag==1)
			{
				int konv=atoi(token.c_str());
				rez[brojRezervacija].vreme=konv;								
				flag=2;
			}
	 		else if((flag%2)==0)
			{	
				for(int k=0;k<brojJel;k++)
				{
					if(token==jel[k].naziv)														//Prolaz kroz petlju jela u datoteci jelovnik da se vidi da li jelo 	
					{																			//Koji se trenutno cita postoji u jelovniku
						rez[brojRezervacija].jel[brojJela[brojRezervacija]].naziv=token;
						rez[brojRezervacija].jel[brojJela[brojRezervacija]].cena=jel[k].cena;	//Ako postoji, smesta se u listu jela, ako ne, daje se korisniku mogucnost
						flag++;																	//Na mestu da upise odmah u datoteku jelovnik novo jelo
						exists=1;																//Cena se isto smesta da bi na kraju mogla da se izracuna cena narudzbine
					}		
				}
				if(exists==0)
					{
						string odluka="b";
						cout<<"Sledece jelo ne postoji u skladistu: "<<token<<endl;
						cout<<"Potrebna je za rezervaciju: "<<rez[brojRezervacija].datum<<" u "<<rez[brojRezervacija].vreme<<endl;
						while(odluka!="y" && odluka!="n")
						{
							cout<<"Da li zelite da je dodate u jelovnik? (y/n) ";
							getline(cin,odluka);
							if(odluka=="y")
							{
								brojJel = unosJela(jel, brojJel, brojSastojaka, token);
								jel=citanjeJelovnika(&brojJel);
								rez[brojRezervacija].jel[brojJela[brojRezervacija]].naziv=token;
								for(int k=0;k<brojJel;k++)
								{
									if(token==jel[k].naziv)															
									{																			
										rez[brojRezervacija].jel[brojJela[brojRezervacija]].cena=jel[k].cena;													
									}		
								}
								flag++;
								break;
							}
							else if(odluka=="n")
							{
								cout<<"\nMoraju postojati sva jela za sve rezervacije u jelovniku. Povratak u glavni meni."<<endl;
								cout<<"Pritisnite enter da nastavite"<<endl;
								system("pause");
								glavniMeni();
								break;
							}
							else
							{
								cout<<"Pogresan unos opcije!"<<endl;
							}
						}
						cout<<endl;
					}
			}		
			else if((flag%2)!=0)
			{
				int konv=atoi(token.c_str());
				rez[brojRezervacija].jel[brojJela[brojRezervacija]].kolicina=konv;	
				brojJela[brojRezervacija]++;
				flag++;		
			}
			red.erase(0, pos + delimiter.length()+1);			
		}
		int konv=atoi(red.c_str());
		rez[brojRezervacija].sto=konv;
		brojRezervacija++;														
	}																
	 	
	//Zatvaranje fajla za citanje
  	myfile.close();

  	*n=brojRezervacija;
  	return rez;
}

//Funkcija za unos novih jela u jelovnik kada ne postoje u jelovniku a u narudzbini su
int unosJela(jelo jel[], int n, int brojSastojaka[], string nazivJela)
{
	int kolicina, cena;
	string opcija;
	string vrsta;
	int konv;
	int indicator=0;
	namirnica *nam;
	int brojNam;
	nam=citanjeSkladista(&brojNam);

	cout<<"\nUnesite vrstu jela koje dodajete: ";
	getline(cin,vrsta);
	cout<<"\nUnesite koliko sastojaka ima novo jelo: ";
	cin>>kolicina;
	cin.ignore();
	
	//Privremeni niz sastojaka za novo jelo u kom se unose sastojci novog jela
	sastojak *privSas;
	privSas=new sastojak[kolicina];
	if(privSas==NULL)
	{
		cerr<<"\nNeuspesna dodela memorije za privremeni niz sastojaka"<<endl;
	}
	
	for(int i=0;i<kolicina;i++)
	{
		int exists=0;
		cout<<"\nUnesite naziv "<<i+1<<". sastojka: "<<endl;
		getline(cin,privSas[i].naziv);
		for(int k=0;k<brojNam;k++)											//Petlja koja prolazi kroz skladiste i trazi da li novouneti naziv sastojka
		{																	//Postoji kao namirnica u skladistu
			if(privSas[i].naziv==nam[k].naziv)								//Ako postoji, samo se dodeljuje novi naziv, ako ne postoji, pita se korisnik
			{																//Da li zeli da unese sastojak u skladiste
				exists=1;
			}		
		}
		if(exists==0)
		{
			string odluka="b";
			cout<<"Sledeca namirnica ne postoji u skladistu: "<<privSas[i].naziv<<endl;
			while(odluka!="y" && odluka!="n")
			{
				cout<<"Da li zelite da je dodate u skladiste? (y/n) ";												//Proces koji unosi novouneti sastojak
				getline(cin,odluka);																				//kao namirnicu u skladiste
				if(odluka=="y")
				{
					brojNam = unosSkladiste(nam, brojNam, privSas[i].naziv);
					nam=citanjeSkladista(&brojNam);
					break;
				}
				else if(odluka=="n")
				{
					cout<<"\nMoraju postojati sve namirnice za sva jela u skladistu. Povratak u jelovnik."<<endl;
					cout<<"Pritisnite enter da nastavite"<<endl;
					system("pause");
					jelovnik();
					break;
				}
				else
				{
					cout<<"Pogresan unos opcije!"<<endl;
				}
			}
		}
		cout<<"\nUnesite kolicinu "<<i+1<<". sastojka: "<<endl;
		cin>>privSas[i].kolicina;
		cin.ignore();
	}
	cout<<"\nUnesite cenu jela koje dodajete: "<<endl;
	cin>>cena;
	cin.ignore();
	
	
	//Povecanje duzine niza struktura za jedan i brisanje starog niza struktura
	n++;
	delete[] jel;																
	jel = new jelo[n]; 	
	if(jel==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za jelo\n"<<endl;
	}
	
	jel=citanjeJelovnika(jel);
	
	//Nova alokacija memorije
	brojSastojaka = new int[n]; 
	if(brojSastojaka==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za namirnice\n"<<endl;
	}
	//Funkcija za dobavljanje niza brojeva koji sadrzi koliko svako jelo ima sastojaka (popunjavanje novo alociranog niza brojSastojaka)
	citanjeBrojaSastojaka(brojSastojaka);
	brojSastojaka[n-1]=kolicina;

	//Dinamicka dodela memorije za sastojke novog jela
	jel[n-1].sas = new sastojak[brojSastojaka[n-1]];
	if(jel[n-1].sas==NULL)
	{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
	}
	
	jel[n-1].vrsta=vrsta;
	jel[n-1].naziv=nazivJela;
	for(int i=0;i<brojSastojaka[n-1];i++)
	{
		jel[n-1].sas[i]=privSas[i];
	}
	jel[n-1].cena=cena;
	upisJelovnik(jel, n, brojSastojaka);

	return n;
}

//Funkcija koja funkciji rezervacije vraca niz koji sadrzi brojeve jela za svaku rezervaciju
int * citanjeRezervacija()
{
	rezervacija *rez;
	string red;
	int x;
	int brojRezervacija=0;
	jelo *jel;
	int brojJel;
	jel=citanjeJelovnika(&brojJel);
	
	//Otvaranje fajla za citanje vec upisanih podataka u skladiste
	ifstream myfile;
	myfile.open("rezervacije.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{	
		brojRezervacija++;
	}
	
	//Dinamicka dodela memorije za jela i njena provera
    rez = new rezervacija[brojRezervacija]; 
	if(rez==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za jela\n"<<endl;
	}
	
	//dinamicka alokacija memorije za niz koji sadrzi broj jela za svaku rezervaciju, niz je koncipiran tako da ako je brojJela[0]=3, to znaci da prva rezervacija ima 3 narucena jela
	int *brojJela;
	brojJela = new int[brojRezervacija];
	if(brojJela==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za broj sastojaka\n"<<endl;
	}
	
	//Priprema za brojanje sastojaka
	brojRezervacija=0;
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje jela za svaku rezervaciju i dinamicka alokacija niza struktura Jelo za svaku rezervaciju
	while(getline(myfile, red)) 
	{	
		brojJela[brojRezervacija]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojJela[brojRezervacija]++;
			red.erase(0, pos + delimiter.length());	
		}	
		
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze datum, vreme, sto i kolicine jela (broji se samo polje gde je naziv jela)
		brojJela[brojRezervacija]=brojJela[brojRezervacija]-2;
		brojJela[brojRezervacija]=brojJela[brojRezervacija]/2;
		
		//Dinamicka dodela memorije za niz jela ove rezervacije i provera dodele
		rez[brojRezervacija].jel = new jelo[brojJela[brojRezervacija]];
		if(rez[brojRezervacija].jel==NULL)
		{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
		}
		brojRezervacija++;
	}															
	 	
	//Zatvaranje fajla za citanje
  	myfile.close();

  	return brojJela;
}

//Funkcija za pisanje svih rezervacija koja su u nizu struktura
void ispisRezervacija(rezervacija *rez, int n, int brojJela[])
{
	
	for(int i=0;i<n;i++)
	{
			int cena=0;
			cout<<"________________________________________\n"<<endl;					//Posto su u prethodnoj funkciji sve namirnice ucitane iz datoteke u strukture									
			cout<<"Rezervisano za: "<<rez[i].datum<<" u "<<rez[i].vreme<<endl;			//U ovoj funkciji se samo cita niz struktura svih rezervacija i izlistava
			cout<<"Porucena jela: "<<endl;
			for(int j=0;j<brojJela[i];j++)
			{
				cout<<j+1<<". Jelo: "<<rez[i].jel[j].naziv<<endl;
				cout<<"	Kolicina: "<<rez[i].jel[j].kolicina<<" porcije"<<endl;
				cena = cena + (rez[i].jel[j].cena*rez[i].jel[j].kolicina);
			}
			cout<<"Rezervisano za sto: "<<rez[i].sto<<endl;
			cout<<"Cena jela: "<<endl;
			cout<<cena<<endl;
	}	
	cout<<"________________________________________"<<endl;
}

//Funkcija za azuriranje vec postojecih rezervacija
void azuriranjeRezervacije(rezervacija *rez, int n, int brojJela[])
{
	int indicator=0;
	string dan, mesec, godina, datum;
	string sto, vreme;
	int bSto, bVrem;
	cout<<"\nUnesite dan u mesecu rezervacije koju zelite da azurirate: "<< endl;
	getline(cin,dan);
	cout<<"\nUnesite mesec rezervacije koju zelite da azurirate: "<< endl;
	getline(cin,mesec);
	cout<<"\nUnesite godinu rezervacije koju zelite da azurirate: "<< endl;
	getline(cin,godina);
	datum = dan + "/" + mesec + "/" + godina;
	cout<<datum<<endl;
	cout<<"\nUnesite rezervisano vreme: "<< endl;
	getline(cin,vreme);
	bVrem=atoi(vreme.c_str());
	cout<<"\nUnesite rezervisani sto: "<< endl;
	getline(cin,sto);
	bSto=atoi(sto.c_str());
	for(int i=0;i<n;i++)
	{
		if(rez[i].datum==datum && rez[i].vreme==bVrem && rez[i].sto==bSto)                                    //Provera da li postoji rezervacija
		{
			string nDan, nMesec, nGodina, nDatum, opcija;
			string nSto, nVreme;
			int bnSto, bnVreme;																		
			int konv;																						
			int flag=0;																							
			cout<<"\nPronadjena rezervacija!"<< endl;																
			indicator=1;
			while(flag==0)																					
			{																								
				cout<<"Unesite sta zelite da azurirate: (1 za datum, 2 za vreme, 3 za jela, 4 za sto): "<<endl;
				getline(cin, opcija);
				konv=atoi(opcija.c_str());
				switch(konv)
				{
					case 1: cout<<"Unesite novi datum rezervacije: " <<endl;
							cout<<"Unesite dan u mesecu rezervacije koju zelite da azurirate: "<< endl;
							getline(cin,nDan);
							cout<<"Unesite mesec rezervacije koju zelite da azurirate: "<< endl;
							getline(cin,nMesec);
							cout<<"Unesite godinu rezervacije koju zelite da azurirate: "<< endl;
							getline(cin,nGodina);
							nDatum = nDan + "/" + nMesec + "/" + nGodina;
							rez[i].datum=nDatum;
							flag=1;
							break;
					case 2: cout<<"Unesite novo vreme rezervacije: " <<endl;
							getline(cin,nVreme);
							bnVreme=atoi(nVreme.c_str());
							rez[i].vreme=bnVreme;
							flag=1;
							break;
					case 3: azuriranjeJela(rez[i], n, brojJela[i]);
							flag=1;
							break;
					case 4: cout<<"Unesite novi sto za rezervaciju: " <<endl;
							getline(cin,nSto);
							bnSto=atoi(nSto.c_str());
							for(int j=0;j<n;j++)
							{
								if(rez[j].datum==datum && rez[j].vreme==bVrem && rez[j].sto==bnSto)
								{
									cout<<"Ovaj sto je zauzet u zahtevanom vremenu i datumu"<<endl;
									cout<<"Pritisnite enter da bi ste nastavili"<<endl;
									system("pause");
									rezervacije();
								}		
							}
							rez[i].sto=bnSto;
							flag=1;
							break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;	
							 system("pause");
				}		
			}
			break;	
		}	
	}
	if(indicator==0) 
	{
		string nazad;
		int konv;
		while(konv<1 || konv>2)
		{
			cout<<"\nNije pronadjena rezervacija"<<endl;
			cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u rezervacije: "<< endl;		       //Deo programa koji hvata neodgovarajuce upise i daje opciju da korisnik pokusa 
			getline(cin,nazad);																	   //pokusa ponovo ili da se vrati na prvobitni meni
			konv=atoi(nazad.c_str());
			if(konv==1)
			{
				azuriranjeRezervacije(rez, n, brojJela);
			}
			else if(konv==2)
	    	{
	    		rezervacije();
			}
			else
			{
				cout<<"Pogresan unos\n"<<endl;
			}
		}	
	}
	//upis u datoteku
	else{
		upisRezervacije(rez, n, brojJela);
		rezervacije();
	}			
}

//Funkcija za azuriranje jela u postojecoj rezervaciji
void azuriranjeJela(rezervacija rez, int n, int brojJela)
{
	jelo *jel;
	int brojJel;
	int *brojSastojaka;
	
	jel=citanjeJelovnika(&brojJel);
	brojSastojaka = citanjeJelovnika();
	
	namirnica *nam;
	int brojNam;
	nam=citanjeSkladista(&brojNam);
	
	string nazivJela;
	string jos="y";
	while(jos=="y")																		//Ova while petlja je tu da bi moglo da se azurira vise jela
	{
		cout<<"Unesite naziv jela koji zelite da azurirate: "<<endl;
		getline(cin,nazivJela);
		int jelIndicator=0;
		int konv;
		for(int j=0;j<brojJela;j++)
		{
			if(rez.jel[j].naziv==nazivJela)														//Provera da li postoji jelo unetim nazivom
			{
				int exists=0;
				jelIndicator=1;                                                 						//Ako postoji, indikator se stavlja na 1, cime se izbegava deo na kraju programa
				int jelFlag=0;																			//koji se pusta ako se ne pronadje jelo, Flag se stavlja na 0 dok se uspesno
				string jelOpcija, noviNaziv, kol;														//ne promeni naziv, kolicina ili oba
				cout<<"\nPronadjeno Jelo!"<< endl;
				while(jelFlag==0)
				{
					cout<<"Unesite sta zelite da azurirate: (1 za naziv jela, 2 za kolicinu): ";
					getline(cin, jelOpcija);
					konv=atoi(jelOpcija.c_str());
					switch(konv)
						{
							case 1: cout<<"\nUnesite novo jelo: " <<endl;
									getline(cin,noviNaziv);
									for(int k=0;k<brojJel;k++)											//Petlja koja prolazi kroz jelovnik i trazi da li novouneti naziv jela
									{																	//Postoji kao jelo u jelovniku
										if(noviNaziv==jel[k].naziv)										//Ako postoji, samo se dodeljuje novi naziv, ako ne postoji, pita se korisnik
										{																//Da li zeli da unese jelo u jelovnik
											rez.jel[j].naziv=noviNaziv;
											jelFlag=1;
											exists=1;
											for(int k=0;k<brojJel;k++)								
											{																	
												if(noviNaziv==jel[k].naziv)										
												{															
													for(int m=0;m<brojSastojaka[k];m++)								//Petlje koje prolaze kroz sastojke trenutnog jela i svih namirnica u skladistu
													{																	//I kada naidju u skladistu na namirnice koje su sastojci trenutnog jela, oduzimaju za kolicinu koja je narucena
														for(int l=0;l<brojNam;l++)													
														{																			
															if(jel[k].sas[m].naziv==nam[l].naziv)
															{
																nam[l].kolicina=nam[l].kolicina-(rez.jel[j].kolicina*jel[k].sas[m].kolicina);
																if(nam[l].kolicina<0)
																{
																	cout<<"Nestao je sledeci sastojak "<<nam[l].naziv<<" u skladistu. Nemoguce je napraviti ovu rezervaciju."<<endl;
																	cout<<"Pritisnite enter da nastavite nazad u rezervacije"<<endl;
																	system("pause");
																	rezervacije();
																}
															}
														}
													}
												}		
											}
										}		
									}
									if(exists==0)
									{
										string odluka="b";
										cout<<"Sledece jelo ne postoji u jelovniku: "<<noviNaziv<<endl;
										while(odluka!="y" && odluka!="n")
										{
											cout<<"Da li zelite da ga dodate u jelovnik? (y/n) ";												//Proces koji unosi novouneto jelo
											getline(cin,odluka);																				//u jelovnik
											if(odluka=="y")
											{
												brojJel = unosJela(jel, brojJel, brojSastojaka, noviNaziv);
												jel=citanjeJelovnika(&brojJel);
												rez.jel[j].naziv=noviNaziv;
												jelFlag=1;
												break;
											}
											else if(odluka=="n")
											{
												cout<<"\nMoraju postojati sva jela za sve rezervacije u jelovniku. Povratak u rezervacije."<<endl;
												cout<<"Pritisnite enter da nastavite"<<endl;
												system("pause");
												rezervacije();
												break;
											}
											else
											{
												cout<<"Pogresan unos opcije!"<<endl;
											}
										}
									}
									while(jelOpcija!="n" || jelOpcija!="y")												//While petlja koja je tu u slucaju da se unese pogresna opcija
									{
										cout<<"Zelite li da azurirate i kolicinu(y/n): ";
										getline(cin,jelOpcija);
										if(jelOpcija=="y" || jelOpcija=="n")												//Dok ovaj if postoji samo da bi u slucaju da se nije unela
										{																					//odgovarajuca opcija moze izbaciti poruka greske
											break;
										}
										else cout<<"Nije uneta odgovarajuca opcija!\n"<<endl;
									}
									if(jelOpcija=="n")																	//U slucaju da se unese n, izlazi se iz switcha, u suprotnom se 
									{																					//Nastavlja na case 2 sto znaci da korisnik zeli da menja i kolicinu sastojka
										break;
									}
							case 2: cout<<"\nUnesite novu kolicinu porcija jela: "<<endl;
									getline(cin,kol);
									konv = atoi(kol.c_str());
									rez.jel[j].kolicina=konv;
									for(int k=0;k<brojJel;k++)								
									{																	
										if(noviNaziv==jel[k].naziv)										
										{															
											for(int m=0;m<brojSastojaka[k];m++)								//Petlje koje prolaze kroz sastojke trenutnog jela i svih namirnica u skladistu
												{																	//I kada naidju u skladistu na namirnice koje su sastojci trenutnog jela, oduzimaju za kolicinu koja je narucena
													for(int l=0;l<brojNam;l++)													
													{																			
														if(jel[k].sas[m].naziv==nam[l].naziv)
														{
															nam[l].kolicina=nam[l].kolicina-(rez.jel[j].kolicina*jel[k].sas[m].kolicina);
															if(nam[l].kolicina<0)
															{
																cout<<"Nestao je sledeci sastojak "<<nam[l].naziv<<" u skladistu. Nemoguce je napraviti ovu rezervaciju."<<endl;
																cout<<"Pritisnite enter da nastavite nazad u rezervacije"<<endl;
																system("pause");
																rezervacije();
															}
														}
													}
												}
										}		
									}
										
									jelFlag=1;
									break;
							default: cout<<"\nNije uneta odgovarajuca opcija!\n"<<endl;
							system("pause");
							}
						cout<<"\nZelite li da azurirate jos neko jelo ove rezervacije(y/n): ";
						getline(cin,jos);
						if(jos!="y" && jos!="n")
						{
							cout<<"\nPogresan unos opcije!\n"<<endl;
						}
						else if(jos=="y")
						{
							azuriranjeJela(rez, n, brojJel);
						}
						else if(jos=="n")
						{
							break;
						}
				}
			}
		}
		if(jelIndicator==0) 
		{
			string nazad;
			int konv;
			while(konv<1 || konv>2)
			{
				cout<<"\nNije pronadjen sastojak\n"<<endl;
				cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u jelovnik: "<< endl;		       //Deo programa koji hvata neodgovarajuce upise i daje opciju da korisnik pokusa 
				getline(cin,nazad);																	   //pokusa ponovo ili da se vrati na prvobitni meni
				konv=atoi(nazad.c_str());
				if(konv==1)
				{
					azuriranjeJela(rez, n, brojJela);
				}
				else if(konv==2)
	    		{
	    			cout<<"Jela koje ste do sad azurirali se nece sacuvati!"<<endl;
	    			system("pause");
	    			rezervacije();
				}
				else
				{
					cout<<"Pogresan unos\n"<<endl;
				}
			}	
		}
	}
	//Upis azuriranih zaliha u skladiste
	upisSkladiste(nam, brojNam);	
}

void upisRezervacije(rezervacija *rez, int n, int brojJela[])
{
	std::ofstream ofs;
	ofs.open("rezervacije.txt", ofstream::out);
	for(int i=0;i<n;i++)
	{
		ofs<<rez[i].datum<<" | "<<rez[i].vreme<<" | ";
		for(int j=0;j<brojJela[i];j++)												//Ova petlja prolazi kroz sva jela u rezervaciji na kom se trenutno nalazi prva petlja
		{
			ofs<<rez[i].jel[j].naziv<<" | "<<rez[i].jel[j].kolicina<<" | ";
		}
		ofs<<rez[i].sto<<endl;
	}
	ofs.close();
}

int brisanjeRezervacije(rezervacija rez[], int n, int brojJela[])
{
	int indicator = 0;
	string dan, mesec, godina, datum;
	string sto, vreme;
	int bSto, bVrem;
	rezervacija *ptr1, *ptr2;
	int *ptr3, *ptr4;
	ptr1=ptr2=rez;
	ptr3=ptr4=brojJela;
	cout<<"\nUnesite dan u mesecu rezervacije koju zelite da obrisete: "<< endl;
	getline(cin,dan);
	cout<<"\nUnesite mesec rezervacije koju zelite da obrisete: "<< endl;
	getline(cin,mesec);
	cout<<"\nUnesite godinu rezervacije koju zelite da obrisete: "<< endl;
	getline(cin,godina);
	datum = dan + "/" + mesec + "/" + godina;
	cout<<datum<<endl;
	cout<<"\nUnesite rezervisano vreme: "<< endl;
	getline(cin,vreme);
	bVrem=atoi(vreme.c_str());
	cout<<"\nUnesite rezervisani sto: "<< endl;
	getline(cin,sto);
	bSto=atoi(sto.c_str());
	for(int i=0;i<n;i++)                            
	{		 										 										//Sazimanje niza struktura rezervacija i niza broja jela svake rezervacije
													 										//ptr1 i ptr 2 su inicijalizovani na niz struktura rezervacija i vrednost ptr2 se izjednacava sa vrednoscu ptr1 ukoliko
		if(rez[i].datum!=datum && rez[i].vreme!=bVrem && rez[i].sto!=bSto)					//jos uvek nismo pronasli trazene parametre, isto se desava sa pokazivacima ptr4 i ptr3
		{										  										    //Ovo znaci da jos nismo pronasli parametre rezervacije koje treba da se brise i samim tim pokazivaci ptr2 i ptr4 treba da prate
			*ptr2=*ptr1;																	//svoje respektivne ekvivalente i primaju njihove vrednosti. Kada se naidje na trazeni element, ptr2 i ptr4 ne dobijaju  
			ptr2++;									 										//vrednosti svojih ekvivalenata i ostaju na istoj adresi, a ptr1 i ptr3 se inkrementiraju (prebacuje im se adresa na 
			*ptr4=*ptr3;							 										//element jedno mesto posle trazenog). U sledecim ciklusu, ptr2 i ptr4 ce primiti vrednosti od ptr1 i ptr3 koji se nalaze
			ptr4++;																			//jedno mesto posle trazenog elementa, i tako ce u sustini adresa trazenog mesta dobiti vrednost mesta ispred sebe
		}											 										//i ovo se nastavlja do kraja niza
		else if(rez[i].datum==datum && rez[i].vreme==bVrem && rez[i].sto==bSto)            
		{					 						 
		    indicator=1;														 	
		}	
		ptr1++;	
		ptr3++;	
	}
			
	if(indicator==0) 
	{
		string nazad;
		int konv;
		while(konv<1 || konv>2)
		{
			cout<<"\nNije pronadjena namirnica"<<endl;
			cout<<"Unesite 1 za ponovnu pretragu, 2 za povratak u skladiste: "<< endl;		
			getline(cin,nazad);
			konv=atoi(nazad.c_str());
			if(konv==1)
			{
				brisanjeRezervacije(rez, n, brojJela);
			}
			else if(konv==2)
	    	{
	    		rezervacije();
			}
			else
			{
				cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;
			}
		}
	}
	return (ptr2-rez);
}

//Funkcija za unosenje nove rezervacije u sistem
int unosRezervacije(rezervacija rez[], int n, int brojJela[])
{
	string dan, mesec, godina, datum;
	string sto, vreme;
	int bSto, bVrem, kolicina;
	cout<<"\nUnesite dan u mesecu kada zelite da rezervisete: "<< endl;
	getline(cin,dan);
	cout<<"\nUnesite mesec kada zelite da rezervisete: "<< endl;
	getline(cin,mesec);
	cout<<"\nUnesite godinu rezervacije: "<< endl;
	getline(cin,godina);
	datum = dan + "/" + mesec + "/" + godina;
	cout<<datum<<endl;
	cout<<"\nUnesite rezervisano vreme: "<< endl;
	getline(cin,vreme);
	bVrem=atoi(vreme.c_str());
	cout<<"\nUnesite rezervisani sto: "<< endl;
	getline(cin,sto);
	bSto=atoi(sto.c_str());
	string opcija;
	int konv;
	int indicator=0;
	
	jelo *jel;
	int brojJel=0;
	int *brojSastojaka;
	
	jel=citanjeJelovnika(&brojJel);
	brojSastojaka=citanjeJelovnika();
	
	namirnica *nam;
	int brojNam;
	nam=citanjeSkladista(&brojNam);
	

	for(int i=0;i<n;i++)															//Petlja za proveru da li vec postoji uneta rezervacija
	{
		if(rez[i].datum==datum && rez[i].vreme==bVrem && rez[i].sto==bSto)
		{
			cout<<"\nUneti datum je u to vreme rezervisan za taj sto!"<<endl;
			while(indicator==0)
			{
				cout<<"\nUnesite 1 za azuriranje unete rezervacije, 2 za ponovnu pretragu, 3 za povratak u rezervacije: "<<endl;
				getline(cin,opcija);
				konv=atoi(opcija.c_str());
			
				switch(konv)
				{
					case 1: azuriranjeRezervacije(rez, n, brojJela, datum, bVrem, bSto);break;
					case 2: unosRezervacije(rez, n, brojJela);break;
					case 3: rezervacije();break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;
				}		
			}	
		}
	}
	cout<<"\nUnesite koliko jela narucujete: ";
	cin>>kolicina;
	cin.ignore();
	
	//Privremeni niz struktura za jela koja ce biti deo rezervacije
	jelo *privJel;
	privJel=new jelo[kolicina];
	if(privJel==NULL)
	{
		cerr<<"\nNeuspesna dodela memorije za privremeni niz sastojaka"<<endl;
	}
	
	for(int i=0;i<kolicina;i++)
	{
		cout<<"\nUnesite broj porcija "<<i+1<<". jela: "<<endl;
		cin>>privJel[i].kolicina;
		cin.ignore();
		
		int exists=0;
		cout<<"\nUnesite naziv "<<i+1<<". jela: "<<endl;
		getline(cin,privJel[i].naziv);
		for(int k=0;k<brojJel;k++)											//Petlja koja prolazi kroz skladiste i trazi da li novouneti naziv jela
		{																	//Postoji kao jelo u jelovniku
			if(privJel[i].naziv==jel[k].naziv)								//Ako postoji, samo se dodeljuje novi naziv, ako ne postoji, pita se korisnik
			{																//Da li zeli da unese jelo u jelovnik
				exists=1;
				for(int m=0;m<brojSastojaka[k];m++)								//Petlje koje prolaze kroz sastojke trenutnog jela i svih namirnica u skladistu
				{																	//I kada naidju u skladistu na namirnice koje su sastojci trenutnog jela, oduzimaju za kolicinu koja je narucena
					for(int l=0;l<brojNam;l++)													
					{																			
						if(jel[k].sas[m].naziv==nam[l].naziv)
						{
							nam[l].kolicina=nam[l].kolicina-(privJel[i].kolicina*jel[k].sas[m].kolicina);
							if(nam[l].kolicina<0)
							{
								cout<<"Nestao je sledeci sastojak "<<nam[l].naziv<<" u skladistu. Nemoguce je napraviti ovu rezervaciju."<<endl;
								cout<<"Pritisnite enter da nastavite nazad u rezervacije"<<endl;
								system("pause");
								rezervacije();
							}
						}
					}
				}
			}		
			
		}
		if(exists==0)
		{
			string odluka="b";
			cout<<"Sledece jelo ne postoji u jelovniku: "<<privJel[i].naziv<<endl;
			while(odluka!="y" && odluka!="n")
			{
				cout<<"Da li zelite da ga dodate u jelovnik? (y/n) ";												//Proces koji unosi novouneto jelo
				getline(cin,odluka);																				//u jelovnik
				if(odluka=="y")
				{
					brojJel = unosJela(jel, brojJel, brojSastojaka, privJel[i].naziv);
					jel=citanjeJelovnika(&brojJel);
					break;
				}
				else if(odluka=="n")
				{
					cout<<"\nMoraju postojati sva jela iz rezervacija u jelovniku. Povratak u rezervacije."<<endl;
					cout<<"Pritisnite enter da nastavite"<<endl;
					system("pause");
					rezervacije();
					break;
				}
				else
				{
					cout<<"Pogresan unos opcije!"<<endl;
				}
			}
		}
	}
	
	//Povecanje duzine niza struktura za jedan i brisanje starog niza struktura
	n++;
	delete[] rez;																
	rez = new rezervacija[n]; 	
	if(rez==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za rezervaciju\n"<<endl;
	}
	
	rez=citanjeRezervacija(rez);
	
	//Nova alokacija memorije
	brojJela = new int[n]; 
	if(brojJela==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za Jela\n"<<endl;
	}
	//Funkcija za dobavljanje niza brojeva koji sadrzi koliko svaka rezervacija obuhvata jela (popunjavanje novoalociranog niza brojJela)
	citanjeBrojaJela(brojJela);

	brojJela[n-1]=kolicina;

	//Dinamicka dodela memorije za jela nove rezervacije
	rez[n-1].jel = new jelo[brojJela[n-1]];
	if(rez[n-1].jel==NULL)
	{
			cerr<<"Nije uspela dinamicka dodela memorije za rezrvaciju\n"<<endl;
	}
	
	rez[n-1].datum=datum;
	rez[n-1].vreme=bVrem;
	for(int i=0;i<brojJela[n-1];i++)
	{
		rez[n-1].jel[i]=privJel[i];
	}
	rez[n-1].sto=bSto;
	upisRezervacije(rez, n, brojJela);
	delete[] privJel;
	delete[] rez[n-1].jel;

	//Upis azuriranih zaliha u skladiste
	upisSkladiste(nam, brojNam);

	return n;
}

//Funkcija za azuriranje rezervacije kad se pri unosu nove rezervacije unese vec postojeca
void azuriranjeRezervacije(rezervacija *rez, int n, int brojJela[], string datum, int bVrem, int bSto)
{
	
	for(int i=0;i<n;i++)
	{
		if(rez[i].datum==datum && rez[i].vreme==bVrem && rez[i].sto==bSto)
		{
			string nDan, nMesec, nGodina, nDatum, opcija;
			string nSto, nVreme;
			int bnSto, bnVreme;																		
			int konv;																						
			int flag=0;																							
			cout<<"\nPronadjena rezervacija!"<< endl;																
			while(flag==0)																					
			{																								
				cout<<"Unesite sta zelite da azurirate: (1 za datum, 2 za vreme, 3 za jela, 4 za sto): "<<endl;
				getline(cin, opcija);
				konv=atoi(opcija.c_str());
				switch(konv)
				{
					case 1: cout<<"Unesite novi datum rezervacije: " <<endl;
							cout<<"Unesite dan u mesecu rezervacije koju zelite da azurirate: "<< endl;
							getline(cin,nDan);
							cout<<"Unesite mesec rezervacije koju zelite da azurirate: "<< endl;
							getline(cin,nMesec);
							cout<<"Unesite godinu rezervacije koju zelite da azurirate: "<< endl;
							getline(cin,nGodina);
							nDatum = nDan + "/" + nMesec + "/" + nGodina;
							rez[i].datum=nDatum;
							flag=1;
							break;
					case 2: cout<<"Unesite novo vreme rezervacije: " <<endl;
							getline(cin,nVreme);
							bnVreme=atoi(nVreme.c_str());
							rez[i].vreme=bnVreme;
							flag=1;
							break;
					case 3: azuriranjeJela(rez[i], n, brojJela[i]);
							flag=1;
							break;
					case 4: cout<<"Unesite novi sto za rezervaciju: " <<endl;
							getline(cin,nSto);
							bnSto=atoi(nSto.c_str());
							for(int j=0;j<n;j++)
							{
								if(rez[j].datum==datum && rez[j].vreme==bVrem && rez[j].sto==bnSto)
								{
									cout<<"Ovaj sto je zauzet u zahtevanom vremenu i datumu"<<endl;
									cout<<"Pritisnite enter da bi ste nastavili"<<endl;
									system("pause");
									rezervacije();
								}		
							}
							rez[i].sto=bnSto;
							flag=1;
							break;
					default: cout<<"\nNiste uneli odgovrarajuc broj!\n"<<endl;	
							 system("pause");
				}		
			}
			break;	
		}	
	}
	

	upisRezervacije(rez, n, brojJela);
	rezervacije();			
}

//Funkcija za popunjavanje niza koji sadrzi koliko jela je naruceno za svaku rezervaciju (koristi se kad se dodaje nova rezervacija)
void citanjeBrojaJela(int brojJela[])
{
	string red;
	int n=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u rezervacije
	ifstream myfile;
	myfile.open("rezervacije.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Priprema za brojanje rezervacija
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje sastojaka za svako jelo i dinamicka alokacija niza sastojaka za svaku rezervaciju
	while(getline(myfile, red)) 
	{	
		brojJela[n]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojJela[n]++;
			red.erase(0, pos + delimiter.length()+1);	
		}	
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze datum, sto i vreme rezervacije, kao i kolicine svakog jela
		brojJela[n]=brojJela[n]-2;
		brojJela[n]=brojJela[n]/2;
		n++;
	}
	
	//Zatvaranje fajla za citanje
  	myfile.close();
}

//Funkcija koja vraca samo niz struktura rezerervacija (dobija novoalocirani niz pri unosu nove rezervacije u niz struktura)
rezervacija * citanjeRezervacija(rezervacija rez[])
{
	string red;
	int x;
	int brojRez=0;
	
	//Otvaranje fajla za citanje vec upisanih podataka u rezervacije
	ifstream myfile;
	myfile.open("rezervacije.txt");
	string delimiter="|";
	
	//Provera otvaranja fajla
	if(!myfile.is_open()) 
	{
      cerr<<"Neuspesno otvaranje fajla"<<endl;
    }
	
	int pos=0;
	
	//Brojanje linija u txt datoteci radi dinamicke dodele memorije
	while(getline(myfile, red)) 
	{	
		brojRez++;
	}
	
	//dinamicka alokacija memorije za niz koji sadrzi broj jela za svaku rezervaciju
	int *brojJela;
	brojJela = new int[brojRez];
	if(brojJela==NULL)
	{
		cerr<<"Nije uspela dinamicka dodela memorije za broj sastojaka\n"<<endl;
	}
	
	//Priprema za brojanje jela
	brojRez=0;
	myfile.clear();
	myfile.seekg(0);
	
	//Brojanje jela za svaku rezervaciju i dinamicka dodela memorije
	while(getline(myfile, red)) 
	{	
		brojJela[brojRez]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	 
		{																				
    		brojJela[brojRez]++;
			red.erase(0, pos + delimiter.length());	
		}	
		
		//Ova izracunavanja se rade da bi se iskljucila polja gde se nalaze datum, vreme, sto i kolicina svakog jela
		brojJela[brojRez]=brojJela[brojRez]-2;
		brojJela[brojRez]=brojJela[brojRez]/2;
		
		//Dinamicka dodela memorije za niz jela u rezervaciji i provera
		rez[brojRez].jel = new jelo[brojJela[brojRez]];
		if(rez[brojRez].jel==NULL)
		{
			cerr<<"Nije uspela dinamicka dodela memorije za sastojak\n"<<endl;
		}
		brojRez++;
	}
	
	//Priprema za ucitavanje txt datoteke u strukture
	pos=0;
	brojRez=0;
	myfile.clear();
	myfile.seekg(0);

	
	//Petlja cita datoteku red po red, izvlaci podatke iz nje i smesta u strukturu
	while(getline(myfile, red)) 
	{
		int flag=0;
		int i=0;
		brojJela[brojRez]=0;
		while ((pos = red.find(delimiter)) != std::string::npos) 	
		{															
    		string token = red.substr(0, pos-1);					
    		if(flag==0)												
			{	
				rez[brojRez].datum=token;													
				flag=1;												
			}
			else if(flag==1)
			{
				int konv=atoi(token.c_str());
				rez[brojRez].vreme=konv;								
				flag=2;
			}
	 		else if((flag%2)==0)
			{	
				rez[brojRez].jel[brojJela[brojRez]].naziv=token;
				flag++;
			}		
			else if((flag%2)!=0)
			{
				int konv=atoi(token.c_str());
				rez[brojRez].jel[brojJela[brojRez]].kolicina=konv;
				brojJela[brojRez]++;
				flag++;		
			}
			red.erase(0, pos + delimiter.length()+1);			
		}
		int konv=atoi(red.c_str());
		rez[brojRez].sto=konv;
		brojRez++;														
	}																
	 	
	//Zatvaranje fajla za citanje
  	myfile.close();
  	
  	return rez;
}
