using System;
using System.Collections.Generic;
using System.IO;

public class Program
{
    public static int[,] ReadConnectivityMatrixFromFile(string path)
    {
        try
        {
            int[,] connectivityMatrix = new int[10, 10];
            string[] data = File.ReadAllLines(path);

            for (int i = 0; i < data.Length; i++)
            {
                string[] numbers = data[i].Split(' ');

                for (int j = 0; j < data.Length; j++)
                {
                    connectivityMatrix[i, j] = int.Parse(numbers[j]);
                }
            }

            return connectivityMatrix;
        }

        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка чтения из файла: {ex.Message}");
            return null;
        }
    }

    static void ShowConnectivityMatrix(int[,] matrix)
    {
        for (int i = 0; i < matrix.GetLength(0); i++)
        {
            for (int j = 0; j < matrix.GetLength(1); j++)
            {
                Console.Write(matrix[i, j] + " ");
            }
            Console.WriteLine();
        }
    }

    public static int[,] AlgorithmPrima(int[,] connectivityMatrix)
    {
        List<int> spanningTree = new List<int> { 0 };
        int nodesCount = connectivityMatrix.GetLength(0);
        int[,] resMatrix = new int[10, 10];


        while (spanningTree.Count < nodesCount)
        {
            int min = int.MaxValue;
            int from = -1, to = -1;

            foreach (int i in spanningTree)
            {
                for (int j = 0; j < nodesCount; j++)
                {

                    if (!spanningTree.Contains(j) && connectivityMatrix[i, j] != 0 && connectivityMatrix[i, j] < min)
                    {
                        min = connectivityMatrix[i, j];
                        from = i; to = j;
                    }
                }
            }

            if (to != -1)
            {
                spanningTree.Add(to);
                resMatrix[from, to] = min;
                resMatrix[to, from] = min;
            }

            else
            {
                Console.WriteLine("Граф несвязный");
                break;
            }
        }

        return resMatrix;
    }

}
