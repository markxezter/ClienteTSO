/********************************************************
        UNIVERSIDAD AUTONOMA DEL ESTADO DE MEXICO
        Tipos de Sistemas Operativos
        Programa: fi-cliente.c
        Desc    : Cliente transaccional

**********************************************************/

/*********************************************************
DeclaraciÃ³n de librerÃ­as
**********************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*********************************************************
DeclaraciÃ³n de constantes
**********************************************************/
#define WHITE    "\e[m"
#define RED      "\e[31m"
#define B_RED    "\e[31;1m"
#define GREEN    "\e[32m"
#define B_GREEN  "\e[32;1m"
#define YELLOW   "\e[33m"
#define B_YELLOW "\e[33;1m"
#define BLUE     "\e[34m"
#define B_BLUE   "\e[34;1m"
#define MAGENTA  "\e[35m"
#define CYAN     "\e[36m"
#define BRIGHT   "\e[1m"
#define IP       "192.168.1.75"

/*********************************************************
DeclaraciÃ³n de prototipos
**********************************************************/
void menuPrincipal();
void menuATM(char cuenta[],char nip[],char nom[]);
int validaNum(char a[]);
int noVacio(char a[]);
char getch();
void transaccionesTres(char cve[],char tarjeta[],char nip[]);
void transaccionesCuatro(char cve[],char tarjeta[],char nip[],char cant[]);
int productos();
/********************************************************
Variables globales
*********************************************************/
int sockfd,n;
struct sockaddr_in servaddr,cliaddr;
char sendline[10000];
char recvline[10000];
char mensaje[50];

int opc,opc2,opc3,lo,val1,val2,suma=0,acum = 0,retiro,saldo;
int valC,vaC,valN,vaN;
char nvo_nip[30],nvo_nip2[30],cant[10];
char nombre[30],disponer[20],cuenta[30],nip[30];
char c;
/*********************************************************
Programa principal
**********************************************************/
int main(int argc, char**argv)
{

   sockfd = socket(AF_INET,SOCK_STREAM,0);
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(IP);
   servaddr.sin_port = htons (32000);
   connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   menuPrincipal();
}

