# Vagrant + Docker Swarm

*Fecha: 2021-08-10*

*Última actualización: 2025-05-23*

En este repositorio se encuentran los archivos que permiten levantar un pequeño clúster de máquinas virtuales con Docker instalado, y opcionalmente habilitar la funcionalidad de orquestación con Docker Swarm.

Para llevar a cabo el despliegue de este cluster se necesitan las siguientes herramientas instaladas en su computador (adicional se indica la versión donde fueron probados estos scripts y el comando para validar la disponibilidad de la herramienta en su sistema):

<table>
<tr>
<td> <b> Herramienta </b> </td> 
<td> <b> Versión </b> </td> 
<td> <b> Comando validación </b> </td> 
</tr>
<tr>
<td> Vagrant </td> 
<td> 2.2.6 </td> 
<td> <code>vagrant --version</code> </td> 
</tr>
<tr>
<td> VirtualBox </td> 
<td> 6.1.50_Ubuntur161033 </td> 
<td> <code>VBoxManage --version</code> </td> 
</tr>
</table>

Una vez clonado este repositorio en su máquina, ingrese al directorio `Vagrant-Docker-Swarm`.

## ¿Qué ofrece este repositorio?

1. Crea tres máquinas virtuales con Ubuntu (1 `manager` y 2 `workers`).
2. Instala Docker en los tres nodos de forma automática.
3. Permite habilitar o deshabilitar, de forma opcional, la configuración de Docker Swarm sobre estos nodos.

## Configuración: habilitar o deshabilitar Docker Swarm

El comportamiento del despliegue se controla mediante el archivo `vagrant-settings.yml` ubicado en la raíz del repositorio.

```yaml
enable_swarm: true
```

- Si `enable_swarm: true` (valor por defecto): se instala Docker y se configura automáticamente un clúster Docker Swarm (1 nodo `manager` y 2 nodos `worker`).
- Si `enable_swarm: false`: se instalan las máquinas virtuales con Docker, pero **no** se inicializa ni se configura Docker Swarm.

Antes de ejecutar `vagrant up` puede editar este archivo para seleccionar el comportamiento deseado.

## Cómo levantar el entorno

1. Asegúrese de tener instalados Vagrant y VirtualBox (ver tabla anterior).
2. Clone este repositorio y cambie al directorio `Vagrant-Docker-Swarm`.
3. Opcionalmente edite `vagrant-settings.yml` para activar o desactivar Docker Swarm.
4. Ejecute:

```bash
vagrant up
```

Esto creará las tres máquinas virtuales, instalará Docker en todas y, si está habilitado, configurará Docker Swarm.

Para acceder al nodo `manager`:

```bash
vagrant ssh manager
```

Si Docker Swarm está habilitado, desde el nodo `manager` puede seguir la guía oficial para desplegar stacks:

https://docs.docker.com/engine/swarm/stack-deploy/

---

## Inicialización manual de Docker Swarm (manager y workers)

Las máquinas virtuales creadas por Vagrant tienen dos interfaces de red:

1. Una interfaz NAT (por ejemplo, IP `10.2.0.15`).
2. Una interfaz privada tipo host-only, usada para la comunicación del clúster (rango `192.168.56.x`).

Para que el clúster Docker Swarm funcione correctamente, el `advertise-addr` debe apuntar a la IP de la red privada (`192.168.56.x`), no a la IP NAT.

### En el nodo manager

1. Obtener la IP de la interfaz privada (por ejemplo `enp0s8`):

```bash
ip addr show enp0s8
```

Busque una dirección en el rango `192.168.56.x` (por ejemplo `192.168.56.10`).

2. Inicializar el clúster Swarm anunciando esa IP:

```bash
docker swarm init --advertise-addr 192.168.56.10
```

También puede usar directamente la interfaz:

```bash
docker swarm init --advertise-addr enp0s8
```

El comando mostrará una línea similar a:

```bash
docker swarm join --token <TOKEN> 192.168.56.10:2377
```

Guarde esa línea para usarla en los workers.

### En los nodos worker

En cada worker (`worker-01`, `worker-02`), ejecute la línea que se mostró en el `manager`, asegurándose de que la IP sea la del rango `192.168.56.x` (no `10.x.x.x`). Por ejemplo:

```bash
docker swarm join --token <TOKEN> 192.168.56.10:2377
```

Después de esto, desde el `manager` puede verificar los nodos del clúster:

```bash
docker node ls
```

---

Es posible que se ejecute una vez la creación de las máquinas virtuales `docker swarm init` y el comando arroje la cadena de conexión para nuevos nodos. 
Si por alguna razón esa cadena se pierde, es posible pedirle a `docker` que muestre nuevamente la cadena de la siguiente manera:

```
docker swarm join-token manager
```
