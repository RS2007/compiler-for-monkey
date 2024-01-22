make compile && valgrind -s --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./bin/mc ./examples/fun_in_fun.monkey


