#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
typedef struct Medic Medic;
typedef struct MaxHeap MaxHeap;
typedef struct Nod Nod;
struct Medic {
	int id;
	char* nume;
	int varsta;
	char* specializare;
	float salariu;
	char* cnp;
	double rating; //[1.0 - 5.0]
};
struct MaxHeap {
    Medic* vector;
    int dim;
};
struct Nod {
    Medic info;
    struct Nod* stanga;
    struct Nod* dreapta;
};

Medic citireMedic(FILE* fileStream)
{
    Medic medic;
    char buffer[100];

    fgets(buffer, 100, fileStream);
    medic.id = atoi(buffer);

    fgets(buffer, 100, fileStream);
    char* ptr = strtok(buffer, "\n");
    medic.nume = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
    strcpy(medic.nume, ptr);

    fgets(buffer, 100, fileStream);
    medic.varsta = atoi(buffer);

    fgets(buffer, 100, fileStream);
    ptr = strtok(buffer, "\n");
    medic.specializare = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
    strcpy(medic.specializare, ptr);

    fgets(buffer, 100, fileStream);
    medic.salariu = atof(buffer);

    fgets(buffer, 100, fileStream);
    ptr = strtok(buffer, "\n");
    medic.cnp = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
    strcpy(medic.cnp, ptr);

    fgets(buffer, 100, fileStream);
    medic.rating = atof(buffer);

    return medic;
}
void afisareMedic(Medic m) {
    printf("\nID: %d | %s | Varsta: %d | %s | Salariu: %.2f | CNP: %s | Rating: %.2f",
        m.id, m.nume, m.varsta, m.specializare, m.salariu, m.cnp, m.rating);
}
Medic copieMedic(Medic original) {
    Medic copie;
    copie.id = original.id;
    copie.varsta = original.varsta;
    copie.salariu = original.salariu;
    copie.rating = original.rating;

    if (original.nume != NULL) {
        copie.nume = (char*)malloc(strlen(original.nume) + 1);
        strcpy(copie.nume, original.nume);
    }
    else {
        copie.nume = NULL;
    }

    if (original.specializare != NULL) {
        copie.specializare = (char*)malloc(strlen(original.specializare) + 1);
        strcpy(copie.specializare, original.specializare);
    }
    else {
        copie.specializare = NULL;
    }

    if (original.cnp != NULL) {
        copie.cnp = (char*)malloc(strlen(original.cnp) + 1);
        strcpy(copie.cnp, original.cnp);
    }
    else {
        copie.cnp = NULL;
    }

    return copie;
}

