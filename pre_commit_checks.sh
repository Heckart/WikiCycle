#!/bin/bash
BGreen='\033[1;32m'
BIRed='\033[1;91m'
BIPurple='\033[1;35m'
NC='\033[0m'
TEST_COMPILER_FLAGS="-std=c2y -fdefer-ts -lcurl -Og -g3 -march=native -pipe"
RELEASE_COMPILER_FLAGS="-std=c2y -fdefer-ts -lcurl -O3 -flto -march=native -pipe"
TEST_SOURCE_FILES="tests/*.c src/dsa/*.c tests/test_infrastructure/*.c tests/unit/*.c"
TEST_INCLUDE_FILES="src/include/*.h tests/include/*.h"
RELEASE_SOURCE_FILES="src/*.c src/dsa/*.c"
RELEASE_INCLUDE_FILES="src/include/*.h"
SANITIZER_COMPILER="clang"

# needed so compile_commands.json is up to date if run from a container
# only need to uncomment this if a container will run the entire script (in which case valgrind doesn't have to be containerized)
if [[ "${1}" == "release" ]]; then
    bear -- clang ${RELEASE_COMPILER_FLAGS} ${RELEASE_SOURCE_FILES} -I ${RELEASE_INCLUDE_FILES}
else
    bear -- clang ${TEST_COMPILER_FLAGS} ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
fi

#################### FORMATTING CHECK ####################
echo -e "${BIPurple}Checking formatting.."
no_violations=true
while IFS='' read -r -d '' filename; do
    if [[ "${filename}" == *.c || "${filename}" == *.h ]]; then
        clang-format --style=file --dry-run --Wclang-format-violations --Werror "${filename}" >/dev/null 2>&1
        rc="${?}"
        if (("${rc}" != 0)); then
            echo -e "${BIRed}${filename} is not properly formatted."
            echo "Run 'clang-format --style=file -i ${filename}' to fix the formatting."
            no_violations=false
        fi
    fi
done < <(find . -type f -print0)
if [[ "${no_violations}" == true ]]; then
    echo -e "${BGreen}Formatting checks passed."
fi
echo -e "${BIPurple}Done.\n"
#################### FORMATTING CHECK ####################

#################### STATIC ANALYSIS SUITE ####################
echo -e "${BIPurple}Running static analysis suite..."

