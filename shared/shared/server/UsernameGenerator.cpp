#include "UsernameGenerator.h"
#include "../utils/MathUtils.h"
#include <algorithm>

namespace card {
	std::string generateUsername() {
		static std::vector<std::string> const usernames = {
"Marie", "Sophie", "Emilia", "Paul", "Maria", "Alexander", "Maximilian", "Anton", "Johanna", "Elias", "Jonathan", "Anna", "Felix", "Jakob", "Elisabeth", "Noah", "Clara", "Emma", "Julius", "Julian", "Ella", "Benjamin", "David", "Mia", "Leo", "Karl", "Ben", "Leon", "Louis", "Johann", "Jonas", "Tim", "Helene", "Mila", "Luis", "Elisa", "Greta", "Marlene", "Lukas", "Moritz", "Amelie", "Antonia", "Hannah", "Lea", "Linus", "Luisa", "Daniel", "Lara", "Lina", "Laura", "Aaron", "Niklas", "Valentin", "Frieda", "Julia", "Sarah", "Viktoria", "Bruno", "Carl", "Raphael", "Rosa", "Adrian", "Jan", "Liam", "Luca", "Philipp", "Vincent", "Amalia", "Maja", "Paula", "Benedikt", "Finn", "Max", "Hanna", "Isabella", "Leni", "Valentina", "Simon", "Tom", "Alexandra", "Emily", "Lilly", "Richard", "Samuel", "Thomas", "Carla", "Eva", "Lena", "Leonie", "Christian", "Fabian", "Fritz", "Michael", "Peter", "Carlotta", "Klara", "Adam", "Caspar", "Gabriel", "Martin", "Mateo", "Nele", "Andreas", "Robert", "Sebastian", "Nora", "Olivia", "Alina", "Felicitas", "Mira", "Miriam", "Franz", "Rafael", "Timo", "Tobias", "Victor", "Annika", "Christine", "Lisa", "Lucia", "Magdalena", "Nina", "Florian", "Klaus", "Ludwig", "Nils", "Stefan", "Alice", "Chiara", "Elena", "Esther", "Finja", "Marta", "Pia", "Manuel", "Matteo", "Nikolas", "Oliver", "William", "Amelia", "Annabell", "Anne", "Barbara", "Caroline", "Claire", "Felicia", "Jasmin", "Julie", "Karla", "Karolina", "Linda", "Bernhard", "Christoph", "Emanuel", "Mario", "Maurice", "Robin", "Sam", "Walter", "Werner", "Willi", "Agnes", "Celine", "Christina", "Diana", "Fanny", "Livia", "Nelly", "Nicole", "Veronika", "Frank", "Jack", "Julien", "Marcel", "Matheo", "Matthias", "Michel", "Mike", "Pascal", "Amy", "Angelika", "Annabelle", "Aurora", "Brigitte", "Denise", "Ellen", "Erika", "Fabienne", "Ines", "Inge", "Jana", "Joanna", "Lene", "Bernd", "Margareta", "Carlo", "Melina", "Nathalie", "Rachel", "Susanne", "Erwin", "Harry", "Valeria", "Hermann", "Jannis", "Kai", "Kevin", "Lars", "Laurin", "Marvin", "Nico", "Ricardo", "Roman", "Sven", "Alexa", "Amely", "Ana", "Andrea", "Annalena", "Bella", "Elissa", "Alex", "Heidi", "Jessica", "Dirk", "Karina", "Kerstin", "Eduard", "Kim", "Mimi", "Nadine", "Ronja", "Sabine", "Selina", "Mark", "Markus", "Marlon", "Simone", "Massimo", "Stephanie", "Toni", "Valerie", "Vanessa", "Vera", "Vivien", "Nicholas", "Amalie", "Annelies", "Beate", "Bettina", "Bibi", "Birgit", "Benedict", "Coco", "Cornelia", "Carsten", "Christan", "Elke", "Dieter", "Emmy", "Fibi", "Gabriela", "Fabio", "Helmut", "Jessy", "Juliane", "Joe", "Carina", "Annemarie", "Michelle"
		};

		std::size_t index = randomInRange<std::size_t>(0, usernames.size() - 1);
		return usernames[index];
	}
	std::string generateUsernameNotIn(const std::vector<std::string>& otherUsernames) {
		std::string generated;
		do {
			generated = generateUsername();
		} while(std::find(otherUsernames.cbegin(), otherUsernames.cend(), generated) != otherUsernames.cend());

		return generated;
	}
}
