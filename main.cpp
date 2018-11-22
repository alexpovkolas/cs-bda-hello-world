#include <iostream>
#include <mpi.h>
#include <map>
#include <string>
#include <vector>
#include <random>


using namespace std;

string get_process_data(int rank) {
    srand(rank);
    int random_number = rand() % 10;
    string random(random_number, 'I');
    return "Hey. I'm process # " + to_string(rank) + " " + random;
}

#define ROOT 0
#define COMMON_TAG 1
int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<string> result;
    if (rank == ROOT) {
        for (int i = 1; i < world_size; i++) {
            MPI_Status status;
            MPI_Probe(MPI_ANY_SOURCE, COMMON_TAG, MPI_COMM_WORLD, &status);
            int lenght;
            MPI_Get_count(&status, MPI_CHAR, &lenght);
            string buffer(lenght, ' ');

            MPI_Recv((char *) buffer.c_str(), lenght, MPI_CHAR, status.MPI_SOURCE, COMMON_TAG, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            result.push_back(buffer);
        }


        for (auto i = result.begin(); i < result.end(); ++i) {
            cout << *i << endl;
        }

    } else {
        string s = get_process_data(rank);
        MPI_Send(s.c_str(), s.size(), MPI_CHAR, ROOT, COMMON_TAG, MPI_COMM_WORLD);
    }


    MPI_Finalize();
}

