echo "Test script start!"

set -e

make project
make assembler

./assembler test/test_print_simple.S libs/stdio.S
./project -c 10000 a.out

echo "Build finished!"