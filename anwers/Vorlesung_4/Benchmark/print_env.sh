#!/bin/bash
{
echo "Operating System Info:"
cat /etc/*release

echo "Kernel Version:"
uname -r

echo "Memory Info:"
free -h

echo "CPU Info:"
lscpu

echo "GCC Version:"
gcc --version

} > env_info.txt