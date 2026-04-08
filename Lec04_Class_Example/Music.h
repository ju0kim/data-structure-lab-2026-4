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
	//앵 그러면 이건 다른 타이틀인가
	string getArtist() { return artist; }
	string getAlbum() { return album; }
	int getYear() { return year; }

};

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
					// return &music~ 이거랑 musicList+i 이거랑 똑같음

			}
			return NULL;
		}
		//아티스트로검색하기
		vector<Music*>searchByArtist(string artist) {
			vector<Music*> result;
			//musiclist에 있는아티스트와 입력아티스트가 같으면 리서트에 푸시백
			for (int i = 0; i < musicList.size(); i++) {
				if (musicList[i].getArtist() == artist) {
					result.push_back(&musicList[i]);
				}
			}
			return result;
		}

	};
	


