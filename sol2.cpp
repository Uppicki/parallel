#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "utils/reader/reader.cpp"
#include <chrono>



int main(int argc, char* argv[]) {

    std::vector<int> original;

    std::vector<int> array;

    int sliceSize = 1000000;
    int checkStep = 10000;

    readDataFromFile("res/numbers.txt", original);

    array.assign(original.begin(), original.begin()+sliceSize);

    double start_time, end_time, elapsed_time;



    MPI_Init(&argc, &argv);

    
    long long res = 0;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    start_time = MPI_Wtime();


    if (size > 1) {
        if (rank == 0) {
            std::vector<int> targets(size - 1);
            std::vector<MPI_Request> requests(size - 1);
            std::vector<long long> responses(size-1);

            int curr = 0;
            int active_requests = size - 1;


            // Инициализация неполученных запросов для каждого процесса
            for (int i = 0; i < size - 1; i++) {
                targets[i] = 1 + i;
                MPI_Irecv(&responses[i], 1, MPI_LONG_LONG, targets[i], 0, MPI_COMM_WORLD, &requests[i]);
            }

            int index;

            while (active_requests > 0) {
                if (curr % checkStep == 0) std::cout << curr << " " << res << std::endl;
                
                MPI_Waitany(size - 1, requests.data(), &index, MPI_STATUS_IGNORE);
                
                if (responses[index] >= 0) res += responses[index];
                
                if (curr < array.size()-1) {
                    MPI_Send(&curr, 1, MPI_INT, targets[index], 0, MPI_COMM_WORLD);
                    curr++;
                    MPI_Irecv(&responses[index], 1, MPI_LONG_LONG, targets[index], 0, MPI_COMM_WORLD, &requests[index]);
                } else {
                    int end_signal = array.size();
                    MPI_Send(&end_signal, 1, MPI_INT, targets[index], 0, MPI_COMM_WORLD);
                    active_requests--;
                }
            }

            end_time = MPI_Wtime();
            elapsed_time = end_time - start_time;


            std::cout << "Process end: " << rank << std::endl;
            std::cout << "Result: " << res << std::endl;
            std::cout << "Program executed in " << elapsed_time *1000 << " milliseconds" << std::endl;


        } else {
            int value = 0;
            long long localInversions = -1;

            //Отправка сообщения о том что процесс готов принимать значения
            MPI_Send(&localInversions, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);


            while (true) {
                MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (value >= array.size()-1) break;

                localInversions = 0;
                
                for (int i = (value+1); i < array.size(); i++)
                {
                    if (array[value] > array[i]) localInversions++;
                }
                

                MPI_Send(&localInversions, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
            }
            std::cout << "Process end: " << rank << std::endl;
        }
    }

    MPI_Finalize();

    return 0;
}