echo -e "${NC}Running cppcheck static analysis..."
cppcheck -v --platform=unix64 --language=c --std=c2y --check-level=exhaustive --enable=all --inconclusive --safety --library=posix --error-exitcode=9 --inline-suppr --suppress=missingIncludeSystem --output-file=cppcheck_report.txt src/*.c src/dsa/*.c tests/*.c tests/test_infrastructure/*.c tests/unit/*.c >/dev/null 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}cppcheck static analysis passed."
    rm cppcheck_report.txt
else
    echo -e "${BIRed}Issues found by cppcheck."
    cat cppcheck_report.txt
fi

echo -e "${NC}Running flawfinder static analysis..."
flawfinder --minlevel=0 --error-level=0 src/*.c src/dsa/*.c src/include/*.h tests/*.c tests/unit/*.c tests/test_infrastructure/*.c tests/include/*.h >flawfinder_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}flawfinder static analysis passed."
    rm flawfinder_report.txt
else
    echo -e "${BIRed}Issues found by flawfinder."
    cat flawfinder_report.txt
fi

echo -e "${NC}Running lizard static analysis..."
lizard -V --sort cyclomatic_complexity -o lizard_report.txt src/*.c src/dsa/*.c src/include/*.h tests/*.c tests/test_infrastructure/*.c tests/unit/*.c tests/include/*.h >/dev/null 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}lizard static analysis passed."
    rm lizard_report.txt
else
    echo -e "${BIRed}Issued found by lizard"
    cat lizard_report.txt
fi

echo -e "${NC}Running scan-build static analysis..."
scan-build --status-bugs -v clang ${TEST_COMPILER_FLAGS} ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES} >scan-build_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}scan-build static analysis passed."
    rm scan-build_report.txt
    rm a.out
else
    echo -e "${BIRed}Issues found by scan-build."
    cat scan-build_report.txt
fi

echo -e "${NC}Running clang-tidy static analysis..."
clang-tidy --config-file=.clang-tidy --warnings-as-errors=* --format-style=file --experimental-custom-checks src/dsa/*.c src/include/*.h src/*.c tests/*.c tests/unit/*.c tests/include/*.h tests/test_infrastructure/*.c >clang-tidy_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}clang-tidy static analysis passed."
    rm clang-tidy_report.txt
else
    echo -e "${BIRed}Issues found by clang-tidy."
    cat clang-tidy_report.txt
fi

echo -e "${BIPurple}Done.\n"
#################### STATIC ANALYSIS SUITE ####################

#################### BUILD TESTS ####################
echo -e "${BIPurple}Running build tests..."

echo -e "${NC}Running clang build tests with all warnings as errors..."
clang ${RELEASE_COMPILER_FLAGS} -Weverything -Werror -Wno-c++98-compat -Wno-c99-compat -Wno-c++-keyword -Wno-declaration-after-statement -Wno-implicit-void-ptr-cast -Wno-padded -Wno-pre-c23-compat -Wno-unused-function -Wno-unsafe-buffer-usage -Wno-used-but-marked-unused -Wno-vla ${RELEASE_SOURCE_FILES} -I ${RELEASE_INCLUDE_FILES} >clang_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}clang release build succeeded with no issues."
    rm clang_report.txt a.out
else
    echo -e "${BIRed}clang release build has issues."
    cat clang_report.txt
fi

clang ${TEST_COMPILER_FLAGS} -Weverything -Werror -Wno-c++98-compat -Wno-c99-compat -Wno-c++-keyword -Wno-declaration-after-statement -Wno-implicit-void-ptr-cast -Wno-padded -Wno-pre-c23-compat -Wno-unused-function -Wno-unsafe-buffer-usage -Wno-used-but-marked-unused -Wno-vla ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES} >clang_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}clang test build succeeded with no issues."
    rm clang_report.txt
else
    echo -e "${BIRed}clang test build has issues."
    cat clang_report.txt
fi

# NOTE: gcc, as of september 2026, does not support defer statements from C29 :(
#echo -e "${NC}Running gcc build test with all warnings as errors..."
#gcc ${RELEASE_COMPILER_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wrestrict -Werror ${RELEASE_SOURCE_FILES} -Include ${RELEASE_INCLUDE_FILES} >gcc_report.txt 2>&1
#rc="${?}"
#if (("${rc}" == 0)); then
#    echo -e "${BGreen}gcc release build succeeded with no issues."
#    rm gcc_report.txt a.out
#else
#    echo -e "${BIRed}gcc release build has issues."
#    cat gcc_report.txt
#fi
#
#gcc ${TEST_COMPILER_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wrestrict -Werror ${TEST_SOURCE_FILES} -Include ${TEST_INCLUDE_FILES} >gcc_report.txt 2>&1
#rc="${?}"
#if (("${rc}" == 0)); then
#    echo -e "${BGreen}gcc test build succeeded with no issues."
#    rm gcc_report.txt
#else
#    echo -e "${BIRed}gcc test build has issues."
#    cat gcc_report.txt
#fi

echo -e "${BIPurple}Done.\n"
#################### BUILD TESTS ####################

#################### UNIT/INTEGRATION/E2E/PERFORMANCE TESTS ####################
echo -e "${BIPurple}Running code tests..."

./a.out 1>/dev/null 2> >(tee test_report.txt >&2)
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}Code tests passed."
    rm test_report.txt a.out
else
    echo -e "${BIRed}Code tests failed. Output from above saved at test_report.txt"
fi

echo -e "${BIPurple}Done.\n"
#################### UNIT/INTEGRATION/E2E/PERFORMANCE TESTS ####################

#################### DYNAMIC ANALYSIS SUITE ####################
echo -e "${BIPurple}Running dynamic analysis suite..."

echo -e "${NC}Running valgrind memcheck dynamic analysis..."
# NOTE: valgrind does not support axv-512, so on systems where glibc is compiled with avx-512, it will not run. This disgusting workaround involves
# building a podman container just to run valgrind on it.
podman build --tag pre_commit_check . >/dev/null 2>&1
podman run --rmi pre_commit_check >valgrind_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}valgrind memcheck dynamic analysis passed."
    rm valgrind_report.txt
else
    echo -e "${BIRed}Issues found by valgrind memcheck."
    cat valgrind_report.txt
fi

#valgrind --tool=cachegrind --cachegrind-out-file=cachegrind_report.txt --cache-sim=yes --branch-sim=yes ./a.out
#valgrind --tool=massif --heap=yes --stacks=yes ./a.out

echo -e "${NC}Running ${SANITIZER_COMPILER} address sanitizer dynamic analysis..."
if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    clang ${TEST_COMPILER_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fno-optimize-sibling-calls ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
elif [[ "${SANITIZER_COMPILER}" == "gcc" ]]; then
    detect_invalid_pointer_pairs=2
    export detect_invalid_pointer_pairs
    gcc ${TEST_COMPILER_FLAGS} -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fno-ipa-icf -fno-omit-frame-pointer ${TEST_SOURCE_FILES} -Include ${TEST_INCLUDE_FILES}
fi
./a.out >asan_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}${SANITIZER_COMPILER} address sanitizer dynamic analysis passed."
    rm asan_report.txt
    rm a.out
else
    echo -e "${BIRed}${SANITIZER_COMPILER} Issues found by clang address sanitizer."
    cat asan_report.txt
    rm a.out
fi

echo -e "${NC}${SANITIZER_COMPILER} thread sanitizer dynamic analysis..."
if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    clang ${TEST_COMPILER_FLAGS} -fsanitize=thread ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
elif [[ "${SANITIZER_COMPILER}" == "gcc" ]]; then
    gcc ${TEST_COMPILER_FLAGS} -fsanitize=thread ${TEST_SOURCE_FILES} -Include ${TEST_INCLUDE_FILES}
fi
./a.out >thread_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}${SANITIZER_COMPILER} thread sanitizer dynamic analysis passed."
    rm thread_report.txt
    rm a.out
else
    echo -e "${BIRed}Issues found by ${SANITIZER_COMPILER} thread sanitizer."
    cat thread_report.txt
    rm a.out
fi

if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    echo -e "${NC}Running ${SANITIZER_COMPILER} memory sanitizer dynamic analysis..."
    clang ${TEST_COMPILER_FLAGS} -fsanitize=memory -DMSAN_SKIP -fno-omit-frame-pointer -fno-optimize-sibling-calls ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
    ./a.out >memory_report.txt 2>&1
    rc="${?}"
    if (("${rc}" == 0)); then
        echo -e "${BGreen}${SANITIZER_COMPILER} memory sanitizer dynamic analysis passed."
        rm memory_report.txt
        rm a.out
    else
        echo -e "${BIRed}Issues found by ${SANITIZER_COMPILER} memory sanitizer."
        cat memory_report.txt
        rm a.out
    fi
fi

echo -e "${NC}Running ${SANITIZER_COMPILER} undefined behavior sanitizer dynamic analysis..."
if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    clang ${TEST_COMPILER_FLAGS} -fsanitize=undefined,local-bounds,float-divide-by-zero,unsigned-integer-overflow,implicit-conversion,vptr,nullability -fno-sanitize-merge -fno-omit-frame-pointer ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
elif [[ "${SANITIZER_COMPILER}" == "gcc" ]]; then
    gcc ${TEST_COMPILER_FLAGS} -fsanitize=undefined -fsanitize=shift -fsanitize=shift-base -fsanitize=shift-exponent -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum -fsanitize=vptr -fsanitize=pointer-overflow -fsanitize=builtin ${TEST_SOURCE_FILES} -Include ${TEST_INCLUDE_FILES}
fi
./a.out >ub_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}${SANITIZER_COMPILER} undefined behavior sanitizer dynamic analysis passed."
    rm ub_report.txt
    rm a.out
else
    echo -e "${BIRed}Issues found by ${SANITIZER_COMPILER} undefined behavior sanitizer."
    cat ub_report.txt
    rm a.out
fi

echo -e "${NC}Running ${SANITIZER_COMPILER} leak sanitizer dynamic analysis..."
if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    clang ${TEST_COMPILER_FLAGS} -fsanitize=leak ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
elif [[ "${SANITIZER_COMPILER}" == "gcc" ]]; then
    gcc ${TEST_COMPILER_FLAGS} -fsanitize=leak ${TEST_SOURCE_FILES} -Include ${TEST_INCLUDE_FILES}
fi
./a.out >leak_report.txt 2>&1
rc="${?}"
if (("${rc}" == 0)); then
    echo -e "${BGreen}${SANITIZER_COMPILER} leak sanitizer dynamic analysis passed."
    rm leak_report.txt
    rm a.out
else
    echo -e "${BIRed}Issues found by ${SANITIZER_COMPILER} leak sanitizer."
    cat leak_report.txt
    rm a.out
fi

if [[ "${SANITIZER_COMPILER}" == "gcc" ]]; then
    echo -e "${NC}Running ${SANITIZER_COMPILER} address use after scope sanitizer dynamic analysis..."
    gcc ${TEST_COMPILER_FLAGS} -fsanitize-address-use-after-scope ${TEST_SOURCE_FILES} -Include ${TEST_INCLUDE_FILES}
    a.out >address_scope_report.txt 2>&1
    rc="${?}"
    if (("${rc}" == 0)); then
        echo -e "${BGreen}${SANITIZER_COMPILER} address use after scope sanitizer dynamic analysis passed."
        rm address_scope_report.txt
        rm a.out
    else
        echo -e "${BIRed}Issues ${SANITIZER_COMPILER} address use after scope found by sanitizer."
        cat address_scope_report.txt
        rm a.out
    fi
fi

if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    echo -e "${NC}Running ${SANITIZER_COMPILER} type sanitizer dynamic analysis..."
    clang ${TEST_COMPILER_FLAGS} -fsanitize=type -fno-omit-frame-pointer -fno-optimize-sibling-calls ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
    ./a.out >type_report.txt 2>&1
    rc="${?}"
    if (("${rc}" == 0)); then
        echo -e "${BGreen}${SANITIZER_COMPILER} type sanitizer dynamic analysis passed."
        rm type_report.txt
        rm a.out
    else
        echo -e "${BIRed}Issues found by ${SANITIZER_COMPILER} type sanitizer."
        cat type_report.txt
        rm a.out
    fi
fi

if [[ "${SANITIZER_COMPILER}" == "clang" ]]; then
    echo -e "${NC}Running ${SANITIZER_COMPILER} real time sanitizer dynamic analysis..."
    clang ${TEST_COMPILER_FLAGS} -fsanitize=realtime ${TEST_SOURCE_FILES} -I ${TEST_INCLUDE_FILES}
    ./a.out >rt_report.txt 2>&1
    rc="${?}"
    if (("${rc}" == 0)); then
        echo -e "${BGreen}${SANITIZER_COMPILER} real time sanitizer dynamic analysis passed."
        rm rt_report.txt
        rm a.out
    else
        echo -e "${BIRed}Issues found by ${SANITIZER_COMPILER} real time sanitizer."
        cat rt_report.txt
        rm a.out
    fi
fi

echo -e "${BIPurple}Done.\n"
#################### DYNAMIC ANALYSIS SUITE ####################

echo -e "${BIPurple}Test suite finished${NC}"
