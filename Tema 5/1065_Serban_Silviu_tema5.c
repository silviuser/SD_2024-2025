#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
typedef struct Soft Soft;
typedef struct NodAVL NodAVL;
typedef struct NodLista NodLista;
typedef struct ListaDubla ListaDubla;
typedef struct Nod Nod;
struct Soft {
    int id;
    char* denumire;
    char* versiune;
    char* producator;
    int anLansare;
    int nrUtilizatori;
    float marimeGB;
};
struct NodPrincipal {
    Soft info;
    NodPrincipal* next;
    NodSecundar* vecini;
};

struct NodSecundar {
    NodPrincipal* info;
    NodSecundar* next;
};

struct NodAVL {
	Soft info;
	struct NodAVL* stanga;
	struct NodAVL* dreapta;
};

struct NodLista {
	Soft info;
	struct NodLista* next;
};

struct Nod {
	int ID;
	struct Nod* next;
	struct Nod* prev;
};

struct ListaDubla {
	struct Nod* first;
	struct Nod* last;
};

void inserareListaPrincipala(NodPrincipal** cap, Soft soft) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = soft;
	nou->next = NULL;
	nou->vecini = NULL;
	NodPrincipal* aux = *cap;
	if (*cap) {
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}
void inserareListaSecundara(NodSecundar** cap, NodPrincipal* info)
{
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->info = info;
	nou->next = NULL;
	if (*cap)
	{
		NodSecundar* aux = *cap;
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		*cap = nou;
	}
}
NodPrincipal* cautareNodDupaId(NodPrincipal* graf, int id) {
	while (graf != NULL && id != graf->info.id) {
		graf = graf->next;
	}
	return graf;
}
void adaugaMuchie(NodPrincipal* graf, int id1, int id2) {
	NodPrincipal* nod1 = cautareNodDupaId(graf, id1);
	NodPrincipal* nod2 = cautareNodDupaId(graf, id2);
	if (nod1 != NULL && nod2 != NULL) {
		inserareListaSecundara(&nod1->vecini, nod2);
		inserareListaSecundara(&nod2->vecini, nod1);
	}
}
void citireFisier(NodPrincipal** graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return;
	}

	char linie[256];
	fgets(linie, sizeof(linie), f);
	int nrNoduri = atoi(linie);

	for (int i = 0; i < nrNoduri; i++) {
		fgets(linie, sizeof(linie), f);
		char* token = strtok(linie, "|");

		Soft s;
		s.id = atoi(token);

		token = strtok(NULL, "|");
		s.denumire = (char*)malloc(strlen(token) + 1);
		strcpy(s.denumire, token);

		token = strtok(NULL, "|");
		s.versiune = (char*)malloc(strlen(token) + 1);
		strcpy(s.versiune, token);

		token = strtok(NULL, "|");
		s.producator = (char*)malloc(strlen(token) + 1);
		strcpy(s.producator, token);

		token = strtok(NULL, "|");
		s.anLansare = atoi(token);

		token = strtok(NULL, "|");
		s.nrUtilizatori = atoi(token);

		token = strtok(NULL, "\n");
		s.marimeGB = atof(token);

		inserareListaPrincipala(graf, s);
	}
	fgets(linie, sizeof(linie), f);
	int nrMuchii = atoi(linie);

	for (int i = 0; i < nrMuchii; i++) {
		fgets(linie, sizeof(linie), f);
		int id1 = atoi(strtok(linie, " "));
		int id2 = atoi(strtok(NULL, "\n"));
		adaugaMuchie(*graf, id1, id2);
	}
	fclose(f);
}
void afisareSoft(Soft s) {
	printf("ID: %d | %s %s | %s | An: %d | Utilizatori: %d | Marime: %.2f GB\n",
		s.id, s.denumire, s.versiune, s.producator, s.anLansare, s.nrUtilizatori, s.marimeGB);
}

void afisareGraf(NodPrincipal* graf) {
	while (graf != NULL) {
		afisareSoft(graf->info);

		NodSecundar* vecini = graf->vecini;
		printf("  Vecini: ");
		while (vecini) {
			printf("[ID: %d, %s] ", vecini->info->info.id, vecini->info->info.denumire);
			vecini = vecini->next;
		}

		printf("\n\n");
		graf = graf->next;
	}
}
float medieMarimeSofturi(NodPrincipal* graf) {
	float suma = 0;
	int nr = 0;
	while (graf != NULL) {
		suma += graf->info.marimeGB;
		nr++;
		graf = graf->next;
	}
	if (nr == 0)
		return 0;
	return suma / nr;
}

