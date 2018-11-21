/*Pinger, por Juan Ureña.
El programa recibe una lista de parámetros 
e intentara hacer ping a cada uno de ellos. 
El ping enviará un unico mensaje y esperará
5 segundos antes de indicar como no recibido
el mensaje de respuesta. Solo acabara bien
nuestro programa (0) si todos los ping han
obtenido respuesta correctamente, en caso
contrario, acabra fallido (1)*/

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

enum{
//Elementos minimos necesarios de entrada
Elems_Min=2
};

//Esperar hijos y comprobar que todos han acabado bien 
int wait_child (int num_child)
{
	int status;
	int result=0;
	//Variable para controlar que acaben bien 
	for(int i=0;i<num_child;i++){ //espero el numero de hijos 
    	wait(&status);
    	if WIFEXITED(status){
    		if (WEXITSTATUS(status)){
    		//Si ambos son distintos de cero, algo ha fallado, 
    		//por lo que asigno un numero de error. 
    			result=1;
    		}
		} 
	}
	return result;
}

void ping( char *word) 
{ 
    execl("/bin/ping", "/bin/ping", "-c", "1", "-W", "5", word, NULL); 
    err(1, NULL);
} 

int main(int argc, char *argv[])
{ 
	int failed=0;
	if (argc<Elems_Min){
	//Si no se indica ningún elemento al que hacer ping,
	//indico un error y acabo como fallo. 
		warnx("Se debe introducir al menos un elemento al que hacer ping");
		exit(EXIT_FAILURE);
	}
    for(int i=1;i<argc;i++){ 
        if(fork() == 0){
        	//Desde los hijos hago ping y termino simepre bien, 
        	//controlare posibles salidas mal desde el padre
			ping(argv[i]); 
           	exit(0);
        } 
    }
    failed=wait_child(argc-1);
    //Salgo con failed, que sera 0 si todo ha ido bien
    //1 si algun ping ha falaldo.
	exit(failed);
} 
