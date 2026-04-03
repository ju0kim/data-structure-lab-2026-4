#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Music {
private:
	string title;
	string artist;
	string album;
	int year;

public:
	//생성자
	Music(string title, string artist, string album, int year) {
		this->title = title;
		//title= t 이렇게 써도 ㄱㅊ (이건 매개변수 타이틀임)
		this->artist = artist;
		this->album = album;
		this->year = year;


	}
	//getters

	string getTitle() { return title; }
	//앵 그러면 이건 다른 타이틀인가 (제미나이한테 물어보기)
	string getArtist() { return artist; }
	string getAlbum() { return album; }
	int getYear() { return year; }
	

	//music streaming service class
	class MusicStreamingService {
	private:
		string serviceName;
		vector<Music> musicList;

	public:
		MusicStreamingService(string serviceName) {
			this->serviceName = serviceName;

		}
		//Music 객체를 service에추가

		void addMusic(string title, string artist, string album, int year) {
			Music newMusic(title, artist, album, year);
			musicList.push_back(newMusic);
			cout << title << "by " << artist << "added to " << serviceName << endl;
		}

		//타이틀로 검색하기
		Music* searchByTitle(string title) {
			for (int i = 0; i < musicList.size(); i++) {
				if (musicList[i].getTitle() == title)
					return &musicList[i];

			}
			return NULL;
		}


	};
	



};