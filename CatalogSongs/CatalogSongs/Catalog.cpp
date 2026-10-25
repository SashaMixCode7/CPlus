#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;



struct Song {
	string title;
	string author;
	string lyrics;
	int year;

	Song(string t, string a, string l, int y) : title(t), author(a), lyrics(l), year(y) {}
	string listSongs() const
	{
		string result = "";
		result += " title songs:" + title + "\n";
		result += " author songs:" + author + "\n";
		result += " year of songs:" + to_string(year) + "\n";
		result += " lyrics songs:" + lyrics + "\n";
		return result;
	}
};


struct LyricsofSongs {
	vector<Song> songs;

	//Добавляем текст песни.
	void addSong(Song song) {
		songs.push_back(song);
	}

	//Изменяем текст песни.
	void editSong(int index, const Song& s) {
		if (index >= 0 && index < (int)songs.size()) songs[index] = s;
	}

	//Удаляем текст песни.
	void deleteSong(int index) {
		if (index >= 0 && index < (int)songs.size()) {
			songs.erase(songs.begin() + index);
		}
	}

	string toLower(string s) const {
		transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return (char)std::tolower(c); });
		return s;
	}


	//Поиск  песен одного автора.
	vector<Song> displaySongsofAuthor(const string& author) const {
		vector<Song> r;
		string a = toLower(author);
		for (const auto& s : songs)
			if (toLower(s.author) == a) r.push_back(s);
		return r;
	}

	//Поиск песен содержащее слово введенное пользователем.

	vector<Song> displaySongContainingWord(const string& word) const {
		vector<Song> r;
		string w = toLower(word);
		for (const auto& s : songs)
			if (toLower(s.lyrics).find(w) != string::npos) r.push_back(s);
		return r;
	}

};


struct DirectorySongs {
	//Отображаем песню
	/*void showSong(Song& songs) {
		cout << songs.listSongs() << "\n";
	}*/

	// Сохраняем текст в файл
	void saveSongs(Song& songs, string& filename) const{
		ofstream file(filename);
		file << songs.listSongs();
		file.close();
	}

	string ReadFile(string& filename) const {
		ifstream file(filename);
		if (!file.is_open()) return "";
		string str, line;
		while (getline(file, line)) str += line + "\n";
		return str;
	}

	void WriteFile(const string& filename, const string& str) const {
		ofstream file(filename);
		if (!file.is_open()) { cout << "Не удалось открыть файл\n"; return; }
		file << str;
	}

	// Отображение песни
	void displaySong(const Song& songs) const{
		cout << songs.listSongs() << endl;
	}

	void saveAll(const LyricsofSongs& pl, const string& filename) const {
		string data;
		for (const auto& s : pl.songs) data += s.listSongs() + "\n";
		WriteFile(filename, data);
		cout << "Сохранено\n";
	}

	void loadFromFile(LyricsofSongs& pl, const string& filename) const {
		ifstream f(filename);
		if (!f.is_open()) { cout << "Файл не найден, каталог пуст.\n"; return; }

		string line, title, author, lyrics;
		int year = 0;
		while (getline(f, line)) {
			if (line.rfind(" title songs:", 0) == 0)       title = line.substr(13);
			else if (line.rfind(" author songs:", 0) == 0) author = line.substr(14);
			else if (line.rfind(" year of songs:", 0) == 0) year = stoi(line.substr(15));
			else if (line.rfind(" lyrics songs:", 0) == 0) {
				lyrics = line.substr(14);
				pl.addSong(Song(title, author, lyrics, year));
				title.clear(); author.clear(); lyrics.clear(); year = 0;
			}
		}
	}

	void MenuSongs() {
		LyricsofSongs  playlist;
		loadFromFile(playlist, "songs.txt");

		int choice = 0;
		do
		{

			cout << "1-Добавить текст\n";
			cout << "2-Изменить текст\n";
			cout << "3-Удалить текст\n";
			cout << "4-Отобразить текст\n";
			cout << "5-Поиск песен по автору\n";
			cout << "6-Поиск песен по введенному слову\n";
			cout << "7-Сохранение в файл\n";
			cout << "8-поиск автор и слово\n";
			cout << "9-Выход\n";
			cin >> choice;
			cin.ignore();


			switch (choice) {


			case 1: {
				string title, author, lyrics; int year;
				cout << "Запишите название песни:";
				getline(cin, title);
				cout << "Запишите автора:";
				getline(cin, author);
				cout << "Запишите год песни:";
				if (!(cin >> year)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					year = 0;
				}
				cin.ignore();
				cout << "Вводим текст песни:\n";
				getline(cin, lyrics);


				playlist.addSong(Song(title, author, lyrics, year));
				break;

			}

			case 2: {
				int songIndex;
				cout << "Индекс песни: "; cin >> songIndex; cin.ignore();
				if (songIndex < 0 || songIndex >= (int)playlist.songs.size()) { cout << "Нет такой\n"; break; }
				string title, author, lyrics; int year;
				cout << "Название песни: "; getline(cin, title);
				cout << "Автор песни: ";    getline(cin, author);
				cout << "Год песни: ";      if (!(cin >> year)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					year = 0;
				}
				cin.ignore();
				cout << "Текст песни: ";    getline(cin, lyrics);
				playlist.editSong(songIndex, Song(title, author, lyrics, year));
				break;
			}
			case 3: {
				int songIndex;
				cout << "Удалите песню";
				cin >> songIndex; cin.ignore();
				if (songIndex >= 0 && songIndex < (int)playlist.songs.size()) {
					playlist.deleteSong(songIndex);
					cout << "Удалено\n";
				}
				else {
					cout << "Нет такой\n";
				}
				break;

			}

			case 4: {
				int songIndex;
				cout << "Индекс: "; cin >> songIndex; cin.ignore();
				if (songIndex >= 0 && songIndex < (int)playlist.songs.size())
					displaySong(playlist.songs[songIndex]);
				else cout << "Нет такой\n";
				break;
			}
			case 5: {
				string author;
				cout << "Введите имя автор: "; getline(cin, author);
				for (const auto& s : playlist.displaySongsofAuthor(author)) displaySong(s);
				break;
			}
			case 6: {
				string word;
				cout << "Введите слово: "; getline(cin, word);
				for (const auto& s : playlist.displaySongContainingWord(word)) displaySong(s);
				break;
			}
			case 7: {
				string filesong;
				cout << "Введите файл для сохранения: "; getline(cin, filesong);
				saveAll(playlist, filesong);
				break;
			}
			case 8: {
				string a, w;
				cout << "Автор: "; getline(cin, a);
				cout << "Слово: "; getline(cin, w);
				for (const auto& s : FindSong(playlist, a, w))
					displaySong(s);
				break;
			}
			case 9: return;
			default:cout << "вы неправильно ввели номер!";

			}
		} while (true);


	}

	vector <Song> FindSong(LyricsofSongs& playlist, const string& author, const string& word) const
	{

		vector<Song> result;
		for (const auto& s : playlist.songs) {
			bool okAuthor = author.empty() || playlist.toLower(s.author) == playlist.toLower(author);
			bool okWord = word.empty() || playlist.toLower(s.lyrics).find(playlist.toLower(word)) != string::npos;
			if (okAuthor && okWord) result.push_back(s);
		}
		return result;
	}
};


int main()
{
	setlocale(LC_ALL, "");

	DirectorySongs  sM;
	sM.MenuSongs();

	return 0;
}