#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct Serviciu Serviciu;
typedef struct Nod Nod;

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

void inserareLaSfarsit(Nod** cap, Serviciu info)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->serviciu = clonaServiciu(info); //pentru a evita shallow copy
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
	printf("\nAfisare Serviciu de %s :",serviciu.denumire);
	printf("\nDepartament: %s",serviciu.departament);
	printf("\nDurata: %d minute",serviciu.durata);
	printf("\nPret: %.2f",serviciu.pret);
	printf("\nCod: %d",serviciu.id);
	printf((serviciu.esteUrgent ? "\nEste urgent" : "\nNu este urgent"));
	printf("\nTermen Limita: %s\n",serviciu.termenLimita);

}

void citireVectorServicii(FILE* file, Serviciu** vectorServicii, int* nrServicii)
{
	if (file == NULL) return;
	while (!feof(file)) {
		*vectorServicii = (Serviciu*)realloc(*vectorServicii, ((*nrServicii) + 1) * sizeof(Serviciu));
		(*vectorServicii)[*nrServicii] = citireServiciu(file);
		(*nrServicii)++;
	}
}

void afisareVector(int nr, Serviciu* lista)
{
	for (int i = 0; i < nr; i++)
	{
		afisareServiciu(lista[i]);
		printf("\n");
	}
}

void traversareLista(Nod* cap)
{
	while (cap != NULL)
	{
		afisareServiciu(cap->serviciu);
		cap = cap->next;
	}
}
void dezalocareServiciu(Serviciu serviciu)
{
	free(serviciu.denumire);
	free(serviciu.departament);
	free(serviciu.termenLimita);
}
void stergerePrimulElement(Nod** cap)
{
	if ((*cap) != NULL) {
		Nod* temp = *cap;
		*cap = temp->next;
		dezalocareServiciu(temp->serviciu);
		free(temp);
	}
}

void sortareLista(Nod** cap)
{
	//Bubble Sort
	if (!((*cap) == NULL || (*cap)->next == NULL)) {
		int sortat = 1;

		do {
			sortat = 1;
			Nod* temp = *cap;

			while (temp->next != NULL)
			{
				if (temp->serviciu.id > temp->next->serviciu.id)
				{
					Serviciu copie = temp->serviciu;
					temp->serviciu = temp->next->serviciu;
					temp->next->serviciu = copie;

					sortat = 0;
				}
				temp = temp->next;
			}

		} while (!sortat);
	}
}

void stergereDupaID(Nod** cap, int _id)
{
	if ((*cap) != NULL)
	{
		if ((*cap)->serviciu.id == _id)
		{
			stergerePrimulElement(cap);
		}
		else
		{
			Nod* temp = *cap;
			while (temp->next != NULL)
			{
				if(temp->next->serviciu.id == _id)
				{
					stergerePrimulElement(&(temp->next));
				}else
					temp = temp->next;
			}
		}
	}
}

void main()
{
	FILE* fileStream = fopen("listaDeServicii.txt", "r");
	Serviciu* listaServicii = NULL;
	int nrServicii = 0;
	citireVectorServicii(fileStream, &listaServicii, &nrServicii);
	afisareVector(nrServicii, listaServicii);

	//3. Inserare la sfarsit in lista simplu inlantuita.
	Nod* cap = NULL;
	inserareLaSfarsit(&cap, listaServicii[0]);
	inserareLaSfarsit(&cap, listaServicii[1]);
	inserareLaSfarsit(&cap, listaServicii[2]);

	//4. Ștergere primului element din lista simplu inlantuita.
	stergerePrimulElement(&cap);
	inserareLaSfarsit(&cap, listaServicii[0]);

	//5. Sa se sorteze lista. Se alege un tip de sortare studiat in primul an de facultate.
	sortareLista(&cap);
	traversareLista(cap);

	//7. Ștergere element in functie de un id definit la nivelul informatiei utile. 
	// Exemplu: Se sterge nodul cu id-ul 4.(id ul face parte din informatia utila) 
	stergereDupaID(&cap, 51101);

	//6. Se afișează toate elementele listei. (Se traverseaza lista) 
	traversareLista(cap);

	fclose(fileStream);
}