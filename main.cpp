#include <iostream>
#include <fstream>
using namespace std;

class Date;
class SocialNetworkApp;
class Object;
class Page;
class User;
class Post;
class Activity;
class Comment;

class Date {
private:
	int day;
	int month;
	int year;
public:
	static Date todays_date;
	void ReadFromFile(ifstream& posts_file) {
		posts_file >> day;
		posts_file >> month;
		posts_file >> year;
	}
	void SetDate(int d, int m, int y) {
		day = d;
		month = m;
		year = y;
	}
	void PrintDate() {
		cout << day << "/" << month << "/" << year;
	}
	int GetYear() {
		return year;
	}
	bool CheckIfLatest() {
		if (todays_date.day - day <= 1 && month == todays_date.month && year == todays_date.year)
			return true;
		return false;
	}
	bool operator<(Date& date2) {
		if (year < date2.year)
			return true;
		else if (year > date2.year)
			return false;
		else {
			if (month < date2.month)
				return true;
			else if (month > date2.month)
				return false;
			else {
				if (day < date2.day)
					return true;
				else if (day > date2.day)
					return false;
				else
					return false;
			}
		}
			
	}
	bool CheckIfMemory() {
		if (day == todays_date.day && month == todays_date.month && year < todays_date.year)
			return true;
		return false;
	}
	int CompareDate(int old_year) {
		return (year - old_year);
	}
};

Date Date::todays_date;

class Object {
private:
	char* ID;
public:
	~Object(){
		//cout << "object deleted\n";
		if (ID) {
			delete[] ID;
		}
	}
	void SetID(const char* temp) {
		int length = strlen(temp);
		ID = new char[length + 1];
		strcpy_s(ID, length + 1, temp);
	}
	char* GetID() {
		return ID;
	}
	void PrintID() {
		cout << ID;
	}
	virtual void AddPostToTimeline(Post* post) = 0;
	virtual void PrintName() = 0;
	virtual void PrintListView() = 0;
};

class Activity {
private:
	int type;
	char* value;
public:
	~Activity() {
		if (value)
			delete[] value;
	}
	void ReadFromFile(ifstream& posts_file) {
		posts_file >> type;
		char temp[50];
		posts_file.ignore();
		posts_file.getline(temp, 50);
		int length = strlen(temp);
		value = new char[length + 1];
		strcpy_s(value, length + 1, temp);
	}
	void Print() {
		if (type == 1)
			cout << " is feeling ";
		else if (type == 2)
			cout << " is thinking about ";
		else if (type == 3)
			cout << " is making ";
		else if (type == 4)
			cout << " is celebrating ";
		cout << value << endl;
	}
};

class Comment {
private:
	char* ID;
	Post* comment_on;
	Object* comment_by;
	char* text;
public:
	~Comment() {
		//cout << "\t\t" << ID << " comment deleted\n";
		if (ID)
			delete[] ID;
		if (text)
			delete[] text;
	}
	void SetValues(char* id, Post* post, Object* obj, char* txt) {
		ID = id;
		comment_on = post;
		comment_by = obj;
		text = txt;
	}
	void PrintComment() {
		comment_by->PrintName();
		cout << " wrote: \"" << text << "\"" << endl;
	}
};

