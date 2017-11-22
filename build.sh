#!/bin/sh

init() {
    [ -d bin ] && rm -fr bin
    [ -d .build ] && rm -fr .build
    [ -f cli ] && rm -f cli
}

build() {
    echo "[Building]-----------------------------"
    echo ""
    mkdir .build
    cd .build
    cmake ..
    [ $? -eq 0 ] || exit 1
    make
    mv bin ../bin
    cd ..
    rm -fr .build
}

generate_test_cases() {
    echo "[Creating random test cases]-----------"
    echo ""
    cd bin
    ./create_cases
    cd ..
}

run_unit_tests() {
    echo "[Running unit tests]-------------------"
    cd bin
    ./unit_test
    cd ..
    echo "[unit test finished]-------------------"
}

create_wrapper() {
    printf "#!/bin/sh\n\n" > cli
    printf "LD_PRELOAD=bin/lib/libhamming.so bin/cli \"%s\"" '$@' >> cli
    chmod +x cli
}

run_integration_tests() {
    echo "[Running integration tests]------------"
    bin/integration_test
    echo "[integration test finished]------------"
}

main() {
    init
    build
    generate_test_cases
    echo "[build finished]"
    echo ""
    run_unit_tests
    echo ""
    create_wrapper
    run_integration_tests
    echo ""
    echo "  run: ./cli <string> <string>"
}

main