int inaltimeArbore(NodAVL* radacina) {
	if (radacina != NULL) {
		int inaltimeSt = inaltimeArbore(radacina->stanga);
		int inaltimeDr = inaltimeArbore(radacina->dreapta);
		if (inaltimeSt > inaltimeDr) {
			return 1 + inaltimeSt;
		}
		else {
			return 1 + inaltimeDr;
		}
	}
	else {
		return 0;
	}
}

int diferentaInaltimiSubarbori(NodAVL* radacina) {
	if (radacina != NULL) {
		return inaltimeArbore(radacina->stanga) - inaltimeArbore(radacina->dreapta);
	}
	else {
		return 0;
	}
}

void rotireLaDreapta(NodAVL** radacina) {
	NodAVL* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = *radacina;
	*radacina = aux;
}

void rotireLaStanga(NodAVL** radacina) {
	NodAVL* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = *radacina;
	*radacina = aux;
}
void inserareAVL(NodAVL** radacina, Soft s) {
	if (*radacina == NULL) {
		NodAVL* nou = (NodAVL*)malloc(sizeof(NodAVL));
		nou->info = s;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*radacina = nou;
	}
	else if (s.id < (*radacina)->info.id) {
		inserareAVL(&(*radacina)->stanga, s);
	}
	else {
		inserareAVL(&(*radacina)->dreapta, s);
	}

	int diferentaInaltimi = diferentaInaltimiSubarbori(*radacina);
	if (diferentaInaltimi == 2) {
		if (diferentaInaltimiSubarbori((*radacina)->stanga) == -1) {
			rotireLaStanga(&(*radacina)->stanga);
		}
		rotireLaDreapta(radacina);
	}
	if (diferentaInaltimi == -2) {
		if (diferentaInaltimiSubarbori((*radacina)->dreapta) == 1) {
			rotireLaDreapta(&(*radacina)->dreapta);
		}
		rotireLaStanga(radacina);
	}
}
void parcurgereInordineAVL(NodAVL* radacina) {
	if (radacina) {
		parcurgereInordineAVL(radacina->stanga);
		printf("Soft AVL: ID=%d, %s\n", radacina->info.id, radacina->info.denumire);
		parcurgereInordineAVL(radacina->dreapta);
	}
}
void conversieGrafInAVL(NodPrincipal* graf, NodAVL** radacinaAVL) {
	while (graf) {
		Soft copie;
		copie.id = graf->info.id;

		copie.denumire = (char*)malloc(strlen(graf->info.denumire) + 1);
		strcpy(copie.denumire, graf->info.denumire);

		copie.versiune = (char*)malloc(strlen(graf->info.versiune) + 1);
		strcpy(copie.versiune, graf->info.versiune);

		copie.producator = (char*)malloc(strlen(graf->info.producator) + 1);
		strcpy(copie.producator, graf->info.producator);

		copie.anLansare = graf->info.anLansare;
		copie.nrUtilizatori = graf->info.nrUtilizatori;
		copie.marimeGB = graf->info.marimeGB;

		inserareAVL(radacinaAVL, copie);
		graf = graf->next;
	}
}

void inserareInLista(NodLista** cap, Soft s) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info.id = s.id;

	nou->info.denumire = (char*)malloc(strlen(s.denumire) + 1);
	strcpy(nou->info.denumire, s.denumire);

	nou->info.versiune = (char*)malloc(strlen(s.versiune) + 1);
	strcpy(nou->info.versiune, s.versiune);

	nou->info.producator = (char*)malloc(strlen(s.producator) + 1);
	strcpy(nou->info.producator, s.producator);

	nou->info.anLansare = s.anLansare;
	nou->info.nrUtilizatori = s.nrUtilizatori;
	nou->info.marimeGB = s.marimeGB;

	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodLista* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}
void extragereSofturiPesteUnMilion(NodPrincipal* graf, NodLista** lista) {
	while (graf) {
		if (graf->info.nrUtilizatori > 1000000) {
			inserareInLista(lista, graf->info);
		}
		graf = graf->next;
	}
}

void afisareLista(NodLista* cap) {
	printf("\n=== Lista softuri cu peste 1 milion de utilizatori ===\n");
	while (cap) {
		afisareSoft(cap->info);
		cap = cap->next;
	}
}

float sumaMarimeGB_AVL(NodAVL* radacina) {
	if (radacina == NULL)
		return 0;
	float suma = 0;
	if (radacina->info.nrUtilizatori > 1000000 && radacina->info.anLansare > 2021) {
		suma += radacina->info.marimeGB;
	}
	suma += sumaMarimeGB_AVL(radacina->stanga);
	suma += sumaMarimeGB_AVL(radacina->dreapta);
	return suma;
}

