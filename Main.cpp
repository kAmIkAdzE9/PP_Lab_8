#include <iostream>
#include <mpi.h>

using namespace std;

const static int N = 1000;

long calculateSum(long* arr, long* temp_arr, int lengthOfArray, int argc, char** argv) {
	long tempSum = 0;
	long sum = 0;

	int rank = 0;
	int countOfPartOfArray = 2;
	int lengthOfPartOfArray = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &countOfPartOfArray);

	MPI_Status mpiStatus;

	if (rank == 0)
	{
		int rightIndex = 0;
		lengthOfPartOfArray = lengthOfArray / countOfPartOfArray;
		for (int i = 0; i < --countOfPartOfArray; i++)
		{
			rightIndex = i + lengthOfPartOfArray;
			MPI_Send(&lengthOfPartOfArray, 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
			MPI_Send(&arr[rightIndex], lengthOfPartOfArray, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
		}

		for (int i = 0; i < lengthOfPartOfArray; i++)
		{
			tempSum += arr[i];
		}
	}
	else
	{
		MPI_Recv(&lengthOfPartOfArray, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
		MPI_Recv(&temp_arr, lengthOfPartOfArray, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
		for (int i = 0; i < lengthOfPartOfArray; i++)
		{
			tempSum += temp_arr[i];
		}

		MPI_Send(&tempSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Reduce(&tempSum, &sum, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Finalize();

	return sum;
}

int main(int argc, char** argv)
{
	long arr[N];
	for (int i = 0; i < N; i++) {
		arr[i] = i + 1;
	}

	long temp_arr[N];
	cout << "Sum is  " << calculateSum(arr, temp_arr, N, argc, argv) << endl;
}