/********************************************************************
Menu Principal
*********************************************************************/
void menuPrincipal(){
	char canti[20];
	do{
		printf(B_GREEN"\n");
		system("clear");
		printf("\n\t*************************************************************");
		printf("\n\t*\tUniversidad AutÃ³noma del Estado de MÃ©xico           *");
		printf("\n\t*\t         Facultad de IngenierÃ­a                     *");
		printf("\n\t*\t       IngenierÃ­a en ComputaciÃ³n                    *");
		printf("\n\t*\t      Tipos de Sistemas Operativos                  *");
		printf("\n\t*************************************************************");
		printf("\n\n\t\t\tMenÃº Principal");
		printf("\n\n\t1.-Transacciones ATM\n\t2.-Compra POS\n\t3.-Compra en Internet\n\t4.-Acerca de\n\t5.-Salir");
		printf("\n\n\tElige una opciÃ³n: ");
		
		printf(B_BLUE);								///////////////////
		scanf("%d",&opc);
		printf(B_GREEN);							//////////////////
		getchar();
		
		switch(opc){
			case 1:
				do{
					printf("\n\tIngresa tu nÃºmero de tarjeta: ");
					printf(B_BLUE);								///////////////////
					scanf("%s",&cuenta);
					printf(B_GREEN);							//////////////////
					vaC = noVacio(cuenta);
					valC = validaNum(cuenta);
				}while(valC<0 || vaC<0);
				
				do{
					salto:
					printf("\n\tIngresa NIP: ");
					int i = 0;
					printf(B_BLUE);								//////////////////
					while((c = getch())!='\n'){
						nip[i] = c;
						printf("*");
						i++;
					}
					printf(B_GREEN);							//////////////////
					if(strlen(nip)!=4){
						memset(nip,'\0',30);
						goto salto;
					}
					vaN = noVacio(nip);
					valN = validaNum(nip);
				}while(valN<0 || vaN<0);
				
				transaccionesTres("CNOM",cuenta,nip);
      			getchar();
				if(strlen(recvline)==1){
					printf(B_BLUE);									//////////////////
					printf("\n\n\tNo. Tarjeta/NIP Incorrectos");
					printf(B_GREEN);								//////////////////
					getchar();
					break;
				}else{
					menuATM(cuenta,nip,recvline);	
				}					
				break;
			case 2:
				do{
					printf("\n\tIngresa tu nÃºmero de tarjeta: ");
					printf(B_BLUE);									//////////////////
					scanf("%s",&cuenta);
					printf(B_GREEN);								//////////////////
					vaC = noVacio(cuenta);
					valC = validaNum(cuenta);
				}while(valC<0 || vaC<0);
				do{
					salto3:
					printf("\n\tIngresa NIP: ");
					int i = 0;
					printf(B_BLUE);									//////////////////
					while((c = getch())!='\n'){
						nip[i] = c;
						printf("*");
						i++;
					}
					printf(B_GREEN);								//////////////////
					if(strlen(nip)!=4){
						memset(nip,'\0',30);
						goto salto3;
					}
					vaN = noVacio(nip);
					valN = validaNum(nip);
				}while(valN<0 || vaN<0);
				
				transaccionesTres("CNOM",cuenta,nip);
      			getchar();
				
				if(strlen(recvline)==1){
					printf(B_BLUE);									//////////////////
					printf("\n\n\tNo. Tarjeta/NIP Incorrectos");
					printf(B_GREEN);								//////////////////
					getchar();
					break;
				}else{
					do{
						printf("\n\tCantidad a pagar: ");
						printf(B_BLUE);									//////////////////
						scanf("%s",&disponer);
						printf(B_GREEN);								//////////////////
						valN = noVacio(disponer);
						vaN = validaNum(disponer);
					}while(valN<0 || vaN<0);
					retiro  = atoi(disponer);
					transaccionesTres("CONS",cuenta,nip);
					saldo = atoi(recvline);	
					if(retiro>saldo){
						printf(B_BLUE);									//////////////////
						printf("\n\tNo cuentas con saldo suficiente    Saldo disponible($ %d)",saldo);
						printf(B_GREEN);								//////////////////
						getchar();
						getchar();
						break;
					}else{
						transaccionesCuatro("CPOS",cuenta,nip,disponer);
						if(strcmp(recvline,"0")==0){
							printf(B_BLUE);									//////////////////
							printf("\n\tCompra realizada con Ã©xito");
							printf(B_GREEN);								//////////////////
							getchar();
						}else{
							printf(B_BLUE);									//////////////////
							printf("\n\tError al realizar compra");
							printf(B_GREEN);								//////////////////
							getchar();
						}
					}
				}	
				getchar();
				break;
			case 3:
				do{	
					system("clear");			
					printf("\n\n\t\t\tMenÃº de compra");
					printf("\n\n\t\t1.-Agregar producto\n\t\t2.-Hacer compra\n\t\t3.-Cancelar compra\n\t\t4.-Regresar");
					printf("\n\t\t\t\t\t\tTotal: ");
					printf(B_BLUE);										//////////////////
					printf("$ %d",suma);
					printf(B_GREEN);									//////////////////
					printf("\n\n\tDigita tu opciÃ³n: ");
					
					printf(B_BLUE);										//////////////////
					scanf("%d",&opc2);
					printf(B_GREEN);									//////////////////
				
					switch(opc2){
						case 1:
							acum = productos();
							suma = suma + acum;
							break;
						case 2:
							if(suma==0){
								printf(B_BLUE);										//////////////////
								printf("\n\tNo ha elegido productos para comprar");
								printf(B_GREEN);									//////////////////
								getchar();
							}else{
								do{
									printf("\n\tIngresa tu nÃºmero de tarjeta: ");
									printf(B_BLUE);										//////////////////
									scanf("%s",&cuenta);
									printf(B_GREEN);									//////////////////
									vaC = noVacio(cuenta);
									valC = validaNum(cuenta);
								}while(valC<0 || vaC<0);
								do{
									etq1:
									printf("\n\tIngresa NIP: ");
									int i = 0;
									printf(B_BLUE);										//////////////////
									while((c = getch())!='\n'){
										nip[i] = c;
										printf("*");
										i++;
									}
									printf(B_GREEN);									//////////////////
									if(strlen(nip)!=4){
										memset(nip,'\0',30);
										goto etq1;
									}
									vaN = noVacio(nip);
									valN = validaNum(nip);
								}while(valN<0 || vaN<0);
							
								transaccionesTres("CNOM",cuenta,nip);
      							getchar();
				
								if(strlen(recvline)==1){
									printf(B_BLUE);										//////////////////
									printf("\n\n\tNo. Tarjeta/NIP Incorrectos");
									printf(B_GREEN);									//////////////////
									getchar();
									break;
								}else{
									transaccionesTres("CONS",cuenta,nip);
									saldo = atoi(recvline);
									if(suma>saldo){
										printf(B_BLUE);										//////////////////
										printf("\n\n\tNo cuentas con saldo suficiente    	Saldo disponible($ %d)",saldo);
										printf(B_GREEN);									//////////////////
										suma = 0;
										getchar();
										break;
									}else{
										sprintf(canti,"%d",suma);
										transaccionesCuatro("CINT",cuenta,nip,canti);
										if(strcmp(recvline,"0")==0){
											printf(B_BLUE);										//////////////////
											printf("\n\n\tCompra realizada con Ã©xito");
											printf(B_GREEN);									//////////////////
										}else{
											printf(B_BLUE);										//////////////////
											printf("\n\n\tError al realizar compra");
											printf(B_GREEN);									//////////////////
										}
										suma = 0;
									}
								}
							}
							getchar();
							break;
						case 3:
							printf(B_BLUE);											//////////////////
							printf("\n\tLa compra ha sido cancelada");
							printf(B_GREEN);										//////////////////
							suma = 0;
							getchar();
							getchar();
							break;
						case 4:
							suma = 0;
							break;
					}
				}while(opc2!=4);
				break;
			case 4:
				system("clear");
				printf("\n\n\tSimular las operacion transaccional de ATM/POS/WEB\n\n\tUtilizando Bases de Datos y Sockets de ComunicaciÃ³n");
				printf("\n\n\t\tVersiÃ³n 2.0.1 Release");
				printf("\n\n\t\t\tAutores:\n\n\t\tGabriela PiÃ±a Remigio\n\n\t\tAlberto Cerro Camilo\n\n\t\tMarcos Reynoso GonzÃ¡lez");
				printf("\n\n\t\t\t2015");
				printf("\n\n\tRegresar");
				getchar();
				break;
			case 5:
				system("clear");
				strcpy(sendline,"TERMINAR");
				sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
				close(sockfd);
				exit(1);
				break;
		}
	}while(opc!=5);
}

