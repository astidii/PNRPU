using System;

class Program()
{
    static void Main()
    {
         string path = "C:\\Users\\Asus\\Desktop\\testFiles\\g42.txt";
         int[,] connectivityMatrix = ReadMatrixFromFile(path);

       /* int[,] connectivityMatrix =
        {
            { 0, 1, 3, 2 },
            { 2, 0, 2, 0 },
            { 0, 0, 0, 0 },
            { 0, 2, 1, 0 }
        };*/

        ShowMatrix(connectivityMatrix);
        


        int k;
        while (true)
        {
            Console.Write("Введите длину пути: ");
           
            if (int.TryParse(Console.ReadLine(), out k) && k > 0)
            {
                Console.WriteLine(new string('-', 40));
                break;
            }
            Console.WriteLine("Ошибка ввода! Введите целое положительное число.");

          
        }


        bool isMinPath;
        while (true)
        {
            Console.Write("Искать пути минимальной длины или максимальной? (min/max): ");
            string mode = Console.ReadLine().Trim().ToLower();

            if (mode == "min")
            {
                isMinPath = true;
                Console.WriteLine(new string('-', 40));
                break;
            }

            else if (mode == "max")
            {
                isMinPath = false;
                Console.WriteLine(new string('-', 40));
                break;
            }

            else
            {
                Console.WriteLine("Ошибка ввода! Введите 'min' или 'max'.");
            }
        }


        int vertexCount = connectivityMatrix.GetLength(0);

        ShowMatrix(ShimbellAlgorithm(connectivityMatrix, vertexCount, k, isMinPath));

    }

    static int[,] ReadMatrixFromFile(string path)
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

    static void ShowMatrix(int[,] matrix)
    {
        Console.WriteLine("Матрица связности графа:\n");

        for (int i = 0; i < matrix.GetLength(0); i++)
        {
            for (int j = 0; j < matrix.GetLength(1); j++)
            {
                Console.Write(matrix[i, j] + " ");
            }
            Console.WriteLine();
        }

        Console.WriteLine(new string('-', 40));
    }


    static int[,] ShimbellAlgorithm(int[,] matrix, int vertexCount, int k, bool isMinPath)
    {
        int[,] current = (int[,])matrix.Clone(); //Копия матрицы

        for (int step = 2; step <= k; step++) 
        {
            int[,] next = new int[vertexCount, vertexCount];

            for(int i = 0; i < vertexCount; i++)
            {
                for (int j = 0; j < vertexCount; j++)
                {
                    {
                        int pathValue = isMinPath ? int.MaxValue : int.MinValue;

                        for (int m = 0; m < vertexCount; m++)
                        {
                            if (current[i, m] > 0 && (matrix[m, j] > 0))
                            {
                                int newValue = current[i, m] + matrix[m, j];

                                if (isMinPath)
                                {
                                    pathValue = Math.Min(pathValue, newValue);
                                }

                                else
                                {
                                    pathValue = Math.Max(pathValue, newValue);
                                }
                            }
                        }

                        next[i, j] = (isMinPath && pathValue == int.MaxValue) || (!isMinPath && pathValue == int.MinValue) ? 0 : pathValue;
                    }
                }

            }
            current = next;
        }
        return current;
    }

}
