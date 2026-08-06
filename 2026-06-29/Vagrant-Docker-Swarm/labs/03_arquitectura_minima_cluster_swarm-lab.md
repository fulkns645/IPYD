# Lab 03: Arquitectura mínima del clúster Swarm (2 VMs)

**Objetivo**
Observar la arquitectura mínima de Swarm y su separación en:
1. **Plano de control** (managers): estado del clúster, scheduling, reconciliación.
1. **Plano de ejecución** (workers): descarga de imágenes y ejecución de contenedores.

También practicar lectura de `docker node ls` (STATUS, AVAILABILITY, MANAGER STATUS) y ver qué ocurre si falta capacidad o si el plano de control no está disponible.

**Duración sugerida**: 10 a 15 minutos

**Topología**
1. VM1: `manager`
1. VM2: `worker`

Usaremos el stack `labs/03_arquitectura_minima_cluster_swarm-stack.yml`.

## 1) Prerequisitos

En ambas VMs:
1. Docker instalado.
1. Conectividad por IP entre VMs.

Puertos Swarm a permitir entre nodos (según tu firewall / security group):
1. `2377/tcp`
1. `7946/tcp` y `7946/udp`
1. `4789/udp`

## 2) Crear el clúster

En `manager` (reemplaza `MANAGER_IP`):

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

Qué observar en la salida:
1. `STATUS` (por ejemplo `Ready`).
1. `AVAILABILITY` (`Active`, `Pause`, `Drain`).
1. `MANAGER STATUS` (solo para managers: `Leader` / `Reachable`).

## 3) El plano de control se administra desde managers

En `worker`, intenta ver los nodos:

```bash
docker node ls
```

Esperado: error indicando que el nodo no es manager. Esta es una forma simple de ver que:
1. El **plano de control** vive en los managers.
1. Los workers ejecutan carga, pero no toman decisiones globales.

## 4) Etiquetar el nodo de ejecución (worker)

En `manager`, etiqueta el nodo worker para fijar allí la carga del servicio:

```bash
WORKER_NODE=$(docker node ls --filter role=worker --format '{{.Hostname}}' | head -n 1)
docker node update --label-add labrole=app "$WORKER_NODE"
docker node inspect "$WORKER_NODE" --format '{{json .Spec.Labels}}'
```

## 5) Desplegar el stack y observar control vs ejecución

En `manager`, desde la raíz del repo:

```bash
docker stack deploy -c labs/03_arquitectura_minima_cluster_swarm-stack.yml lab03
```

Este stack crea un servicio `web` (2 réplicas) publicado en `:8080` y con una restricción:
1. Solo puede correr en el nodo con `node.labels.labrole == app` (el worker).

Observa el servicio y sus réplicas (estado deseado):

```bash
docker stack services lab03
docker service ls
```

Observa en qué nodo quedaron las tasks (scheduling):

```bash
docker service ps lab03_web
```

Observa contenedores reales en el plano de ejecución:

En `worker`:

```bash
docker container ls
```

En `manager`:

```bash
docker container ls
```

Esperado: los contenedores de `lab03_web` se ven en el worker (plano de ejecución). En el manager no deberían aparecer.

## 6) Comprobar acceso al servicio (dato: routing mesh)

Aunque el contenedor esté en el worker, el puerto publicado en Swarm suele ser accesible desde cualquier nodo.

En `manager`:

```bash
curl -s http://localhost:8080
```

En `worker`:

```bash
curl -s http://localhost:8080
```

## 7) Plano de ejecución: quitar capacidad (Drain) y ver el efecto

En `manager`, drena el nodo worker (deja de aceptar tasks nuevas y mueve las existentes):

```bash
docker node update --availability drain "$WORKER_NODE"
docker node ls
docker service ps lab03_web
```

Esperado:
1. Swarm intentará reubicar tasks, pero la restricción por etiqueta impide correrlas en el manager.
1. El servicio quedará degradado (no podrá mantener 2 réplicas activas).

Vuelve a activar el worker:

```bash
docker node update --availability active "$WORKER_NODE"
docker service ps lab03_web
```

## 8) (Opcional) ¿Qué pasa si el plano de control cae?

Este paso muestra la diferencia entre:
1. Contenedores ya corriendo (plano de ejecución).
1. Capacidad de orquestar cambios y reconciliar (plano de control).

1. Deja el servicio corriendo y confirma que responde:

En `worker`:

```bash
curl -s http://localhost:8080
```

1. En `manager`, detén Docker Engine temporalmente.

En sistemas con systemd:

```bash
sudo systemctl stop docker
```

Alternativa:

```bash
sudo service docker stop
```

1. Mientras el manager está apagado, fuerza una falla en el plano de ejecución (en `worker`) y observa que no hay reconciliación:

```bash
docker ps --filter label=com.docker.swarm.service.name=lab03_web
CID=$(docker ps -q --filter label=com.docker.swarm.service.name=lab03_web | head -n 1)
docker kill "$CID"

# Espera ~20s y verifica: no debería reaparecer una nueva réplica
docker ps --filter label=com.docker.swarm.service.name=lab03_web
```

Esperado:
1. El servicio puede seguir respondiendo si queda al menos 1 contenedor vivo.
1. Pero, sin plano de control, Swarm no puede tomar decisiones para volver al estado deseado (no crea una réplica nueva).

1. Enciende Docker de nuevo en `manager`:

```bash
sudo systemctl start docker
```

Alternativa:

```bash
sudo service docker start
```

Verifica recuperación (en `manager`):

```bash
docker node ls
docker service ps lab03_web
```

Verifica recuperación (en `worker`):

```bash
docker ps --filter label=com.docker.swarm.service.name=lab03_web
```

### (Opcional extra) Quórum con 2 managers (por qué se prefieren números impares)

Este extra transforma el clúster temporalmente en **2 managers** para mostrar que perder 1 implica perder mayoría.

1. En `manager`, promociona el worker a manager:

```bash
docker node promote "$WORKER_NODE"
docker node ls
```

2. Apaga Docker en el manager original (igual que arriba con `systemctl` o `service`).

3. En el nodo que quedó encendido (VM2), intenta un comando de manager:

```bash
docker node ls
```

Esperado: errores relacionados a falta de líder o quórum (el clúster queda sin mayoría con 2 managers y 1 caído).

4. Enciende Docker en VM1 y revierte (en `manager`):

```bash
docker node demote "$WORKER_NODE"
docker node ls
```

## 9) Limpieza

En `manager`:

```bash
docker stack rm lab03
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
