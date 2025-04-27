using System;

class Program
{
    static void Main()
    {
        string word;
        do
        {
            Console.WriteLine(new string('-', 39));
            Console.Write("Введите слово для проверки (0 - Выход): ");
            if ((word = Console.ReadLine()) != "0")
                Console.WriteLine(RunAutomat(word) ? "<<Принадлежит языку>>" : "<<Не принадлежит языку>>");
        }
        while (word != "0");
    }

    enum State
    {
        Start,
        FirstC,
        EvenB,
        OddB,
        Finish
    }

    static bool RunAutomat(string word)
    {
        State currentState = State.Start;

        foreach (char letter in word)
        {
            switch (currentState)
            {
                case State.Start:
                    if (letter == 'c') currentState = State.FirstC;
                    else currentState = State.Finish;
                    break;

                case State.FirstC:
                    if (letter == 'c') currentState = State.EvenB;
                    else currentState = State.Finish;
                    break;

                case State.EvenB:
                    if (letter == 'b') currentState = State.OddB;
                    else if (letter == 'a' || letter == 'd' || letter == 'c')
                    {
                        currentState = State.EvenB;
                    }
                    else currentState = State.Finish;
                    break;

                case State.OddB:
                    if (letter == 'b') currentState = State.EvenB;
                    else if (letter == 'a' || letter == 'd' || letter == 'c')
                    {
                        currentState = State.OddB;
                    }
                    else currentState = State.Finish;
                    break;

                case State.Finish:
                    return false;
            }
        }

        return currentState == State.EvenB;
    }
}


