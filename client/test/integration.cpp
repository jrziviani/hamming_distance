#include <string>
#include <limits>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

uint64_t execute_cli(const string &a, const string &b)
{
    const int READ = 0;
    const int WRITE = 1;
    const uint64_t ERROR_CODE = numeric_limits<uint64_t>::max();

    int fd_out[2];

    if (pipe(fd_out) != 0) {
        return ERROR_CODE;
    }

    auto pid = fork();
    if (pid == -1) {
        return ERROR_CODE;
    }

    if (pid == 0) {
        if (setsid() == -1) {
            return ERROR_CODE;
        }

        dup2(fd_out[WRITE], WRITE);
        close(2);
        close(fd_out[READ]);
        close(fd_out[WRITE]);
        execl("cli", "cli", a.c_str(), b.c_str(), nullptr);
        _exit(0);
    }
    else {
        int status = 0;
        char buffer;
        string output;

        close(fd_out[WRITE]);
        waitpid(pid, &status, 0);
        while (read(fd_out[READ], &buffer, 1) > 0) {
            output += buffer;
        }
        return stoi(output);
    }

    return ERROR_CODE;
}

void test_empty_string()
{
    if (execute_cli("", "") != 0) {
        cout << "[ FAIL ] test empty string\n";
        return;
    }
    cout << "[ OK ] test empty string\n";
}

void test_null_string()
{
    try {
        execute_cli(nullptr, nullptr);
    }
    catch (const logic_error &e) {
        cout << "[ OK ] test null string\n";
        return;
    }
    catch (...) {
    }
    cout << "[ FAIL ] test null string\n";
}

void test_different_size_string()
{
    try {
        execute_cli("a", "ab");
    }
    catch (const invalid_argument &e) {
        cout << "[ OK ] 1 - test different size string\n";
    }

    try {
        execute_cli("aabbcc", "ab");
    }
    catch (const invalid_argument &e) {
        cout << "[ OK ] 2 - test different size string\n";
    }

    try {
        execute_cli(" i", "a  i");
    }
    catch (const invalid_argument &e) {
        cout << "[ OK ] 3 - test different size string\n";
        return;
    }

    cout << "[ FAIL ] test different size string\n";
}

void test_strings()
{
    bool fail = false;
    if (execute_cli("qHqAJ", "qHqAJ") != 0) {
        cout << "[ FAIL ] 1 test string\n";
        fail = true;
    }

    if (execute_cli("MmTIN", "MmTiN") != 1) {
        cout << "[ FAIL ] 2 test string\n";
        fail = true;
    }

    if (execute_cli("wfaNz", "w  Nz") != 2) {
        cout << "[ FAIL ] 3 test string\n";
        fail = true;
    }

    if (execute_cli("COrav", "COrav") != 0) {
        cout << "[ FAIL ] 4 test string\n";
        fail = true;
    }

    if (execute_cli("FCE53", "     ") != 5) {
        cout << "[ FAIL ] 5 test string\n";
        fail = true;
    }

    if (execute_cli("XPMtb", "XPMt0") != 1) {
        cout << "[ FAIL ] 6 test string\n";
        fail = true;
    }

    if (execute_cli("znFaz", "_nFa_") != 2) {
        cout << "[ FAIL ] 7 test string\n";
        fail = true;
    }

    if (execute_cli("8gRrW", "****W") != 4) {
        cout << "[ FAIL ] 8 test string\n";
        fail = true;
    }

    if (execute_cli("awjTx", "AWJtX") != 5) {
        cout << "[ FAIL ] 9 test string\n";
        fail = true;
    }

    if (execute_cli("hxIlY", "hxIlY") != 0) {
        cout << "[ FAIL ] 10 test string\n";
        fail = true;
    }

    if (execute_cli("GNivenGmzspn3zqy0bRb", "GNivenGmzspn3zqy0bRb") != 0) {
        cout << "[ FAIL ] 11 test string\n";
        fail = true;
    }

    if (execute_cli("cO2bNa1QmuH2NJ8Ce2iK", "cO2bNa1QmuH2NJ8Ce2i_") != 1) {
        cout << "[ FAIL ] 12 test string\n";
        fail = true;
    }

    if (execute_cli("eeauIltBHWu5FmeWWhqo", "eeauIlt  Wu5FmeWWhqo") != 2) {
        cout << "[ FAIL ] 13 test string\n";
        fail = true;
    }

    if (execute_cli("n2aU9mZrNmHvzrEmltkp", "N2Au9MzRnMhVZReMLtkp") != 15) {
        cout << "[ FAIL ] 14 test string\n";
        fail = true;
    }

    if (execute_cli("iX0cL7J36yz4CEH1YDMt", "                    ") != 20) {
        cout << "[ FAIL ] 15 test string\n";
        fail = true;
    }

    if (execute_cli("f1DyNpfXVQzV4z2oHUZS", "f1DyNpfXVQzV4z2oHUZS") != 0) {
        cout << "[ FAIL ] 16 test string\n";
        fail = true;
    }

    if (execute_cli("iTgAJMVEmSnqj99E78AfwakxnhEPxWIAH3FmiJ9veksuBHZivLhwDNNNXeEpg7KGHW2DDcY9b8P7UTnuG4Jz7AQjNLrNF5fLFvfH", "iTgAJMVEmSnqj99E78AfwakxnhEPxWIAH3FmiJ9veksuBHZivLhwDNNNXeEpg7KGHW2DDcY9b8P7UTnuG4Jz7AQjNLrNF5fLFvfH") != 0) {
        cout << "[ FAIL ] 17 test string\n";
        fail = true;
    }

    if (execute_cli("l0  9W D G 9 H 0 1 M A S h a Q a h t o N zXYZyGQaeVBOTXB78OGi52ReQlUmDNz53iHDrJzGPMsa0LVeQsopARGrSQO", "l0y19WQD1GQ9mHe0F1RMvAESthraZQAaDhztZoPNmzXYZyGQaeVBOTXB78OGi52ReQlUmDNz53iHDrJzGPMsa0LVeQsopARGrSQO") != 20) {
        cout << "[ FAIL ] 18 test string\n";
        fail = true;
    }

    if (execute_cli("k2y13wyf1N          i5QJskou26          MoVS7LpgtJ          g4ZmvEtakN          2CU8shlurj          ", "k2y13wyf1N9M5xikJF3ni5QJskou26kwuNNUVoWqMoVS7LpgtJJLuIaoYFGAg4ZmvEtakNViZ27SiJsc2CU8shlurjVIsxNYh7xb") != 50) {
        cout << "[ FAIL ] 19 test string\n";
        fail = true;
    }

    if (execute_cli("7f54MuMhV7ByFF3XPnKqP6A3rViKiwG0aY85gERsqElyhV5IPlcRNxx85wMqHI6xj1ifT1vn7vPlsEFseAuPo6cnHmB4eAuX2Pcp", "====================================================================================================") != 100) {
        cout << "[ FAIL ] 20 test string\n";
        fail = true;
    }

    cout << "[ OK ] 20 test string\n";
}

int main()
{
    test_empty_string();
    test_null_string();
    test_different_size_string();
    test_strings();
    return 0;
}
