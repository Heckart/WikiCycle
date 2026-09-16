# project uses c2y, so need rawhide
FROM fedora:rawhide

# hadolint ignore=DL3002,DL3066
USER root
WORKDIR /work
COPY . .

# hadolint ignore=DL3041
RUN dnf install clang curl libcurl libcurl-devel valgrind --assumeyes && dnf clean all

# yes, this entire container is literally just to be able to run valgrind on my dev box
CMD ["bash", "-c", "clang -std=c2y -fdefer-ts -lcurl -Og -g3 -march=native -mno-avx512f -mno-avx512bw -mno-avx512vl -mno-avx512dq -pipe /work/src/dsa/*.c /work/tests/*.c /work/tests/unit/*.c /work/tests/test_infrastructure/*.c -I /work/src/include/*.h /work/tests/include/*.h; valgrind -s --trace-children=yes --leak-check=full --leak-resolution=high --show-leak-kinds=all --leak-check-heuristics=all --show-reachable=yes --show-possibly-lost=yes --track-origins=yes --undef-value-errors=yes --expensive-definedness-checks=yes --error-exitcode=1 /work/a.out"]
