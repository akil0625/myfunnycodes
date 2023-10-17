#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "olcNoiseMaker.h"
using namespace std;
#define pi 3.1415926

atomic<double> dFrequencyOutput = 0.0;

double MakeNoise(double dTime)
{
	return 0.5 * sin(dFrequencyOutput * 2 * pi*dTime) > 0 ? 0.2 : -0.2;
}


int main()
{
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();
	if (!devices.empty())
		wcout << "connected to audio device!" << endl;

	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	sound.SetUserFunction(MakeNoise);

	double dOctaveBaseFrequency = 220;//A2
	double d12thRootOf2 = pow(2, 1.0 / 12.0);
	int nCurrentKey = -1;
	bool bKeyPressed;
	wcout << endl <<
		"|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << endl <<
		"|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << endl <<
		"|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << endl <<
		"|     |     |     |     |     |     |     |     |     |     |" << endl <<
		"|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |" << endl <<
		"|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << endl << endl;
	while (1)
	{
		bKeyPressed = false;
		for (int k = 0; k < 16; k++)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000)
			{
				if (nCurrentKey != k)
				{
					dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
					wcout << "\rNote On : " << sound.GetTime() << "s " << dFrequencyOutput << "Hz";
					nCurrentKey = k;
				}

				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			if (nCurrentKey != -1)
			{
				wcout << "\rNote Off: " << sound.GetTime() << "s                        ";
				nCurrentKey = -1;
			}

			dFrequencyOutput = 0.0;
		}
	}
	

	return 0;
}