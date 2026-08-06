# Lab 04: Crear el Swarm y desplegar un servicio (2 VMs)

Basado en `slides/04_laboratorio_01_crear_swarm_desplegar_servicio.html`.

**Objetivo**
Crear un clúster Swarm mínimo, declarar un **servicio** HTTP, observar `service -> task -> container`, escalar réplicas y validar acceso por puerto publicado.

**Duración sugerida**: 15 minutos

**Topología**
1. VM1: `manager`
1. VM2: `worker`

## 1) Prerrequisitos

En ambas VMs:
1. Docker instalado.
1. Conectividad por IP entre VMs.
1. Acceso a Internet para descargar `nginx:alpine` (o imagen ya cacheada).

Puertos Swarm a permitir entre nodos (según tu firewall / security group):
1. `2377/tcp`
1. `7946/tcp` y `7946/udp`
1. `4789/udp`

## 2) Paso 1: inicializar el Swarm (manager)

En `manager` (reemplaza `MANAGER_IP` por la IP alcanzable desde la VM2):

```bash
docker swarm init --advertise-addr MANAGER_IP
```

Si no hay múltiples interfaces, también funciona:

```bash
docker swarm init
```

## 3) Paso 2: unir el worker

En `manager`, obtén el comando para unir workers:

```bash
docker swarm join-token worker
```

En `worker`, pega y ejecuta el `docker swarm join ...`.

Verifica el estado del clúster (en `manager`):

```bash
docker node ls
```

Qué observar:
1. `STATUS` en `Ready`.
1. `AVAILABILITY` en `Active`.
1. `MANAGER STATUS` en `Leader` para el manager.

## 4) Paso 3: crear un servicio web

En `manager`:

```bash
docker service create \
  --name web \
  -p 8080:80 \
  nginx:alpine
```

## 5) Paso 4: observar servicio, tasks y contenedores

En `manager`:

```bash
docker service ls
docker service ps web
```

En cada nodo, observa los contenedores locales:

En `manager`:

```bash
docker container ls
```

En `worker`:

```bash
docker container ls
```

Qué observar:
1. `docker service ps web` muestra las **tasks** y el nodo asignado.
1. `docker container ls` solo muestra contenedores del nodo donde ejecutas el comando.

## 6) Paso 5: escalar el servicio a 3 réplicas

En `manager`:

```bash
docker service scale web=3
docker service ls
docker service ps web
```

Qué observar:
1. El servicio `web` pasa a `3/3` réplicas.
1. Las tasks se distribuyen entre nodos (según capacidad/decisiones del scheduler).

## 7) Paso 6: validar acceso al servicio

En `manager`:

```bash
curl -s http://localhost:8080 | head
```

En `worker` (mismo comando):

```bash
curl -s http://localhost:8080 | head
```

También puedes probar desde tu máquina host (si tienes reachability):
1. `http://IP_DEL_MANAGER:8080`
1. `http://IP_DEL_WORKER:8080`

Pregunta guía:
1. Con 3 réplicas, ¿estás accediendo a un contenedor específico o al servicio publicado?

## 8) Checklist

1. El Swarm quedó inicializado y el worker se unió correctamente.
1. `docker node ls` muestra ambos nodos en `Ready`.
1. Se creó el servicio `web` con `nginx:alpine`.
1. `docker service ls` muestra réplicas y el puerto `*:8080->80/tcp`.
1. `docker service ps web` muestra tasks y ubicación.
1. El servicio se escaló a 3 réplicas.
1. `curl http://localhost:8080` responde en ambos nodos.

## 9) Limpieza

En `manager`:

```bash
docker service rm web
```

Salir del Swarm:

En `worker`:

```bash
docker swarm leave
```

En `manager`:

```bash
docker swarm leave --force
```
