using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

public class GraphForm : Form
{
    static string path = "C:\\Users\\Asus\\Desktop\\testFiles\\g24.txt";
    private int[,] adjacencyMatrix = Program.ReadConnectivityMatrixFromFile(path);
    

    private int vertexCount;
    private PointF[] leftGraphVertices;
    private PointF[] rightGraphVertices;
    private int[,] spanningTreeMatrix;

    public GraphForm()
    {
        this.Text = "Граф в Windows Forms";
        this.Size = new Size(800, 500);
        this.vertexCount = adjacencyMatrix.GetLength(0);
        this.leftGraphVertices = new PointF[vertexCount];
        this.rightGraphVertices = new PointF[vertexCount];
        this.Paint += new PaintEventHandler(DrawGraph);
        this.DoubleBuffered = true;
        this.Resize += (s, e) => this.Invalidate();

        spanningTreeMatrix = Program.AlgorithmPrima(adjacencyMatrix);
    }

    void DrawOutlinedText(Graphics g, string text, Font font, Brush fillBrush, Brush outlineBrush, float x, float y, StringFormat format)
    {
        // Рисуем обводку черным цветом (слегка смещаем текст в разные стороны)
        g.DrawString(text, font, outlineBrush, x - 1, y, format);
        g.DrawString(text, font, outlineBrush, x + 1, y, format);
        g.DrawString(text, font, outlineBrush, x, y - 1, format);
        g.DrawString(text, font, outlineBrush, x, y + 1, format);

        // Рисуем основной текст поверх обводки
        g.DrawString(text, font, fillBrush, x, y, format);
    }

    private void DrawGraph(object sender, PaintEventArgs e)
    {
        Graphics g = e.Graphics;
        g.Clear(this.BackColor);
        g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

        Brush vertexBrush = Brushes.LightBlue;
        Font font = new Font("Arial", 12);
        Brush vertexTextBrush = Brushes.Black;
        Brush edgeWeightTextBrush = Brushes.Red;
        Pen edgePen = new Pen(Color.Black, 2);
        Pen spanningTreePen = new Pen(Color.Black, 2);

        int radius = 150;
        int centerLeftX = this.ClientSize.Width / 4;
        int centerRightX = 3 * this.ClientSize.Width / 4;
        int centerY = this.ClientSize.Height / 2;
        int vertexSize = 30;

        // Выравнивание текста
        StringFormat format = new StringFormat();
        format.Alignment = StringAlignment.Center;
        format.LineAlignment = StringAlignment.Center;


        for (int i = 0; i < vertexCount; i++)
        {
            double angle = 2 * Math.PI * i / vertexCount;
            int xLeft = centerLeftX + (int)(radius * Math.Cos(angle));
            int yLeft = centerY + (int)(radius * Math.Sin(angle));
            leftGraphVertices[i] = new PointF(xLeft, yLeft);

            int xRight = centerRightX + (int)(radius * Math.Cos(angle));
            int yRight = centerY + (int)(radius * Math.Sin(angle));
            rightGraphVertices[i] = new PointF(xRight, yRight);
        }


        // Отрисовка рёбер основного графа
        for (int i = 0; i < vertexCount; i++)
        {
            for (int j = i + 1; j < vertexCount; j++)
            {
                if (adjacencyMatrix[i, j] != 0)
                {
                    g.DrawLine(edgePen, leftGraphVertices[i], leftGraphVertices[j]);


                    // Координаты центра ребра (для веса ребра)
                    float midX = (leftGraphVertices[i].X + leftGraphVertices[j].X) / 2;
                    float midY = (leftGraphVertices[i].Y +  leftGraphVertices[j].Y) / 2;


                    string weightText = adjacencyMatrix[i, j].ToString();
                    RectangleF textRect = new RectangleF(midX - 10, midY - 10, 20, 20);
                    DrawOutlinedText(g, adjacencyMatrix[i, j].ToString(), font, Brushes.Cyan, Brushes.Black, midX, midY, format);

                }
            }
        }


        // Отрисовка рёбер остовного графа
        for (int i = 0; i < vertexCount; i++)
        {
            for (int j = i + 1; j < vertexCount; j++)
            {
                if (spanningTreeMatrix[i, j] != 0)
                {
                    g.DrawLine(spanningTreePen, rightGraphVertices[i], rightGraphVertices[j]);

                    float midX = (rightGraphVertices[i].X + rightGraphVertices[j].X) / 2;
                    float midY = (rightGraphVertices[i].Y + rightGraphVertices[j].Y) / 2;
                    DrawOutlinedText(g, spanningTreeMatrix[i, j].ToString(), font, Brushes.Cyan, Brushes.Black, midX, midY, format);
                }
            }
        }


        // Отрисовка вершин
        for(int i = 0;i < vertexCount;i++)
        {
            float xLeft = leftGraphVertices[i].X - vertexSize / 2;
            float yLeft = leftGraphVertices[i].Y - vertexSize / 2;
            g.FillEllipse(vertexBrush, xLeft, yLeft, vertexSize, vertexSize);
            g.DrawEllipse(Pens.Black, xLeft, yLeft, vertexSize, vertexSize);
            RectangleF textRectLeft = new RectangleF(xLeft, yLeft, vertexSize, vertexSize);
            g.DrawString(Convert.ToChar('A' + i).ToString(), font, vertexTextBrush, textRectLeft, format);


            float xRight = rightGraphVertices[i].X - vertexSize / 2;
            float yRight = rightGraphVertices[i].Y - vertexSize / 2;
            g.FillEllipse(vertexBrush, xRight, yRight, vertexSize, vertexSize);
            g.DrawEllipse(Pens.Black, xRight, yRight, vertexSize, vertexSize);
            RectangleF textRectRight = new RectangleF(xRight, yRight, vertexSize, vertexSize);
            g.DrawString(Convert.ToChar('A' + i).ToString(), font, vertexTextBrush, textRectRight, format);
        }



    }

    [STAThread]
    public static void Main()
    {
        Application.Run(new GraphForm());
    }
}
           
                