class Post {
protected:
	Date shared_date;
	char* ID;
	char* text;
	Activity* activity;
	Object* shared_by;
	Object** liked_by;
	Comment** comments;
public:
	~Post() {
		//cout << "\t" << ID << " post destructed\n";
		if (ID)
			delete[] ID;
		if (text)
			delete[] text;
		if (activity)
			delete activity;
		if (liked_by)
			delete[] liked_by;
		if (comments) {
			for (int i = 0; i < 10 && comments[i] != nullptr; i++)
				delete comments[i];
			delete[] comments;
		}
	}
	void ReadFromFile(ifstream& posts_file) {
		int type;
		posts_file >> type;
		char temp[100];
		posts_file >> temp;
		int length = strlen(temp);
		ID = new char[length + 1];
		strcpy_s(ID, length + 1, temp);
		shared_date.ReadFromFile(posts_file);
		posts_file.ignore();
		posts_file.getline(temp, 100);
		length = strlen(temp);
		text = new char[length + 1];
		strcpy_s(text, length + 1, temp);
		if (type == 1)
			activity = nullptr;
		else {
			activity = new Activity;
			activity->ReadFromFile(posts_file);
		}
		shared_by = nullptr;
		liked_by = nullptr;
		comments = nullptr;
	}
	void SetSharedBy(Object* obj) {
		shared_by = obj;
	}
	void SetLikedBy(Object* obj) {
		if (!liked_by) {
			liked_by = new Object * [10];
			for (int i = 0; i < 10; i++)
				liked_by[i] = nullptr;
		}
		bool set = false;
		for (int i = 0; i < 10 && set == false; i++) {
			if (liked_by[i] == obj)
				set = true;
			if (liked_by[i] == nullptr) {
				liked_by[i] = obj;
				set = true;
			}
		}
		if (!set) {
			cout << "Can't Add More Likes\n";
		}
	}
	void AddComment(Comment* com) {
		if (!comments) {
			comments = new Comment * [10];
			for (int i = 0; i < 10; i++)
				comments[i] = nullptr;
		}
		bool added = false;
		for (int i = 0; i < 10 && added == false; i++) {
			if (comments[i] == com)
				added = true;
			if (comments[i] == nullptr) {
				comments[i] = com;
				added = true;
			}
		}
		if (!added) {
			cout << "Can't Add More Comments\n";
		}
	}
	bool FindID(const char* search) {
		if (strcmp(ID, search) == 0)
			return true;
		return false;
	}
	void PrintLikedBy() {
		cout << "Post Liked By:\n\n";
		for (int i = 0; i < 10 && liked_by[i] != nullptr; i++) {
			liked_by[i]->PrintListView();
			cout << endl;
		}
	}
	void PrintSharedBy() {
		cout << "Post Shared By:\n\n";
		shared_by->PrintListView();
		cout << endl;
	}
	bool CheckIfLatest() {
		if (shared_date.CheckIfLatest()) 
			return true;
		return false;
	}
	bool CompareDate(Post* post2) {
		return (shared_date < post2->shared_date);
	}
	bool CheckIfMemory(Object* obj) {
		if (shared_by == obj) {
			return (shared_date.CheckIfMemory());
		}
		return false;
	}
	virtual void PrintPost(bool show_date, bool show_comments) {
		cout << "--";
		shared_by->PrintName();
		if (activity) {
			activity->Print();
			cout << "\"" << text << "\"";
		}
		else {
			cout << " shared \"" << text << "\"";
			
		}
		if (show_date) {
			cout << "... (";
			shared_date.PrintDate();
			cout << ")";
		}
		cout << endl;
		if (show_comments) {
			if (comments) {
				for (int i = 0; comments[i] != nullptr; i++) {
					cout << "\t\t";
					comments[i]->PrintComment();
				}
			}
		}
		cout << endl;
	}
	int GetYear() {
		return shared_date.GetYear();
	}
	virtual void AddPost(const char* id, const char* txt, Object* posted_by, Post* org_post) {

	}
};

class Memory : public Post {
private:
	Post* original;
public:
	~Memory(){}
	void AddPost(const char* id, const char* txt, Object* posted_by, Post* org_post) {
		int length = strlen(id);
		ID = new char[length + 1];
		strcpy_s(ID, length + 1, id);
		length = strlen(txt);
		text = new char[length + 1];
		strcpy_s(text, length + 1, txt);
		activity = nullptr;
		shared_by = posted_by;
		liked_by = nullptr;
		comments = nullptr;
		shared_date = Date::todays_date;
		original = org_post;
	}
	void PrintPost(bool show_date, bool show_comments) {
		cout << "~~~";
		shared_by->PrintName();
		cout << " shared a memory~~~";
		if (show_date) {
			cout << "...(";
			shared_date.PrintDate();
			cout << ")\n";
		}
		cout << "\"" << text << "\"\n";
		int diff = shared_date.CompareDate(original->GetYear());
		cout << "\t(" << diff << " years ago)\n";
		original->PrintPost(true, false);
		if (show_comments) {
			if (comments) {
				for (int i = 0; comments[i] != nullptr; i++) {
					cout << "\t\t";
					comments[i]->PrintComment();
				}
			}
		}
		cout << endl;
	}
};

