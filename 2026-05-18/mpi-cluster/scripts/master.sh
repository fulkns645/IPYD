#!/bin/bash
set -e

sudo install -d -m 700 -o mpi -g mpi /home/mpi/.ssh

if ! sudo -u mpi test -f /home/mpi/.ssh/id_rsa; then
  sudo -u mpi ssh-keygen -t rsa -N "" -f /home/mpi/.ssh/id_rsa -q
fi

conf='Host worker1 worker2
  StrictHostKeyChecking no
  UserKnownHostsFile /dev/null'

echo "$conf" | sudo -u mpi tee /home/mpi/.ssh/config > /dev/null
sudo -u mpi chmod 600 /home/mpi/.ssh/config
