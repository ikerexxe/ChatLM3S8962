/****************************************************************************************/
*																						 *
/										README											 /
*																						 *
/****************************************************************************************/

	EL PROGRAMA:
	
	Sirve para chatear entre la placa Stellaris EKS-LM3S8962 y un PC, usando los cuatro
	botones de posici�n para seleccionar el texto a enviar y el bot�n select para
	enviarlo.
	Tambi�n se usa el display para ver lo que quiere enviar el usuario y para poder ver
	los �ltimos mensajes intercambiados. Al recibir un nuevo mensaje se reproduce un tono
	de alerta.
	El requisito principal del programa es que solo se pueden enviar y recibir mensajes
	con una longitud igual o inferior a 10 caracteres. A la hora de enviar no se muestran
	los caracteres hasta haber recibido $, que se usa como caracter de fin de comunicaci�n.
	
/****************************************************************************************/

	ORGANIZACI�N DEL PROGRAMA:
	
	El proyecto consta de un fichero principal llamado "chat.c" y su fichero de 
	cabecera "chat.h". Las funciones correspondientes al sonido (al PWM,"sonido.c"), 
	al teclado ("keypad.c"), al display ("display.c"), al led("displayLED") al clock 
	("clock.c"), han sido definidas en distintos ficheros, cada uno con sus respectivos
	nombres y ficheros de cabecera,	de manera que pueden ser reemplazadas f�cilmente en
	el caso de que el HW utilizado no sea exactamnete el mismo. 
	Por otra parte, el fichero "display.c" hace uso del Frame Buffer definido en "frame-
	buffer.c" para hacer mejor uso de la pantalla. Este Frame Buffer es gen�rico, es
	decir, no est� especificado para ning�n display en concreto, por lo tanto, hace uso
	del fichero "displayRIT.c" para utilizar las funciones espec�ficas del display
	RIT128x94x4.
	Finalmente, la comunicaci�n se hace mediante el fichero "comunicacion.c" que usa el
	archivo "uartDrv.c", el cual dispone de interrupciones y de buffers de software para
	no saturar el hardware propio del hardware. Este driver est� creado espec�ficamente
	para la Stellaris, por lo que en caso de cambio de placa habr� que crear uno nuevo.
	