class Page : public Object {
private:
	char* title;
	Post** timeline;
public:
	~Page() {
		//PrintID();
		//cout << " page deleted" << endl;
		if (title)
			delete[] title;
		if (timeline) {
			for (int i = 0; timeline[i] != nullptr; i++)
				delete timeline[i];
			delete[] timeline;
		}
		
	}
	void ReadFromFile(ifstream& pages_file) {
		char temp[50];
		pages_file >> temp;
		SetID(temp);
		pages_file.ignore();
		pages_file.getline(temp, 50);
		int length = strlen(temp);
		title = new char[length + 1];
		strcpy_s(title, length + 1, temp);
		timeline = nullptr;
	}
	bool FindID(const char* search) {
		if (strcmp(GetID(), search) == 0)
			return true;
		return false;
	}
	void PrintListView() {
		PrintID();
		cout << "-" << title;
	}
	void AddPostToTimeline(Post* post) {
		if (!timeline) {
			timeline = new Post * [1];
			timeline[0] = nullptr;
		}
		int count = 0;
		for (int i = 0; timeline[i] != nullptr; i++) {
			count++;
		}
		Post** temp_timeline = new Post * [count + 2];
		for (int i = 0; timeline[i] != nullptr; i++) {
			temp_timeline[i] = timeline[i];
		}
		temp_timeline[count] = post;
		temp_timeline[count + 1] = nullptr;
		delete[] timeline;
		timeline = temp_timeline;
	}
	void PrintName() {
		cout << title;
	}
	void PrintLatestPosts() {
		if (timeline) {
			for (int i = 0; timeline[i] != nullptr; i++) {
				bool latest = timeline[i]->CheckIfLatest();
				if (latest) {
					timeline[i]->PrintPost(false, true);
				}
			}
		}
	}
	void ViewTimeline() {
		for (int i = 0; timeline[i] != nullptr; i++) {
			for (int j = i; timeline[j] != nullptr; j++) {
				if (timeline[i]->CompareDate(timeline[j])) {
					swap(timeline[i], timeline[j]);
				}
			}
		}
		cout << title << "- Timeline\n\n";
		for (int i = 0; timeline[i] != nullptr; i++) {
			timeline[i]->PrintPost(true, true);
		}
	}
};

