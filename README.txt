Proiectarea cu microprocesoare, seria CC
Proiect - Minesweeper pe ecran Nokia

@autor: Cristian-Andrei Sandu
@grupa: 333CC
@asistent: Ana Aldescu

User input: 6 butoane
	- sus, jos, stanga, dreapta
	- select(stanga-jos) <=> click-stanga din versiunea pentru desktop
	- flag(dreapta-jos) <=> click-dreapta din versiunea pentru desktop

Pentru compilare, e nevoie de avr-gcc, care vine la pachet, pe windows, cu
WinAVR.

Se genereaza fisierul .hex prin rularea make si acesta se incarca pe
microcontroller folosind utilitarul pentru bootloader.