/***************************************************************
MenÃº ATM
****************************************************************/
void menuATM(char cuenta[],char nip[],char nom[]){
	strcpy(nombre,nom);
	
	do{
		printf(B_BLUE"\n");								////////////////
		system("clear");
		printf("\n\tBienvenido(a): ");
		printf(B_GREEN"%s",nombre);						////////////////
		printf(B_BLUE);									////////////////
		printf("\n\n\t\t\tMenÃº ATM");
		printf("\n\n\t1.-Consultar saldo\n\t2.-Cambiar NIP\n\t3.-DisposiciÃ³n\n\t4.-Compra de tiempo aire\n\t5.-Consultar movimientos\n\t6.-Regresar");
		printf("\n\n\tElige una opciÃ³n: ");
		
		printf(B_GREEN);								////////////////
		scanf("%d",&opc3);
		getchar();
		printf(B_BLUE);									////////////////
		
		switch(opc3){
			case 1:
				transaccionesTres("CONS",cuenta,nip);
     			printf("\n\tSu saldo total es: ");
     			printf(B_GREEN"$");						////////////////
      			fputs(recvline,stdout);
      			printf(B_BLUE);							///////////////
				getchar();
				break;
			case 2:
				do{
					salto:
					printf("\n\tIngresa nuevo NIP: ");
					int i = 0;
					printf(B_GREEN);					////////////////
					while((c = getch())!='\n'){
						nvo_nip[i] = c;
						printf("*");
						i++;
					}
					printf(B_BLUE);						////////////////
					if(strlen(nvo_nip)!=4){
						memset(nvo_nip,'\0',30);
						goto salto;
					}
					val1 = noVacio(nvo_nip);
					val2 = validaNum(nvo_nip);
				}while(val1<0 || val2<0);
				
				do{
					salto2:
					printf("\n\tConfirmar el nuevo NIP: ");
					int j = 0;
					printf(B_GREEN);					////////////////
					while((c = getch())!='\n'){
						nvo_nip2[j] = c;
						printf("*");
						j++;
					}
					printf(B_BLUE);						////////////////
					if(strlen(nvo_nip2)!=4){
						memset(nvo_nip2,'\0',30);
						goto salto2;
					}
					val1 = noVacio(nvo_nip2);
					val2 = validaNum(nvo_nip2);
				}while(val1<0 || val2<0);
				
				if(strcmp(nvo_nip,nvo_nip2)!=0){
					printf(B_GREEN);							//////////////////
					printf("\n\n\tConfirmaciÃ³n errÃ³nea");
					printf(B_BLUE);								/////////////////
					goto salto;
				}else{
					transaccionesTres("CMBN",cuenta,nvo_nip);
     				printf("\n\n\t"B_GREEN);					//////////////////
      				fputs(recvline,stdout);
      				printf(B_BLUE);								/////////////////
					strcpy(nip,nvo_nip);
					getchar();
				}
				break;
			case 3:
				do{
					printf("\n\tCantidad que desea retirar: ");
					printf(B_GREEN);							//////////////////
					scanf("%s",&disponer);
					printf(B_BLUE);								/////////////////
					val1 = noVacio(disponer);
					val2 = validaNum(disponer);
				}while(val1<0 || val2<0);
				retiro  = atoi(disponer);
				transaccionesTres("CONS",cuenta,nip);
				saldo = atoi(recvline);
				
				if(retiro > saldo){
					printf(B_GREEN);							//////////////////
					printf("\n\tNo cuentas con saldo suficiente    Saldo disponible($ %d)",saldo);
					printf(B_BLUE);								/////////////////
					getchar();
					getchar();
					break;
				}else{
					transaccionesTres("CCAJ",cuenta,nip);
					if(retiro > atoi(recvline)){
						printf(B_GREEN);							//////////////////
						printf("\n\tEl cajero no tiene dinero suficiente\n\tElegir una cantidad menor");
						printf(B_BLUE);								/////////////////
						getchar();
					}else{
						transaccionesCuatro("DISP",cuenta,nip,disponer);
						if(strcmp(recvline,"0")==0){
							printf(B_GREEN);							//////////////////
							printf("\n\tDisposiciÃ³n realizada conn Ã©xito");
							printf(B_BLUE);								/////////////////
							getchar();
						}else{
							printf(B_GREEN);							//////////////////
							printf("\n\tDisposiciÃ³n fallida");
							printf(B_BLUE);								/////////////////
							getchar();
						}
					}
				}
				getchar();
				break;
			case 4:
				do{
					printf("\n\tIntroduce la cantidad de la recarga: ");
					printf(B_GREEN);									//////////////////
					scanf("%s",&disponer);
					printf(B_BLUE);										/////////////////
					val1 = noVacio(disponer);
					val2 = validaNum(disponer);
				}while(val1<0 || val2<0);
				retiro  = atoi(disponer);
				transaccionesTres("CONS",cuenta,nip);
				saldo = atoi(recvline);
				
				if(retiro>saldo){
					printf(B_GREEN);									//////////////////
					printf("\n\tNo cuentas con saldo suficiente    Saldo disponible($ %d)",saldo);
					printf(B_BLUE);										/////////////////
					getchar();
					getchar();
					break;
				}else{
					transaccionesCuatro("CTIA",cuenta,nip,disponer);
					if(strcmp(recvline,"0")==0){
						printf(B_GREEN);									//////////////////
						printf("\n\tRecarga realizada con Ã©xito");
						printf(B_BLUE);										/////////////////
						getchar();
					}else{
						printf(B_GREEN);									//////////////////
						printf("\n\tError al realizar la recarga");
						printf(B_BLUE);										/////////////////
						getchar();
					}
				}
				getchar();
				break;
			case 5:
				transaccionesTres("CMOV",cuenta,nip);
				if(strlen(recvline)==1){
					printf(B_GREEN);									//////////////////
					printf("\n\tSin moviemientos");
					printf(B_BLUE);										/////////////////
					getchar();
					break;
				}else{
					printf(B_GREEN);									//////////////////
					printf("\n\tFecha      Tipo                Monto\n");
					printf(B_BLUE);										/////////////////
					fputs(recvline,stdout);
					getchar();
				}	
				break;
			case 6:
				memset(cuenta,'\0',30);
				memset(nip,'\0',30);
				memset(recvline,'\0',1000);
				memset(sendline,'\0',1000);
				break;
		}	
	}while(opc3!=6);	
}