void inserareBST(Nod** radacina, Medic medic)
{
    if (*radacina == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = medic;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        *radacina = nou;
    }
    else if (medic.id < (*radacina)->info.id)
        inserareBST(&(*radacina)->stanga, medic);
    else
        inserareBST(&(*radacina)->dreapta, medic);
}
int nrFrunze(Nod* radacina) {
    if (radacina == NULL)
        return 0;
    if (radacina->stanga == NULL && radacina->dreapta == NULL)
        return 1;
    return nrFrunze(radacina->stanga) + nrFrunze(radacina->dreapta);
}
int nrFrunzeSubarboreDrept(Nod* radacina) {
    if (radacina != NULL && radacina->dreapta != NULL)
        return nrFrunze(radacina->dreapta);
    else
        return 0;
}
void copiereBSTinHeap(Nod* radacina, MaxHeap heap, int* index)
{
    if (radacina != NULL) {
        heap.vector[*index] = copieMedic(radacina->info);
        (*index)++;
        copiereBSTinHeap(radacina->stanga, heap, index);
        copiereBSTinHeap(radacina->dreapta, heap, index);
    }
}
int numaraNoduri(Nod* radacina) {
    if (radacina == NULL)
        return 0;
    else
        return 1 + numaraNoduri(radacina->stanga) + numaraNoduri(radacina->dreapta);
}
void filtrareMaxHeap(MaxHeap heap, int index) {
    int pozMax = index;
    int pozSt = 2 * index + 1;
    int pozDr = 2 * index + 2;
    if (pozSt < heap.dim && heap.vector[pozMax].id < heap.vector[pozSt].id)
        pozMax = pozSt;
    if (pozDr < heap.dim && heap.vector[pozMax].id < heap.vector[pozDr].id)
        pozMax = pozDr;
    if (pozMax != index) {
        Medic aux = heap.vector[index];
        heap.vector[index] = heap.vector[pozMax];
        heap.vector[pozMax] = aux;
        if (pozMax <= heap.dim / 2 - 1)
            filtrareMaxHeap(heap, pozMax);
    }
}
void afisareFrunzeHeap(MaxHeap heap) {
    printf("\nFrunzele din heap sunt: ");
    for (int i = heap.dim / 2; i < heap.dim; i++) {
        afisareMedic(heap.vector[i]);
    }
}
void traversareMaxHeap(MaxHeap heap) {
    printf("\nTraversare MaxHeap: ");
    for (int i = 0; i < heap.dim; i++)
        afisareMedic(heap.vector[i]);
}
void traversareBTS_inordine(Nod* radacina) {
    if (radacina) {
        traversareBTS_inordine(radacina->stanga);
        afisareMedic(radacina->info);
        traversareBTS_inordine(radacina->dreapta);
    }
}
void stergereElementeHeap(MaxHeap* heap, double pragRating) {
    int i = 0;
    while (i < heap->dim) {
        if (heap->vector[i].rating < pragRating) {
            free(heap->vector[i].nume);
            free(heap->vector[i].specializare);
            free(heap->vector[i].cnp);

            heap->vector[i] = heap->vector[heap->dim - 1];
            heap->dim--;

            filtrareMaxHeap(*heap, i);
        }
        else i++;
    }
}
void dezalocareBST(Nod** radacina) {
    if (*radacina != NULL) {
        dezalocareBST(&((*radacina)->stanga));
        dezalocareBST(&((*radacina)->dreapta));
        free((*radacina)->info.nume);
        free((*radacina)->info.specializare);
        free((*radacina)->info.cnp);
        free(*radacina);
        *radacina = NULL;
    }
}
void dezalocareHeap(MaxHeap* heap) {
    for (int i = 0; i < heap->dim; i++) {
        free(heap->vector[i].nume);
        free(heap->vector[i].specializare);
        free(heap->vector[i].cnp);
    }
    free(heap->vector);
    heap->vector = NULL;
    heap->dim = 0;
}
void main() {
    //Cerinta 1 - strctura Medic
    FILE* fisier = fopen("medici.txt", "r");
    Nod* radacina = NULL;
    //Cerinta 2
    while (!feof(fisier))
    {
        Medic m = citireMedic(fisier);
        inserareBST(&radacina, m);
    }
    fclose(fisier);
    //Cerinta 3
    printf("\nNr frunze subarbore drept: %d", nrFrunzeSubarboreDrept(radacina));
    //Cerinta 4
    MaxHeap heap;
    heap.dim = numaraNoduri(radacina);
    heap.vector = (Medic*)malloc(sizeof(Medic) * heap.dim);
    int index = 0;
    copiereBSTinHeap(radacina, heap, &index);
    //Cerinta 5
    for (int i = heap.dim / 2 - 1; i >= 0; i--)
        filtrareMaxHeap(heap, i);
    //Cerinta 6
    afisareFrunzeHeap(heap);
    //Cerinta 7
    traversareMaxHeap(heap);
    printf("\nTraversare BTS in Inordine:");
    traversareBTS_inordine(radacina);
    //Cerinta 8
    printf("\nStergere elemente cu rating < 4.5:\n");
    stergereElementeHeap(&heap, 4.5);
    traversareMaxHeap(heap);
    //Cerinta 9
    dezalocareBST(&radacina);
    dezalocareHeap(&heap);
}
//AutoEvaluare: 9