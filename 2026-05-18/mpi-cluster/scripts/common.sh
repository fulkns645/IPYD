#!/bin/bash
set -e

apt-get update
apt-get install -y build-essential mpich sshpass

id -u mpi &>/dev/null || useradd -m -s /bin/bash mpi
echo "mpi:mpi" | chpasswd

cat >/etc/ssh/sshd_config.d/99-password-auth.conf <<'EOF'
PasswordAuthentication yes
KbdInteractiveAuthentication yes
UsePAM yes
EOF

systemctl restart ssh

for u in vagrant mpi; do
  mkdir -p /home/$u/.ssh
  chown $u:$u /home/$u/.ssh
  chmod 700 /home/$u/.ssh
done

if ! grep -q '192.168.56.10 master' /etc/hosts; then
  cat >> /etc/hosts <<'EOF'
192.168.56.10 master
192.168.56.11 worker1
192.168.56.12 worker2
EOF
fi