/**************************************************************
MÃ³dulo que valida la entrada de nÃºmeros
***************************************************************/
int validaNum(char Cad[]){
    int lon, i = 0, ver;
    lon = strlen(Cad);
    while (i < lon) {
        ver = isdigit(Cad[i]);
        if (ver != NULL) {
            i++;
        } else {
            return (-1);
            break;
        }
    }
    return (1);
}

/***************************************************************
MÃ³dulo que vÃ¡lida cadenas vacÃ­as 
****************************************************************/
int noVacio(char a[]){
    int w,x;
    w = strlen(a);
    if (w == 0) {
        x = -1;
    } else {
        x =  1;
    }
    return x;
}

/*****************************************************************
Modulo para leer caracter por caracter
******************************************************************/
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }
 
 /******************************************************************************
 Modulo para consultar saldo, cambio NIP, consulta movimientos y consulta nombre
 *******************************************************************************/
 void transaccionesTres(char cve[],char tarjeta[],char nip[]){
 	//memset(recvline,'\0',1000);
 	//memset(sendline,'\0',1000);
 	strcpy(sendline,cve);
 	strcat(sendline,"|");
	strcat(sendline,tarjeta);
	strcat(sendline,"|");
	strcat(sendline,nip);
	strcat(sendline,"|");
	sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	n = recvfrom(sockfd,recvline,1000,0,NULL,NULL);
    recvline[n] = 0;
 } 
 /******************************************************************************
 Modulo para disposiciÃ³n, compra POS, compra por internet, compra tiempo aire
 *******************************************************************************/
 void transaccionesCuatro(char cve[],char tarjeta[],char nip[],char cant[]){
 	//memset(recvline,'\0',1000);
 	//memset(sendline,'\0',1000);
 	strcpy(sendline,cve);
 	strcat(sendline,"|");
	strcat(sendline,tarjeta);
	strcat(sendline,"|");
	strcat(sendline,nip);
	strcat(sendline,"|");
	strcat(sendline,cant);
	strcat(sendline,"|");
	sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	n = recvfrom(sockfd,recvline,1000,0,NULL,NULL);
    recvline[n] = 0;
 }
 
 int productos(){
 	char opcion[10];
 	int acumulado = 0, num,op;
 	char cantidad[10];
 	system("clear");
 	printf("\n\t\tProductos existentes");
 	printf(B_BLUE);											//////////////////
	printf("\n\n\tProducto\t\tPrecio");
	printf(B_GREEN);										//////////////////
	printf("\n\n\t1.-Guitarra\t\t$1500");
	printf("\n\t2.-Piano\t\t$5000");
	printf("\n\t3.-ViolÃ­n\t\t$1000");
	printf("\n\t4.-BaterÃ­a\t\t$3000");
	
	etq:
	do{
		printf("\n\n\tProducto a comprar: ");
		printf(B_BLUE);											//////////////////
		scanf("%s",&opcion);
		printf(B_GREEN);										//////////////////
		val1 = noVacio(opcion);
		val2 = validaNum(opcion);
	}while(val1<0 || val2<0);
	if(atoi(opcion)>4){
		goto etq;
	}
	op = atoi(opcion);
	
	do{	
		etq2:
		printf("\n\tCantidad de producto a comprar: ");
		printf(B_BLUE);											//////////////////
		scanf("%s",&cantidad);
		printf(B_GREEN);										//////////////////
		val1 = noVacio(cantidad);
		val2 = validaNum(cantidad);
	}while(val1<0 || val2<0);
	num = atoi(cantidad);
	if(num<=0){
		printf(B_BLUE);											//////////////////
		printf("\n\tCantidad no valida");
		printf(B_GREEN);										//////////////////
		getchar();
		goto etq2;
	}else{
		switch(op){
			case 1:
				acumulado = (num * 1500);
				break;
			case 2: 
				acumulado = (num * 5000);
				break;
			case 3: 
				acumulado = (num * 1000);
				break;
			case 4:
				acumulado = (num * 3000);
				break;
			default:
				printf(B_BLUE);											//////////////////
				printf("\n\tOpciÃ³n no vÃ¡lida (1-4)");
				printf(B_GREEN);										//////////////////
				getchar();
				break;
		}
	}
	return acumulado;
 }
