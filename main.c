#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <inttypes.h>

data_structure *linie_noua (char *s)
{
	data_structure *elementnou=malloc(sizeof(data_structure));
	elementnou->header=malloc(sizeof(head));
	char *p=strtok(NULL," ");
	//retinem tipul
	unsigned char tip=atoi(p);
	elementnou->header->type=tip;
	p=strtok(NULL," ");
	//scoatem cele 2 nume si cele 2 intr-uri
	char *nume1=malloc(strlen(p)+1);
	strcpy(nume1,p);
	p=strtok(NULL," ");
	int numar1=atoi(p);
	p=strtok(NULL," ");
	int numar2=atoi(p);
	p=strtok(NULL," ");
	char *nume2=malloc(strlen(p)+1);
	strcpy(nume2,p);
	int nr_bytes=strlen(nume1)+1+strlen(nume2)+1;\
	//avem 2 int_8, deci 2 bytes ocupati
	if(tip==1)
		nr_bytes+=2;
	//un int_16 si un int_32, deci 6 bytes ocupati
	if(tip==2)
		nr_bytes+=6;
	//2 int_32, deci 8 bytes ocupati
	if(tip==3)
		nr_bytes+=8;
	elementnou->data=malloc(nr_bytes);
	elementnou->header->len=nr_bytes;
	nr_bytes=0;
	//introducem nume1,numar1,numar2,nume2 in elementnou->data
	memcpy(elementnou->data+nr_bytes,nume1,strlen(nume1)+1);
	nr_bytes+=strlen(nume1)+1;
	if(tip==1)
	{
		memcpy(elementnou->data+nr_bytes,&numar1,1);
		nr_bytes+=1;
		memcpy(elementnou->data+nr_bytes,&numar2,1);
		nr_bytes+=1;
	}
	if(tip==2)
	{
		memcpy(elementnou->data+nr_bytes,&numar1,2);
		nr_bytes+=2;
		memcpy(elementnou->data+nr_bytes,&numar2,4);
		nr_bytes+=4;
	}
	if(tip==3)
	{
		memcpy(elementnou->data+nr_bytes,&numar1,4);
		nr_bytes+=4;
		memcpy(elementnou->data+nr_bytes,&numar2,4);
		nr_bytes+=4;
	}
	memcpy(elementnou->data+nr_bytes,nume2,strlen(nume2)+1);
	nr_bytes+=strlen(nume2)+1;
	free(nume1);
	free(nume2);
	return elementnou;
}

int add_last(void **arr, int *len, data_structure *data)
{
	int i,nr_bytes=0;
	//calculam numarul de bytes ocupati de elementele din arr
	for(i=0;i<*(len);i++)
	{
		unsigned char tip= *((unsigned char*)(*arr+nr_bytes));
		nr_bytes+=sizeof(head);
		nr_bytes+=strlen((char*)(*arr+nr_bytes))+1;
		if(tip==1)
			nr_bytes+=2;
		if(tip==2)
			nr_bytes+=6;
		if(tip==3)
			nr_bytes+=8;
		nr_bytes+=strlen((char*)(*arr+nr_bytes))+1;

	}
	//realocam memorie lui arr ca sa introducem un nou element
	*arr=realloc(*arr,nr_bytes+data->header->len+sizeof(head));
	//introducem header
	memcpy(*arr+nr_bytes,data->header,sizeof(head));
	nr_bytes+=sizeof(head);
	//introducem continut
	memcpy(*arr+nr_bytes,data->data,data->header->len);
	nr_bytes+=data->header->len;
	//eliberam memoria din data
	free(data->header);
	free(data->data);
	free(data);
	return 0;
}

