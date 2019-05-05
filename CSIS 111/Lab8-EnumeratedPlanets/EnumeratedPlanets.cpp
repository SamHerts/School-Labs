//CSIS111-Lab8-EnumeratedPlanets-New
//CSIS 111-001

#include <iostream>
#include <string>

using namespace std;

enum PlanetType { Mercury, Venus, Earth, Moon, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto, Unknown };
void planetPrinter(PlanetType, string, float, float);

int main()
{
	cout << "Samuel Herts -- Lab 8" << endl << endl;

	string yourPlanet;//user input as string
	PlanetType planet;//user input as enumerated type
	float weight, weightFactor; //calculating weight
	bool flag = true;//correct user input

	while (flag == true) {
		cout << "Enter your weight: ";
		cin >> weight;

		cout << "Name a planet: ";
		cin >> yourPlanet;
		flag = false;
		if (yourPlanet == "Mercury") {//IF case to turn string to enumerated type and set weightfactor
			planet = Mercury;
			weightFactor = 0.4155;
		}
		else if (yourPlanet == "Venus") {
			planet = Venus;
			weightFactor = 0.8975;
		}
		else if (yourPlanet == "Earth") {
			planet = Earth;
			weightFactor = 1.0;
		}
		else if (yourPlanet == "Moon") {
			planet = Moon;
			weightFactor = 0.166;
		}
		else if (yourPlanet == "Mars") {
			planet = Mars;
			weightFactor = 0.3507;
		}
		else if (yourPlanet == "Jupiter") {
			planet = Jupiter;
			weightFactor = 2.5374;
		}
		else if (yourPlanet == "Saturn") {
			planet = Saturn;
			weightFactor = 1.0677;
		}
		else if (yourPlanet == "Uranus") {
			planet = Uranus;
			weightFactor = 0.8947;
		}
		else if (yourPlanet == "Neptune") {
			planet = Neptune;
			weightFactor = 1.1794;
		}
		else if (yourPlanet == "Pluto") {
			planet = Pluto;
			weightFactor = 0.0899;
		}
		else {//Input error checking
			cout << "Unknown Planet. Please enter a planet with a capital letter, " << endl << "from Mercury to Pluto, including Moon" << endl;
			flag = true;
		}
	}

	planetPrinter(planet, yourPlanet, weightFactor, weight);//send values to printing function

	system("pause");
	return 0;
}

void planetPrinter(PlanetType whichPlanet, string yourPlanet, float weightFactor, float weight) {//Lab needed switch case, so I added a switch case.
	switch (whichPlanet) {
	case Mercury: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Venus: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Earth: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Moon: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Mars: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Jupiter: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Saturn: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Neptune: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Uranus: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	case Pluto: cout << "Your weight on " << yourPlanet << " is " << weightFactor*weight << ". " << endl; break;
	}
}
