using UnityEngine;
using System.Net.Sockets;
using System.Runtime.InteropServices;

public class ControleMao : MonoBehaviour
{
    private TcpClient client;
    private NetworkStream stream;

    // Objetos dos dedos
    public Transform polegar;
    public Transform indicador;
    public Transform medio;
    public Transform anelar;
    public Transform mindi;

    private void Start()
    {
        client = new TcpClient("127.0.0.1", 65432);
        stream = client.GetStream();
    }

    private void Update()
    {
        if (stream.DataAvailable)
        {
            byte[] data = new byte[20]; // 5 floats (4 bytes cada)
            stream.Read(data, 0, data.Length);

            float[] fingerAngles = new float[5];
            for (int i = 0; i < 5; i++)
            {
                fingerAngles[i] = BitConverter.ToSingle(data, i * 4);
            }

            // Atualizar posicao
            UpdateFingers(fingerAngles);
        }
    }

    private void UpdateFingers(float[] angles)
    {
        polegar.localRotation = Quaternion.Euler(0, 0, angles[0] * 90); //Colocar mapeamento correto!!
        indicador.localRotation = Quaternion.Euler(0, 0, angles[1] * 90);//Colocar mapeamento correto!!
        medio.localRotation = Quaternion.Euler(0, 0, angles[2] * 90);//Colocar mapeamento correto!!
        anelar.localRotation = Quaternion.Euler(0, 0, angles[3] * 90);//Colocar mapeamento correto!!
        mindi.localRotation = Quaternion.Euler(0, 0, angles[4] * 90);//Colocar mapeamento correto!!
    }

    private void OnApplicationQuit()
    {
        stream.Close();
        client.Close();
    }
}
