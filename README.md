# RP2_P1_MQTT

Repositorio para el desarrollo memoria de la práctica 1 de la asignatura RP2.

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

Una vez finalizado el proceso de subscripción, el publicante envía al subscrito las publicaciones relativas a los temas pertinentes.


**Ejercicio. Explicar los segmentos TCP y tramas ARP que se observan.**





 
 
 
        
 