class User : public Object {
private:
	char* first_name;
	char* last_name;
	Page** liked_pages;
	User** friends_list;
	Post** timeline;
public:
	~User() {
		//PrintID();
		//cout << " user deleted" << endl;
		if (first_name)
			delete[] first_name;
		if (last_name)
			delete[] last_name;
		if (liked_pages)
			delete[] liked_pages;
		if (friends_list)
			delete[] friends_list;
		if (timeline) {
			for (int i = 0; timeline[i] != nullptr; i++)
				delete timeline[i];
			delete[] timeline;
		}
	}
	void ReadFromFile(ifstream& users_file) {
		char temp[50];
		users_file >> temp;
		SetID(temp);
		users_file >> temp;
		int length = strlen(temp);
		first_name = new char[length + 1];
		strcpy_s(first_name, length + 1, temp);
		users_file >> temp;
		length = strlen(temp);
		last_name = new char[length + 1];
		strcpy_s(last_name, length + 1, temp);
		friends_list = nullptr;
		liked_pages = nullptr;
		timeline = nullptr;
	}
	void AddFriend(User* user) {
		if (!friends_list) {
			friends_list = new User * [10];
			for (int i = 0; i < 10; i++) {
				friends_list[i] = nullptr;
			}
		}
		bool added = false;
		for (int i = 0; i < 10 && added == false; i++) {
			if (friends_list[i] == user)
				added = true;
			if (friends_list[i] == nullptr) {
				friends_list[i] = user;
				added = true;
			}
		}
		if (!added) {
			cout << "Can't Add More Friends\n";
		}
	}
	void LikePage(Page* page) {
		if (!liked_pages) {
			liked_pages = new Page * [10];
			for (int i = 0; i < 10; i++) {
				liked_pages[i] = nullptr;
			}
		}
		bool liked = false;
		for (int i = 0; i < 10 && liked == false; i++) {
			if (liked_pages[i] == page)
				liked = true;
			if (liked_pages[i] == nullptr) {
				liked_pages[i] = page;
				liked = true;
			}
		}
		if (!liked) {
			cout << "Can't Like More Pages\n";
		}
	}
	void AddPostToTimeline(Post* post) {
		if (!timeline) {
			timeline = new Post * [1];
			timeline[0] = nullptr;
		}
		int count = 0;
		for (int i = 0; timeline[i] != nullptr; i++) {
			count++;
		}
		Post** temp_timeline = new Post * [count + 2];
		for (int i = 0; timeline[i] != nullptr; i++) {
			temp_timeline[i] = timeline[i];
		}
		temp_timeline[count] = post;
		temp_timeline[count + 1] = nullptr;
		delete[] timeline;
		timeline = temp_timeline;
	}
	bool FindID(const char* search) {
		if (strcmp(GetID(), search) == 0)
			return true;
		return false;
	}
	void PrintListView() {
		PrintID();
		cout <<  "-" << first_name << " " << last_name;
	}
	void PrintName() {
		cout << first_name << " " << last_name;
	}
	void ViewFriendList() {
		cout << first_name << " " << last_name << "- Friends List\n\n";
		if (friends_list) {
			for (int i = 0; friends_list[i] != nullptr; i++) {
				friends_list[i]->PrintListView();
				cout << endl;
			}
		}
		
	}
	void ViewPageList() {
		cout << first_name << " " << last_name << "- Liked Pages\n\n";
		if (liked_pages) {
			for (int i = 0; liked_pages[i] != nullptr; i++) {
				liked_pages[i]->PrintListView();
				cout << endl;
			}
		}
		
	}
	void PrintLatestPosts() {
		if (timeline) {
			for (int i = 0; timeline[i] != nullptr; i++) {
				bool latest = timeline[i]->CheckIfLatest();
				if (latest) {
					timeline[i]->PrintPost(false, true);
				}
			}
		}
	}
	void ViewHome() {
		cout << first_name << " " << last_name << "- Home\n\n";
		if (friends_list) {
			for (int i = 0; i < 10 && friends_list[i] != nullptr; i++) {
				friends_list[i]->PrintLatestPosts();
			}
		}
		if (liked_pages) {
			for (int i = 0; i < 10 && liked_pages[i] != nullptr; i++) {
				liked_pages[i]->PrintLatestPosts();
			}
		}
	}
	void ViewTimeline() {
		if (timeline) {
			for (int i = 0; timeline[i] != nullptr; i++) {
				for (int j = i; timeline[j] != nullptr; j++) {
					if (timeline[i]->CompareDate(timeline[j])) {
						swap(timeline[i], timeline[j]);
					}
				}
			}

			cout << first_name << " " << last_name << "- Timeline\n\n";
			for (int i = 0; timeline[i] != nullptr; i++) {
				timeline[i]->PrintPost(true, true);
			}
		}
	}
	void SeeYourMemories() {
		cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n";
		cout << "\nOn this Day\n";
		bool found = false; 
		if (timeline) {
			for (int i = 0; timeline[i] != nullptr; i++) {
				if (timeline[i]->CheckIfMemory(this)) {
					cout << Date::todays_date.GetYear() - timeline[i]->GetYear() << " years ago\n";
					timeline[i]->PrintPost(true, true);
					found = true;
				}
			}
			if (!found)
				cout << "You have no memories\n";
		}
	}
};


