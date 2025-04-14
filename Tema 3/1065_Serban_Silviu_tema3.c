#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
typedef struct Serviciu Serviciu;
typedef struct Nod Nod;
typedef struct HashTable HashTable;
struct Serviciu //Serban Silviu
{
	char* denumire;
	char* departament;
	int durata;
	float pret;
	int id;
	int esteUrgent; // 0 sau 1 <=> simulez un bool 
	char* termenLimita;
};

struct Nod
{
	Serviciu info;
	Nod* next;
};

struct HashTable
{
	int dimensiune;
	Nod** vector;
};
Serviciu clonaServiciu(Serviciu serviciu)
{
	Serviciu copie = serviciu;
	//deepcopy 
	copie.denumire = (char*)malloc(sizeof(char) * (strlen(serviciu.denumire) + 1));
	strcpy(copie.denumire, serviciu.denumire);
	copie.departament = (char*)malloc(sizeof(char) * (strlen(serviciu.departament) + 1));
	strcpy(copie.departament, serviciu.departament);
	copie.termenLimita = (char*)malloc(sizeof(char) * (strlen(serviciu.termenLimita) + 1));
	strcpy(copie.termenLimita, serviciu.termenLimita);
	return copie;
}

Serviciu citireServiciu(FILE* fileStream)
{
	Serviciu serviciu;
	char buffer[50];
	fgets(buffer, 50, fileStream);
	char* ptr = strtok(buffer, "\n");
	serviciu.denumire = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
	strcpy(serviciu.denumire, ptr);

	fgets(buffer, 50, fileStream);
	ptr = strtok(buffer, "\n");
	serviciu.departament = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
	strcpy(serviciu.departament, ptr);

	fgets(buffer, 50, fileStream);
	serviciu.durata = atoi(buffer);

	fgets(buffer, 5, fileStream);
	serviciu.pret = atof(buffer);

	fgets(buffer, 10, fileStream);
	serviciu.id = atoi(buffer);

	fgets(buffer, 50, fileStream);
	ptr = strtok(buffer, "\n");
	if (strcmp(ptr, "DA") == 0)
		serviciu.esteUrgent = 1;
	else
		serviciu.esteUrgent = 0;

	fgets(buffer, 50, fileStream);
	ptr = strtok(buffer, "\n");
	serviciu.termenLimita = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
	strcpy(serviciu.termenLimita, ptr);
	return serviciu;
}

Serviciu creareServiciu(char* _denumire, char* _departament, int _durata, float _pret, int _id, int _esteUrgent, char* _termenLimita)
{
	Serviciu serviciu;
	if (_denumire != NULL) {
		serviciu.denumire = (char*)malloc(sizeof(char) * (strlen(_denumire) + 1));
		strcpy(serviciu.denumire, _denumire);
	}else serviciu.denumire = NULL;

	if (_departament != NULL) {
		serviciu.departament = (char*)malloc(sizeof(char) * (strlen(_departament) + 1));
		strcpy(serviciu.departament, _departament);
	}else serviciu.departament = NULL;

	serviciu.durata = _durata;
	serviciu.pret = _pret;
	serviciu.id = _id;

	serviciu.esteUrgent = _esteUrgent;
	if (_termenLimita != NULL) {
		serviciu.termenLimita = (char*)malloc(sizeof(char) * (strlen(_termenLimita) + 1));
		strcpy(serviciu.termenLimita, _termenLimita);
	}
	else serviciu.termenLimita = NULL;

	return serviciu;
}
void afisareServiciu(Serviciu serviciu)
{
	printf("\nAfisare Serviciu de %s :", serviciu.denumire);
	printf("\nDepartament: %s", serviciu.departament);
	printf("\nDurata: %d minute", serviciu.durata);
	printf("\nPret: %.2f", serviciu.pret);
	printf("\nCod: %d", serviciu.id);
	printf((serviciu.esteUrgent ? "\nEste urgent" : "\nNu este urgent"));
	printf("\nTermen Limita: %s\n", serviciu.termenLimita);

}

void put(Nod** coada, Serviciu s)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	nou->next = NULL;
	if (*coada == NULL) {
		*coada = nou;
	}
	else
	{
		Nod* temp = *coada;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}
Serviciu get(Nod** coada)
{
	if (*coada == NULL)
		return creareServiciu(NULL, NULL, 0, 0, 0, 0, NULL);
	else {
		Serviciu s = (*coada)->info;
		Nod* temp = *coada;
		*coada = (*coada)->next;
		free(temp);
		temp = NULL;
		return s;
	}
}

void citireCoadaDinFisier(Nod** coada)
{
	FILE* fileStream = fopen("listaDeServicii.txt", "r");
	if (fileStream == NULL) return;
	while (!feof(fileStream)) {
		Serviciu s = citireServiciu(fileStream);
		put(coada, s);
	}
	fclose(fileStream);
}
void traversareLista(Nod* cap) {
	while (cap) {
		afisareServiciu(cap->info);
		cap = cap->next;
	}
}
void dezalocareServiciu(Serviciu serviciu)
{
	free(serviciu.denumire);
	free(serviciu.departament);
	free(serviciu.termenLimita);
}
Serviciu returnareElementDupaCriteriu(Nod** coada, float _pret)
{
	while (*coada != NULL)
	{
		Serviciu s = get(coada);
		if (s.pret == _pret)
			return s;
		dezalocareServiciu(s);
	}
	Serviciu s = creareServiciu(NULL, NULL, 0, 0, 0, 0, NULL);
	return s;
}
void dezalocareCoada(Nod** coada)
{
	while (*coada != NULL)
	{
		Serviciu s1 = get(coada);
		dezalocareServiciu(s1);
	}
}
void inserareLaSfarsit(Nod** cap, Serviciu info)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = clonaServiciu(info); //pentru a evita shallow copy
	nou->next = NULL;

	if ((*cap) == NULL) *cap = nou;
	else
	{
		Nod* temp = *cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}
