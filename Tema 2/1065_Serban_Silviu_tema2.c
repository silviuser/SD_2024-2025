#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct Serviciu Serviciu;
typedef struct Nod Nod;
typedef struct NodLS NodLS;

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
	Serviciu serviciu;
	Nod* next;
	Nod* prev;
};
struct NodLS
{
	Serviciu serviciu;
	NodLS* next;
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


void inserareLaInceput(Nod** cap, Nod** coada, Serviciu serviciu) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->serviciu = serviciu;
	nou->next = *cap;
	nou->prev = NULL;

	if ((*cap) != NULL) {
		(*cap)->prev = nou;
		*cap = nou;
	}
	else {
		*cap = *coada = nou;
	}
}

void inserareLaSfarsit(Nod** cap, Nod** coada, Serviciu serviciu) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->serviciu = serviciu;
	nou->next = NULL;
	nou->prev = NULL;

	if ((*coada) != NULL) {
		nou->prev = *coada;
		(*coada)->next = nou;
		*coada = nou;
	}
	else {
		*cap = *coada = nou;
	}
}

void citireListaDubluInlantuita(FILE* filestream, Nod** cap, Nod** coada)
{
	if (filestream == NULL) return;
	while (!feof(filestream)) {
		Serviciu s = citireServiciu(filestream);
		//inserareLaInceput(cap, coada, s); // -> vom obtine lista invers
		inserareLaSfarsit(cap, coada, s);
	}
}
void traversareLista(Nod* cap) {
	while (cap) {
		afisareServiciu(cap->serviciu);
		cap = cap->next;
	}
}
void traversareListaSI(NodLS* cap) {
	while (cap) {
		afisareServiciu(cap->serviciu);
		cap = cap->next;
	}
}
void listaDeServiciiUrgente(Nod** out_cap, Nod** out_coada, Nod* in_cap, Nod* in_coada)
{
	while (in_cap) {
		if (in_cap->serviciu.esteUrgent == 1) // <=> verific conditia de adaugare in lista rezultat
			inserareLaSfarsit(out_cap, out_coada, clonaServiciu(in_cap->serviciu));
		// adaug in lista rezultat o copie deep pentru a evita partajarea unei zone de meorie comune

		in_cap = in_cap->next;
	}
}

void listaDeServicii(Nod** out_cap, Nod** out_coada, Nod* in_cap, Nod* in_coada)
{
	while (in_cap) {
		inserareLaSfarsit(out_cap, out_coada, clonaServiciu(in_cap->serviciu));
		in_cap = in_cap->next;
	}
}

