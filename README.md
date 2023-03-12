# Analiza unui sunet în domeniul timp și frecvență

Acest proiect își propune analiza datelor dintr-un semnal audio în domeniul timp și domeniul frecvență. Analiza în domeniul timp constă în calcularea valorilor de minim, medie, maxim, indexul minim, indexul maxim, dispersie, mediana, frecvența de
eșantionare, numărul de eșantioane, ulterior și totalul de zero-crossing. 

Analiza în domeniul timp presupune și filtarea semnalului prin mediere (pe 16 sau 32 de elemente)
și filtru de ordin I cu alpha în intervalul (0,1), precum și aplicarea anvelopei semnalului.

Analiza în domeniul frecvență constă în reprezentarea spectrului și în aplicarea a doua tipuri de ferestre: Blackman si Hamming și a două tipuri de filtre: Kaiser trece sus
și Chebyshev I trece jos cu fpass = 1200 Hz si fstop = 1450.
