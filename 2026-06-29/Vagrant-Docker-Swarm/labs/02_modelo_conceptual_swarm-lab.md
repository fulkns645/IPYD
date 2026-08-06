# Lab 02: Modelo Conceptual de Swarm (2 VMs)

**Objetivo**
Ver en acción el vocabulario mínimo de Swarm y cómo se relaciona:
1. `node` (manager/worker)
1. `service` (estado deseado)
1. `task` (unidad que el scheduler asigna a un nodo)
1. `container` (ejecución concreta en un nodo)

También observar el ciclo de **reconciliación**: si el estado real se aleja del deseado, Swarm actúa para corregirlo.

**Duración sugerida**: 15 minutos

**Topología**
1. VM1: `manager`
1. VM2: `worker`

Usaremos el stack `labs/02_modelo_conceptual_swarm-stack.yml`.

## 1) Prerequisitos

En ambas VMs:
1. Docker instalado.
1. Conectividad por IP entre VMs.

Puertos Swarm a permitir entre nodos (según tu firewall / security group):
1. `2377/tcp`
1. `7946/tcp` y `7946/udp`
1. `4789/udp`

## 2) Crear el cluster (manager/worker)

En `manager` (reemplaza `MANAGER_IP`):

```bash
docker swarm init --advertise-addr MANAGER_IP
```

En `manager`, obtén el comando para unir un worker:

```bash
docker swarm join-token worker
```

En `worker`, pega y ejecuta el `docker swarm join ...`.

Verificación (en `manager`):

```bash
docker node ls
```

Qué observar:
1. Rol `Manager` vs `Worker`.
1. Estado `Ready`.

## 3) Desplegar un servicio (service -> tasks -> containers)

En `manager`, desde la raíz del repo:

```bash
docker stack deploy -c labs/02_modelo_conceptual_swarm-stack.yml lab02
```

Observa el **service** y sus réplicas (estado deseado vs real):

```bash
docker service ls
docker stack services lab02
```

Observa las **tasks** del servicio (planificación/scheduling):

```bash
docker service ps lab02_web
```

Qué observar:
1. Un solo servicio: `lab02_web`.
1. Varias tasks: `lab02_web.1`, `lab02_web.2`, ...
1. Cada task está asignada a un nodo específico.

## 4) Contenedores: vista local por nodo

En `manager`:

```bash
docker container ls
```

En `worker`:

```bash
docker container ls
```

Qué observar:
1. `docker container ls` solo muestra lo que corre en el nodo actual.
1. Para ver el servicio distribuido, el comando correcto es `docker service ps` (en el manager).

## 5) Estado deseado: escalar (service scale)

En `manager` (aumenta el estado deseado a 5 réplicas):

```bash
docker service scale lab02_web=5
docker service ps lab02_web
```

Qué observar:
1. El manager crea nuevas tasks hasta llegar a 5.
1. Las tasks se asignan a nodos según disponibilidad.

## 6) Estado real: provocar una falla y ver reconciliación

1. En `manager`, identifica en qué nodos están corriendo las tasks:

```bash
docker service ps lab02_web
```

1. En el nodo que tenga al menos un contenedor `lab02_web`, lista y mata uno.

En `manager` o `worker` (según donde exista):

```bash
docker ps --filter label=com.docker.swarm.service.name=lab02_web
CID=$(docker ps -q --filter label=com.docker.swarm.service.name=lab02_web | head -n 1)
docker container rm -f "$CID"
```

1. En `manager`, observa cómo Swarm repara el estado:

```bash
docker service ps lab02_web
```

Esperado:
1. Aparece una task con estado `Shutdown`/`Failed`.
1. Swarm crea una task nueva para volver al estado deseado (5 réplicas activas).

## 7) Scheduling: drenar un nodo (mover tasks)

Este paso muestra que Swarm reubica tasks cuando un nodo deja de aceptar carga.

En `manager`, drena el propio manager (no ejecutará tasks; solo control-plane):

```bash
MANAGER_NODE=$(docker info --format '{{.Name}}')
docker node update --availability drain "$MANAGER_NODE"
docker node ls
docker service ps lab02_web
```

Qué observar:
1. Las tasks que estaban en el manager se reprograman en el worker.
1. El servicio intenta mantener el estado deseado (misma cantidad de réplicas).

Revertir (volver a aceptar carga en el manager):

```bash
docker node update --availability active "$MANAGER_NODE"
docker node ls
```

## 8) Limpieza

En `manager`:

```bash
docker stack rm lab02
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

## Checklist (mapea a las diapositivas)

1. Puedo explicar la cadena **Service -> Task -> Container** usando `docker service ps` y `docker container ls`.
1. Entiendo la diferencia entre **estado deseado** (réplicas) y **estado real** (tasks efectivas por nodo).
1. Si elimino un contenedor, Swarm lo corrige creando una task nueva (reconciliación).
1. Si un nodo se drena, las tasks se reubican (scheduling).
