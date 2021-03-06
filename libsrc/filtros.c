/** \file filtros.c
* \brief Biblioteca de filtros para textos.
* Aplicados a archivos y cuyo resultado se enviará a la salida estándar
* \author Francisco Maya Sarasty
* \date 19 de diciembre de 2015
**/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "filtros.h"

/// Función auxiliar impresión de conteos
/**
* Permite imprimir los conteos realizados en las funciones para que puedan ser manejados
* como caracteres
* \param num Número que será convertido a caracter
* \return Impresión por salida estándar
**/
void printstr(int num)
{
	int aux, i;
	char *nstr;

        aux = 1;
        for( i = 1; i < num; i*=10 ) aux++;
        nstr = (char *)malloc( aux + 2 );
        sprintf(nstr, "%d ", num);
        write( 1, nstr, strlen( nstr ) );
	free(nstr);
}

/// Función que muestra las tres primeras líneas leídas
/**
* Este filtro muestra las tres primeras líneas leídas, las cuales deben estar separadas por el caracter
* salto de línea "\n"
* \see fs_wc() Filtro contador de caracteres, palabras y líneas de entrada
* \see fs_nl() Filtro que numera las líneas de la entrada
* \see fs_cut() Filtro que muestra la palabra que se encuentra en la posición dada
* \param fd Descriptor de archivo
* \return Impresión por salida estándar
**/
void fs_head( int fd )
{
	int count = 0;
	char c;
	
	while( read( fd, &c, 1 ) && (count < 3) ){
		write( 1, &c, 1 );
		if ( c == '\n' ) count++;
	}
}

/// Función que cuenta el número de caracteres, palabras y líneas de entrada
/**
* Permite contar el número de caracteres, palabras y líneas de la entrada y lo muestra por pantalla,
* se considera como separadores de palabras los caracteres salto de línea "\n", tabulador "\t", y 
* espacio en blanco " " y como separador de líneas solo el salto de línea "\n".
* \see printstr() Función utilizada internamente para impresión
* \see fs_head() Filtro impresión tres primeras líneas
* \see fs_nl() Filtro que numera las líneas de la entrada
* \see fs_cut() Filtro que muestra la palabra que se encuentra en la posición dada
* \param fd Descriptor de archivo
* \return Impresión por salida estándar
**/
void fs_wc( int fd )
{
	int nl = 0, np = 0, nc = 0, sep = 0;
	char c;
	
	lseek ( fd, 0, SEEK_SET );
	while( read( fd, &c, 1 ) ){
		nc++;
		switch( c ){
			case '\n': nl++;
			case '\t':
			case ' ': if( !sep ) np++;
				sep = 1;
				break;
			default: sep = 0;
		}
	 }
	printstr( nc );
	printstr( np );
	printstr( nl );
	c = 10;
	write( 1, &c, 1 );
}

/// Función que numera las líneas de la entrada
/**
* Permite conocer el número de cada línea del archivo de entrada
* \see printstr() Función utilizada internamente para impresión
* \see fs_head() Filtro impresión tres primeras líneas
* \see fs_wc() Filtro contador de caracteres, palabras y líneas de entrada
* \see fs_cut() Filtro que muestra la palabra que se encuentra en la posición dada
* \param fd Descriptor de archivo
* \return Impresión por salida estándar
**/
void fs_nl( int fd )
{
	int nl = 1, new = 1;
	char c;

	lseek ( fd, 0, SEEK_SET );
	while( read( fd, &c, 1) ){
		if( new ){
			printstr( nl );
			nl++;
			new = 0;
		}
		if ( c == '\n' ) new = 1;
		write( 1, &c, 1 );		
	}
}

/// Función que muestra una palabra por línea dada determinada posición númerica
/**
* Muestra la palabra en la posición determinada por "col" de cada línea.
* \see fs_head() Filtro impresión tres primeras líneas
* \see fs_wc() Filtro contador de caracteres, palabras y líneas de entrada
* \see fs_nl() Filtro que numera las líneas de la entrada
* \param fd Descriptor de archivo
* \param col Número de palabra a buscar en cada línea del archivo
* \return Impresión por salida estándar 
**/
void fs_cut( int fd, int col )
{
	char c;
	int pal = 1, nc = 0, sep = 0;

	lseek ( fd, 0, SEEK_SET );
	while( read( fd, &c, 1 ) ){
		if ( c == '\n' ){
			if((pal >= col)) write( 1, &c, 1 );
			if((pal < col)) write( 1, "Esta linea no tiene esa cantidad de palabras\n", 46 );
			pal = 1;
			nc = 0;
		} 
		else if ( pal == col ){ nc++; write( 1, &c, 1 );}
		
		if ( (c == ' ') || (c == '\t') ){
			if(!sep){
				pal++;
			}
			sep = 1;
		}
		
		if ( (c != ' ') && (c != '\t') && (c != '\n') ){
			sep = 0;
		}
	}
}

