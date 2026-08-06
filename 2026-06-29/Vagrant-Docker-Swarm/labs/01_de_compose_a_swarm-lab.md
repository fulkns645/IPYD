# Lab 01: De Compose a Swarm (2 VMs)

**Objetivo**
Observar en acción el cambio de modelo: de contenedores a servicios, réplicas, tasks y estado deseado. Ver routing mesh (publicación distribuida) y redes overlay.

**Duración sugerida**: 10 a 15 minutos

**Topologia**
1. VM1: `manager`
1. VM2: `worker`

En el lab usaremos el stack: `labs/01_de_compose_a_swarm-stack.yml`.

## 1) Prerequisitos

En ambas VMs:
1. Docker instalado.
1. Las VMs deben poder comunicarse entre sí por IP.

Puertos Swarm a permitir entre nodos (según tu firewall / security group):
1. `2377/tcp` (control plane, manager)
1. `7946/tcp` y `7946/udp` (gossip)
1. `4789/udp` (VXLAN overlay)

## 2) Crear el cluster Swarm

En `manager` (reemplaza `MANAGER_IP` por la IP real de la VM1):

```bash
docker swarm init --advertise-addr MANAGER_IP
```

En `manager`, obtén el comando para unir el worker:

```bash
docker swarm join-token worker
```

En `worker`, pega y ejecuta el `docker swarm join ...`.

Verificación (en `manager`):

```bash
docker node ls
```

Esperado: 2 nodos, uno `Leader` (manager) y uno `Ready` (worker).

## 3) Desplegar el stack

En `manager`, desde la raíz del repo:

```bash
docker stack deploy -c labs/01_de_compose_a_swarm-stack.yml lab01
```

Este archivo `labs/01_de_compose_a_swarm-stack.yml` contiene:
1. Un servicio `web` basado en `traefik/whoami`.
1. `deploy.replicas: 4` para ver el modelo de servicio con múltiples instancias.
1. Publicación del puerto `8080` en modo `ingress` (routing mesh), para que el servicio sea accesible desde cualquier nodo.
1. Una red `overlay` llamada `appnet` (creada como `lab01_appnet` al desplegar el stack) para comunicación entre servicios en distintos hosts.
1. `placement.max_replicas_per_node: 2` para forzar distribución entre nodos cuando hay 2 VMs.

Observa servicios y réplicas:

```bash
docker stack services lab01
docker service ls
```

Observa tasks y en que nodo cayeron:

```bash
docker service ps lab01_web
```

Esperado: `lab01_web` con `4/4` réplicas. Las tasks se distribuyen entre `manager` y `worker` (por `max_replicas_per_node: 2`).

## 4) Routing mesh (publicación distribuida)

En `manager`:

```bash
curl -s http://localhost:8080
```

En `worker` (mismo comando):

```bash
curl -s http://localhost:8080
```

Repite varias veces (en ambos nodos). El contenido de `whoami` incluye el hostname del contenedor que respondió, lo que te permite ver balanceo entre réplicas.

## 5) Overlay + DNS de servicio dentro del stack

La red se llama `lab01_appnet` (Swarm antepone el nombre del stack).

En `manager` (o `worker`):

```bash
docker network ls | grep lab01_appnet
docker run --rm --network lab01_appnet curlimages/curl:8.8.0 curl -s http://web
```

Esperado: `web` resuelve por nombre dentro de la overlay y responde.

## 6) Estado deseado y reconciliación (falla de una réplica)

1. En `manager`, mira las tasks actuales:

```bash
docker service ps lab01_web
```

1. En el nodo que tenga al menos un contenedor `lab01_web`, mata uno:

En `manager` o `worker` (según donde exista):

```bash
docker ps --filter label=com.docker.swarm.service.name=lab01_web
CID=$(docker ps -q --filter label=com.docker.swarm.service.name=lab01_web | head -n 1)
docker kill "$CID"
```

1. Vuelve a observar el servicio (en `manager`):

```bash
docker service ps lab01_web
```

Esperado: aparece una task en estado `Shutdown`/`Failed` y Swarm crea otra para volver al estado deseado (4 réplicas activas).

## 7) Escala rápida (réplicas)

En `manager`:

```bash
docker service scale lab01_web=6
docker service ps lab01_web
```

Esperado: Swarm crea nuevas tasks y las asigna a nodos disponibles.

## 8) Limpieza

En `manager`:

```bash
docker stack rm lab01
```

Salir del swarm:

En `worker`:

```bash
docker swarm leave
```

En `manager`:

```bash
docker swarm leave --force
```

## Checklist de observaciones

1. `docker service ps` muestra tasks (unidad de scheduling), no contenedores "manuales".
1. Si una réplica muere, Swarm re-crea otra para mantener el estado deseado.
1. `curl localhost:8080` funciona desde ambos nodos (routing mesh).
1. Dentro de la overlay, `web` resuelve por DNS del servicio.
