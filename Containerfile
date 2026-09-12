# project uses c2y, so need rawhide
FROM fedora:rawhide

# hadolint ignore=DL3002,DL3066
USER root
WORKDIR /work
COPY . .

# hadolint ignore=DL3041
RUN dnf install clang valgrind --assumeyes && dnf clean all

# yes, this entire container is literally just to be able to run valgrind on my dev box
CMD ["bash", "-c", "clang -std=c2y -fdefer-ts -Og -g3 -march=native -pipe /work/src/ds/*.c /work/tests/*.c /work/tests/unit/*.c -I /work/src/ds/*.h /work/tests/unit/*.h; valgrind -s --trace-children=yes --leak-check=full --leak-resolution=high --show-leak-kinds=all --leak-check-heuristics=all --show-reachable=yes --show-possibly-lost=yes --track-origins=yes --undef-value-errors=yes --expensive-definedness-checks=yes --error-exitcode=1 /work/a.out"]