void count_bytes(void **arr,int *nr_bytes, int *nr_bytes1, int *nr_bytes2, int index, int i)
{
	//in nr_bytes retinem nr total de bytes ocupati in arr
	//in nr_bytes1 nr de bytes ocupati pana la pozitia index
	//in nr_bytes2 nr de bytes ocupati pana la poz index inclusiv
	unsigned char tip= *((unsigned char*)(*arr+*(nr_bytes)));
	(*nr_bytes)+=sizeof(head);
	(*nr_bytes)+=strlen((char*)(*arr+*(nr_bytes)))+1;
	if(i<index)
    {
        (*nr_bytes1)+=sizeof(head);
        (*nr_bytes1)+=strlen((char*)(*arr+*(nr_bytes1)))+1;
    }
    if(i==index)
    {
        (*nr_bytes2)=(*nr_bytes1);
        (*nr_bytes2)+=sizeof(head);
        (*nr_bytes2)+=strlen((char*)(*arr+*(nr_bytes2)))+1;
    }
	if(tip==1)
    {
        (*nr_bytes)+=2;
        if(i<index)
            (*nr_bytes1)+=2;
        if(i==index)
            (*nr_bytes2)+=2;
    }
	if(tip==2)
    {
		(*nr_bytes)+=6;
		if(i<index)
			(*nr_bytes1)+=6;
		if(i==index)
			(*nr_bytes2)+=6;
	}
	if(tip==3)
	{
		(*nr_bytes)+=8;
		if(i<index)
			(*nr_bytes1)+=8;
		if(i==index)
			(*nr_bytes2)+=8;
	}
	(*nr_bytes)+=strlen((char*)(*arr+*(nr_bytes)))+1;
	if(i<index)
		(*nr_bytes1)+=strlen((char*)(*arr+*(nr_bytes1)))+1;
	if(i==index)
		(*nr_bytes2)+=strlen((char*)(*arr+*(nr_bytes2)))+1;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
    int i,nr_bytes=0,nr_bytes1=0,nr_bytes2=0;
    if(index<0)
    	return -1;
	for(i=0;i<*(len);i++)
		count_bytes(*(&arr),&nr_bytes,&nr_bytes1,&nr_bytes2,index,i);
	*arr=realloc(*arr,nr_bytes+data->header->len+sizeof(head));
	if(index>=(*len))
	{
		memcpy(*arr+nr_bytes,data->header,sizeof(head));
		nr_bytes+=sizeof(head);
		memcpy(*arr+nr_bytes,data->data,data->header->len);
		nr_bytes+=data->header->len;
	}
	else
	{
		int l=nr_bytes-nr_bytes1;
		//mutam elementele de la poz index la dreapta cu cati bytes contine noul element
		memcpy(*arr+nr_bytes1+data->header->len+sizeof(head),*arr+nr_bytes1,l);
		memcpy(*arr+nr_bytes1,data->header,sizeof(head));
		nr_bytes1+=sizeof(head);
		memcpy(*arr+nr_bytes1,data->data,data->header->len);
		nr_bytes1+=data->header->len;
	}
	free(data->header);
	free(data->data);
	free(data);
	return 0;
}

void print2(void *data_block, int *nr_bytes)
{
	//printam elementele de pe o anumita pozitie
	printf("Tipul ");
	unsigned char tip= *((unsigned char*)(data_block+*(nr_bytes)));
	printf("%u\n", tip);
	(*nr_bytes)+=sizeof(head);
	int l=strlen((char*)(data_block+*(nr_bytes)))+1;
	char *nume1=malloc(l);
	strcpy(nume1,(char*)(data_block+*(nr_bytes)));
	printf("%s pentru ",nume1);
	(*nr_bytes)+=strlen((char*)(data_block+*(nr_bytes)))+1;
	int8_t numar1_8,numar2_8;
	int16_t numar1_16;
	int32_t numar1_32, numar2_32;
	if(tip==1)
	{
		numar1_8 = *((int8_t*)(data_block+*(nr_bytes)));
		(*nr_bytes)+=1;
		numar2_8 = *((int8_t*)(data_block+*(nr_bytes)));
		(*nr_bytes)+=1;
	}
	if(tip==2)
	{
		numar1_16=*((int16_t*)(data_block+*(nr_bytes)));
		(*nr_bytes)+=2;
		numar2_32=*((int32_t*)(data_block+*(nr_bytes)));
		(*nr_bytes)+=4;
	}
	if(tip==3)
	{
		numar1_32=*((int32_t*)(data_block+*(nr_bytes)));
		(*nr_bytes)+=4;
		numar2_32=*((int32_t*)(data_block+*(nr_bytes)));
		(*nr_bytes)+=4;
	}
	free(nume1);
	l=strlen((char*)(data_block+*(nr_bytes)))+1;
	nume1=malloc(l);
	strcpy(nume1,(char*)(data_block+*(nr_bytes)));
	printf("%s",nume1);
	free(nume1);
	(*nr_bytes)+=strlen((char*)(data_block+*(nr_bytes)))+1;
	if(tip==1)
	{
		printf("%"PRId8"\n",numar1_8);
		printf("%"PRId8"\n",numar2_8);
		printf("\n");
	}
	if(tip==2)
	{
		printf("%"PRId16"\n",numar1_16);
		printf("%"PRId32"\n",numar2_32);
		printf("\n");
	}
	if(tip==3)
	{
		printf("%"PRId32"\n",numar1_32);
		printf("%"PRId32"\n",numar2_32);
		printf("\n");
	}	
}

