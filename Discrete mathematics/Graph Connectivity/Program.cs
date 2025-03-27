using System;

class Program
{
    static bool[] visited;
    static void Main()
    {
        string path = "C:\\Users\\Asus\\Desktop\\testFiles\\g14.txt";
        int[,] connetivityMatrix = ReadMatrixFromFile(path);
        ShowMatrix(connetivityMatrix);
        ShowComponents(CountConnectivityComponents(connetivityMatrix));
        ShowMatrix(WarshallAlgorithm(connetivityMatrix));
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
        for (int i = 0; i < matrix.GetLength(0);i++)
        {
            for (int j = 0;j < matrix.GetLength(1);j++)
            {
                Console.Write(matrix[i, j] + " ");
            }
            Console.WriteLine();
        }
    }


    static void DFS(int startVertex, int[,] matrix, List<int> component)
    {
        Stack<int> stack = new Stack<int>();
        stack.Push(startVertex);

        while (stack.Count > 0)
        {
            int v = stack.Pop();
            if (!visited[v])
            {
                visited[v] = true;
                component.Add(v);
                for(int i = 0; i < matrix.GetLength(0); i++)
                {
                    if (matrix[v, i] == 1 && !visited[i])
                    {
                        stack.Push(i);
                    }
                }
            }

        }
    }

    static List<List<int>> CountConnectivityComponents(int[,] matrix)
    {
        int vertexQuantity = matrix.GetLength(0);
        visited = new bool[vertexQuantity];
        List <List<int>> components = new List<List<int>>();

        for (int i = 0; i < vertexQuantity; i++)
        {
            if (!visited[i])
            {
                List<int> component = new List<int>();
                DFS(i, matrix, component);
                components.Add(component);
            }
        }
        return components;

    }

    static void ShowComponents(List<List<int>> components)
    {
        Console.WriteLine($"\nКоличество компонент связности: {components.Count}");

        for (int i = 0; i < components.Count; i++)
        {
            Console.WriteLine(new string('-', 40)); 
            Console.WriteLine($"Компонента {i + 1}:");
            Console.WriteLine($"Количество вершин: {components[i].Count}");
            Console.WriteLine($"Вершины: {string.Join(", ", components[i].Select(v => v + 1))}");
        }
        Console.WriteLine(new string('-', 40)); 
    }

    static int[,] WarshallAlgorithm(int[,] matrix)
    {
        int veretexQuantity = matrix.GetLength(0);
        int[,] reachabilityMatrix = (int[,])matrix.Clone();

        for (int k = 0; k < veretexQuantity; k++)
        {
            for (int i = 0; i < veretexQuantity; i++)
            {
                for (int j = 0; j < veretexQuantity; j++)
                {
                    if (reachabilityMatrix[i, k] == 1 && reachabilityMatrix[k, j] == 1)
                    {
                        reachabilityMatrix[i, j] = 1;
                    }
                }
            }
        }

        return reachabilityMatrix;
    }
}