void traversareListaCirculara(Nod* cap) {
	if (cap == NULL) {
		return;
	}
	Nod* copie = cap;
	do {
		afisareServiciu(cap->serviciu);
		cap = cap->next;
	} while (cap != copie);

}
void inserareLaSfarsitLS(NodLS** cap, Serviciu info)
{
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->serviciu = clonaServiciu(info); //pentru a evita shallow copy
	nou->next = NULL;

	if ((*cap) == NULL) *cap = nou;
	else
	{
		NodLS* temp = *cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}
void creeare_ListaSI_din_ListaDI(NodLS** capLS, Nod* capLD) {
	while (capLD) {
		inserareLaSfarsitLS(capLS, capLD->serviciu);
		capLD = capLD->next;
	}
}

void inserareDupaCriteriu(Nod** cap, Nod** coada, Serviciu serviciuNou) {
	if (*cap == NULL) return; 
	Nod* temp = *cap;
	int esteInserat = 0;
	do {
		if (temp->serviciu.durata < serviciuNou.durata) { //criteriul ales 
			Nod* nou = (Nod*)malloc(sizeof(Nod));
			nou->serviciu = serviciuNou;

			//inseram imd dupa nodul cu criteriul indeplinit, adica temp
			nou->next = temp->next; //succesorul lui temp devine succesorul nodului adaugat
			nou->prev = temp; //predecesorul nodului adaugat devine temp

			// setam prev ul succesorului lui temp ca fiind nodul adaugat
			temp->next->prev = nou;
			// setam ca next ul lui temp sa fie acum nodul adaugat ,astfel realizam toate legaturile
			temp->next = nou;

			// daca am inserat dupa coada, actualizam coada ca fiind ultimul el adaugat
			if (temp == *coada) {
				*coada = nou;
			}
			esteInserat = 1; // marcam ca a avut loc inserarea
			// return; // sau oprire dupa prima inserare
		}
		temp = temp->next;
	} while ((temp != *cap) && (esteInserat == 0));
}
void dezalocareServiciu(Serviciu serviciu)
{
	free(serviciu.denumire);
	free(serviciu.departament);
	free(serviciu.termenLimita);
}
void dezalocareLista(Nod** cap, Nod** coada)
{
	// "taiem legaturile" dintre cap si coada daca exista in cazul listei circulare
	(*coada)->next = NULL;
	(*cap)->prev = NULL;

	while ((*cap)) {
		dezalocareServiciu((*cap)->serviciu);
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
	*cap = NULL;
	*coada = NULL;
}
void dezalocareListaLS(NodLS** cap, NodLS** coada)
{
	while ((*cap)) {
		dezalocareServiciu((*cap)->serviciu);
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
	*cap = NULL;
	*coada = NULL;
}
void main()
{
	FILE* fileStream = fopen("listaDeServicii.txt", "r");
	Nod* cap = NULL;
	Nod* coada = NULL;
	// Cerinta 2
	citireListaDubluInlantuita(fileStream, &cap, &coada);
	fclose(fileStream);
	fileStream = fopen("serviciu.txt", "r"); //preluam serviciul de inserat dintr un fisier
	// Cerinta 3
	inserareLaInceput(&cap, &coada, citireServiciu(fileStream));
	fclose(fileStream);
	// afisare ( se observa lista citita la cerinta 2 ,adaugandu se serviciul citit la cerinta 3 la inceputul liste;
	printf("\n\n Afisare lista ");
	traversareLista(cap);
	printf("\n\n --------------------------------- \n\n ");
	// Cerinta 4
	// Sa se returneze o lista dublu inlantuita care sa contina toate nodurile care nu indeplinesc o conditie definita la alegere.
	// initializare lista noua
	Nod* listaNoua_cap = NULL;
	Nod* listaNoua_coada = NULL;
	listaDeServiciiUrgente(&listaNoua_cap, &listaNoua_coada, cap, coada);
	printf("\n\nLista de servicii urgente");
	traversareLista(listaNoua_cap);

	// Cerinta 5
	// Sa se transforme lista dublu inlantuita intr-o lista dublu inlantuita circulara.

	Nod* coadaLC = NULL;
	Nod* capLC = NULL;
	listaDeServicii(&capLC, &coadaLC, cap, coada);
	coadaLC->next = capLC; // "fac legaturile dintre cap si coada"
	capLC->prev = coadaLC;

	// Cerinta 6
	// Se afișează toate elementele listei dublu inlantuite si ale listei dublu inlantuite circulare. (Se traverseaza cele 2 liste)
	printf("\n\n --------------------------------- \n\n ");
	printf("\n\n Afisare lista ");
	traversareLista(cap);
	printf("\n\n Afisare lista circulara");
	traversareListaCirculara(capLC);
	printf("\n\n --------------------------------- \n\n ");
	// Cerinta 7
	// Sa se converteasca elementele din lista dublu intantuita preluata din fisier in lista simplu inlantuita si sa se traverseze lista simplu inlantuita.
	NodLS* capLS = NULL;
	creeare_ListaSI_din_ListaDI(&capLS, cap);
	printf("\n\n Afisare lista simplu inlantuita ");
	traversareListaSI(capLS);
	printf("\n\n --------------------------------- \n\n ");

	// Cerinta 7
	// Sa se adauge un nod in lista dublu inlantuita circulara langa primul nod care indeplineste un criteriu ales.
	fileStream = fopen("serviciu2.txt", "r"); // preluam serviciul de adaugat dintr un fisier
	inserareDupaCriteriu(&capLC, &coadaLC, citireServiciu(fileStream));

	printf("\n\n Afisare lista circulara dupa inserare serviciu nou");
	traversareListaCirculara(capLC);
	printf("\n\n --------------------------------- \n\n ");
	fclose(fileStream);

	// Cerinta 8
	dezalocareLista(&cap, &coada);
	dezalocareLista(&listaNoua_cap, &listaNoua_coada);
	dezalocareLista(&capLC, &coadaLC);

}
//autoevaluare : 9 