void find(void *data_block, int len, int index)
{
	if(index<0||index>len)
		return;
	int i,nr_bytes=0;
	//calculam nr de bytes ocupati pana la poz index
	//ca sa stim de unde incepe elementul de pe poz index
	for(i=0;i<=index-1;i++)
	{
		unsigned char tip= *((unsigned char*)(data_block+nr_bytes));
		nr_bytes+=sizeof(head);
		nr_bytes+=strlen((char*)(data_block+nr_bytes))+1;
		if(tip==1)
			nr_bytes+=2;
		if(tip==2)
			nr_bytes+=6;
		if(tip==3)
			nr_bytes+=8;
		nr_bytes+=strlen((char*)(data_block+nr_bytes))+1;
	}
	//afisam elementul dorit
	print2(data_block,&nr_bytes);
}

int delete_at(void **arr, int *len, int index)
{
	int i,nr_bytes=0,nr_bytes1=0,nr_bytes2=0;
	for(i=0;i<*(len);i++)
		count_bytes(*(&arr),&nr_bytes,&nr_bytes1,&nr_bytes2,index,i);
	//mutam elementele de la pozitia index+1 incolo la stanga
	//suprascriind ce este pe pozitia index
	memcpy(*arr+nr_bytes1,*arr+nr_bytes2,nr_bytes-nr_bytes2);
	//realocam memoria, scazand nr de bytes ocupati de elementul tocmai sters
	*arr=realloc(*arr,nr_bytes-(nr_bytes2-nr_bytes1));
	return 0;
}

void print(void *arr, int len)
{
	int i,nr_bytes=0;
	for(i=0;i<len;i++)
		print2(arr,&nr_bytes);
}

int main()
{
	void *arr = NULL;
	int len = 0;
	char *s;
	s=malloc(256);
	int contor=1;
	while(contor==1)
	{
		fgets(s,256,stdin);
		if(strcmp(s,"exit\n")==0)
		{
		    contor=0;
		    free(arr);
		}
		else
			if(strcmp(s,"print\n")==0)
				print(arr,len);
		else
        {
            char *comanda=strtok(s," ");
            if(strcmp(comanda,"insert")==0)
            {
                data_structure *newelement=linie_noua(comanda);
                int ok=add_last(&arr,&len,newelement);
				if(ok==0)
					len++;
            }
            if(strcmp(comanda,"insert_at")==0)
            {
                comanda=strtok(NULL," ");
                int index=atoi(comanda);
                data_structure *newelement=linie_noua(comanda);
                int ok=add_at(&arr,&len,newelement,index);
				if(ok==0)
					len++;

            }
            if(strcmp(comanda,"find")==0)
            {
            	comanda=strtok(NULL," ");
                int index=atoi(comanda);
                find(arr,len,index);
			}
			if(strcmp(comanda,"delete_at")==0)
			{
				comanda=strtok(NULL," ");
                int index=atoi(comanda);
                int ok=delete_at(&arr,&len,index);
				if(ok==0)
					len--;
			}
        }
	}
	free(s);
	return 0;
}
