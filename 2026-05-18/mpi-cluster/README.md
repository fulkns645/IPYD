Estructura generada:

```
mpi-cluster/
├── Vagrantfile                   3 VMs (master, worker1, worker2), red privada 192.168.56.x
├── Makefile                      make up → setup → compile → run → clean
├── scripts/
│   ├── common.sh                 Instala MPICH + sshpass, crea usuario mpi, /etc/hosts
│   └── master.sh                 Genera claves SSH (vagrant + mpi), crea ~/.ssh/config
└── shared/                       (synced en /shared en todas las VMs)
    ├── scripts/distribute-keys.sh
    ├── hostfile                  worker1 slots=1, worker2 slots=1
    ├── hello_mpi.c               Programa MPI de prueba
    └── Makefile                  mpicc -o hello_mpi hello_mpi.c
```

**Uso:**

```bash
cd mpi-cluster

# 1. Levantar las 3 VMs (aprox. 5 min la primera vez)
vagrant up

# 2. Distribuir claves SSH desde master a workers
make setup

# 3. Compilar hello_mpi.c
make compile

# 4. Ejecutar mpirun (2 procesos, 1 por worker)
make run
```

O todo en un paso: `make all`.

**Detalles importantes:**
- `common.sh` corre en **los 3 nodos** como root: instala MPICH, crea `/etc/hosts` con las IPs privadas, crea el usuario `mpi` (pass: `mpi`).
- `master.sh` corre **solo en master** como `vagrant`: genera claves RSA y configura `StrictHostKeyChecking=no` para `worker1`/`worker2`.
- `distribute-keys.sh` se ejecuta **después** de que las 3 VMs estén arriba — copia las claves de `vagrant` y `mpi` a los workers usando `sshpass`.
- El `hostfile` lista solo los workers. `mpirun -np 2` asigna rank 0 → worker1, rank 1 → worker2.
- Para cambiar a Ubuntu 24.04, editar `Vagrantfile`: `config.vm.box = "ubuntu/noble64"`.

**Flujo de Makefile:**
| target | comando |
|---|---|
| `make up` | `vagrant up` |
| `make setup` | distribuye claves SSH |
| `make compile` | `mpicc -o hello_mpi hello_mpi.c` |
| `make run` | `mpirun --hostfile hostfile -np 2 ./hello_mpi` |
| `make clean` | `vagrant destroy -f` |
