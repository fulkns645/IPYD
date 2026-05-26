.PHONY: all up provision setup compile run clean

all: up provision setup compile run

up:
	vagrant up

provision:
	vagrant provision

setup:
	vagrant ssh master -c "bash /shared/scripts/distribute-keys.sh"

compile:
	vagrant ssh master -c "cd /shared && make"

run:
	vagrant ssh master -c "cd /shared && sudo -u mpi mpirun --hostfile hostfile -np 2 ./hello_mpi"

clean:
	vagrant destroy -f