class SocialNetworkApp {
private:
	Page** pages;
	User** users;
	Post** posts;
	int total_comments;
	void LoadPagesFromFile(const char* filename) {
		ifstream pages_file;
		pages_file.open(filename);
		if (pages_file) {
			int total_pages;
			pages_file >> total_pages;
			pages = new Page * [total_pages + 1];
			for (int i = 0; i < total_pages; i++) {
				pages[i] = new Page;
				pages[i]->ReadFromFile(pages_file);
			}
			pages[total_pages] = nullptr;
			pages_file.close();
		}
		else {
			cout << "File \"" << filename << "\" not found\n";
		}
	}
	void LoadUsersFromFile(const char* filename) {
		ifstream users_file;
		users_file.open(filename);
		int total_users;
		users_file >> total_users;
		users = new User * [total_users + 1];
		char*** temp_friend_ID = new char** [total_users];
		for (int i = 0; i < total_users; i++) {
			users[i] = new User;
			users[i]->ReadFromFile(users_file);
			temp_friend_ID[i] = new char* [10];
			bool end = false;
			int j;
			for (j = 0; end == false; j++) {
				char temp[30];
				users_file >> temp;
				if (temp[0] == '-' && temp[1] == '1') 
					end = true;
				else {
					int len = strlen(temp);
					temp_friend_ID[i][j] = new char[len + 1];
					strcpy_s(temp_friend_ID[i][j], len + 1, temp);
				}
			}
			if (j - 1 < 10) {
				temp_friend_ID[i][j - 1] = nullptr;
			}
			end = false;
			for (j = 0; end == false; j++) {
				char temp[30];
				users_file >> temp;
				if (temp[0] == '-' && temp[1] == '1')
					end = true;
				else {
					Page* page_ptr = SearchPageByID(temp);
					users[i]->LikePage(page_ptr);
				}
			}
		}
		for (int i = 0; i < total_users; i++) {
			for (int j = 0; j < 10 && temp_friend_ID[i][j] != nullptr; j++) {
				User* user_ptr = SearchUserByID(temp_friend_ID[i][j]);
				users[i]->AddFriend(user_ptr);
			}
		}
		users[total_users] = nullptr;
		users_file.close();
		for (int i = 0; i < total_users; i++) {
			for (int j = 0; temp_friend_ID[i][j] != nullptr; j++) {
				delete[] temp_friend_ID[i][j];
			}
			delete[] temp_friend_ID[i];
		}
		delete[] temp_friend_ID;
	}
	void LoadPostsFromFile(const char* filename) {
		ifstream posts_file;
		posts_file.open(filename);
		if (posts_file) {
			int total_posts;
			posts_file >> total_posts;
			posts = new Post * [total_posts + 1];
			for (int i = 0; i < total_posts; i++) {
				posts[i] = new Post;
				posts[i]->ReadFromFile(posts_file);
				char temp[10];
				posts_file >> temp;
				Object* obj_ptr = SearchObjectByID(temp);
				if (obj_ptr) {
					obj_ptr->AddPostToTimeline(posts[i]);
					posts[i]->SetSharedBy(obj_ptr);
					posts_file >> temp;
					for (int j = 0; temp[0] != '-'; j++) {
						obj_ptr = SearchObjectByID(temp);
						posts[i]->SetLikedBy(obj_ptr);
						posts_file >> temp;
					}
				}
			}
			posts[total_posts] = nullptr;
			posts_file.close();
		}
		else {
			cout << "File \"" << filename << "\" not found\n";
		}
	}
	void LoadCommentsFromFile(const char* filename) {
		ifstream comments_file;
		comments_file.open(filename);
		if (comments_file) {
			int total_com;
			comments_file >> total_com;
			total_comments = total_com;
			for (int i = 0; i < total_comments; i++) {
				Comment* comment = new Comment;
				char temp[100];
				comments_file >> temp;
				int length = strlen(temp);
				char* ID = new char[length + 1];
				strcpy_s(ID, length + 1, temp);
				comments_file >> temp;
				Post* post_ptr = SearchPostByID(temp);
				post_ptr->AddComment(comment);
				comments_file >> temp;
				Object* comment_by = SearchObjectByID(temp);
				comments_file.ignore();
				comments_file.getline(temp, 100);
				length = strlen(temp);
				char* text = new char[length + 1];
				strcpy_s(text, length + 1, temp);
				comment->SetValues(ID, post_ptr, comment_by, text);
			}
		}
		else {
			cout << "File \"" << filename << "\" not found\n";
		}
	}
	User* SearchUserByID(const char* search) {
		bool match = false;
		int index = 0;
		for (int i = 0; match == false && users[i] != nullptr; i++) {
			if (users[i]->FindID(search)) {
				match = true;
				index = i;
			}
		}
		if (match)
			return users[index];
		else
			return nullptr;
	}
	Page* SearchPageByID(const char* search) {
		bool match = false;
		int index = 0;
		for (int i = 0; pages[i]!= nullptr && match == false; i++) {
			if (pages[i]->FindID(search)) {
				match = true;
				index = i;
			}
		}
		if (match)
			return pages[index];
		else
			return nullptr;
	}
	Post* SearchPostByID(const char* search) {
		bool match = false;
		int index = 0;
		for (int i = 0; posts[i] != nullptr && match == false; i++) {
			if (posts[i]->FindID(search)) {
				match = true;
				index = i;
			}
		}
		if (match)
			return posts[index];
		else
			return nullptr;
	}
	void ViewLikedList(const char* search) {
		Post* post_ptr = SearchPostByID(search);
		if (post_ptr) {
			post_ptr->PrintLikedBy();
		}
		else {
			cout << "Post not found\n";
		}
	}
	void LikePost(const char* search, Object* obj) {
		Post* post_ptr = SearchPostByID(search);
		if (post_ptr)
			post_ptr->SetLikedBy(obj);
	}
	void ViewSharedBy(const char* search) {
		Post* post_ptr = SearchPostByID(search);
		if (post_ptr) {
			post_ptr->PrintSharedBy();
		}
	}
	void PostComment(const char* search, Object* obj, const char* txt) {
		Post* post_ptr = SearchPostByID(search);
		if (post_ptr) {
			Comment* temp_com = new Comment;
			total_comments++;
			char temp[10];
			_itoa_s(total_comments, temp, 10);
			char temp_ID[20] = "c";
			strcat_s(temp_ID, temp);
			int length = strlen(temp_ID);
			char* ID = new char[length + 1];
			strcpy_s(ID, length + 1, temp_ID);
			length = strlen(txt);
			char* text = new char[length + 1];
			strcpy_s(text, length + 1, txt);
			temp_com->SetValues(ID, post_ptr, obj, text);
			post_ptr->AddComment(temp_com);
		}
		
	}
	void ViewPost(const char* search) {
		Post* post_ptr = SearchPostByID(search);
		if (post_ptr) {
			post_ptr->PrintPost(true, true);
		}
	}
	void ViewPage(const char* search) {
		Page* page_ptr = SearchPageByID(search);
		if (page_ptr) {
			page_ptr->ViewTimeline();
		}
	}
	void ShareMemory(const char* search, Object* obj, const char* txt) {
		Post* ptr = SearchPostByID("post10");
		if (ptr) {
			int count = 0;
			for (int i = 0; posts[i] != nullptr; i++) {
				count++;
			}
			Post** temp_posts = new Post * [count + 2];
			for (int i = 0; posts[i] != nullptr; i++) {
				temp_posts[i] = posts[i];
			}
			temp_posts[count] = new Memory;
			temp_posts[count + 1] = nullptr;
			delete[] posts;
			posts = temp_posts;
			char temp[10];
			_itoa_s(count + 1, temp, 10);
			char temp_ID[20] = "post";
			strcat_s(temp_ID, temp);
			posts[count]->AddPost(temp_ID, txt, obj, ptr);
			obj->AddPostToTimeline(posts[count]);
		}
		else {
			cout << "No memory to share\n";
		}
	}
public:
	~SocialNetworkApp() {
		//cout << "app destroyed\n";
		if (users) {
			for (int i = 0; users[i] != nullptr; i++) {
				delete users[i];
			}
			delete[] users;
		}	
		if (pages) {
			for (int i = 0; pages[i] != nullptr; i++) {
				delete pages[i];
			}
			delete[] pages;
		}
		if (posts) {
			delete[] posts;
		}
	}
	Object* SearchObjectByID(const char* search) {
		Object* temp = nullptr;
		if (search[0] == 'u') {
			temp = SearchUserByID(search);
		}
		if (search[0] == 'p') {
			temp = SearchPageByID(search);
		}
		return temp;
	}
	void Load() {
		LoadPagesFromFile("SocialNetworkPages.txt");
		LoadUsersFromFile("SocialNetworkUsers.txt");
		LoadPostsFromFile("SocialNetworkPosts.txt");
		LoadCommentsFromFile("SocialNetworkComments.txt");
	}
	void Run() {
		cout << "Command:\tSet current System Date 15 11 2017\n";
		Date::todays_date.SetDate(15, 11, 2017);
		cout << "System Date:\t";
		Date::todays_date.PrintDate();
		cout << "\n\nCommand:\tSet current user \"u7\"\n";
		User* current_user = SearchUserByID("u16");
		if (current_user) {
			current_user->PrintName();
			cout << " successfully set as Current User\n";
			cout << "\nCommand:\tView Friend List\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewFriendList();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tView Liked Pages\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewPageList();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tView Home\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewHome();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tView Timeline\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewTimeline();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tViewLikedList(post5)\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			ViewLikedList("post5");
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tLikePost(post5)\n";
			LikePost("post5", current_user);
			cout << "\nCommand:\tViewLikedList(post5)\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			ViewLikedList("post5");
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tPostComment(post4, Good Luck for your Result!)\n";
			PostComment("post4", current_user, "Good Luck for your Result!");
			cout << "Command:\tViewPost(post4)\n\n";
			ViewPost("post3");
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tPost(post8, Thanks for the wishes)\n";
			PostComment("post8", current_user, "Thanks for the wishes");
			cout << "Command:\tViewPost(post8)\n\n";
			ViewPost("post8");
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tSeeYourMemories()\n\n";
			current_user->SeeYourMemories();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tShareMemory(post10, \"Never thought I will be in this special field...\")\n";
			ShareMemory("post10", current_user, "Never thought I will be in this special field...");
			cout << "\nCommand:\tView Timeline\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewTimeline();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tViewPage(p1)\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			ViewPage("p1");
			cout << "-----------------------------------------------------------------------------------------------\n";
		}
		else {
			cout << "User not found\n\n";
		}
		cout << "\nCommand\tSet current user \"u11\"\n";
		current_user = SearchUserByID("u11");
		if (current_user) {
			current_user->PrintName();
			cout << " successfully set as Current User\n";
			cout << "\nCommand:\tView Home\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewHome();
			cout << "-----------------------------------------------------------------------------------------------\n";
			cout << "\nCommand:\tView Timeline\n";
			cout << "-----------------------------------------------------------------------------------------------\n";
			current_user->ViewTimeline();
			cout << "-----------------------------------------------------------------------------------------------\n";
		}
		else {
			cout << "User not found\n\n";
		}
	}
};

int main() {
	SocialNetworkApp fb;
	fb.Load();
	fb.Run();
	return 0;
}