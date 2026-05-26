#!/bin/bash
set -e

for host in worker1 worker2; do
  sudo -u mpi sshpass -p "mpi" ssh-copy-id -o StrictHostKeyChecking=no mpi@$host
done

echo "Done. Passwordless SSH configured for mpi user."
