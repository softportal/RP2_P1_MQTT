# RP2_P1_MQTT

## Alumnos

Sergio Semedi Barranco

Lucas Segarra Fernández


##  Capturando tráfico

Para comprobar el correcto funcionamiento del broker, podemos suscribirnos utilizando los
siguientes comandos:

  * Nos suscribimos en la Ci40: 
  
        mosquitto_sub -h <ip_publisher> -p 1883 -t ’topic1’

  * Publicamos desde la máquina virtual: 
  
        mosquitto_pub -h <ip_publisher> -p 1883 -t ’topic1’ -m ’hello’
        
 Para capturar y analizar el tráfico generado, utilizamos las herramientas tcpdump y wireshark respectivamente. La llamada concreta desde la máquina virtual es la siguiente:
 
        tcpdump -i interface -w file
        
 La salida obtenida se encuentra en [este repositorio](https://github.com/softportal/Bin/blob/master/broker.out).
 
 **Ejercicio. Describir los mensajes MQTT que se observan.**  
 
 Abriendo con wireshark el fichero generado con tcpdump, y aplicando el filtro *mqtt*, observamos los siguientes paquetes:
 
 |No.|Time|Source|Destination|Protocol|Length|Info|
 |:--|:--:|:--:|:--:|:--|:--|:--:|
 12|1.788921|192.168.1.91|192.168.1.1|MQTT|101|Connect Command|
 14|1.789125|192.168.1.1|192.168.1.91|MQTT|70|Connect Ack|
 16|1.792978|192.168.1.91|192.168.1.1|MQTT|79|Subscribe Request|
 17|1.793027|192.168.1.1|192.168.1.91|MQTT|71|Subscribe Ack|
 33|16.310729|192.168.1.1|192.168.1.91|MQTT|81|Publish Message|
 
Como aclaración, la IP terminada en 91 pertenece a la ci40, y la que termina en 1 a la MV (publisher).
 
Como observamos, cuando la máquina comienza a publicar no se produce tráfico alguno, porque sabe que nadie está suscrito. 
 
Es al suscribirnos desde la ci40 cuando ésta envía el mensaje de conexión que el publicante confirma (mensajes 12 y 14). Una vez ha recibido la confirmación, el dispositivo suscrito informa al publicante de cuales son los topics a los que está suscrito. En la información relativa al protocolo MQTT del paquete 16 observamos existe un único campo llamado *Suscribe Request* con los siguientes datos:

|Field|Data|
|:--|:--:|
1000 0010 = Header Flags|0x82 (Subscribe Request)
Msg Len|11
Message Identifier|1
Topic|topic1
.... ..00 = Granted Qos|Fire and Forget (0)

Como observamos en la cabecera MQTT de la confirmación (*Suscribe ACK*), el identificador sirve para determinar qué subscripción se está confirmando.

|Field|Data|
|:--|:--:|
1001 0000 = Header Flags|0x90 (Subscribe Ack)
Msg Len|3
Message Identifier|1
.... ..00 = Granted Qos|Fire and Forget (0)

Una vez finalizado el proceso de subscripción, el publicante envía al subscrito las publicaciones relativas a los temas pertinentes, como podemos apreciar en el paquete 33.


**Ejercicio. Explicar los segmentos TCP y tramas ARP que se observan.**

Aunque los protocolos MQTT y SSH funcionan sobre el protocolo de transporte TCP, entendemos que el objetivo de este ejercicio es analizar el resto de paquetes que se intercambian utilizando este protocolo.

![tcp frames](https://raw.githubusercontent.com/softportal/RP2_P1_MQTT/master/tabla1.png)

Al estar conectados por SSH la MV y el dispositivo periférico, por el mismo puerto que hemos escuchado con tcpdump, hemos capturado también los paquetes derivados de tal comunicación.

Aplicando el filtro *!ssh*, eliminamos tales paquetes de la visualización en wireshark.

Sin embargo, los dos primeros y el último segmento TCP que observamos, paquetes 5, 8 y 36 corresponden precisamente con confirmaciones a los paquetes SSH 4, 7 y 35 respectivamente, como podemos comprobar con el número de secuencia y longitud de estos paquetes SSH, y el de confirmación del TCP que los sucede.

Los paquetes 9,10 y 11, corresponden con el establecimiento de sesión clásico de TCP (3 vías).

Los paquetes 12-18 corresponden al establecimiento de sesión MQTT. Produciéndose envíos y confirmaciones a dos capas de red distintas (App y Transporte).

Así la conexión MQTT solicitada en el paquete 12, es confirmada a nivel TCP en el 13 y a nivel MQTT en el 14, dicha confirmación MQTT es a su vez confirmada a nivel TCP en el paquete 15. En el paquete 17 en cambio se confirma a ambos niveles la subscripción (al topic1) solicitada en el 16. Dicha confirmación tambien es confirmada a nivel TCP en el 18.

Los paquetes 33 y 34, son la publicación y confirmación del mensaje hello en el topic1.

Los únicos paquetes ARP que hemos capturado:

![ARP frames](https://raw.githubusercontent.com/softportal/RP2_P1_MQTT/master/tabla2.png)
  
Corresponden a la MV preguntando por la MAC de la ci40, y a la respuesta a dicha pregunta.

**En esta práctica se propone realizar un programa que publique la temperatura del sensor a través de MQTT. Para ello solicitaremos la ip y el topic por consola y realizaremos la publicación desde la Ci40.**

**Para comprobar el correcto funcionamiento nos suscribiremos desde un cliente MQTT (la
misma máquina virtual) y estudiaremos el tráfico generado con wireshark.**

La siguiente imagen corresponde con una captura de pantalla de wireshark con la topología propuesta.

![captura mqtt ci40](https://raw.githubusercontent.com/softportal/RP2_P1_MQTT/master/picture.png)

El fichero obtenido se encuentra en este repositorio.

Como podemos observar la comunicación MQTT es análoga a la estudiada en el ejercicio anterior. Como requisito de este ejercico se publica la temperatura detectada a través del sensor desde la ci40.

Observamos el contenido de la capa MQTT del paquete 181, que contiene un único campo publish mesaje con la siguiente información:

|Field|Data|
|:--|:--:|
0011 0000 = Header Flags|0x30 (Publish Message)
Msg Len|14
Topic|topic1
Message|27.312

![publish message](https://raw.githubusercontent.com/softportal/RP2_P1_MQTT/master/app_pic.png)












 
 
 
        
 