int hash(HashTable tabela, int cheie)
{
	return cheie % tabela.dimensiune;
}

HashTable initHashTable(int dimensiune) {
	HashTable tabela;
	tabela.dimensiune = dimensiune;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}
void inserareInHashTable(HashTable tabela, Serviciu serviciu) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(tabela, serviciu.id);
		if (pozitie >= 0 && pozitie < tabela.dimensiune) {
			inserareLaSfarsit(&(tabela.vector[pozitie]), serviciu);
		}
	}
}
void traversareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		printf("\nPozitia: %d", i);
		traversareLista(tabela.vector[i]);
	}
}
Serviciu* vectorDupaCheie(HashTable tabela, int* dim, int cheie) {
	*dim = 0;
	int pozitie = hash(tabela, cheie);
	Nod* cap = tabela.vector[pozitie];
	//aflam dimensiunea
	//traversare lista simpla inlantuita
	while (cap) {
		(*dim)++;
		cap = cap->next;
	}
	Serviciu* vector = (Serviciu*)malloc(sizeof(Serviciu) * (*dim));
	cap = tabela.vector[pozitie];
	for (int i = 0; i < *dim && cap != NULL; i++)
	{
		vector[i] = clonaServiciu(cap->info);
		cap = cap->next;
	}
	return vector;
}

int stergeElementeDinHashTable(HashTable tabela, const char* dep) {
	int nr = 0;
	//traversare hashMap
	for (int i = 0; i < tabela.dimensiune; i++) {
		Nod* cap = tabela.vector[i];
		Nod* prev = NULL;
		//traversare lista
		while (cap) {
			if (strcmp(cap->info.departament, dep) == 0) {
				if (prev == NULL) {
					// stergem primul element
					tabela.vector[i] = cap->next;
				}
				else {
					prev->next = cap->next;
				}
				Nod* temp = cap;
				cap = cap->next;
				dezalocareServiciu(temp->info);
				free(temp);
				nr++;
			}
			else {
				prev = cap;
				cap = cap->next;
			}
		}
	}
	return nr;
}
void dezalocareLista(Nod** cap)
{
	while ((*cap)) {
		dezalocareServiciu((*cap)->info);
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
	*cap = NULL;
}
void dezalocareHashTable(HashTable *tabela) {
	for (int i = 0; i < tabela->dimensiune; i++) {
		dezalocareLista(&(tabela->vector[i]));
	}
	free(tabela->vector);
}
void main()
{
	//2. Sa se citeasca dintr-un fisier txt o coada de tipul structurii definite mai sus. 
	Nod* coada = NULL;
	citireCoadaDinFisier(&coada);
	traversareLista(coada); //cerinta 7 

	//3. Sa se returneze ultimul element de tipul structurii definite care indeplineste un criteriu la alegere, respectand caracteristicile cozii.
	//criteriul ales => pretul == 200
	printf("\n\nCERINTA 3 <=> afisare element cu pret == 200");
	Serviciu s1 = returnareElementDupaCriteriu(&coada, 200.00);
	if (s1.pret != 0) afisareServiciu(s1);
	dezalocareServiciu(s1);
	//stergere coada
	dezalocareCoada(&coada);

	//4. Sa se converteasca elementele din coada preluata din fisier in tabela de dispersie(chaining).
	//5. Inserare in tabela de dispersie.
	citireCoadaDinFisier(&coada);
	HashTable tabela1 = initHashTable(10);
	while (coada != NULL)
	{
		Serviciu s = get(&coada);
		inserareInHashTable(tabela1, s);
	}

	//6. Sa se returneze un vector care sa includa toate nodurile care contin cheia tabelei de dispersie data ca parametru in functie.
	int dim;
	Serviciu* vectorDeServicii = vectorDupaCheie(tabela1, &dim, 2);

	//7. Se afișează toate elementele cozii, ale vectorului si ale tabelei de dispersie. 
	// coada a fost afisata la cerinta 2, acum coada este goala deoarece elementele au fost inserate in hashTable
	printf("\n\n CERINTA 7");
	printf("\n-----------------------------------\n Afisare vector: ");
	for (int i = 0; i < dim; i++)afisareServiciu(vectorDeServicii[i]);
	printf("\n-----------------------------------\n Afisare HashTable: ");
	traversareHashTable(tabela1);

	//8. Sa se stearga toate nodurile din tabela de dispersie care indeplinesc un criteriu la alegere, altul decat cheia de cautare.
	// criteriu <=> departamentul IT
	int nr = stergeElementeDinHashTable(tabela1,"IT");
	printf("\nAu fost efectuate %d stergeri");
	printf("\n\n CERINTA 8");
	printf("\nAu fost efectuate %d stergeri",nr);
	printf("\n-----------------------------------\n Afisare HashTable: ");
	traversareHashTable(tabela1);
	
	//9. Se evita orice fel de memory leaks
	dezalocareHashTable(&tabela1);
	for (int i = 0; i < dim; i++) {
		dezalocareServiciu(vectorDeServicii[i]);
	}
	free(vectorDeServicii);
	
}
//AUTOEVALUARE: 9 