void put(ListaDubla* ld, int ID) {
	struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
	nou->ID = ID;
	nou->next = NULL;
	nou->prev = ld->last;

	if (ld->last == NULL) {
		ld->first = ld->last = nou;
	}
	else {
		ld->last->next = nou;
		ld->last = nou;
	}
}

int pop(ListaDubla* ld) {
	int ID = -1;
	if (ld->first) {
		ID = ld->first->ID;
		struct Nod* aux = ld->first;
		ld->first = ld->first->next;
		if (ld->first == NULL)
			ld->last = NULL;
		else
			ld->first->prev = NULL;
		free(aux);
	}
	return ID;
}


void parcurgereInLatime(NodPrincipal* graf, int idStart) {
	int dimensiune = 6;
	int* vectorVizitate = (int*)malloc(dimensiune * sizeof(int));
	for (int i = 0; i < dimensiune; i++) {
		vectorVizitate[i] = 0;
	}

	ListaDubla coada;
	coada.first = coada.last = NULL;
	put(&coada, idStart);
	vectorVizitate[idStart - 1] = 1;

	while (coada.first) {
		int idExtras = pop(&coada);
		NodPrincipal* nodExtras = cautareNodDupaId(graf, idExtras);
		afisareSoft(nodExtras->info);

		NodSecundar* vecini = nodExtras->vecini;
		while (vecini) {
			int idVecin = vecini->info->info.id;
			if (vectorVizitate[idVecin - 1] == 0) {
				put(&coada, idVecin);
				vectorVizitate[idVecin - 1] = 1;
			}
			vecini = vecini->next;
		}
	}

	free(vectorVizitate);
}
void dezalocareListaVecini(NodSecundar* vecin) {
	while (vecin) {
		NodSecundar* temp = vecin;
		vecin = vecin->next;
		free(temp);
	}
}

void dezalocareGraf(NodPrincipal** cap) {
	while (*cap) {
		NodPrincipal* temp = *cap;
		*cap = (*cap)->next;

		free(temp->info.denumire);
		free(temp->info.versiune);
		free(temp->info.producator);

		dezalocareListaVecini(temp->vecini);
		free(temp);
	}
}
void dezalocareAVL(NodAVL** rad) {
	if (*rad) {
		dezalocareAVL(&(*rad)->stanga);
		dezalocareAVL(&(*rad)->dreapta);

		free((*rad)->info.denumire);
		free((*rad)->info.versiune);
		free((*rad)->info.producator);

		free(*rad);
	}
}
void dezalocareListaFiltrata(NodLista** cap) {
	while (*cap) {
		NodLista* temp = *cap;
		*cap = (*cap)->next;

		free(temp->info.denumire);
		free(temp->info.versiune);
		free(temp->info.producator);

		free(temp);
	}
}
void dezalocareCoada(ListaDubla* coada) {
	while (coada->first) {
		struct Nod* temp = coada->first;
		coada->first = coada->first->next;
		free(temp);
	}
	coada->last = NULL;
}


void main() {
	NodPrincipal* graf = NULL;
	printf("=== CERINTA 2 ===\n");
	citireFisier(&graf, "softuri.txt");

	printf("\n=== GRAF DE SOFTURI ===\n");
	afisareGraf(graf);

	printf("\n=== CERINTA 3  ===\n");
	float medie = medieMarimeSofturi(graf);
	printf("Media marimii softurilor: %.2f GB\n", medie);

	printf("\n=== CERINTA 4 ===\n");
	NodAVL* radacinaAVL = NULL;
	conversieGrafInAVL(graf, &radacinaAVL);
	parcurgereInordineAVL(radacinaAVL);

	printf("\n=== CERINTA 5 ===\n");
	NodLista* lista = NULL;
	extragereSofturiPesteUnMilion(graf, &lista);
	afisareLista(lista);

	printf("\n=== CERINTA 6 ===\n");
	float suma = sumaMarimeGB_AVL(radacinaAVL);
	printf("Suma marimilor GB (peste 1 mil utilizatori & dupa 2021): %.2f GB\n", suma);

	printf("\n=== CERINTA 7 ===\n");
	printf("-> Traversare completa graf:\n");
	afisareGraf(graf);

	printf("\n-> Parcurgere in latime graf:\n");
	parcurgereInLatime(graf, 1);

	printf("\n-> Parcurgere inordine arbore AVL:\n");
	parcurgereInordineAVL(radacinaAVL);
	
	//CERINTA 9
	dezalocareGraf(&graf);
	dezalocareAVL(&radacinaAVL);
	dezalocareListaFiltrata(&lista);
}