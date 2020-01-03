#include "UsernameGenerator.h"
#include "../utils/MathUtils.h"

namespace card {
	std::string generateUsername() {
		static std::vector<std::string> const usernames = {
			"Marie", "Sophie", "Emilia", "Paul", "Maria", "Alexander", "Luise", "Maximilian", "Anton", "Johanna", "Elias", "Emil", "Jonathan", "Anna", "Felix", "Jakob", "Elisabeth", "Noah", "Clara", "Emma", "Julius", "Julian", "Ella", "Helena", "Benjamin", "David", "Mia", "Leo", "Karl", "Ben", "Leon", "Louis", "Johann", "Johannes", "Theodor", "Friedrich", "Jonas", "Tim", "Helene", "Mila", "Luis", "Elisa", "Greta", "Marlene", "Lukas", "Moritz", "Amelie", "Antonia", "Hannah", "Lea", "Linus", "Luisa", "Mathilda", "Daniel", "Oskar", "Lara", "Lina", "Frederik", "Ida", "Josephine", "Laura", "Aaron", "Niklas", "Valentin", "Frieda", "Julia", "Sarah", "Viktoria", "Bruno", "Carl", "Raphael", "Katharina", "Matilda", "Pauline", "Rosa", "Adrian", "Jan", "Liam", "Luca", "Philipp", "Vincent", "Amalia", "Josefine", "Maja", "Paula", "Benedikt", "Finn", "Joshua", "Max", "Alma", "Hanna", "Isabella", "Leni", "Theresa", "Valentina", "Georg", "Henrik", "Justus", "Leander", "Luka", "Simon", "Tom", "Alexandra", "Emily", "Lilly", "Ferdinand", "Konstantin", "Richard", "Samuel", "Thomas", "Carla", "Eva", "Lena", "Leonie", "Christian", "Fabian", "Fritz", "Hugo", "Leopold", "Michael", "Peter", "Carlotta", "Klara", "Adam", "Caspar", "Frederick", "Gabriel", "Martin", "Mateo", "Nele", "Andreas", "August", "Robert", "Sebastian", "Florentine", "Lotta", "Margarete", "Nora", "Olivia", "Wolfgang", "Alina", "Felicitas", "Franziska", "Frida", "Lotte", "Mira", "Miriam", "Franz", "Oscar", "Rafael", "Timo", "Tobias", "Victor", "Annika", "Christine", "Henriette", "Lisa", "Lucia", "Magdalena", "Mara", "Nina", "Florian", "Gustav", "Klaus", "Lennox", "Ludwig", "Nils", "Stefan", "Alice", "Chiara", "Elena", "Esther", "Finja", "Isabelle", "Marta", "Mathilde", "Milla", "Paulina", "Pia", "Edgar", "Gregor", "Heinrich", "John", "Lennard", "Luke", "Manuel", "Matteo", "Nikolas", "Oliver", "William", "Amelia", "Annabell", "Anne", "Barbara", "Caroline", "Claire", "Felicia", "Friederike", "Isabell", "Jasmin", "Julie", "Juna", "Karla", "Karlotta", "Karolina", "Linda", "Albert", "Arda", "Bernhard", "Christoph", "Darian", "Elia", "Emanuel", "Filip", "Fynn", "Gerhard", "Hannes", "Hans", "Hendrik", "Mario", "Maurice", "Robin", "Sam", "Toni", "Tristan", "Walter", "Werner", "Wilhelm", "Willi"
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
